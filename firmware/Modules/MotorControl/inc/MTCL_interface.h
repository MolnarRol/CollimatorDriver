/**
 * @file MTCL_interface.h
 * @brief Motor control submodule.
 * @details Manages motor position requests and motor control states.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @addtogroup MTCL Motor control interface
 * @{
 */

#ifndef MODULES_MOTORCONTROL_INC_MTCL_INTERFACE_H_
#define MODULES_MOTORCONTROL_INC_MTCL_INTERFACE_H_
#include <app_types.h>

/* Motor control - control struct. */
typedef struct
{
    U16 movement_enabled_f1      :1;            /**< Movement enable state. */
    U16 motor_moving_f1          :1;            /**< Motor movement flag - not used. */
    U16 over_torque_error_f1     :1;            /**< Motor was blocked. */
    U16 motor_homed_f1           :1;            /**< Homing status - not used.  */
    U16 over_torque_error_f2     :1;            /**< Motor was blocked during homing. */
    U16 tracking_to_zero         :1;            /**< Flag for homing after blockage. */
} MTCL_Control_struct;


typedef struct
{
    F32 Start_Absolute_Position__rad__F32;
    F32 Ticks__s__F32;
    boolean ticks_enabled;
    struct
    {
        F32 Acceleration__rad_s_2__F32;
        F32 Speed__rad_s__F32;
        F32 Position__rad__F32;
    }tj;                                        /**< Calculated trajectories. */

}PC_Data_struct;                                /**< Trajectory structure. */

extern F32 s_MTCL_ReferencePosition__rad__F32;
boolean MTCL_SetMovementParams(const F32 max_speed__rad_s__F32, const F32 max_accel__rad_s2__F32, const F32 max_torque__Nm__F32);
void    MTCL_GetMovementParams(F32 * const max_speed__rad_s__F32, F32 * const max_accel__rad_s2__F32, F32 * const max_torque__Nm__F32);
boolean MTCL_SetReferencePosition(const F32 new_position__rad__F32);
void    MTCL_MainHandler(void);
void    MTCL_Init(void);
const   PC_Data_struct* PC_GetData_ps(void);
const   MTCL_Control_struct* MTCL_GetControlState_ps(void);
F32     MTCL_GetMaximumPosition_F32(void);
void    MTCL_ResetErrorFlags(void);

extern MTCL_Control_struct s_MTCL_Control_s;

#endif /* MODULES_MOTORCONTROL_INC_MTCL_INTERFACE_H_ */

/**
 * @}
 */
