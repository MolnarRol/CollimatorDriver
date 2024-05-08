/**
 * @file AC_core.h
 * @brief Application control module
 * @details Module for controlling application using user interface or external serial communication.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @addtogroup AC Application control core
 * @{
 */
#include <app_types.h>

#ifndef AC_CORE_H_
#define AC_CORE_H_

#define AC_CORE_CHECK_INDEX_BOUND_dM_b(idx)   ( idx < (sizeof(AC_Functions) / sizeof(AC_ControlFunction_pF)) )

#define AC_BTN1_PRESSED_db          ( (boolean)!GpioDataRegs.GPCDAT.bit.GPIO70 )    /**< Check if button 1 is pressed. */
#define AC_BTN2_PRESSED_db          ( (boolean)!GpioDataRegs.GPCDAT.bit.GPIO69 )    /**< Check if button 1 is pressed. */

/* Button debouncing structure. */
typedef struct
{
    U32 rising_edge_ticks_U32;
    U32 falling_edge_ticks_U32;
    boolean last_state_b;
    boolean debounced_state_b;
} AC_BtnDebounce_struct;

/* Debounced button state. */
typedef enum
{
    DEBOUNCE_NO_CHANGE_e    = 0,
    DEBOUNCE_RISING_EDGE_e  = 1,
    DEBOUNCE_FALLING_EDGE_e = 2
} AC_BtnDebouncedState_enum;

U16 AC_BtnDebounce_U16(AC_BtnDebounce_struct* debounce_ps, boolean current_state_b);

typedef void (*AC_ControlFunction_pF)(const void* const, const U16, U16*, U16*);

/* External communication responses. */
typedef enum
{
    RESPONSE_OK_e   = 0,
    INVALID_CMD_e   = 1,
    INVALID_INPUT_e = 2,
    ERROR_e         = 3
} ControlFunctionResponseStatus_enum;

static void AC_CMD_GetMovementParameters( const void* const payload_p,
                                          const U16 payload_size_U16,
                                          U16 * response_data_pU16,
                                          U16 * response_data_size_pU16);

static void AC_CMD_SetMovementParameters( const void* const payload_p,
                                          const U16 payload_size_U16,
                                          U16 * response_data_pU16,
                                          U16 * response_data_size_pU16);

static void AC_CMD_SetReferncePosition( const void* const payload_p,
                                        const U16 payload_size_U16,
                                        U16 * response_data_pU16,
                                        U16 * response_data_size_pU16);

static void AC_CMD_GetMechanicalData( const void* const payload_p,
                                      const U16 payload_size_U16,
                                      U16 * response_data_pU16,
                                      U16 * response_data_size_pU16);

static void AC_CMD_GetElectricalData( const void* const payload_p,
                                      const U16 payload_size_U16,
                                      U16 * response_data_pU16,
                                      U16 * response_data_size_pU16);

static void AC_CMD_GetMaximumPosition( const void* const payload_p,
                                       const U16 payload_size_U16,
                                       U16 * response_data_pU16,
                                       U16 * response_data_size_pU16);

static void AC_CMD_ResetErrorFlags( const void* const payload_p,
                                    const U16 payload_size_U16,
                                    U16 * response_data_pU16,
                                    U16 * response_data_size_pU16);

static void AC_CMD_SetMovmentEnableState( const void* const payload_p,
                                          const U16 payload_size_U16,
                                          U16 * response_data_pU16,
                                          U16 * response_data_size_pU16);

static AC_ControlFunction_pF AC_Functions[] =
{
     &AC_CMD_GetMovementParameters,
     &AC_CMD_SetMovementParameters,
     &AC_CMD_SetReferncePosition,
     &AC_CMD_GetMechanicalData,
     &AC_CMD_GetElectricalData,
     &AC_CMD_GetMaximumPosition,
     &AC_CMD_ResetErrorFlags,
     &AC_CMD_SetMovmentEnableState
};


#endif /* AC_CORE_H_ */

/**
 * @}
 */
