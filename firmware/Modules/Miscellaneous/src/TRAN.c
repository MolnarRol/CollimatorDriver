/*
 * TRAN.c
 *
 *  Created on: Mar 14, 2024
 *      Author: roland
 */
#include <math.h>
#include <FAST_MATH_FUNC.h>
#include <TRAN.h>
#include <TEST.h>


/* Direct transformations. */
void TRAN_AbcToAlphaBeta(TRAN_struct * const tran_values_s)
{
    tran_values_s->alpha_beta_s.alpha_F32 = 0.667f * tran_values_s->abc_s.a_F32
            - 0.334f * tran_values_s->abc_s.b_F32
            - 0.334f * tran_values_s->abc_s.c_F32;

    tran_values_s->alpha_beta_s.beta_F32 = 0.577f * tran_values_s->abc_s.b_F32
            - 0.577f * tran_values_s->abc_s.c_F32;
}

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

void TRAN_AbcToDq(TRAN_struct * const tran_values_s)
{
    TRAN_AbcToAlphaBeta(tran_values_s);
    TRAN_AlphaBetaToDq(tran_values_s);
}

/* Inverse transformations */
void TRAN_AlphaBetaToAbc(TRAN_struct * const tran_values_s)
{
    tran_values_s->abc_s.a_F32 = tran_values_s->alpha_beta_s.alpha_F32;
    tran_values_s->abc_s.b_F32 = -0.5f * tran_values_s->alpha_beta_s.alpha_F32
            + 0.866f * tran_values_s->alpha_beta_s.beta_F32;
    tran_values_s->abc_s.c_F32 = -0.5f * tran_values_s->alpha_beta_s.alpha_F32
            - 0.866f * tran_values_s->alpha_beta_s.beta_F32;
}

void TRAN_DqToAlphaBeta(TRAN_struct * const tran_values_s)
{
    const F32 sin_angle_F32 = FM_sin_F32(tran_values_s->angle__rad__F32);                    /* sine of angle */
    const F32 cos_angle_F32 = FM_cos_F32(tran_values_s->angle__rad__F32);                    /* cosine of angle */

    tran_values_s->alpha_beta_s.alpha_F32 = cos_angle_F32 * tran_values_s->dq_s.d_F32
            - sin_angle_F32 * tran_values_s->dq_s.q_F32;

    tran_values_s->alpha_beta_s.beta_F32 = sin_angle_F32 * tran_values_s->dq_s.d_F32
            - cos_angle_F32 * tran_values_s->dq_s.q_F32;
}

void TRAN_DqToAbc(TRAN_struct * const tran_values_s)
{
    TRAN_DqToAlphaBeta(tran_values_s);
    TRAN_AlphaBetaToAbc(tran_values_s);
}

