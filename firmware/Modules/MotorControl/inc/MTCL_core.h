/**
 * @file MTCL_core.h
 * @brief Motor control submodule.
 * @details Manages motor position requests and motor control states.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @addtogroup MTCL Motor control core
 * @{
 */

#ifndef MODULES_MOTORCONTROL_INC_MTCL_CORE_H_
#define MODULES_MOTORCONTROL_INC_MTCL_CORE_H_
#include <MTCL_interface.h>
#include <mcu.h>

#define MAX_SPEED__rad_s__dF32                  ( 400.0f )                                              /**< Max motor speed. */
#define MIN_SPEED__rad_s__dF32                  ( 0.1f )                                                /**< Minimum motor speed. */
#define MAX_ACCEL__rad_s2__dF32                 ( 100.0f )                                              /**< Maximum acceleration. */
#define MIN_ACCEL__rad_s2__dF32                 ( 0.1f )                                                /**< Minimum acceleration. */
#define MAX_TORQUE__Nm__dF32                    ( 1.0f )                                                /**< Max torque. */
#define MIN_TORQUE__Nm__dF32                    ( 0.1f )                                                /**< Min torque. */

#define TORQUE_EXCEED_ALARM_DURATION__s__dF32   ( 0.15f )                                               /**< Timeout for torque check. */

#define DEFAULT_RUN_SPEED__rad_s__dF32          ( 2.0f )                                                /**< Default setting for max speed. */
#define DEFAULT_RUN_ACCEL__rad_s2__dF32         ( 50.0f )                                               /**< Default setting for max acceleration. */
#define DEFAULT_RUN_TORQUE__Nm__dF32            ( 0.1f )

/* Homing - not used*/
#define HOMING_SPEED__rad_s__dF32               ( 20.0f )
#define HOMING_ACCEL__rad_s__dF32               ( 20.0f )
#define HOMING_DELAY__ms__dF32                  ( 200.0f )
#define HOMING_DELAT_TICKS_dU32                 ( ATB_MS_TO_TICKS_dM_U32(HOMING_DELAY__ms__dF32) )
#define END_SWITCH_ZERO_STATE_db                ( (boolean)(!GpioDataRegs.GPCDAT.bit.GPIO85) )
#define END_SWITCH_MAX_STATE_db                 ( (boolean)(!GpioDataRegs.GPCDAT.bit.GPIO86) )


typedef struct
{
    U16     torque_exceed_counter_U16;                                                                  /**< Number of PWM cycles of exceeded torque. */
    U16     error_state_torque_exceed_counter_U16;                                                      /**< Number of PWM cycles of exceeded torque. */
} MTCL_TorqueCheck_struct;                                                                              /**< Torque exceed struct. */

typedef enum
{
    MTLC_HOMING_IDLE_e              = 0,
    MTCL_HOMING_MAX_e               = 1,
    MTCL_HOMING_MAX_COMPLETE_e      = 2,
    MTCL_HOMING_ZERO_e              = 3,
    MTCL_HOMING_ZERO_COMPLETE_e     = 4,
    MTCL_HOMING_COMPLETE_e          = 5
} MTCL_HomingState_enum;                                                                                /**< Homing state machine states. */

boolean MTCL_TorqueExceedCheck(void);
static void MTCL_CalculateTrajectory(F32 Requested_Position__rad__F32,
                                     F32 MaxMechSpeed_rad_s1_F32,
                                     F32 MaxAcc_rad_s2_F32);
static void PC_Reset_Data(boolean Full_Reset);
static void MTCL_Homing(F32 * requested_position_pF32);

#endif /* MODULES_MOTORCONTROL_INC_MTCL_CORE_H_ */

/**
 * @}
 */
