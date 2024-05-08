/**
 * @file CRC.c
 * @brief Cyclic redundancy checks
 *
 * =================================================================
 * @author Bc. Samuel Fertal
 * @author Bc. Roland Molnar
 * =================================================================
 * KEM, FEI, TUKE
 * @date 30.03.2024
 */


#ifndef MODULES_CRC_INC_CRC_INTERFACE_H_
#define MODULES_CRC_INC_CRC_INTERFACE_H_

#include <app_types.h>

#define POLY 0x43

U16 CRC_CrcCalculation_U16(const U16 *Data_U16,  U16 DataLength_U16);
U16 CRC_CrcVerificationReceive_U16(const U16 *Data_U16,  U16 DataLength_U16, U16 CRC_CrcCompare_U16);
U16 CRC8_CCITT_U16(const U16 * const data_pU16, const U16 n_bytes_U16);
boolean CRC8_CCITT_Verify_b(const U16 * const data_pU16, const U16 n_bytes_U16, const U16 expected_crc_U16);

#endif /* MODULES_CRC_INC_CRC_INTERFACE_H_ */
