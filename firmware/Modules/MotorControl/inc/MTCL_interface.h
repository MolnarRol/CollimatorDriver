/*
 * MTCL_interface.h
 *
 *  Created on: May 2, 2024
 *      Author: roland
 */

#ifndef MODULES_MOTORCONTROL_INC_MTCL_INTERFACE_H_
#define MODULES_MOTORCONTROL_INC_MTCL_INTERFACE_H_
#include <app_types.h>

typedef struct
{
    U16 movement_enabled_f1      :1;
    U16 motor_moving_f1          :1;
    U16 motor_control_type       :2;
    U16 over_torque_error_f1     :1;
    U16 __reserved               :11;
} MTCL_Control_struct;

typedef enum
{
    SPEED_CTRL_e        = 0,
    POSITION_CTRL_e     = 1,
} MTCL_ControlType_enum;


#endif /* MODULES_MOTORCONTROL_INC_MTCL_INTERFACE_H_ */
