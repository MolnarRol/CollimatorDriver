/**
 * @file PI_Controller.c
 * @brief Controller for motor regulation
 * @details Module made PI controllers for currents, speed and position, calculating output controllers
 *
 * =================================================================
 * @author Bc. Vadym Holysh
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 14.03.2024
 * @defgroup PI Proportional Integration Controller
 * @{
 */

#include <PI_Controller.h>

/**
 * @brief   PI controller calculation output.
 * @details Calculate output for every controller using reference value, action value from previous controller and actual (measured) value.
 * @param Address controller structure where saved action value, reference value and previous integration value
 * @param Actual (measured) value which controlling
 * @return Output value from controller it's action value for next controller
 */
F32 PI_ctrl_CalculateOutput(PI_CTRL_s *controller, F32 y_ref_f32){

    /*
     * e(k) = w(k) - y(k)
     */
    F32 error_f32 = controller->action_value_f32 + controller->ref_value_f32 - y_ref_f32;

    if (error_f32 < 0.0001 && error_f32 > -0.0001){
        error_f32 = 0;
    }
    /*
     * I(k) = T*e(k) + I(k-1)
     */

    F32 integrator_f32 = controller->samp_period__s__f32 * error_f32 * controller->gain_s.Ki_f32 + controller->I_previous_f32;

    /*
     * u(k) = Kp*e(k) + Ki*I(k)
     */
    F32 u_out_f32 = controller->gain_s.Kp_f32 * error_f32 + integrator_f32;

    /* Limit output */
    if(u_out_f32 > controller->limit_s.out_max_f32)
    {
        u_out_f32 = controller->limit_s.out_max_f32;
        integrator_f32 = controller->I_previous_f32;
    }
    else if(u_out_f32 < controller->limit_s.out_min_f32)
    {
        u_out_f32 = controller->limit_s.out_min_f32;
        integrator_f32 = controller->I_previous_f32;
    }

    /* Save previous integrator value */
    controller->I_previous_f32 = integrator_f32;

    return ((F32)u_out_f32);
}

/**
 * @brief   PI controller initialization.
 * @details Reset reference value, action value and previous integration value form controller
 * @param Address to controller structure
 */
void PI_ctrl_Init(PI_CTRL_s *controller){

    controller->I_previous_f32 = (F32)0.0f;
    controller->action_value_f32 = (F32)0.0f;
    controller->ref_value_f32 = (F32)0.0f;
}

/**
 * @brief   PI controller for Id current.
 */
PI_CTRL_s PI_id_current_controller =
{
     .gain_s =
     {
     .Kp_f32 = I_D_KP_df32,
     .Ki_f32 = I_D_KI_df32
     },
     .limit_s =
     {
      .out_max_f32 = MAX_VOLTAGE_D__V__df32,
      .out_min_f32 = MIN_VOLTAGE_D__V__df32
     },
     .samp_period__s__f32 = SAMPLING_TIME__s__df32,
     .ref_value_f32 = ( (F32)0.0f ),
     .action_value_f32 = ( (F32)0.0f ),
     .I_previous_f32 = ( (F32)0.0f )
};

/**
 * @brief   PI controller for Iq current.
 */
PI_CTRL_s PI_iq_current_controller =
{
     .gain_s =
     {
     .Kp_f32 = I_Q_KP_df32,
     .Ki_f32 = I_Q_KI_df32
     },
     .limit_s =
     {
      .out_max_f32 = MAX_VOLTAGE_Q__V__df32,
      .out_min_f32 = MIN_VOLTAGE_Q__V__df32
     },
     .samp_period__s__f32 = SAMPLING_TIME__s__df32,
     .ref_value_f32 = ( (F32)0.0f ),
     .action_value_f32 = ( (F32)0.0f ),
     .I_previous_f32 = ( (F32)0.0f )
};

/**
 * @brief   PI controller for speed.
 */
PI_CTRL_s PI_speed_controller =
{
     .gain_s =
     {
     .Kp_f32 = SPEED_KP_df32,
     .Ki_f32 = SPEED_KI_df32
     },
     .limit_s =
     {
      .out_max_f32 = MAX_CURRENT_Q__A__df32,
      .out_min_f32 = MIN_CURRENT_Q__A__df32
     },
     .samp_period__s__f32 = SAMPLING_TIME_SPEED_REG__s__dF32,
     .ref_value_f32 = ( (F32)0.0f ),
     .action_value_f32 = ( (F32)0.0f ),
     .I_previous_f32 = ( (F32)0.0f )
};

/**
 * @brief   PI controller for position.
 */
PI_CTRL_s PI_position_controller =
{
     .gain_s =
     {
     .Kp_f32 = POSITION_KP_df32,
     .Ki_f32 = POSITION_KI_df32
     },
     .limit_s =
     {
      .out_max_f32 = MAX_SPEED__RPM__df32,
      .out_min_f32 = MIN_SPEED__RPM__df32
     },
     .samp_period__s__f32 = SAMPLING_TIME__s__df32,
     .ref_value_f32 = ( (F32)0.0f ),
     .action_value_f32 = ( (F32)0.0f ),
     .I_previous_f32 = ( (F32)0.0f )
};

/**
 * @}
 */
