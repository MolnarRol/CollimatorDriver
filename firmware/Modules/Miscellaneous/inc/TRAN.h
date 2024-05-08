/**
 * @file TRAN.h
 * @brief Vector control transformations
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 07.04.2024
 * @addtogroup TRAN Transformaion declarations
 * @{
 */

#ifndef MODULES_MISCELANEOUS_INC_TRAN_H_
#define MODULES_MISCELANEOUS_INCTRAN_H_
#include <app_types.h>

#define TRAN_DEG_TO_RAD_dMF32(deg)  ( (F32)deg * (F32)0.017453292519943295 )        /* deg * (pi / 180.0) */

/* Transformation struct. */
typedef struct
{
    struct
    {
        F32 a_F32;
        F32 b_F32;
        F32 c_F32;
    } abc_s;                            /**< Abc frame variable struct. */

    struct
    {
        F32 alpha_F32;
        F32 beta_F32;
    } alpha_beta_s;                     /**< Alpha beta frame variable struct. */

    struct
    {
        F32 d_F32;
        F32 q_F32;
    } dq_s;                             /**< Dq frame varible struct. */

    F32 angle__rad__F32;                /**< Transformation angle. */
} TRAN_struct;

/* Direct transformations. */
void TRAN_AbcToAlphaBeta(TRAN_struct * const tran_values_s);
void TRAN_AlphaBetaToDq(TRAN_struct * const tran_values_s);
void TRAN_AbcToDq(TRAN_struct * const tran_values_s);

/* Inverse transformations */
void TRAN_AlphaBetaToAbc(TRAN_struct * const tran_values_s);
void TRAN_DqToAlphaBeta(TRAN_struct * const tran_values_s);
void TRAN_DqToAbc(TRAN_struct * const tran_values_s);

#endif /* MODULES_MISCELANEOUS_INC_TRAN_H_ */

/**
 * @}
 */
