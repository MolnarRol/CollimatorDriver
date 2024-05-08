/**
 * @file AC.c
 * @brief Application control module
 * @details Module for controlling application using user interface or external serial communication.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @defgroup AC Application control
 * @{
 */
#include <AC_core.h>
#include <AC_interface.h>
#include <ByteConversions.h>
#include <MDA_interface.h>
#include <MTCL_interface.h>
#include <ATB_interface.h>
#include <FOC.h>

/**
 * @brief Control button initialization.
 */
void AC_ManualControlInit(void)
{
    EALLOW;
    /* GPIO69 - Button1 */
    GpioCtrlRegs.GPCDIR.bit.GPIO69 = 0;     /* Pin as input. */
    GpioCtrlRegs.GPCPUD.bit.GPIO69 = 0;     /* Enable internal pull up. */

    /* GPIO70 - Button2 */
    GpioCtrlRegs.GPCDIR.bit.GPIO70 = 0;     /* Pin as input. */
    GpioCtrlRegs.GPCPUD.bit.GPIO70 = 0;     /* Enable internal pull up. */
    EDIS;
}

AC_BtnDebounce_struct AC_Btn1Debounce_s = {0};
AC_BtnDebounce_struct AC_Btn2Debounce_s = {0};

U16 AC_BtnDebounce_U16(AC_BtnDebounce_struct* debounce_ps, boolean current_state_b)
{
    U16 return_state_U16 = DEBOUNCE_NO_CHANGE_e;

    if(current_state_b != debounce_ps->last_state_b)
    {
        if(current_state_b)
        {
            debounce_ps->rising_edge_ticks_U32 = ATB_GetTicks_U32();
        }
        else
        {
            debounce_ps->falling_edge_ticks_U32 = ATB_GetTicks_U32();
        }
    }

    if(current_state_b && ATB_CheckTicksPassed_U16(debounce_ps->rising_edge_ticks_U32, ATB_MS_TO_TICKS_dM_U32(50)))
    {
        if(debounce_ps->debounced_state_b == False_b)
        {
            debounce_ps->debounced_state_b = True_b;
            return_state_U16 = DEBOUNCE_RISING_EDGE_e;
        }
    }
    else if(!current_state_b && ATB_CheckTicksPassed_U16(debounce_ps->falling_edge_ticks_U32, ATB_MS_TO_TICKS_dM_U32(50)))
    {
        if(debounce_ps->debounced_state_b == True_b)
        {
            debounce_ps->debounced_state_b = False_b;
            return_state_U16 = DEBOUNCE_FALLING_EDGE_e;
        }
    }
    debounce_ps->last_state_b = current_state_b;

    return return_state_U16;
}

void AC_ManualControlHandler(void)
{
    if(AC_BtnDebounce_U16(&AC_Btn1Debounce_s, AC_BTN1_PRESSED_db) == DEBOUNCE_RISING_EDGE_e)
    {
        MTCL_SetReferencePosition(MDA_GetData_ps()->angular_position__rad__F32 + 0.058448f);
    }

    if(AC_BtnDebounce_U16(&AC_Btn2Debounce_s, AC_BTN2_PRESSED_db) == DEBOUNCE_RISING_EDGE_e)
    {
        MTCL_SetReferencePosition(MDA_GetData_ps()->angular_position__rad__F32 - 0.058448f);
    }
}

void AC_ExecuteCommand( const U16 * const command_payload_pU16,
                        const U16 payload_size_U16,
                        U16 * response_data_pU16,
                        U16 * response_data_size_pU16 )
{
    if(AC_CORE_CHECK_INDEX_BOUND_dM_b(command_payload_pU16[0]))
    {
        AC_Functions[command_payload_pU16[0]]( (command_payload_pU16 + 1),
                                              (payload_size_U16 - 1),
                                              response_data_pU16,
                                              response_data_size_pU16 );
    }
    else
    {
        response_data_pU16[0] = INVALID_CMD_e;
        *response_data_size_pU16 = 1;
    }
}

static void AC_CMD_GetMovementParameters( const void* const payload_p,
                                          const U16 payload_size_U16,
                                          U16 * response_data_pU16,
                                          U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 0)
    {
        response_data_pU16[0] = INVALID_INPUT_e;
        *response_data_size_pU16 = 1;
        return;
    }

    /* Local variables. */
    F32 max_speed_F32;
    F32 max_accel_F32;
    F32 max_force_F32;
    MTCL_GetMovementParams(&max_speed_F32, &max_accel_F32, &max_force_F32);

    U32 v1 = (U32)(max_speed_F32 * 1000.0f);
    U32 v2 = (U32)(max_accel_F32 * 1000.0f);
    U32 v3 = (U32)(max_force_F32 * 1000.0f);

    response_data_pU16[0] = RESPONSE_OK_e;
    *response_data_size_pU16 = 13;
    BC_32BitDataTo4Bytes(&v1, &response_data_pU16[1]);
    BC_32BitDataTo4Bytes(&v2, &response_data_pU16[5]);
    BC_32BitDataTo4Bytes(&v3, &response_data_pU16[9]);

}

static void AC_CMD_SetMovementParameters( const void* const payload_p,
                                          const U16 payload_size_U16,
                                          U16 * response_data_pU16,
                                          U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 12)
    {
        response_data_pU16[0] = INVALID_INPUT_e;
        *response_data_size_pU16 = 1;
        return;
    }
    U16* data_pU16 = (U16*)payload_p;

    F32 max_speed_F32 = (F32)BC_4BytesTo32BitData(&data_pU16[0]).val_U32 / 1000.0f;
    F32 max_accel_F32 = (F32)BC_4BytesTo32BitData(&data_pU16[4]).val_U32 / 1000.0f;
    F32 max_force_F32 = (F32)BC_4BytesTo32BitData(&data_pU16[8]).val_U32 / 1000.0f;

    response_data_pU16[0] = MTCL_SetMovementParams(max_speed_F32, max_accel_F32, max_force_F32) ? RESPONSE_OK_e : INVALID_INPUT_e;
    *response_data_size_pU16 = 1;

}

static void AC_CMD_GetMechanicalData( const void* const payload_p,
                                      const U16 payload_size_U16,
                                      U16 * response_data_pU16,
                                      U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 0)
    {
        response_data_pU16[0] = INVALID_INPUT_e;
        *response_data_size_pU16 = 1;
        return;
    }
    *response_data_size_pU16 = 13;
    response_data_pU16[0] = RESPONSE_OK_e;

    const MDA_Data_struct* data_ps = MDA_GetData_ps();

    U32 speed__rad_s2__U32 = (U32)(data_ps->rotor_mech_speed__rad_s1__F32 * 1000.0f);
    U32 linear_position_mm_U32 = (U32)((data_ps->angular_position__rad__F32 / 0.058448f) * 1000.0f);
    U32 rotor_position_rad_U32 = (U32)(data_ps->rotor_mech_angle__rad__F32 * 1000.0f);

    BC_32BitDataTo4Bytes(&speed__rad_s2__U32, &response_data_pU16[1]);
    BC_32BitDataTo4Bytes(&linear_position_mm_U32, &response_data_pU16[5]);
    BC_32BitDataTo4Bytes(&rotor_position_rad_U32, &response_data_pU16[9]);

}

static void AC_CMD_GetElectricalData( const void* const payload_p,
                                      const U16 payload_size_U16,
                                      U16 * response_data_pU16,
                                      U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 0)
    {
        response_data_pU16[0] = INVALID_INPUT_e;
        *response_data_size_pU16 = 1;
        return;
    }
    *response_data_size_pU16 = 13;
    response_data_pU16[0] = RESPONSE_OK_e;

    const MDA_Data_struct* data_ps = MDA_GetData_ps();

    U32 id__A__U32 = (U32)(data_ps->currents_s.id__A__F32 * 1000.0f);
    U32 iq__A__U32 = (U32)(data_ps->currents_s.iq__A__F32 * 1000.0f);
    U32 dc_link_voltage__V__F32 = (U32)(data_ps->dc_link_voltage__V__F32 * 1000.0f);

    BC_32BitDataTo4Bytes(&id__A__U32, &response_data_pU16[1]);
    BC_32BitDataTo4Bytes(&iq__A__U32, &response_data_pU16[5]);
    BC_32BitDataTo4Bytes(&dc_link_voltage__V__F32, &response_data_pU16[9]);

}

static void AC_CMD_SetReferncePosition( const void* const payload_p,
                                        const U16 payload_size_U16,
                                        U16 * response_data_pU16,
                                        U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 4)
    {
        response_data_pU16[0] = INVALID_INPUT_e;
        *response_data_size_pU16 = 1;
        return;
    }

    *response_data_size_pU16 = 1;
    // 0.058448f
    F32 reference_position = (F32)BC_4BytesTo32BitData(&((U16*)payload_p)[0]).val_U32 / 1000.0f;
    reference_position = reference_position * 0.058448f;

    response_data_pU16[0] = MTCL_SetReferencePosition(reference_position) ? RESPONSE_OK_e : INVALID_INPUT_e;
}

static void AC_CMD_GetMaximumPosition( const void* const payload_p,
                                       const U16 payload_size_U16,
                                       U16 * response_data_pU16,
                                       U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 0)
    {
        response_data_pU16[0] = INVALID_INPUT_e;
        *response_data_size_pU16 = 1;
        return;
    }
    response_data_pU16[0] = RESPONSE_OK_e;
    *response_data_size_pU16 = 5;
    U32 max_position_U32 = MTCL_GetMaximumPosition_F32() * 1000.0f / 0.058448f;
    BC_32BitDataTo4Bytes(&max_position_U32, &response_data_pU16[1]);
}

static void AC_CMD_ResetErrorFlags( const void* const payload_p,
                                    const U16 payload_size_U16,
                                    U16 * response_data_pU16,
                                    U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 0)
    {
        response_data_pU16[0] = INVALID_INPUT_e;
        *response_data_size_pU16 = 1;
        return;
    }
    MTCL_ResetErrorFlags();
    response_data_pU16[0] = RESPONSE_OK_e;
    *response_data_size_pU16 = 1;
}

static void AC_CMD_SetMovmentEnableState( const void* const payload_p,
                                          const U16 payload_size_U16,
                                          U16 * response_data_pU16,
                                          U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 1)
    {
        response_data_pU16[0] = INVALID_INPUT_e;
        *response_data_size_pU16 = 1;
        return;
    }
    FOC_SetEnableState(((U16*)payload_p)[0]);
    response_data_pU16[0] = RESPONSE_OK_e;
    *response_data_size_pU16 = 1;
}

/**
 * @}
 */
