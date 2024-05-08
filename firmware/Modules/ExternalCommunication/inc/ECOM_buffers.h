/**
 * @file ECOM_buffers.h
 * @brief External communication buffer submodule.
 * @details Manages communication buffers.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @addtogroup ECOM External communication protocol buffers
 * @{
 */
#include <app_types.h>
#include <ECOM_config.h>

#ifndef MODULES_EXTERNALCOMMUNICATION_INC_ECOM_BUFFERS_H_
#define MODULES_EXTERNALCOMMUNICATION_INC_ECOM_BUFFERS_H_

/* Communication buffer structure, */
typedef struct
{
    U16                         data_aU16[ECOM_BUFFER_SIZE_dU16];           /**< Managed byte buffer,  */
    U16                         top_U16;                                    /**< Current buffer top index - for writing. */
    U16                         bottom_U16;                                 /**< Current buffer bottom index - for reading. */
    struct
    {
        U16                     buffer_rdy_flag:1;                            /**< Buffer is prepared. */
        const U16               __reserved: 15;
    }buffer_state_s;
} ECOM_Buffer_struct;

#define ECOM_GET_BUFFER_LEN_dM(buffer_ps)       ( (buffer_ps)->top_U16 - (buffer_ps)->bottom_U16 )

U16     ECOM_WriteBufferToAddress(ECOM_Buffer_struct * const buffer_ps, U16 * const dst_data_pU16, U16 read_size);
void    ECOM_WriteDataToBuffer(ECOM_Buffer_struct * const buffer_ps, const U16 * data_pU16, U16 size_U16);
void    ECOM_ResetBuffer(ECOM_Buffer_struct * const buffer_ps);

#endif /* MODULES_EXTERNALCOMMUNICATION_INC_ECOM_BUFFERS_H_ */

/**
 * @]
 */
