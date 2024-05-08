/**
 * @file FOC.c
 * @brief Field oriented control
 *
 * =================================================================
 * @author Bc. Samuel Fertal
 * @author Bc. Vadym Holysh
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 02.05.2024
 * @defgroup FOC FOC
 * @{
 */
#include <FOC.h>

boolean s_FOC_EnableState_b = False_b;
static TRAN_struct s_trans_s = {0};

/* Debug variables. */
F32 Qcurrent=0;
F32 Dcurrent=0;
F32 requestspeed=0;
F32 requestposition = 0;

/**
 * @brief Function for calculating and generating PWM according to user desired trajectory specifications
 * @param trajectory_data_ps Actual data of desired position, speed, acceleration calculated in MTCL module, pre-correction regulator inputs
 * @details Function processes pre-correction regulator inputs, calculates action values as regulation outputs functions, compensates non-linearities of PMSM, transforms
 * desired voltages in rotational 2-phase system to 3-phase stator system, generates desired PWM output, limits desired voltage action values
 */
void FOC_CalculateOutput(const PC_Data_struct* trajectory_data_ps)
{
    /* Exit when FOC is disabled. */
    if(s_FOC_EnableState_b == False_b)
    {
        return;
    }
    const MDA_Data_struct* motor_data_ps = MDA_GetData_ps();

    /*compensation of nonlinearity in the id current component = Lq*p*iq*wr* */
    F32 CompensationCurrent_id = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * ( MOTOR_INDUCTANCE__H__df32 / 2.0f ) * motor_data_ps->currents_s.iq__A__F32 * motor_data_ps->rotor_mech_speed__rad_s1__F32 );
    /*compensation of nonlinearity in the iq current component = Ld*p*id*wr */
    F32 CompensationCurrent_iq = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * ( MOTOR_INDUCTANCE__H__df32 / 2.0f ) * motor_data_ps->currents_s.id__A__F32 * motor_data_ps->rotor_mech_speed__rad_s1__F32 );
    /*compensation of induced voltage = p*lambda*wr */
    F32 CompensationIndVoltage = (F32) ( (F32)MOTOR_POLE_PAIRS_dU16 * MOTOR_MAGNETIC_FLUX_OF_ROTOR_MAGNTES__Wb__df32 * motor_data_ps->rotor_mech_speed__rad_s1__F32);

    /*Controllers*/
    PI_position_controller.ref_value_f32 = trajectory_data_ps->Start_Absolute_Position__rad__F32 + trajectory_data_ps->tj.Position__rad__F32;;
    PI_position_controller.action_value_f32 = 0.0f;
    /* PI_speed_action = PI_position_output */
    requestspeed = PI_ctrl_CalculateOutput(&PI_position_controller, motor_data_ps->angular_position__rad__F32);
    PI_speed_controller.action_value_f32 = PI_ctrl_CalculateOutput(&PI_position_controller, motor_data_ps->angular_position__rad__F32);
    PI_speed_controller.ref_value_f32 = trajectory_data_ps->tj.Speed__rad_s__F32;

    /* PI_iq_action = PI_speed_output */
    Qcurrent= PI_ctrl_CalculateOutput(&PI_speed_controller, motor_data_ps->rotor_mech_speed__rad_s1__F32);
    PI_iq_current_controller.action_value_f32 = Qcurrent;
    /* PI_iq_ref = acceleration * Jm *kt */
    PI_iq_current_controller.ref_value_f32 = trajectory_data_ps->tj.Acceleration__rad_s_2__F32 * MOTOTR_MOMENT_OF_INERTIA__kg_m2__df32 * MOTOR_TORQUE_CONSTANT__Nm_A__df32;

    PI_id_current_controller.ref_value_f32 = Dcurrent;

    /* ud = output from PI Controller id - compensation of nonlinearity in the current component id */
    s_trans_s.dq_s.d_F32 = (F32)( ( PI_ctrl_CalculateOutput(&PI_id_current_controller, motor_data_ps->currents_s.id__A__F32) - CompensationCurrent_id ) );
    /* uq = output from PI Controller iq - compensation of nonlinearity in the current component iq + compensation of induced voltage*/
    s_trans_s.dq_s.q_F32 = (F32)( ( PI_ctrl_CalculateOutput(&PI_iq_current_controller, motor_data_ps->currents_s.iq__A__F32) + CompensationCurrent_iq + CompensationIndVoltage ) );
    /* electric angle */
    s_trans_s.angle__rad__F32 = motor_data_ps->rotor_el_angle__rad__F32;

    /* voltage limiter */
    FOC_VoltageLimiter(&s_trans_s);

    /* transformation from dq to abc */
    TRAN_DqToAbc(&s_trans_s);

    /* Write new compare values to PWM. */
    PWM_SetCompareValues(PWM_DUTY_TO_CMP_dMU16( (s_trans_s.abc_s.a_F32 / motor_data_ps->dc_link_voltage__V__F32) + 0.5f ),
                         PWM_DUTY_TO_CMP_dMU16( (s_trans_s.abc_s.b_F32 / motor_data_ps->dc_link_voltage__V__F32) + 0.5f ),
                         PWM_DUTY_TO_CMP_dMU16( (s_trans_s.abc_s.c_F32 / motor_data_ps->dc_link_voltage__V__F32) + 0.5f ));
}

/**
 * @brief Limitation of ud and ug voltage
 * @details Function limits desired 2-phase rotary voltage values due to the maximal voltage of DC bus circuit
 * @param tran_values_s Structure, which holds desired ud and uq voltage values
 */


void FOC_VoltageLimiter(TRAN_struct * const tran_values_s)
{
    const F32 ud_squared_F32 = tran_values_s->dq_s.d_F32 * tran_values_s->dq_s.d_F32;
    const F32 uq_squared_F32 =  tran_values_s->dq_s.q_F32 * tran_values_s->dq_s.q_F32;
    const F32 voltage_vector_amplitude_F32 = FM_sqrt_F32( ud_squared_F32 + uq_squared_F32 );
    const F32 max_voltage_vector_F32 = 0.5f * MDA_GetData_ps()->dc_link_voltage__V__F32;
    F32 uq_limited_F32;

    if( voltage_vector_amplitude_F32 > max_voltage_vector_F32 )
    {
        uq_limited_F32 = max_voltage_vector_F32 * max_voltage_vector_F32 - ud_squared_F32;
        tran_values_s->dq_s.q_F32 = (tran_values_s->dq_s.q_F32 > 0.0f) ? uq_limited_F32 : -uq_limited_F32;
    }
}

/**
 * @brief Set FOC on/off
 * @param new_state_b is new enable state of the FOC.
 */
void FOC_SetEnableState(boolean new_state_b)
{


    if(new_state_b != s_FOC_EnableState_b){
        PI_ctrl_Init(&PI_id_current_controller);
        PI_ctrl_Init(&PI_iq_current_controller);
        PI_ctrl_Init(&PI_speed_controller);
        PI_ctrl_Init(&PI_position_controller);
        s_MTCL_ReferencePosition__rad__F32 = MDA_GetData_ps()->angular_position__rad__F32;
    }
    s_FOC_EnableState_b = new_state_b;
}

/**
 * @brief Set FOC enable status
 * @returns True_b when FOC is enabled
 * @returns False when FOC is disabled.
 */
boolean FOC_GetEnableState(void)
{
    return s_FOC_EnableState_b;
}

/**
 * @brief Get current motor torque.
 * @returns current motor torque.
 */
F32 FOC_GetTorque__Nm__F32(void)
{
    return MDA_GetData_ps()->currents_s.iq__A__F32 * MOTOR_TORQUE_CONSTANT__Nm_A__df32;
}

/**
 * @brief Alignment of rotary encoder zero position with magnetic.
 */
void FOC_CommutationAlignment(){

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

/**
 * @}
 */
