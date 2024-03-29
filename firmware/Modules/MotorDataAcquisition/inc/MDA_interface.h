/*
 * MDA_interface.h
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */

#ifndef MODULES_MOTORDATAACQUISITION_INC_MDA_INTERFACE_H_
#define MODULES_MOTORDATAACQUISITION_INC_MDA_INTERFACE_H_
#include <main.h>
#include <MDA_config.h>

typedef struct
{
    F32 dc_link_voltage__V__F32;                /**< DC link voltage. */
    struct
    {
        F32 id__A__F32;                         /**< Current in d axis. */
        F32 iq__A__F32;                         /**< Current in q axis. */
    } currents_s;                               /**< DQ frame currents. */

    /* Motor electrical speed and position. */
    F32 rotor_el_angle__rad__F32;               /**< Rotor electrical angle position. */
    F32 rotor_el_speed__rad_s1__F32;            /**< Rotor electrical speed. */

    /* Motor mechanical speed and position. */
    F32 rotor_mech_angle__rad__F32;             /**< Rotor mechanical angle position. */
    F32 rotor_mech_speed__rad_s1__F32;          /**< Rotor mecjanical speed. */

    /* Linear positioning. */
    F32 linear_position__mm__F32;               /**< Mechanism linear position from origin. */
} MDA_Data_struct;


void MDA_Init(void);
const MDA_Data_struct* MDA_GetData_ps(void);
void MDA_ResetLinearPosition(void);


#endif /* MODULES_MOTORDATAACQUISITION_INC_MDA_INTERFACE_H_ */
