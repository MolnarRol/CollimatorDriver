/**
 * @file CRC.c
 * @brief Cyclic redundancy checks
 *
 * =================================================================
 * @author Bc. Samuel Fertal
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 30.03.2024
 * @defgroup CRC Cyclic redundancy check
 * @{
 */

/*function for crc calculation*/

#include <app_types.h>
#include <Modules/Miscellaneous/inc/CRC_core.h>


U16 CRC_CrcCalculation_U16(const U16 *Data_U16, U16 DataLength_U16){    // length in 16bitovych chlievikov
    U16 i=0;                                            /*index to for*/

    U16 CRC_CrcResult_U16=0;                            /*initialization of return value variable*/

    /*variable for splitting u16 16bits into 2 u16 8 bits number*/

    U16 second_byte_U16=0;

    while(DataLength_U16--){

        /*U16 Data split*/

        second_byte_U16=(*Data_U16) & 0x00FF;           /*second byte masking*/
        Data_U16++;

        CRC_CrcResult_U16 ^= second_byte_U16;

        for(i = (U16)0; i < (U16)8; i++){
            if(CRC_CrcResult_U16 & 0x80){               /*if CRC aligned perform XOR*/
                CRC_CrcResult_U16 ^= (0x43<<1);         /*Align polynom then XOR*/
            }
            CRC_CrcResult_U16=CRC_CrcResult_U16 << (U16)1;   /*shift result*/
        }

    }

    CRC_CrcResult_U16 = CRC_CrcResult_U16>>(U16)2;      /*Neviem com ale mal to tam*/

    return CRC_CrcResult_U16;
}

U16 CRC_CrcVerificationReceive_U16(const U16 *Data_U16,  U16 DataLength_U16, U16 CRC_CrcCompare_U16){
    U16 CRC_OwnResult_U16 = CRC_CrcCalculation_U16(Data_U16, DataLength_U16);

    if(CRC_OwnResult_U16 == CRC_CrcCompare_U16){
        return 0;
    }
    return 1;

}

/**
 * @}
 */
