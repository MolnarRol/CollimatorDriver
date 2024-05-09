/**
 * @file MTCL.c
 *
 * @section MTCL MTCL Motor control module
 * @brief Motor control module
 * @details Module for controlling actual motor data and caluclating motor desired data
 * =================================================================
 * @author Bc. Roland Molnar
 * @author Bc. Vadym Holysh
 * @author Bc. Samuel Fertal
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @defgroup MTCL Motor control module
 * @{
 */

#include <MTCL_core.h>
#include <FOC.h>
#include <ATB_interface.h>
#include <TEST.h>

U16 i = 0;

MTCL_Control_struct s_MTCL_Control_s        = {0,0,0,1,0,0};                                                        /**< Motor control struct. */
MTCL_TorqueCheck_struct s_Torque_check_s    = {0};                                                                  /**< Motor torque error check struct. */
PC_Data_struct s_PC_data_s                  = {0};                                                                  /**< Motor trajectory data struct. */

F32 s_MTCL_ReferencePosition__rad__F32      = 0.0f;                                                                 /**< User requested position. */
F32 s_MTCL_MaxSpeed__rad_s__F32             = DEFAULT_RUN_SPEED__rad_s__dF32;                                       /**< Current value of maximum speed. */
F32 s_MTCL_MaxAccel__rad_s2__F32            = DEFAULT_RUN_ACCEL__rad_s2__dF32;                                      /**< Current value of maximum acceleration. */
F32 s_MTCL_MaxTorque__Nm__F32               = DEFAULT_RUN_TORQUE__Nm__dF32;                                         /**< Current value of maximum Torque. - not used. */

const F32 s_MTCL_MaxPosition__rad__F32      = 5.8448f;  // 10 cm in rad                                             /**< Maximum position for valid request. */
F32 prev_request_pos__F32__                 = 0.0f;                                                                 /**< Previously requested position. */                                                                                                          /**< Debug variable. */

MTCL_HomingState_enum MTCL_HomingState_e    = MTLC_HOMING_IDLE_e;                                                   /**< Homing related variables. - not used. */

/**
 * @brief Main handling function for motor control
 * @details Manages trajectory calculation and FOC requests.
 */
void MTCL_MainHandler(void)
{
    F32 reference_position__rad__F32 = s_MTCL_ReferencePosition__rad__F32;
    if(MTCL_HomingState_e != MTCL_HOMING_COMPLETE_e)                                                                /* Not used. */
    {
        s_MTCL_Control_s.motor_homed_f1 = 1;
        MTCL_HomingState_e = MTCL_HOMING_COMPLETE_e;
    }
    s_MTCL_Control_s.movement_enabled_f1 = FOC_GetEnableState();

    if(s_MTCL_Control_s.tracking_to_zero == 1)
    {
        reference_position__rad__F32 = 0.0f;

        if( (MDA_GetData_ps()->angular_position__rad__F32) < 0.00001f)
        {
            s_Torque_check_s.error_state_torque_exceed_counter_U16++;
            if(s_Torque_check_s.error_state_torque_exceed_counter_U16 == 2000)
            {
                s_MTCL_Control_s.tracking_to_zero = 0;
                FOC_SetEnableState(False_b);
                PC_Reset_Data(0);
                s_MTCL_ReferencePosition__rad__F32 = MDA_GetData_ps()->angular_position__rad__F32;
                s_Torque_check_s.error_state_torque_exceed_counter_U16 = 0;
            }
        }
    }

    MTCL_CalculateTrajectory(reference_position__rad__F32, s_MTCL_MaxSpeed__rad_s__F32, s_MTCL_MaxAccel__rad_s2__F32);
    FOC_CalculateOutput(&s_PC_data_s);
    MTCL_TorqueExceedCheck();
}

/**
 * @brief GPIO init function for end switches
 */
void MTCL_Init(void)
{
    EALLOW;
    /* End stop switch pin configuration. */

    /* GPIO85 - Zero position switch. */
    GpioCtrlRegs.GPCDIR.bit.GPIO85 = (U16)0;
    GpioCtrlRegs.GPCPUD.bit.GPIO85 = (U16)0;

    /* GPIO86 - Max position switch. */
    GpioCtrlRegs.GPCDIR.bit.GPIO86 = (U16)0;
    GpioCtrlRegs.GPCPUD.bit.GPIO86 = (U16)0;
    EDIS;
}

/**
 * @brief Motor end switch homing routine
 * @deprecated
 */
inline void MTCL_Homing(F32 * requested_position_pF32)
{
    static F32 s_max_angular_position__rad__F32;
    static F32 s_time_tick_F32;

    switch(MTCL_HomingState_e)
    {
        case MTLC_HOMING_IDLE_e:
            s_MTCL_MaxSpeed__rad_s__F32 = HOMING_SPEED__rad_s__dF32;            /* Set homing max speed. */
            s_MTCL_MaxAccel__rad_s2__F32 = HOMING_ACCEL__rad_s__dF32;           /* Set homing max acceleration. */
            MTCL_HomingState_e = MTCL_HOMING_MAX_e;                             /* Go to the next state. */

        case MTCL_HOMING_MAX_e:
            *requested_position_pF32 = 1.0e6f;                                  /* Overriding requested position. */
            if(END_SWITCH_MAX_STATE_db == True_b)                               /* Maximum end switch is activated. */
            {
                s_max_angular_position__rad__F32 =
                        MDA_GetData_ps()->angular_position__rad__F32;           /* Saving temporary maximum position. */
                FOC_SetEnableState(False_b);                                    /* Immediately stop FOC - stops motor. */
                s_time_tick_F32 = ATB_GetTicks_U32();                           /* Saving reference time for delay. */
                PC_Reset_Data(True_b);
                MTCL_HomingState_e = MTCL_HOMING_MAX_COMPLETE_e;                /* Go to the next state. */
            }
            break;

        case MTCL_HOMING_MAX_COMPLETE_e:
            /* Delay */
            if(ATB_CheckTicksPassed_U16(s_time_tick_F32, HOMING_DELAT_TICKS_dU32) == (U16)1)
            {
                FOC_SetEnableState(True_b);
                MTCL_HomingState_e = MTCL_HOMING_ZERO_e;
            }
            break;

        case MTCL_HOMING_ZERO_e:
            *requested_position_pF32 = -1.0e6f;                                 /* Overriding requested position. */
            if(END_SWITCH_ZERO_STATE_db == True_b)
            {
                *(F32*)(&s_MTCL_MaxPosition__rad__F32) =
                        s_max_angular_position__rad__F32
                        - MDA_GetData_ps()->angular_position__rad__F32;
                *(F32*)(&MDA_GetData_ps()->angular_position__rad__F32) = 0.0f;  /* Reseting angular position to 0. */

                FOC_SetEnableState(False_b);                                    /* Immediately stop FOC - stops motor. */
                s_time_tick_F32 = ATB_GetTicks_U32();                           /* Saving reference time for delay. */
                PC_Reset_Data(True_b);
                MTCL_HomingState_e = MTCL_HOMING_MAX_COMPLETE_e;                /* Go to the next state. */
            }
            break;

        case MTCL_HOMING_ZERO_COMPLETE_e:
            /* Delay */
            if(ATB_CheckTicksPassed_U16(s_time_tick_F32, HOMING_DELAT_TICKS_dU32) == (U16)1)
            {
                FOC_SetEnableState(True_b);
                MTCL_HomingState_e = MTCL_HOMING_COMPLETE_e;
            }
            break;

        case MTCL_HOMING_COMPLETE_e:
            s_MTCL_MaxSpeed__rad_s__F32 = DEFAULT_RUN_SPEED__rad_s__dF32;
            s_MTCL_MaxAccel__rad_s2__F32 = DEFAULT_RUN_ACCEL__rad_s2__dF32;
            s_MTCL_Control_s.motor_homed_f1 = 1;
            break;

        default:
            MTCL_HomingState_e = MTLC_HOMING_IDLE_e;
            break;
    }
}

/**
 * @brief Function for calculating and generating desired trajectory
 * @param Requested_Position__rad__F32 Desired position according to user linear input, calculated from GUI or button input
 * @param MaxMechSpeed_rad_s1_F32 Maximal possible reachable speed according to user input from GUI in radians per second,
 * @param MaxAcc_rad_s2_F32 Acceleration value, which motor accelerates or deccelaretes with, in radians per second^2
 * @details The function is responsible for generating desired trajectory based on discrete integration each interrupt cycle, considers possible mathematical
 * scenarios and calculates fastest way to reach desired position according to user inputs
 * */
static void MTCL_CalculateTrajectory(F32 Requested_Position__rad__F32, F32 MaxMechSpeed_rad_s1_F32, F32 MaxAcc_rad_s2_F32)
{
    static F32 DeltaMdlPosition__rad__F32 = 0.0f;
    static F32 DeltaMdlTime__s__F32 = 0.0f;
    static S16 Minus_Check = 0;
    static F32 FullTime__s__F32 = 0.0f;
    static boolean roof = False_b;

    F32 start_ramp_rad = DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32));

    if(s_PC_data_s.Ticks__s__F32 == 0)
    {
        if(Requested_Position__rad__F32 - prev_request_pos__F32__> 0.01f || Requested_Position__rad__F32 - prev_request_pos__F32__ < -0.01f)
        {
            s_PC_data_s.ticks_enabled = 1;
            prev_request_pos__F32__= Requested_Position__rad__F32;
            s_PC_data_s.Start_Absolute_Position__rad__F32 = MDA_GetData_ps()->angular_position__rad__F32;
            s_PC_data_s.tj.Position__rad__F32 = 0.0f; //new line

                if(Requested_Position__rad__F32 > MDA_GetData_ps()->angular_position__rad__F32)
                {
                    DeltaMdlPosition__rad__F32 = (Requested_Position__rad__F32 - MDA_GetData_ps()->angular_position__rad__F32) - 2.0f * DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32));
                    DeltaMdlTime__s__F32 = DeltaMdlPosition__rad__F32 / MaxMechSpeed_rad_s1_F32;
                    Minus_Check = 1;
                }
                else
                {
                    DeltaMdlPosition__rad__F32 = (Requested_Position__rad__F32 - MDA_GetData_ps()->angular_position__rad__F32) + 2.0f * DELTA_ACCELERATION_POSITION__rad__df32(MaxAcc_rad_s2_F32,ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32));
                    DeltaMdlTime__s__F32 = -DeltaMdlPosition__rad__F32 / MaxMechSpeed_rad_s1_F32;
                    Minus_Check = -1;
                }
        }
        /*override delta mid time*/
        if(DeltaMdlTime__s__F32 > 0.0f)
        {
            roof = False_b;
            FullTime__s__F32 = ( 2.0f * ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) ) + DeltaMdlTime__s__F32;
        }
        else
        {
            roof = True_b;
            FullTime__s__F32 = 2.0f*FM_sqrt_F32((Minus_Check*(Requested_Position__rad__F32 - MDA_GetData_ps()->angular_position__rad__F32))/MaxAcc_rad_s2_F32);
        }
    }
    /*new lines*/
    if(s_PC_data_s.ticks_enabled)
    {
        s_PC_data_s.Ticks__s__F32 += SAMPLING_TIME__s__df32;
    }
    /*end of new lines*/

    /*not roof*/
    if(roof == 0)
    {
        if( s_PC_data_s.Ticks__s__F32 <= ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32)
            && s_PC_data_s.Ticks__s__F32 > 0.0f)
        {
            s_PC_data_s.tj.Acceleration__rad_s_2__F32 = MaxAcc_rad_s2_F32 * Minus_Check;
            s_PC_data_s.tj.Speed__rad_s__F32 += s_PC_data_s.tj.Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;
            s_PC_data_s.tj.Position__rad__F32 += s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;
        }

        else if( ( s_PC_data_s.Ticks__s__F32 > ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) )
                && s_PC_data_s.Ticks__s__F32 <= ( ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) + DeltaMdlTime__s__F32 ) ) //maybe bug
        {
            s_PC_data_s.tj.Acceleration__rad_s_2__F32 = 0.0f;
            s_PC_data_s.tj.Speed__rad_s__F32 = MaxMechSpeed_rad_s1_F32 * Minus_Check;                            // Opytat sa
            s_PC_data_s.tj.Position__rad__F32 += (F32)( s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32 );
        }

        else if ( ( s_PC_data_s.Ticks__s__F32 > ( ACCELERATOIN_TIME__s__df32(MaxMechSpeed_rad_s1_F32,MaxAcc_rad_s2_F32) + DeltaMdlTime__s__F32 ) ) //maybe bug
                && ( s_PC_data_s.Ticks__s__F32 <= FullTime__s__F32 ) )
        {
            s_PC_data_s.tj.Acceleration__rad_s_2__F32 = -MaxAcc_rad_s2_F32 * Minus_Check;
            s_PC_data_s.tj.Speed__rad_s__F32 += s_PC_data_s.tj.Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;

            if(s_PC_data_s.tj.Speed__rad_s__F32 * Minus_Check < 0.0f)
            {
                s_PC_data_s.tj.Speed__rad_s__F32 = 0.0f;
            }

            s_PC_data_s.tj.Position__rad__F32 += s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32;        //0.5 * Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 * SAMPLING_TIME__s__df32;

            if(s_PC_data_s.tj.Position__rad__F32*Minus_Check >= 2.0f * start_ramp_rad + Minus_Check*DeltaMdlPosition__rad__F32)
            {
                s_PC_data_s.tj.Position__rad__F32 = Minus_Check * 2.0f * start_ramp_rad + DeltaMdlPosition__rad__F32;
            }
        }
    }
    /*end not roof*/

    /*roof*/
    if(roof == 1)
    {
        if( s_PC_data_s.Ticks__s__F32 <= (FullTime__s__F32/2.0f) && s_PC_data_s.Ticks__s__F32 > 0.0f)
        {
            s_PC_data_s.tj.Acceleration__rad_s_2__F32 = MaxAcc_rad_s2_F32 * Minus_Check;
            s_PC_data_s.tj.Speed__rad_s__F32 += s_PC_data_s.tj.Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;
            s_PC_data_s.tj.Position__rad__F32 += s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32;
        }
        else if ( ( s_PC_data_s.Ticks__s__F32 > ( FullTime__s__F32/2.0f ) ) && ( s_PC_data_s.Ticks__s__F32 <= FullTime__s__F32 ) )
        {
            s_PC_data_s.tj.Acceleration__rad_s_2__F32 = -MaxAcc_rad_s2_F32 * Minus_Check;
            s_PC_data_s.tj.Speed__rad_s__F32 += s_PC_data_s.tj.Acceleration__rad_s_2__F32 * SAMPLING_TIME__s__df32 ;
            s_PC_data_s.tj.Position__rad__F32 += s_PC_data_s.tj.Speed__rad_s__F32 * SAMPLING_TIME__s__df32;

            if(s_PC_data_s.tj.Position__rad__F32*Minus_Check >= 2.0f*start_ramp_rad + Minus_Check*DeltaMdlPosition__rad__F32)
            {
                s_PC_data_s.tj.Position__rad__F32 = Minus_Check*2.0f*start_ramp_rad + DeltaMdlPosition__rad__F32;
                s_PC_data_s.tj.Speed__rad_s__F32 = 0.0f;
                s_PC_data_s.tj.Acceleration__rad_s_2__F32 = 0.0f;
            }
        }
    }
    /*end roof*/

    if(s_PC_data_s.Ticks__s__F32 > FullTime__s__F32)
    {
        if(s_PC_data_s.tj.Position__rad__F32*Minus_Check < 2.0f*start_ramp_rad + Minus_Check*DeltaMdlPosition__rad__F32)
        {
            s_PC_data_s.tj.Position__rad__F32 = Minus_Check*2.0f*start_ramp_rad + DeltaMdlPosition__rad__F32;
        }
        PC_Reset_Data(False_b);
    }

    i++;
    if(i > 40){
        if(s_PC_data_s.tj.Speed__rad_s__F32 != 0){
            kukam_prud();
            i=0;
        }
    }
}

/**
 * @brief Function for possible trajectory obstacle checking
 * @details The function is responsible for instant flagging of torque exceeding scenraios which is caused
 * by obstacle in trajectory of blades of mechanism or forced braking or accelerating of mechanism, in case of multiple forced braking,
 * function stops PWM pulses which leads to initial stop of mechanism
 *
 * */
boolean MTCL_TorqueExceedCheck(void)
{
    const F32 motor_torque__Nm__F32 = FOC_GetTorque__Nm__F32();
    if( (motor_torque__Nm__F32 > MAX_MOMENT)
         || ( motor_torque__Nm__F32 < -MAX_MOMENT) )
    {
        s_Torque_check_s.torque_exceed_counter_U16 += (U16)1;
        if( (s_MTCL_Control_s.over_torque_error_f1 == 0)
             && (s_Torque_check_s.torque_exceed_counter_U16 > 3000) )
        {
            s_Torque_check_s.torque_exceed_counter_U16 = 0;
            PC_Reset_Data(1);
            s_MTCL_Control_s.over_torque_error_f1 = 1;
            s_MTCL_Control_s.tracking_to_zero = 1;
        }
        else
        {
            s_Torque_check_s.torque_exceed_counter_U16 += (U16)1;
            if(s_Torque_check_s.torque_exceed_counter_U16 > 10000)
            {
                s_Torque_check_s.torque_exceed_counter_U16 = 0;
                PC_Reset_Data(False_b);
                s_MTCL_Control_s.tracking_to_zero = 0;
                s_MTCL_Control_s.over_torque_error_f2 = 1;
                FOC_SetEnableState(False_b);
                PWM_SetCompareValues(0,0,0);
                s_MTCL_ReferencePosition__rad__F32 = MDA_GetData_ps()->angular_position__rad__F32;
            }
        }
    }
    return True_b;
}

/**
 * @brief Write new movement parameters
 * @param max_speed__rad_s__F32 New maximum speed.
 * @param max_accel__rad_s2__F32 New maximum acceleration.
 * @param max_torque__Nm__F32 New maximum torque.
 * @returns Status
 * @retval 0 Writing failed.
 * @retval 1 Writing was successful.
 */
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

/**
 * @brief get current movement configuration.
 * @param max_speed__rad_s__F32 is a pointer to max speed value to be written.
 * @param max_accel__rad_s2__F32 is a pointer to max acceleration value to be written.
 * @param max_torque__Nm__F32 is a pointer to max torque value to be written.
 */
void MTCL_GetMovementParams(F32 * const max_speed__rad_s__F32, F32 * const max_accel__rad_s2__F32, F32 * const max_torque__Nm__F32)
{
    *max_speed__rad_s__F32 = s_MTCL_MaxSpeed__rad_s__F32;
    *max_accel__rad_s2__F32 = s_MTCL_MaxAccel__rad_s2__F32;
    *max_torque__Nm__F32 = s_MTCL_MaxTorque__Nm__F32;
}

/**
 * @brief Set new reference position
 * @param new_position__rad__F32 is new position in radians.
 * @returns Status
 * @retval 0 Position was not set. Out of defined limits.
 * @retval 1 Position was set.
 */
boolean MTCL_SetReferencePosition(const F32 new_position__rad__F32)
{
    boolean return_state_b = True_b;
    if( (new_position__rad__F32 < 0.0f) || (new_position__rad__F32 > (s_MTCL_MaxPosition__rad__F32 + 0.058448f)) )
    {
        if((new_position__rad__F32 < 0.0f))
        {
        s_MTCL_ReferencePosition__rad__F32 = 0.0f;
        }
        else
        {
        s_MTCL_ReferencePosition__rad__F32 = s_MTCL_MaxPosition__rad__F32;
        }

        return_state_b = False_b;
    }
    else
    {
        s_MTCL_ReferencePosition__rad__F32 = new_position__rad__F32;
    }
    return return_state_b;
}

/**
 * @brief Get trajectory data
 * @returns pointer to trajectory data struct.
 */
inline const PC_Data_struct* PC_GetData_ps(void)
{
    return (const PC_Data_struct*)&s_PC_data_s;
}

/**
 * @brief Reset trajectory data.
 * @param Full_Reset makes the position reset
 */
static inline void PC_Reset_Data(boolean Full_Reset)
{
    s_PC_data_s.Ticks__s__F32 = 0.0f;
    s_PC_data_s.ticks_enabled = False_b;
    s_PC_data_s.tj.Acceleration__rad_s_2__F32 = 0.0f;
    s_PC_data_s.tj.Speed__rad_s__F32 = 0.0f;
    if(Full_Reset) s_PC_data_s.tj.Position__rad__F32 = 0.0f;
}

/**
 * @brief Get motor control struct
 * @returns pointer to structure
 */
inline const MTCL_Control_struct* MTCL_GetControlState_ps(void)
{
    return &s_MTCL_Control_s;
}

/**
 * @brief Get maximum position in radians.
 * @returns Maximum position in radians.
 */
inline F32 MTCL_GetMaximumPosition_F32(void)
{
    return s_MTCL_MaxPosition__rad__F32;
}

/**
 * @brief Resets motor control error flags.
 */
inline void MTCL_ResetErrorFlags(void)
{
    s_MTCL_Control_s.over_torque_error_f1 = 0;
    s_MTCL_Control_s.over_torque_error_f2 = 0;
}

/**
 * @}
 */
