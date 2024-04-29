/*
 * dispCtrl.h
 *
 *  Created on: 10 mar. 2024 ã.
 *      Author: vadym
 */

#ifndef DISPCTRL_H_
#define DISPCTRL_H_

#include "F2837xD_device.h"
#include "spi.h"
#include "mcu.h"
#include "app_types.h"

void dispCtrl_vSendInstruction(Uint16 u16RW, Uint16 u16RS, char data);

void dispCtrl_vSendInitInstruction(char data);

void dispCtrl_vInitDisplay();

void dispCtrl_u16PutString(char* pcData);

void dispCtrl_vSetPosition(Uint16 u16PosX, Uint16 u16PosY);

void float_to_char_array(F32 f, char* buffer, U16 precision);
#endif /* DISPCTRL_H_ */
