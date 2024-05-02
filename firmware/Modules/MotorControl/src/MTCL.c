/*
 * MTCL.c
 *
 *  Created on: May 2, 2024
 *      Author: roland
 */

#include <MTCL_core.h>

static MTCL_Control_struct s_MTCL_Control_s = {0};

F32 s_MTCL_MaxSpeed__rad_s__F32 = 80.0f;
F32 s_MTCL_MaxAccel__rad_s2__F32 = 20.0f;
F32 s_MTCL_MaxTorque__Nm__F32 = 1.0f;


boolean MTCL_SetMovementParams(const F32 max_speed__rad_s__F32, const F32 max_accel__rad_s2__F32, const F32 max_torque__Nm__F32)
{
    boolean return_state_b = True_b;
    if( (max_speed__rad_s__F32 > MAX_SPEED__rad_s__dF32) || (max_speed__rad_s__F32 < MIN_SPEED__rad_s__dF32)
        || (max_accel__rad_s2__F32 > MAX_ACCEL__rad_s2__dF32) || (max_accel__rad_s2__F32 < MIN_ACCEL__rad_s2__dF32)
        || (max_torque__Nm__F32 > MAX_TORQUE__Nm__dF32) || (max_torque__Nm__F32 < MIN_TORQUE__Nm__dF32))
    {
        return_state_b = False_b;
    }
    else
    {
        s_MTCL_MaxSpeed__rad_s__F32 = max_speed__rad_s__F32;
        s_MTCL_MaxAccel__rad_s2__F32 = max_accel__rad_s2__F32;
        s_MTCL_MaxTorque__Nm__F32 = max_torque__Nm__F32;
    }

    return return_state_b;
}

void MTCL_GetMovementParams(F32 * const max_speed__rad_s__F32, F32 * const max_accel__rad_s2__F32, F32 * const max_torque__Nm__F32)
{
    *max_speed__rad_s__F32 = s_MTCL_MaxSpeed__rad_s__F32;
    *max_accel__rad_s2__F32 = s_MTCL_MaxAccel__rad_s2__F32;
    *max_torque__Nm__F32 = s_MTCL_MaxTorque__Nm__F32;
}
