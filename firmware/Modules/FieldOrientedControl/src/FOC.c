/*
 * FOC.c
 *
 *  Created on: 4 ap. 2024 �.
 *      Author: vadym
 */

#include "FOC.h"

TRAN_struct trans_s =
{
    .dq_s =
    {
         .d_F32 = (F32)0.0,
         .q_F32 = (F32)0.0
    },
    .angle__rad__F32 = (F32)0.0
};


PC_Data_struct s_PC_data_s;

F32 Qcurrent=0;
F32 Dcurrent=0;
F32 requestspeed=0;
F32 requestposition = 0;

F32 prev_request_pos__F32__ = 0;
U16 error_moment_counter_U16 = 0;

F64 samp = SAMPLING_TIME__s__df32;

extern boolean enable_FOC;
extern boolean alarm_state;


void FOC_CalculateOutput(F32 ReferencePosition__rad__F32, F32 MaxMechSpeed_rad_s1_F32, F32 MaxAcc_rad_s2_F32){

//    static U16 error_moment_counter_U16 = 0;
    static U16 error_disable_FOC_counter_U16 = 0;
    static F32 Requested_Positionn = 0;

    if(!alarm_state)
    {
        Requested_Positionn = ReferencePosition__rad__F32;
    }
    else
    {
        Requested_Positionn = 0.0;
    }

    if(alarm_state && MDA_GetData_ps()->angular_position__rad__F32 == 0.0)
    {
        error_disable_FOC_counter_U16++;
        if(error_disable_FOC_counter_U16 == 2000)
        {
        alarm_state = 0;
        enable_FOC = 0;
        error_disable_FOC_counter_U16 = 0;
        }
    }

    /*Calculating trajectories*/
    PC_CalculateData(Requested_Positionn, MaxMechSpeed_rad_s1_F32, MaxAcc_rad_s2_F32);

    /*compensation of nonlinearity in the id current component = Lq*p*iq*wr* */
    F32 CompensationCurrent_id = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * ( MOTOR_INDUCTANCE__H__df32 / 2 ) * MDA_GetData_ps()->currents_s.iq__A__F32 * MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32 );
    /*compensation of nonlinearity in the iq current component = Ld*p*id*wr */
    F32 CompensationCurrent_iq = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * ( MOTOR_INDUCTANCE__H__df32 / 2 ) * MDA_GetData_ps()->currents_s.id__A__F32 * MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32 );
    /*compensation of induced voltage = p*lambda*wr */
    F32 CompensationIndVoltage = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * MOTOR_MAGNETIC_FLUX_OF_ROTOR_MAGNTES__Wb__df32 * MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32);

    /*Controllers*/
    PI_position_controller.ref_value_f32 = PC_GetData_ps()->Start_Absolute_Position__rad__F32 + PC_GetData_ps()->tj.Position__rad__F32;;
    PI_position_controller.action_value_f32 = 0.0;
    /* PI_speed_action = PI_position_output */
    requestspeed = PI_ctrl_CalculateOutput(&PI_position_controller, MDA_GetData_ps()->angular_position__rad__F32);
    PI_speed_controller.action_value_f32 = PI_ctrl_CalculateOutput(&PI_position_controller, MDA_GetData_ps()->angular_position__rad__F32);
    PI_speed_controller.ref_value_f32 = PC_GetData_ps()->tj.Speed__rad_s__F32;

    /* PI_iq_action = PI_speed_output */
    Qcurrent= PI_ctrl_CalculateOutput(&PI_speed_controller, MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32);
    PI_iq_current_controller.action_value_f32 = Qcurrent;
    /* PI_iq_ref = acceleration * Jm *kt */
    PI_iq_current_controller.ref_value_f32 = PC_GetData_ps()->tj.Acceleration__rad_s_2__F32 * MOTOTR_MOMENT_OF_INERTIA__kg_m2__df32 * MOTOR_TORQUE_CONSTANT__Nm_A__df32;

    PI_id_current_controller.ref_value_f32 = Dcurrent;

    /* ud = output from PI Controller id - compensation of nonlinearity in the current component id */
    trans_s.dq_s.d_F32 = (F32)( ( PI_ctrl_CalculateOutput(&PI_id_current_controller, MDA_GetData_ps()->currents_s.id__A__F32) - CompensationCurrent_id ) );
    /* uq = output from PI Controller iq - compensation of nonlinearity in the current component iq + compensation of induced voltage*/
    trans_s.dq_s.q_F32 = (F32)( ( PI_ctrl_CalculateOutput(&PI_iq_current_controller, MDA_GetData_ps()->currents_s.iq__A__F32) + CompensationCurrent_iq + CompensationIndVoltage ) );
    /* electric angle */
    trans_s.angle__rad__F32 = MDA_GetData_ps()->rotor_el_angle__rad__F32;

    /* torque limiter */

     if( ( MDA_GetData_ps()->currents_s.iq__A__F32 * MOTOR_TORQUE_CONSTANT__Nm_A__df32 ) > MAX_MOMENT
          || ( MDA_GetData_ps()->currents_s.iq__A__F32 * MOTOR_TORQUE_CONSTANT__Nm_A__df32 ) < -MAX_MOMENT )
     {
         error_moment_counter_U16++;
         if(!alarm_state){
         if(error_moment_counter_U16 > 3000){
             error_moment_counter_U16 = 0;
             PC_Reset_Data(1);
             alarm_state = 1;
         }
         }
         else{
             error_moment_counter_U16++;
            if(error_moment_counter_U16 > 10000){
                error_moment_counter_U16 = 0;
                PC_Reset_Data(1);
                alarm_state = 0;
                enable_FOC = 0;
                trans_s.dq_s.q_F32 = 0;
                trans_s.dq_s.d_F32 = 0;
            }
         }
       }

    /* voltage limiter */
    Voltage_Limiter(&trans_s);

    /* transformation from dq to abc */
    TRAN_DqToAbc(&trans_s);


    PWM_SetCompareValues(PWM_DUTY_TO_CMP_dMU16( (trans_s.abc_s.a_F32 / MDA_GetData_ps()->dc_link_voltage__V__F32) + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( (trans_s.abc_s.b_F32 / MDA_GetData_ps()->dc_link_voltage__V__F32) + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( (trans_s.abc_s.c_F32 / MDA_GetData_ps()->dc_link_voltage__V__F32) + (F32)0.5 ));
}


static void PC_CalculateData(F32 Requested_Position__rad__F32, F32 MaxMechSpeed_rad_s1_F32, F32 MaxAcc_rad_s2_F32)
{

    static F32 DeltaMdlPosition__rad__F32 = 0;
    static F32 DeltaMdlTime__s__F32 = 0;
    static S16 Minus_Check = 0;
    static F32 FullTime__s__F32 = 0;
    static boolean roof = 0;

    F32 start_ramp_rad = DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32));

    if(s_PC_data_s.Ticks__s__F32 == 0)
    {
        if(Requested_Position__rad__F32 - prev_request_pos__F32__> 0.08 || Requested_Position__rad__F32 - prev_request_pos__F32__ < -0.08)
        {
            s_PC_data_s.ticks_enabled = 1;
            prev_request_pos__F32__= Requested_Position__rad__F32;
            s_PC_data_s.Start_Absolute_Position__rad__F32 = MDA_GetData_ps()->angular_position__rad__F32;
            s_PC_data_s.tj.Position__rad__F32 = 0; //new line

                if(Requested_Position__rad__F32 > MDA_GetData_ps()->angular_position__rad__F32)
                {
                    DeltaMdlPosition__rad__F32 = (Requested_Position__rad__F32 - MDA_GetData_ps()->angular_position__rad__F32) - 2 * DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32));
                    DeltaMdlTime__s__F32 = DeltaMdlPosition__rad__F32 / MaxMechSpeed_rad_s1_F32;
                    Minus_Check = 1;
                }
                else
                {
                    DeltaMdlPosition__rad__F32 = (Requested_Position__rad__F32 - MDA_GetData_ps()->angular_position__rad__F32) + 2 * DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32));
                    DeltaMdlTime__s__F32 = -DeltaMdlPosition__rad__F32 / MaxMechSpeed_rad_s1_F32;
                    Minus_Check = -1;
                }
        }
        /*override delta mid time*/
        if(DeltaMdlTime__s__F32 > 0)
        {
            roof = 0;
            FullTime__s__F32 = ( 2.0 * ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) ) + DeltaMdlTime__s__F32;
        }
        else
        {
            roof = 1;
            FullTime__s__F32 = 2.0*FM_sqrt_F32((Minus_Check*(Requested_Position__rad__F32 - MDA_GetData_ps()->angular_position__rad__F32))/MaxAcc_rad_s2_F32);
        }

        }

        /*new lines*/
        if(s_PC_data_s.ticks_enabled)
        {
            s_PC_data_s.Ticks__s__F32 += SAMPLING_TIME__s__df32;
        }
        /*end of new lines*/

        /*not roof*/
        if(roof == 0){

            if( s_PC_data_s.Ticks__s__F32 <= ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32)
                && s_PC_data_s.Ticks__s__F32 > 0)
            {
                s_PC_data_s.tj.Acceleration__rad_s_2__F32 = MaxAcc_rad_s2_F32 * Minus_Check;
                s_PC_data_s.tj.Speed__rad_s__F32 += s_PC_data_s.tj.Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;
                s_PC_data_s.tj.Position__rad__F32 += s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;
            }

            else if( ( s_PC_data_s.Ticks__s__F32 > ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) )
                    && s_PC_data_s.Ticks__s__F32 <= ( ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) + DeltaMdlTime__s__F32 ) ) //maybe bug
            {
                s_PC_data_s.tj.Acceleration__rad_s_2__F32 = 0;
                s_PC_data_s.tj.Speed__rad_s__F32 = MaxMechSpeed_rad_s1_F32 * Minus_Check;                            // Opytat sa
                s_PC_data_s.tj.Position__rad__F32 += (F32)( s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32 );
            }

            else if ( ( s_PC_data_s.Ticks__s__F32 > ( ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) + DeltaMdlTime__s__F32 ) ) //maybe bug
                    && ( s_PC_data_s.Ticks__s__F32 <= FullTime__s__F32 ) )
            {
                s_PC_data_s.tj.Acceleration__rad_s_2__F32 = -MaxAcc_rad_s2_F32 * Minus_Check;
                s_PC_data_s.tj.Speed__rad_s__F32 += s_PC_data_s.tj.Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;

                if(s_PC_data_s.tj.Speed__rad_s__F32 * Minus_Check < 0.0)
                {
                    s_PC_data_s.tj.Speed__rad_s__F32 = 0.0;
                }

                s_PC_data_s.tj.Position__rad__F32 += s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;

                if(s_PC_data_s.tj.Position__rad__F32*Minus_Check >= 2.0f*start_ramp_rad + Minus_Check*DeltaMdlPosition__rad__F32)
                {
                    s_PC_data_s.tj.Position__rad__F32 = Minus_Check*2.0f*start_ramp_rad + DeltaMdlPosition__rad__F32;
                }
            }
        }
        /*end not roof*/

        /*roof*/
        if(roof == 1)
        {
            if( s_PC_data_s.Ticks__s__F32 <= (FullTime__s__F32/2.0) && s_PC_data_s.Ticks__s__F32 > 0)
            {
                s_PC_data_s.tj.Acceleration__rad_s_2__F32 = MaxAcc_rad_s2_F32 * Minus_Check;
                s_PC_data_s.tj.Speed__rad_s__F32 += s_PC_data_s.tj.Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;
                s_PC_data_s.tj.Position__rad__F32 += s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32;
            }

            else if ( ( s_PC_data_s.Ticks__s__F32 > ( FullTime__s__F32/2.0 ) ) && ( s_PC_data_s.Ticks__s__F32 <= FullTime__s__F32 ) )
            {
                s_PC_data_s.tj.Acceleration__rad_s_2__F32 = MaxAcc_rad_s2_F32 * Minus_Check;
                s_PC_data_s.tj.Speed__rad_s__F32 += s_PC_data_s.tj.Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;
                s_PC_data_s.tj.Position__rad__F32 += s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32;

                if(s_PC_data_s.tj.Position__rad__F32*Minus_Check >= 2.0f*start_ramp_rad + Minus_Check*DeltaMdlPosition__rad__F32)
                {
                    s_PC_data_s.tj.Position__rad__F32 = Minus_Check*2.0f*start_ramp_rad + DeltaMdlPosition__rad__F32;
                }
            }
        }
        /*end roof*/

        if(s_PC_data_s.Ticks__s__F32 > FullTime__s__F32)
        {
            if(s_PC_data_s.tj.Position__rad__F32*Minus_Check < 2.0f*start_ramp_rad + Minus_Check*DeltaMdlPosition__rad__F32)
            {
                s_PC_data_s.tj.Position__rad__F32 = Minus_Check*2.0f*start_ramp_rad + DeltaMdlPosition__rad__F32;
            }
            error_moment_counter_U16 = 0;
            PC_Reset_Data(0);
        }
}


static inline void PC_Reset_Data(boolean Full_Reset)
{
    s_PC_data_s.Ticks__s__F32 = 0.0;
    s_PC_data_s.ticks_enabled = 0;
    s_PC_data_s.tj.Acceleration__rad_s_2__F32 = 0.0;
    s_PC_data_s.tj.Speed__rad_s__F32 = 0.0;
    if(Full_Reset) s_PC_data_s.tj.Position__rad__F32 = 0.0;
}

inline const PC_Data_struct* PC_GetData_ps(void)
{
    return (const PC_Data_struct*)&s_PC_data_s;
}



void Voltage_Limiter(TRAN_struct * const tran_values_s){

    if( FM_sqrt_F32( ( tran_values_s->dq_s.d_F32 * tran_values_s->dq_s.d_F32 ) + ( tran_values_s->dq_s.q_F32 * tran_values_s->dq_s.q_F32 ) ) > ( 0.5 * MDA_GetData_ps()->dc_link_voltage__V__F32 ) )
    {
        if(trans_s.dq_s.q_F32 > 0)
        tran_values_s->dq_s.q_F32 = (F32) FM_sqrt_F32( ( 0.5 * MDA_GetData_ps()->dc_link_voltage__V__F32 ) - ( tran_values_s->dq_s.d_F32 * tran_values_s->dq_s.d_F32 ) );   /*  sqrt(12*12 - Ud^2) */
        else
        tran_values_s->dq_s.q_F32 = (F32)-FM_sqrt_F32( ( 0.5 * MDA_GetData_ps()->dc_link_voltage__V__F32 ) - ( tran_values_s->dq_s.d_F32 * tran_values_s->dq_s.d_F32 ) );   /*  sqrt(12*12 - Ud^2) */

    }
}


void CommutationAlignment(){

    PWM_SetOutputEnable(True_b);


    PWM_ForceAngle(0.0, 2.0, 24.0);

    DELAY_US(1000000);

    // Incializacia POSCNT
    EALLOW;
    EQep1Regs.QEPCTL.bit.SWI = 0;
    EQep1Regs.QEPCTL.bit.SWI = 1;
    EQep1Regs.QEPCTL.bit.SWI = 0;
    EDIS;

    DELAY_US(50000);
    PWM_SetCompareValues(0, 0, 0);
    PWM_SetOutputEnable(False_b);
    DELAY_US(1000000);
}
