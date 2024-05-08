/**
 * @file TRAN.c
 * @brief Vector control transformations
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 07.04.2024
 * @defgroup TRAN Transformaions
 * @{
 */
#include <math.h>
#include <FAST_MATH_FUNC.h>
#include <TRAN.h>
#include <TEST.h>


/**
 * @brief Calculate transformation from ABC to Alpha beta frame.
 * @param tran_values_s transformation struct.
 */
void TRAN_AbcToAlphaBeta(TRAN_struct * const tran_values_s)
{
    tran_values_s->alpha_beta_s.alpha_F32 = tran_values_s->abc_s.a_F32;

    tran_values_s->alpha_beta_s.beta_F32 = 0.577f * tran_values_s->abc_s.b_F32
            - 0.577f * tran_values_s->abc_s.c_F32;
}

/**
 * @brief Calculate transformation from Alpha beta to DQ frame.
 * @param tran_values_s transformation struct.
 */
void TRAN_AlphaBetaToDq(TRAN_struct * const tran_values_s)
{
    const F32 sin_angle_F32 = FM_sin_F32(tran_values_s->angle__rad__F32);                    /* sine of angle */
    const F32 cos_angle_F32 = FM_cos_F32(tran_values_s->angle__rad__F32);                    /* cosine of angle */

    /* d calculation */
    tran_values_s->dq_s.d_F32 = cos_angle_F32 * tran_values_s->alpha_beta_s.alpha_F32
            + sin_angle_F32 * tran_values_s->alpha_beta_s.beta_F32;
    /* q calculation */
    tran_values_s->dq_s.q_F32 = -sin_angle_F32 * tran_values_s->alpha_beta_s.alpha_F32
            + cos_angle_F32 * tran_values_s->alpha_beta_s.beta_F32;
}

/**
 * @brief Calculate transformation from ABC to DQ frame.
 * @param tran_values_s transformation struct.
 */
void TRAN_AbcToDq(TRAN_struct * const tran_values_s)
{
    TRAN_AbcToAlphaBeta(tran_values_s);
    TRAN_AlphaBetaToDq(tran_values_s);
}

/**
 * @brief Calculate transformation from Alpha beta to ABC frame.
 * @param tran_values_s transformation struct.
 */
void TRAN_AlphaBetaToAbc(TRAN_struct * const tran_values_s)
{
    tran_values_s->abc_s.a_F32 = tran_values_s->alpha_beta_s.alpha_F32;
    tran_values_s->abc_s.b_F32 = -0.5f * tran_values_s->alpha_beta_s.alpha_F32
            + 0.866f * tran_values_s->alpha_beta_s.beta_F32;
    tran_values_s->abc_s.c_F32 = -0.5f * tran_values_s->alpha_beta_s.alpha_F32
            - 0.866f * tran_values_s->alpha_beta_s.beta_F32;
}

/**
 * @brief Calculate transformation from DQ to Alpha beta frame.
 * @param tran_values_s transformation struct.
 */
void TRAN_DqToAlphaBeta(TRAN_struct * const tran_values_s)
{
    const F32 sin_angle_F32 = FM_sin_F32(tran_values_s->angle__rad__F32);                    /* sine of angle */
    const F32 cos_angle_F32 = FM_cos_F32(tran_values_s->angle__rad__F32);                    /* cosine of angle */

    tran_values_s->alpha_beta_s.alpha_F32 = cos_angle_F32 * tran_values_s->dq_s.d_F32
            - sin_angle_F32 * tran_values_s->dq_s.q_F32;

    tran_values_s->alpha_beta_s.beta_F32 = sin_angle_F32 * tran_values_s->dq_s.d_F32
            + cos_angle_F32 * tran_values_s->dq_s.q_F32;
}

/**
 * @brief Calculate transformation from DQ to ABC frame.
 * @param tran_values_s transformation struct.
 */
void TRAN_DqToAbc(TRAN_struct * const tran_values_s)
{
    TRAN_DqToAlphaBeta(tran_values_s);
    TRAN_AlphaBetaToAbc(tran_values_s);
}

/**
 * @}
 */
