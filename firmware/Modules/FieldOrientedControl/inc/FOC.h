/**
 * @file FOC.h
 * @brief Field oriented control
 *
 * =================================================================
 * @author Bc. Samuel Fertal
 * @author Bc. Vadym Holysh
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 02.05.2024
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
#include "dispCtrl.h"


//#define MAX_SPEED__rad_s__df32                             ( (F32)5.0f )
//#define MAX_ACCELERATON__rad_s_2__df32                     ( (F32)1.0f ) // (MAX TORQUE / INERTON)
/* t1 = wmax/a */
#define ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32)                   (F32)(MaxMechSpeed_rad_s1_F32 / MaxAcc_rad_s2_F32)
/* phi1 = 0.5*a*samp_time^2 */
#define DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,acceleration_time)             (F32)( 0.5 * MaxAcc_rad_s2_F32 * acceleration_time * acceleration_time )

/*MOTOR CONSTANT*/
#define MOTOTR_MOMENT_OF_INERTIA__kg_m2__df32              ( (F64)48.0e-6f )
#define MOTOR_TORQUE_CONSTANT__Nm_A__df32                  ( (F32)0.036f ) // kt
#define MOTOR_INDUCTANCE__H__df32                          ( (F32)0.6e-3f )
/* lambda = kt/(1.5*p) */
#define MOTOR_MAGNETIC_FLUX_OF_ROTOR_MAGNTES__Wb__df32     (F32)( 1.414 * (MOTOR_TORQUE_CONSTANT__Nm_A__df32) / ( ( 3.0/2.0 ) * (F32)MOTOR_POLE_PAIRS_dU16 ) )

#define MAX_MOMENT                                         ( (F32)0.1)

void FOC_CalculateOutput(F32 ReferencePosition__rad__F32, F32 MaxMechSpeed_rad_s1_F32, F32 MaxAcc_rad_s2_F32);

void CommutationAlignment();

void Voltage_Limiter(TRAN_struct * const tran_values_s);

static void PC_CalculateData(F32 ReferencePosition__rad__F32, F32 MaxMechSpeed_rad_s1_F32, F32 MaxAcc_rad_s2_F32);

typedef struct
{
    F32 Start_Absolute_Position__rad__F32;
    F32 Ticks__s__F32;
    boolean ticks_enabled;
    struct
    {
        F32 Acceleration__rad_s_2__F32;
        F32  Speed__rad_s__F32;
        F32 Position__rad__F32;
    } tj;

}PC_Data_struct;

const PC_Data_struct* PC_GetData_ps(void);

static inline void PC_Reset_Data(boolean Full_Reset);

void FOC_SetEnableState(boolean new_state_b);
boolean FOC_GetEnableState(void);

#endif /* MODULES_FIELDORIENTEDCONTROL_INC_FOC_H_ */
