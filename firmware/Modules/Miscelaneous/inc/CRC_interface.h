/*
 * CRC_interface.h
 *
 *  Created on: 3. 3. 2024
 *      Author: samof
 */

#ifndef MODULES_CRC_INC_CRC_INTERFACE_H_
#define MODULES_CRC_INC_CRC_INTERFACE_H_

#include <app_types.h>

U16 CRC_CrcCalculation_U16(const U16 *Data_U16,  U16 DataLength_U16);s
U16 CRC_CrcVerificationReceive_U16(const U16 *Data_U16,  U16 DataLength_U16, U16 CRC_CrcCompare_U16);



#endif /* MODULES_CRC_INC_CRC_INTERFACE_H_ */
