/**
 * @file FOC.h
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
 */

#ifndef MODULES_FIELDORIENTEDCONTROL_INC_FOC_H_
#define MODULES_FIELDORIENTEDCONTROL_INC_FOC_H_

#include <MDA_interface.h>
#include <PI_Controller.h>
#include <TRAN.h>
#include <PWM_interface.h>
#include <FAST_MATH_FUNC.h>
#include <MTCL_interface.h>

/* t1 = wmax/a */
#define ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32)                   (F32)(MaxMechSpeed_rad_s1_F32 / MaxAcc_rad_s2_F32)    /**< MACRO function for calculating acceleration time */

/* phi1 = 0.5*a*samp_time^2 */
#define DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,acceleration_time)             (F32)( 0.5 * MaxAcc_rad_s2_F32 * acceleration_time * acceleration_time )  /**< MACRO function for calculating length of trajectory passed during acceleration or decceleration */


/*MOTOR CONSTANT*/
#define MOTOTR_MOMENT_OF_INERTIA__kg_m2__df32              ( (F32)48.0e-6f )            /**< Motor moment of inertia value */
#define MOTOR_TORQUE_CONSTANT__Nm_A__df32                  ( (F32)0.036f )              /**< Motor torque constant value */
#define MOTOR_INDUCTANCE__H__df32                          ( (F32)0.6e-3f )             /**< Motor inductance value */

/* lambda = kt/(1.5*p) */
#define MOTOR_MAGNETIC_FLUX_OF_ROTOR_MAGNTES__Wb__df32     (F32)( 1.414 * (MOTOR_TORQUE_CONSTANT__Nm_A__df32) / ( ( 3.0/2.0 ) * (F32)MOTOR_POLE_PAIRS_dU16 ) ) /**< MACRO function for calculating magnetic flux value */


#define MAX_MOMENT                                         ( (F32)0.1)                  /** < Threshold value of Moment for safety stop */

void    FOC_CalculateOutput(const PC_Data_struct* trajectory_data_ps);
void    FOC_VoltageLimiter(TRAN_struct * const tran_values_s);
void    FOC_CommutationAlignment();
void    FOC_SetEnableState(boolean new_state_b);
boolean FOC_GetEnableState(void);
F32     FOC_GetTorque__Nm__F32(void);

#endif /* MODULES_FIELDORIENTEDCONTROL_INC_FOC_H_ */
