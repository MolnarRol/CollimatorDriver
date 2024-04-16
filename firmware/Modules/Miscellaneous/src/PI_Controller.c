/*
 * Controller.c
 *
 *  Created on: 14 mar. 2024 �.
 *      Author: vadym
 */

#include <PI_Controller.h>
F32 Iq_error = 0;
F32 Id_error = 0;
F32 W_error = 0;
F32 Phi_error = 0;

F32 Iq_integrator = 0;
F32 Id_integrator = 0;
F32 W_integrator = 0;
F32 Phi_integrator = 0;

F32 Iq_out = 0;
F32 Id_out = 0;
F32 W_out = 0;
F32 Phi_out = 0;



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

    if(controller==&PI_id_current_controller){
        Id_error = error_f32;
        Id_integrator = integrator_f32;
        Id_out = u_out_f32;
    }
    if(controller==&PI_iq_current_controller){
        Iq_error = error_f32;
        Iq_integrator = integrator_f32;
        Iq_out = u_out_f32;
    }
    if(controller==&PI_speed_controller){
        W_error = error_f32;
        W_integrator = integrator_f32;
        W_out = u_out_f32;
    }
    if(controller==&PI_position_controller){
        Phi_error = error_f32;
        Phi_integrator = integrator_f32;
        Phi_out = u_out_f32;
    }


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

    controller->I_previous_f32 = integrator_f32;

    return ((F32)u_out_f32);
}

void PI_ctrl_Init(PI_CTRL_s *controller){

    controller->I_previous_f32 = (F32)0.0f;

}

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
     .samp_period__s__f32 = SAMPLING_TIME__s__df32,
     .ref_value_f32 = ( (F32)0.0f ),
     .action_value_f32 = ( (F32)0.0f ),
     .I_previous_f32 = ( (F32)0.0f )
};

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
