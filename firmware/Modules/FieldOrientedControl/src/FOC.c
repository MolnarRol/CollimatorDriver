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

F32 FOC_TEST_u_q__V__F32 = (F32)2.0;

F32 Qcurrent=0;
F32 Dcurrent=0;
F32 requestspeed=0;
F32 requestposition = 0;

F32 QCompensation = 0;
F32 DCompensation = 0;
F32 VCompensation = 0;

F32 interval1_final_ticks = 0;
F32 interval2_final_ticks = 0;
F32 interval3_final_ticks = 0;
F32 finalinterval_final_ticks = 0;

F32 delta_pos_middle = 0;
F32 delta_time_middle = 0;
F32 fulltime = 0;



F32 ref_position_regulator = 0;

F32 Ticks = 0;
F32 prev_request_pos__F32__ = 0;

F32 start = 0;
F32 posrad = 0;

F32 interval1strecha_final_ticks=0;
F32 interval2strecha_final_ticks=0;

F32 finalint1speed = 0;
F32 finalint1pos = 0;

F32 finalint2speed = 0;
F32 finalint2pos = 0;


F32 finalint3speed = 0;
F32 finalint3pos = 0;


F32 int3initspeed = -1000;

F32 finalint1pos_strecha = 0;
F32 finalint1speed_strecha = 0;


F32 finalint2pos_strecha = 0;
F32 finalint2speed_strecha = 0;

F32 scnd3initspeed = -1000;


F32 speedpredkorekcia[1000];
F32 pospredkorekcia[1000];
U16 indexpredkorekcia = 0;
U16 index_predkorekcia2 = 0;

extern boolean enable_FOC;

void FOC_CalculateOutput(F32 ReferencePosition__rad__F32, F32 MaxMechSpeed_rad_s1_F32, F32 MaxAcc_rad_s2_F32){

//    /* GENERATION S-CURVE*/

    static F32 DeltaMdlPosition__rad__F32 = 0;
    static F32 DeltaMdlTime__s__F32 = 0;
    static S16 Minus_Check = 0;
    static F32 Position__rad__F32 = 0;
    static F32 Speed__rad_s__F32 = 0;
    static F32 Acceleration__rad_s_2__F32 = 0;

    static F32 Ticks__s__F32 = 0;
    static F32 Start_Absolute_position__rad__F32 = 0;
    static F32 FullTime__s__F32 = 0;

    static U16 ticks_enabled = 0;
    static U16 strecha = 0;

    static U16 error_moment_counter_U16 = 0;

    F32 start_ramp_rad = DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32));

    if(Ticks__s__F32 == 0)
    {


        if(requestposition-prev_request_pos__F32__> 0.08 || requestposition-prev_request_pos__F32__ < -0.08){
            ticks_enabled = 1;
            prev_request_pos__F32__= requestposition;
            Start_Absolute_position__rad__F32 = MDA_GetData_ps()->angular_position__rad__F32;

            Position__rad__F32 = 0; //new line
            if(requestposition > MDA_GetData_ps()->angular_position__rad__F32)
            {
                DeltaMdlPosition__rad__F32 = (requestposition - MDA_GetData_ps()->angular_position__rad__F32) - 2 * DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32));
                DeltaMdlTime__s__F32 = DeltaMdlPosition__rad__F32 / MaxMechSpeed_rad_s1_F32;
                Minus_Check = 1;
            }
            else
            {
                DeltaMdlPosition__rad__F32 = (requestposition - MDA_GetData_ps()->angular_position__rad__F32) + 2 * DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32));
                DeltaMdlTime__s__F32 = -DeltaMdlPosition__rad__F32 / MaxMechSpeed_rad_s1_F32;
                Minus_Check = -1;
            }

       }
       /*overebie deltamidtime*/
       if(DeltaMdlTime__s__F32 > 0){
           strecha = 0;
           FullTime__s__F32 = ( 2.0 * ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) ) + DeltaMdlTime__s__F32;
           delta_time_middle = DeltaMdlTime__s__F32;
           delta_pos_middle = DeltaMdlPosition__rad__F32;
           fulltime = FullTime__s__F32;
       }
       else{
           strecha = 1;
           FullTime__s__F32 = 2.0*sqrt((Minus_Check*(requestposition - MDA_GetData_ps()->angular_position__rad__F32))/MaxAcc_rad_s2_F32);

       }
    }

    /*new lines*/
    if(ticks_enabled){
        Ticks__s__F32 += SAMPLING_TIME__s__df32;
    }
    /*end of new lines*/


    /*rozhodnutie ci strecha anoo alebo nie*/

    /*bez strechy*/
    if(strecha == 0){

        if( Ticks__s__F32 <= ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) && Ticks__s__F32 > 0){

            Acceleration__rad_s_2__F32 = MaxAcc_rad_s2_F32 * Minus_Check;
            Speed__rad_s__F32 += Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;


            /*
            if(Speed__rad_s__F32*Minus_Check  > MaxMechSpeed_rad_s1_F32){
                Speed__rad_s__F32 = MaxMechSpeed_rad_s1_F32*Minus_Check;
            }
            */
            Position__rad__F32 += Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;



            interval1_final_ticks = Ticks__s__F32;
            finalint1pos = Position__rad__F32;
            finalint1speed = Speed__rad_s__F32;
        }

        else if( ( Ticks__s__F32 > ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) ) && Ticks__s__F32 <= ( ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) + DeltaMdlTime__s__F32 ) ){   //mozny bug

            Acceleration__rad_s_2__F32 = 0;
            Speed__rad_s__F32 = MaxMechSpeed_rad_s1_F32 * Minus_Check;                      // Opytat sa
            Position__rad__F32 += Speed__rad_s__F32 * SAMPLING_TIME__s__df32;



            interval2_final_ticks = Ticks__s__F32;
            finalint2pos = Position__rad__F32;
            finalint2speed = Speed__rad_s__F32;


            if(indexpredkorekcia<500){
                if(index_predkorekcia2 == 100){
                    speedpredkorekcia[indexpredkorekcia] = Speed__rad_s__F32;
                    pospredkorekcia[indexpredkorekcia] = Position__rad__F32;
                    indexpredkorekcia++;
                    index_predkorekcia2 = 0;
                }
                index_predkorekcia2++;
            }
            else{
                indexpredkorekcia = 0;
            }


        }

        else if ( ( Ticks__s__F32 > ( ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) + DeltaMdlTime__s__F32 ) ) && ( Ticks__s__F32 <= FullTime__s__F32 ) ){       //mozny bug

            if(int3initspeed == -1000){
                int3initspeed = Speed__rad_s__F32;
            }

            Acceleration__rad_s_2__F32 = -MaxAcc_rad_s2_F32 * Minus_Check ;
            Speed__rad_s__F32 += Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;



            if(Speed__rad_s__F32 * Minus_Check < 0){
                Speed__rad_s__F32 = 0;
            }

            Position__rad__F32 += Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;

            if(Position__rad__F32*Minus_Check > 2.0f*start_ramp_rad + Minus_Check*DeltaMdlPosition__rad__F32){
                Position__rad__F32 = Minus_Check*2.0f*start_ramp_rad + DeltaMdlPosition__rad__F32;
            }


            interval3_final_ticks = Ticks__s__F32;
            finalint3pos = Position__rad__F32;
            finalint3speed = Speed__rad_s__F32;

            if(indexpredkorekcia<1000){
                if(index_predkorekcia2 == 100){
                    speedpredkorekcia[indexpredkorekcia] = Speed__rad_s__F32;
                    pospredkorekcia[indexpredkorekcia] = Position__rad__F32;
                    indexpredkorekcia++;
                    index_predkorekcia2 = 0;
                }
                index_predkorekcia2++;
            }

        }
    }
    /*koniec bez strechy*/

    /*so strechou*/

    if(strecha == 1){
        if( Ticks__s__F32 <= (FullTime__s__F32/2) && Ticks__s__F32 > 0){

           Acceleration__rad_s_2__F32 = MaxAcc_rad_s2_F32 * Minus_Check;
           Speed__rad_s__F32 += Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;
           Position__rad__F32 += Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;
           interval1strecha_final_ticks = Ticks__s__F32;
           finalint1pos_strecha = Position__rad__F32;
           finalint1speed_strecha = Speed__rad_s__F32;

           if(indexpredkorekcia<500){
                           if(index_predkorekcia2 == 100){
                               speedpredkorekcia[indexpredkorekcia] = Speed__rad_s__F32;
                               pospredkorekcia[indexpredkorekcia] = Position__rad__F32;
                               indexpredkorekcia++;
                               index_predkorekcia2 = 0;
                           }
                           index_predkorekcia2++;
                       }
           else{
                           indexpredkorekcia = 0;
           }



        }

        else if ( ( Ticks__s__F32 > ( FullTime__s__F32/2 ) ) && ( Ticks__s__F32 <= FullTime__s__F32 ) ){

           Acceleration__rad_s_2__F32 = -MaxAcc_rad_s2_F32 * Minus_Check ;
           Speed__rad_s__F32 += Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;


           Position__rad__F32 += Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;
           interval2strecha_final_ticks = Ticks__s__F32;
           finalint2pos_strecha = Position__rad__F32;
           finalint2speed_strecha = Speed__rad_s__F32;

           if(Position__rad__F32*Minus_Check > 2.0f*start_ramp_rad + Minus_Check*DeltaMdlPosition__rad__F32){
                           Position__rad__F32 = Minus_Check*2.0f*start_ramp_rad + DeltaMdlPosition__rad__F32;
           }


           if(indexpredkorekcia<1000){
                           if(index_predkorekcia2 == 100){
                               speedpredkorekcia[indexpredkorekcia] = Speed__rad_s__F32;
                               pospredkorekcia[indexpredkorekcia] = Position__rad__F32;
                               indexpredkorekcia++;
                               index_predkorekcia2 = 0;
                           }
                           index_predkorekcia2++;
                       }
        }
    }




    /*koniec strechy*/

    if(Ticks__s__F32 > FullTime__s__F32){
        finalinterval_final_ticks = Ticks__s__F32;
        Ticks__s__F32 = 0;
        Acceleration__rad_s_2__F32 = 0;
        Speed__rad_s__F32 = 0;
        //Position__rad__F32 = 0;

        ticks_enabled = 0;
    }

    Ticks = Ticks__s__F32;
    /*compensation of nonlinearity in the id current component = Lq*p*iq*wr* */
    F32 CompensationCurrent_id = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * ( MOTOR_INDUCTANCE__H__df32 / 2 ) * MDA_GetData_ps()->currents_s.iq__A__F32 * MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32 );
    DCompensation = CompensationCurrent_id;
    /*compensation of nonlinearity in the iq current component = Ld*p*id*wr */
    F32 CompensationCurrent_iq = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * ( MOTOR_INDUCTANCE__H__df32 / 2 ) * MDA_GetData_ps()->currents_s.id__A__F32 * MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32 );
    QCompensation = CompensationCurrent_iq;
    /*compensation of induced voltage = p*lambda*wr */
    F32 CompensationIndVoltage = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * MOTOR_MAGNETIC_FLUX_OF_ROTOR_MAGNTES__Wb__df32 * MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32);
    VCompensation = CompensationIndVoltage;

    start = Start_Absolute_position__rad__F32;
    posrad  = Position__rad__F32;

    /*Controllers*/
    ref_position_regulator = Start_Absolute_position__rad__F32 + Position__rad__F32;
    PI_position_controller.ref_value_f32 = ref_position_regulator;

    /* PI_speed_action = PI_position_output */
    requestspeed = PI_ctrl_CalculateOutput(&PI_position_controller, MDA_GetData_ps()->angular_position__rad__F32);
    PI_speed_controller.action_value_f32 = requestspeed;//PI_ctrl_CalculateOutput(&PI_position_controller, MDA_GetData_ps()->rotor_mech_angle__rad__F32);
    PI_speed_controller.ref_value_f32 = Speed__rad_s__F32;

    /* PI_iq_action = PI_speed_output */

    Qcurrent= PI_ctrl_CalculateOutput(&PI_speed_controller, MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32);
    PI_iq_current_controller.action_value_f32 = Qcurrent;
    /* PI_iq_ref = acceleration * Jm *kt */
    PI_iq_current_controller.ref_value_f32 = Acceleration__rad_s_2__F32 * MOTOTR_MOMENT_OF_INERTIA__kg_m2__df32 * MOTOR_TORQUE_CONSTANT__Nm_A__df32;

    PI_id_current_controller.ref_value_f32 = Dcurrent;

    /* ud = output from PI Controller id - compensation of nonlinearity in the current component id */
    trans_s.dq_s.d_F32 = (F32)( ( PI_ctrl_CalculateOutput(&PI_id_current_controller, MDA_GetData_ps()->currents_s.id__A__F32) - CompensationCurrent_id ) );

    /* uq = output from PI Controller iq - compensation of nonlinearity in the current component iq + compensation of induced voltage*/
    trans_s.dq_s.q_F32 = (F32)( ( PI_ctrl_CalculateOutput(&PI_iq_current_controller, MDA_GetData_ps()->currents_s.iq__A__F32) + CompensationCurrent_iq + CompensationIndVoltage ) );

    /* electric angle */
    trans_s.angle__rad__F32 = MDA_GetData_ps()->rotor_el_angle__rad__F32;

    if( ( MDA_GetData_ps()->currents_s.iq__A__F32 * MOTOR_TORQUE_CONSTANT__Nm_A__df32 ) > MAX_MOMENT )
    {
        error_moment_counter_U16++;
        if(error_moment_counter_U16 > 1000){
            error_moment_counter_U16 = 0;
            trans_s.dq_s.d_F32 = 0;
            trans_s.dq_s.q_F32 = 0;
            enable_FOC = 0;
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

void Voltage_Limiter(TRAN_struct * const tran_values_s){

    if( sqrt( ( tran_values_s->dq_s.d_F32 * tran_values_s->dq_s.d_F32 ) + ( tran_values_s->dq_s.q_F32 * tran_values_s->dq_s.q_F32 ) ) > ( 0.5 * MDA_GetData_ps()->dc_link_voltage__V__F32 ) )
    {
        if(trans_s.dq_s.q_F32 > 0)
        tran_values_s->dq_s.q_F32 = (F32) sqrt( ( 0.5 * MDA_GetData_ps()->dc_link_voltage__V__F32 ) - ( tran_values_s->dq_s.d_F32 * tran_values_s->dq_s.d_F32 ) );   /*  sqrt(12*12 - Ud^2) */
        else
        tran_values_s->dq_s.q_F32 = (F32)-sqrt( ( 0.5 * MDA_GetData_ps()->dc_link_voltage__V__F32 ) - ( tran_values_s->dq_s.d_F32 * tran_values_s->dq_s.d_F32 ) );   /*  sqrt(12*12 - Ud^2) */

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
