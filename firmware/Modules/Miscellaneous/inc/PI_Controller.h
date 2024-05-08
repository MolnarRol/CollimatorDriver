/**
 * @file PI_Controller.h
 * @brief Controller for motor regulation
 * @details Module made PI controllers for currents, speed and position, calculating output controllers
 *
 * =================================================================
 * @author Bc. Vadym Holysh
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 14.03.2024
 * @addgroup PI Proportional Integration Controller
 * @{
 */

#ifndef MODULES_CONTROLLER_INC_CONTROLLER_H_
#define MODULES_CONTROLLER_INC_CONTROLLER_H_

#include "F2837xD_device.h"
#include <main.h>

#define I_Q_KP_df32            ( (F32)1.2f )     /**< MACRO proportional part for Iq controller*/
#define I_D_KP_df32            ( (F32)1.2f )     /**< MACRO proportional part for Id controller*/
#define SPEED_KP_df32          ( (F32)0.237f )   /**< MACRO proportional part for speed controller*/
#define POSITION_KP_df32       ( (F32)40.0f )    /**< MACRO proportional part for position controller */
#define I_Q_KI_df32            ( (F32)720.0f )   /**< MACRO integration part for Iq controller*/
#define I_D_KI_df32            ( (F32)720.0f )   /**< MACRO integration part for Id controller*/
#define SPEED_KI_df32          ( (F32)4.0f )     /**< MACRO integration part for speed controller*/
#define POSITION_KI_df32       ( (F32)0.0f )     /**< MACRO integration part for position controller*/
#define MAX_VOLTAGE_D__V__df32 ( (F32)12.0f )
#define MIN_VOLTAGE_D__V__df32 ( (F32)-12.0f )
#define MAX_VOLTAGE_Q__V__df32 ( (F32)12.0f )
#define MIN_VOLTAGE_Q__V__df32 ( (F32)-12.0f )
#define MAX_CURRENT_Q__A__df32 ( (F32)15.0f )
#define MIN_CURRENT_Q__A__df32 ( (F32)-15.0f )
#define MAX_SPEED__RPM__df32   ( (F32)475.0f )
#define MIN_SPEED__RPM__df32   ( (F32)-475.0f )
#define SAMPLING_TIME__s__df32 ( (F64)50.0e-6f )              /**< MACRO sampling time for current loop */
#define SAMPLING_TIME_SPEED_REG__s__dF32 ( (F64)250e-6f )     /**< MACRO sampling time for speed loop */


typedef struct {

    struct {
        F32 Kp_f32;              /*Proportional gain*/
        F32 Ki_f32;              /*Integral gain*/
    }gain_s;

    struct {
        F32 out_max_f32;         /*upper value output*/
        F32 out_min_f32;         /*lower value output*/
    }limit_s;

    F32 samp_period__s__f32;     /*sampling time*/
    F32 ref_value_f32;           /*reference value S-curve*/
    F32 action_value_f32;        /*action intervention from previous PI controller*/
    F32 I_previous_f32;          /*previous integral value*/

}PI_CTRL_s;

float PI_ctrl_CalculateOutput(PI_CTRL_s *controller, F32 y_ref_f32);

void PI_ctrl_Init(PI_CTRL_s *controller);

extern PI_CTRL_s PI_id_current_controller;
extern PI_CTRL_s PI_iq_current_controller;
extern PI_CTRL_s PI_speed_controller;
extern PI_CTRL_s PI_position_controller;

#endif /* MODULES_CONTROLLER_INC_CONTROLLER_H_ */

/**
 * @}
 */

