/*
 * MTCL_core.h
 *
 *  Created on: May 2, 2024
 *      Author: roland
 */

#ifndef MODULES_MOTORCONTROL_INC_MTCL_CORE_H_
#define MODULES_MOTORCONTROL_INC_MTCL_CORE_H_
#include <MTCL_interface.h>

#define MAX_SPEED__rad_s__dF32                  ( 200.0f )
#define MIN_SPEED__rad_s__dF32                  ( 20.0f )
#define MAX_ACCEL__rad_s2__dF32                 ( 80.0f )
#define MIN_ACCEL__rad_s2__dF32                 ( 10.0f )
#define MAX_TORQUE__Nm__dF32                    ( 1.0f )
#define MIN_TORQUE__Nm__dF32                    ( 0.1f )

#define TORQUE_EXCEED_ALARM_DURATION__s__dF32   ( 0.1f )


#endif /* MODULES_MOTORCONTROL_INC_MTCL_CORE_H_ */
