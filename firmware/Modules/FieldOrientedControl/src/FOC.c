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

void FOC_CalculateOutput(F32 ReferencePosition__rad__F32){

    /* GENERATION S-CURVE*/

    F32 DeltaMdlPosition__rad__F32 = ReferencePosition__rad__F32 - 2 * DELTA_ACCELERATION_POSITION__rad__df32;
    F32 DeltaMdlTime__s__F32 = DeltaMdlPosition__rad__F32 / MAX_SPEED__rad_s__df32;
    F32 FullTime__s__F32 = ( 2 * ACCELERATOIN_TIME__s__df32 ) + DeltaMdlTime__s__F32;

    static F32 Position__rad__F32 = 0;
    static F32 Speed__rad_s__F32 = 0;
    static F32 Acceleration__rad_s_2__F32 = 0;
    static F32 Ticks__s__F32 = 0;

    Ticks__s__F32 += SAMPLING_TIME__s__df32;

    if( Ticks__s__F32 <= ACCELERATOIN_TIME__s__df32 ){

        Acceleration__rad_s_2__F32 = MAX_ACCELERATON__rad_s_2__df32;
        Speed__rad_s__F32 += Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32;
        Position__rad__F32 += Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;

    }

    else if( ( Ticks__s__F32 > ACCELERATOIN_TIME__s__df32 ) && Ticks__s__F32 <= ( ACCELERATOIN_TIME__s__df32 + DeltaMdlTime__s__F32 ) ){

        Acceleration__rad_s_2__F32 = 0;
        Speed__rad_s__F32 = MAX_SPEED__rad_s__df32;
        Position__rad__F32 += Speed__rad_s__F32 * SAMPLING_TIME__s__df32;
    }

    else if ( ( Ticks__s__F32 > ( ACCELERATOIN_TIME__s__df32 + DeltaMdlTime__s__F32 ) ) && ( Ticks__s__F32 <= FullTime__s__F32 ) ){

        Acceleration__rad_s_2__F32 = -MAX_ACCELERATON__rad_s_2__df32;
        Speed__rad_s__F32 += Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32;
        Position__rad__F32 += Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;

    }

    if(Ticks__s__F32 > FullTime__s__F32){
        Ticks__s__F32 = 0;
        Acceleration__rad_s_2__F32 = 0;
        Speed__rad_s__F32 = 0;
    }

    /*compensation of nonlinearity in the id current component = Lq*p*iq*wr* */
    F32 CompensationCurrent_id = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * ( MOTOR_INDUCTANCE__H__df32 / 2 ) * MDA_GetData_ps()->currents_s.iq__A__F32 * MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32 );

    /*compensation of nonlinearity in the iq current component = Ld*p*id*wr */
    F32 CompensationCurrent_iq = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * ( MOTOR_INDUCTANCE__H__df32 / 2 ) * MDA_GetData_ps()->currents_s.id__A__F32 * MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32 );

    /*compensation of induced voltage = p*lambda*wr */
    F32 CompensationIndVoltage = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * MOTOR_MAGNETIC_FLUX_OF_ROTOR_MAGNTES__Wb__df32 * MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32);

    /*Controllers*/

    PI_position_controller.ref_value_f32 = Position__rad__F32;

    /* PI_speed_action = PI_position_output */
    PI_speed_controller.action_value_f32 = PI_ctrl_CalculateOutput(&PI_position_controller, MDA_GetData_ps()->rotor_mech_angle__rad__F32);
    PI_speed_controller.ref_value_f32 = Speed__rad_s__F32;

    /* PI_iq_action = PI_speed_output */
    PI_iq_current_controller.action_value_f32 = PI_ctrl_CalculateOutput(&PI_speed_controller, MDA_GetData_ps()->rotor_mech_speed__rad_s1__F32);

    /* PI_iq_ref = acceleration * Jm *kt */
    PI_iq_current_controller.ref_value_f32 = Acceleration__rad_s_2__F32 * MOTOTR_MOMENT_OF_INERTIA__kg_m2__df32 * MOTOR_TORQUE_CONSTANT__Nm_A__df32;

    PI_id_current_controller.ref_value_f32 = 0;



    /* ud = output from PI Controller id - compensation of nonlinearity in the current component id */
    trans_s.dq_s.d_F32 = (F32)( ( PI_ctrl_CalculateOutput(&PI_id_current_controller, MDA_GetData_ps()->currents_s.id__A__F32) ) - CompensationCurrent_id );

    /* uq = output from PI Controller iq - compensation of nonlinearity in the current component iq + compensation of induced voltage*/
    trans_s.dq_s.q_F32 = (F32)( ( PI_ctrl_CalculateOutput(&PI_iq_current_controller, MDA_GetData_ps()->currents_s.iq__A__F32) ) + CompensationCurrent_iq + CompensationIndVoltage );

    /* electric angle */
    trans_s.angle__rad__F32 = 6.2831 * ( MDA_GetData_ps()->rotor_el_angle__rad__F32 / (F32)U16_MAX );

    /* transformation from dq to abc */
    TRAN_DqToAbc(&trans_s);

    PWM_SetCompareValues(PWM_DUTY_TO_CMP_dMU16( (trans_s.abc_s.a_F32 / MDA_GetData_ps()->dc_link_voltage__V__F32) + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( (trans_s.abc_s.b_F32 / MDA_GetData_ps()->dc_link_voltage__V__F32) + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( (trans_s.abc_s.c_F32 / MDA_GetData_ps()->dc_link_voltage__V__F32) + (F32)0.5 ));


}

void CommutationAlignment(){

    PWM_SetOutputEnable(True_b);

    TRAN_struct comut_S;
    //1.krok DC napatie na fazy U V so striedou z = 0.5

    comut_S.dq_s.d_F32 = 2.0;
    comut_S.dq_s.q_F32 = 2.0;
    // Cakacka -- idealne timer 2
    TRAN_DqToAbc(&comut_S);

    PWM_SetCompareValues(PWM_DUTY_TO_CMP_dMU16( comut_S.abc_s.a_F32 / 24.0 + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( comut_S.abc_s.b_F32 / 24.0 + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( comut_S.abc_s.c_F32 / 24.0 + (F32)0.5 ));

    DELAY_US(100000);

    // Incializacia POSCNT
    EALLOW;
    EQep1Regs.QEPCTL.bit.SWI = 0;
    EQep1Regs.QEPCTL.bit.SWI = 1;
    EQep1Regs.QEPCTL.bit.SWI = 0;
    EDIS;

    DELAY_US(100000);

    // Odpojenie napatia

//    comut_S.dq_s.d_F32 = 0;
//    comut_S.dq_s.q_F32 = 0;
//
//    TRAN_DqToAbc(&comut_S);
//
//    PWM_SetCompareValues(PWM_DUTY_TO_CMP_dMU16( comut_S.abc_s.a_F32 / 24.0 + (F32)0.5 ),
//                         PWM_DUTY_TO_CMP_dMU16( comut_S.abc_s.b_F32 / 24.0 + (F32)0.5 ),
//                         PWM_DUTY_TO_CMP_dMU16( comut_S.abc_s.c_F32 / 24.0 + (F32)0.5 ));


    PWM_SetOutputEnable(False_b);

}