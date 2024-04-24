/*
 * FOC.h
 *
 *  Created on: 4 ар. 2024 г.
 *      Author: vadym
 */

#ifndef MODULES_FIELDORIENTEDCONTROL_INC_FOC_H_
#define MODULES_FIELDORIENTEDCONTROL_INC_FOC_H_

#include "F2837xD_device.h"
#include <main.h>
#include "MDA_interface.h"
#include "PI_Controller.h"
#include <Modules/Miscellaneous/inc/TRAN.h>
#include <PWM_interface.h>
#include <math.h>


#define MAX_SPEED__rad_s__df32                             ( (F32)5.0f )
#define MAX_ACCELERATON__rad_s_2__df32                     ( (F32)1.0f ) // (MAX TORQUE / INERTON)
/* t1 = wmax/a */
#define ACCELERATOIN_TIME__s__df32                         (F32)(MAX_SPEED__rad_s__df32 / MAX_ACCELERATON__rad_s_2__df32)
/* phi1 = 0.5*a*samp_time^2 */
#define DELTA_ACCELERATION_POSITION__rad__df32             (F32)( 0.5 * MAX_ACCELERATON__rad_s_2__df32 * ACCELERATOIN_TIME__s__df32 * ACCELERATOIN_TIME__s__df32 )

/*MOTOR CONSTANT*/
#define MOTOTR_MOMENT_OF_INERTIA__kg_m2__df32              ( (F32)48.0e-6f )
#define MOTOR_TORQUE_CONSTANT__Nm_A__df32                  ( (F32)0.036f ) // kt
#define MOTOR_INDUCTANCE__H__df32                          ( (F32)0.6e-3f )
/* lambda = kt/(1.5*p) */
#define MOTOR_MAGNETIC_FLUX_OF_ROTOR_MAGNTES__Wb__df32     (F32)( 1.414 * (MOTOR_TORQUE_CONSTANT__Nm_A__df32) / ( ( 3.0/2.0 ) * (F32)MOTOR_POLE_PAIRS_dU16 ) )


void FOC_CalculateOutput(F32 ReferencePosition__rad__F32);

void CommutationAlignment();

void Voltage_Limiter(TRAN_struct * const tran_values_s);

#endif /* MODULES_FIELDORIENTEDCONTROL_INC_FOC_H_ */
