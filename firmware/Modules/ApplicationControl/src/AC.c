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


U32 receive_data_U32 = (U32)0;          /* Debug variable */

/* Test parameters - for debug. */
F32 max_speed_F32 = 40.0f;
F32 max_accel_F32 = 20.0f;
F32 max_force_F32 = 0.25f;
F32 refPos_F32 = 0.0f;

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
    U32 linear_position_mm_U32 = (U32)(data_ps->linear_position__mm__F32 * 1000.0f);
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

