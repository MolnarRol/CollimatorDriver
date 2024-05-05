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
 */
#include <AC_core.h>
#include <AC_interface.h>
#include <ByteConversions.h>
#include <MDA_interface.h>
#include <ATB_interface.h>

U32 receive_data_U32 = (U32)0;          /* Debug variable */

/* Test parameters - for debug. */
F32 max_speed_F32 = 40.0f;
F32 max_accel_F32 = 20.0f;
F32 max_force_F32 = 0.25f;
F32 refPos_F32 = 0.0f;

S16 test_var = 0;
S16 test_counter = 0;

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
//    static boolean btn1_lstate_b = False_b;
//    static boolean btn1_set_b = False_b;
//    static U32 btn1_rising_edge_ticks_U32 = 0;
//    static U32 btn1_falling_edge_ticks_U32 = 0;

    if(AC_BtnDebounce_U16(&AC_Btn1Debounce_s, AC_BTN1_PRESSED_db) == DEBOUNCE_RISING_EDGE_e)
    {
        test_var += 1;
    }

    if(AC_BtnDebounce_U16(&AC_Btn2Debounce_s, AC_BTN2_PRESSED_db) == DEBOUNCE_RISING_EDGE_e)
    {
        test_var -= 1;
    }


//    if(btn1_state_b != btn1_lstate_b)
//    {
//        if(btn1_state_b)
//        {
//            btn1_rising_edge_ticks_U32 = ATB_GetTicks_U32();
//        }
//        else
//        {
//            btn1_falling_edge_ticks_U32 = ATB_GetTicks_U32();
//            btn1_set_b = False_b;
//        }
//    }
//
//    if(btn1_state_b && ATB_CheckTicksPassed_U16(btn1_rising_edge_ticks_U32, ATB_MS_TO_TICKS_dM_U32(25)))
//    {
//        if(!btn1_set_b)
//        {
//            test_var += 1;
//            btn1_set_b = True_b;
//        }
//    }
//
//    btn1_lstate_b = btn1_state_b;

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

/* Application functions */
static void AC_TestFunction(const void* const payload_p,
                            const U16 payload_size_U16,
                            U16 * response_data_pU16,
                            U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 4)
    {
        return;
    }
    receive_data_U32 = BC_4BytesTo32BitData(payload_p).val_U32;
}

/* TestFunction */
static void AC_AddOne(const void* const payload_p,
                      const U16 payload_size_U16,
                      U16 * response_data_pU16,
                      U16 * response_data_size_pU16)
{
    U16 num;
    /* Check command input validity. */
    if(payload_size_U16 != 2){
        response_data_pU16[0] = INVALID_INPUT_e;
        *response_data_size_pU16 = 1;
        return;
    }
    /* Input is OK -> Respond with data */
    else
    {
        num = BC_2BytesTo16BitData(payload_p).val_U16;
        num += 1;
        response_data_pU16[0] = RESPONSE_OK_e;
        response_data_pU16[1] = num;
        *response_data_size_pU16 = 2;
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

    max_speed_F32 = (F32)BC_4BytesTo32BitData(&data_pU16[0]).val_U32 / 1000.0f;
    max_accel_F32 = (F32)BC_4BytesTo32BitData(&data_pU16[4]).val_U32 / 1000.0f;
    max_force_F32 = (F32)BC_4BytesTo32BitData(&data_pU16[8]).val_U32 / 1000.0f;

    response_data_pU16[0] = RESPONSE_OK_e;
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
    U32 linear_position_mm_U32 = (U32)(data_ps->angular_position__rad__F32 * 1000.0f);
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

    response_data_pU16[0] = RESPONSE_OK_e;
    *response_data_size_pU16 = 1;
    refPos_F32 = (F32)BC_4BytesTo32BitData(&((U16*)payload_p)[0]).val_U32 / 1000.0f;
}

