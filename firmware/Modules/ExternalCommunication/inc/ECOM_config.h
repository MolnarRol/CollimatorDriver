/**
 * @file ECOM_config.h
 * @brief External communication buffer submodule.
 * @details Manages communication buffers.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @addtogroup ECOM External communication configuration
 * @{
 */

#include <app_types.h>
#define ECOM_BUFFER_SIZE_dU16       ( (U16)64 )
#define ECOM_CRC_ENABLED            ( 1 )
#define ECOM_MSG_TIMEOUT__us__dU16  ( (U16)1000 )

/**
 * @}
 */
