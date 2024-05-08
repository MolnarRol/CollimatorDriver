/**
 * @file dispCtrl.c
 * @brief LCD display control using SPI communication
 * @details Instruction for display DOGS164-A
 *
 * =================================================================
 * @author Bc. Vadym Holysh
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 10.03.2024
 * @defgroup dispCtrl Display control
 * @{
 */

#include "dispCtrl.h"
#include "MTCL_interface.h"
#include "FOC.h"

 char buffer[12] = {};                 /**< Buffer for message which send*/
 U16 states = 0;                       /**< States display value*/
 U16 f2_error_display_state_U16 = 0;   /**< State indicator for second over torque state*/

 /**
  * @brief Reverse data byte according with display manual.
  * @param Byte.
  * @return Reverse Byte
  */
 unsigned char reverse(unsigned char b) {
     b = (b * 0x0202020202ULL & 0x010884422010ULL) % 1023;
   return b;
}

 /**
  * @brief Sending instruction to display.
  * @param 1 - Read, 0 - Write.
  * @param 1 - Configuration, 0 - Instruction.
  * @param Data
  */
void dispCtrl_vSendInstruction(Uint16 u16RW, Uint16 u16RS, char cData){

    char packet_3_byte [4];
    char reverse_data = reverse(cData);
    packet_3_byte[0] = (0xF8^(u16RW << 2)^(u16RS << 1));
    packet_3_byte[1] = (reverse_data & 0xF0);
    packet_3_byte[2] = (reverse_data & 0x0F) << 4;

    spi_u16SendData(packet_3_byte,3);
}

/**
 * @brief Sending instruction to display without head.
 * @param Data
 */
void dispCtrl_vSendInitInstruction(char data){

    char packet_3_byte [3];
    char reverse_data = reverse(data);
    packet_3_byte[0] = (reverse_data & 0xF0);
    packet_3_byte[1] = (reverse_data & 0x0F) << 4;

    spi_u16SendData(packet_3_byte,2);
}

/**
 * @brief Initialization display.
 * @details Send instructions for configuration
 */
void dispCtrl_vInitDisplay(void){

    DELAY_US(7000);
    /* set RESET pin for display*/

    GpioCtrlRegs.GPCGMUX1.bit.GPIO72 = 1;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO72 = 0;
    GpioCtrlRegs.GPCDIR.bit.GPIO72 = 1;
    GpioDataRegs.GPCCLEAR.bit.GPIO72 = 1;

    GpioCtrlRegs.GPAGMUX2.bit.GPIO30 = 1;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO30 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO30 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;
    GpioDataRegs.GPASET.bit.GPIO30 = 1;

    DELAY_US(12000);
    GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;
    DELAY_US(2000);

    //spi_vSendChar( (0xF8 ^ ( (char)0 << 2 ) ^ ( (char)0 << 1) ) ); /*start byte - 5 high,RW,RS*/


    dispCtrl_vSendInstruction(0,0,(char)0x3A); /*8 bit length*/
    DELAY_US(1000);
//    dispCtrl_vSendInstruction(0,0,(char)0x3F); /*normal display*/
//    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x09); /*4 line display 0x09*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x06); /*bottom view*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x1E); /*Bias setting BS = 1*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x39); /*set instruction*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x1B); /*internal OSC*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x6C); /*divider on*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x54); /*booster on 56 */
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x72); /*set contrast 7A*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x38); /*set instruction*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x0F); /*display on, cursor on, blink on*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x01); /*clear display*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x80); /*set DDRAM on position 0:0*/
    DELAY_US(1000);

    DELAY_US(55000);

}

/**
 * @brief Write string on display.
 * @param Address to char array.
 */
void dispCtrl_u16PutString(char* pcData){

    Uint16 i = 0;
    spi_vSendChar( (0xF8 ^ ( (char)0 << 2 ) ^ ( (char)1 << 1) ) ); /*start byte - 5 high,RW,RS*/
    while(pcData[i] != '\0')
    {
        spi_vSendChar(reverse(pcData[i]) & 0xF0);
        spi_vSendChar((reverse(pcData[i]) & 0x0F) << 4);
        i++;
    }
}

/**
 * @brief Set display cursor position.
 * @param Column 1-16.
 * @param Line 1-4.
 */
void dispCtrl_vSetPosition(Uint16 u16PosX, Uint16 u16PosY)
{
    char cAddress = (char)( ( ( (Uint16)u16PosY - 1 )*(Uint16)32 ) + ( (Uint16)u16PosX - 1 ) ) ;
    dispCtrl_vSendInstruction(0,0,((char)0x80 + (char)cAddress)); /*set DDRAM on position 0:0*/
}

/**
 * @brief Clear display.
 */
void dispCtrl_clear()
{
    dispCtrl_vSendInstruction(0,0,0x01);
}

/**
 * @brief Convert float to char array.
 * @param Converting float.
 * @param Address to char array.
 * @param Float precision.
 */
void float_to_char_array(F32 f, char* buffer, U16 precision) {
    // Handle negative numbers
    if (f < 0) {
        *buffer++ = '-';
        f = -f;
    }

    // Extract integer part
    int int_part = (int)f;

    // Extract fractional part
    float fractional_part = f - int_part;

    // Convert integer part to characters
    int digits = 0;
    do {
        *buffer++ = '0' + int_part % 10;
        int_part /= 10;
        digits++;
    } while (int_part > 0);

    // Reverse integer part in the buffer
    char *start = buffer - digits;
    char *end = buffer - 1;
    while (start < end) {
        char temp = *start;
        *start++ = *end;
        *end-- = temp;
    }

    // Add decimal point
    *buffer++ = '.';

    // Convert fractional part to characters
    int i = 0;
    for (i = 0; i < precision; i++) {
        fractional_part *= 10;
        int digit = (int)fractional_part;
        *buffer++ = '0' + digit;
        fractional_part -= digit;
    }

    // Null-terminate the string
    *buffer = '\0';
}

/**
 * @brief Refresh display according with motor state.
 * @details Executed in the main loop of the code.
 */
void DisplayRefresh(void)
{
    static U32 ref_ticks_U32 = 0;
    static boolean over_torque_written_b = False_b;

    if(s_MTCL_Control_s.over_torque_error_f1 == 1 && states == 0)
    {
        dispCtrl_vSetPosition(1,2);
        dispCtrl_u16PutString("     ERROR:     ");
        dispCtrl_vSetPosition(1,3);
        dispCtrl_u16PutString(" Route obstacle ");
        dispCtrl_vSetPosition(1,4);
        dispCtrl_u16PutString("  Motor homing  ");
        states = 1;
    }
                if( ATB_CheckTicksPassed_U16(ref_ticks_U32, ATB_MS_TO_TICKS_dM_U32(100)) && s_MTCL_Control_s.tracking_to_zero == 0 )
                {
                    ref_ticks_U32 = ATB_GetTicks_U32();
                    if(s_MTCL_Control_s.over_torque_error_f2 && states == 1)
                    {
                        if(over_torque_written_b == False_b)
                        {
                            dispCtrl_vSetPosition(1,1);
                            dispCtrl_u16PutString("HOMING PROCEDURE");
                            dispCtrl_vSetPosition(1,2);
                            dispCtrl_u16PutString("  INTERRUPTED,  ");
                            dispCtrl_vSetPosition(1,3);
                            dispCtrl_u16PutString(" SERVICE CHECK  ");
                            dispCtrl_vSetPosition(1,4);
                            dispCtrl_u16PutString("     NEEDED     ");
                            f2_error_display_state_U16 = 1;
                        }
                        over_torque_written_b = True_b;
                    }
                    else
                    {
                        over_torque_written_b = False_b;
                    }

                    if(FOC_GetEnableState())
                    {
                        dispCtrl_vSetPosition(1,3);
                        float_to_char_array( ceiling_F32(( (MDA_GetData_ps()->angular_position__rad__F32 / 0.058448f) )), &buffer, 1);
                        dispCtrl_u16PutString(&buffer);
                        dispCtrl_u16PutString(" mm   ");
                        dispCtrl_vSetPosition(14,3);
                        dispCtrl_u16PutString(" ON");
                        s_MTCL_Control_s.over_torque_error_f2 = 0;
                        f2_error_display_state_U16 = 0;
                    }
                    else if ( !FOC_GetEnableState() && f2_error_display_state_U16 == 0)
                    {
                        dispCtrl_vSetPosition(1,3);
                        float_to_char_array( ceiling_F32(( (MDA_GetData_ps()->angular_position__rad__F32 / 0.058448f) )), &buffer, 1);
                        dispCtrl_u16PutString(&buffer);
                        dispCtrl_u16PutString(" mm   ");
                        dispCtrl_vSetPosition(14,3);
                        dispCtrl_u16PutString("OFF");
                    }

                    if(states && f2_error_display_state_U16 == 0){
                            dispCtrl_vSetPosition(1,1);
                            dispCtrl_u16PutString("Collimator Blade");
                            dispCtrl_vSetPosition(1,2);
                            dispCtrl_u16PutString("Position:      ");
                            dispCtrl_vSetPosition(1,3);
                            dispCtrl_u16PutString("                ");
                            dispCtrl_vSetPosition(1,4);
                            dispCtrl_u16PutString("<-1 mm    +1 mm>");
                            states = 0;
                    }
                }
}

/**
 * @brief Round float number.
 * @param Float number.
 * @return Rounded float
 */
F32 ceiling_F32(F32 number){
    F32 whole_num= (F32)((U16)number);
    if(number-whole_num > 0.94f){
        number = whole_num+1.0f;
    }
    return number;
}

/**
 * @}
 */
