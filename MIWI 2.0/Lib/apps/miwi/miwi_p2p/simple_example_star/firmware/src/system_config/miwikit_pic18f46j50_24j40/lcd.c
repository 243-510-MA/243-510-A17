/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

#include "system.h"
#include "system_config.h"
#include "string.h"


uint8_t LCDText[16*2+1];


void LCDWriteCmd(uint8_t data)
{
    LCD_CS = 0;
    LCD_RS = 0;
    
    SPIPut2(data);
    
    LCD_CS = 1;
}


void LCDWriteData(uint8_t data)
{
    LCD_CS = 0;
    LCD_RS = 1;
    
    SPIPut2(data);
    
    LCD_CS = 1;
}


void LCD_Initialize(void)
{    
    LCD_CS = 1;
    LCD_RESET = 0;    
    DELAY_ms(8);
    LCD_RESET = 1;
    DELAY_ms(80);
    LCDWriteCmd(LCD_WAKEUP);
    DELAY_ms(8);
    LCDWriteCmd(LCD_WAKEUP);
    LCDWriteCmd(LCD_WAKEUP);
    LCDWriteCmd(LCD_FUNCTION_SET);
    LCDWriteCmd(LCD_INT_OSC);
    LCDWriteCmd(LCD_PWR_CTRL);
    LCDWriteCmd(LCD_FOLLOWER_CTRL);
    
    LCDWriteCmd(LCDCMD_CONTRASTSET_LOWBYTE);
    LCDWriteCmd(LCD_ON);
    LCDWriteCmd(LCD_ENTRY_MODE);
    LCDWriteCmd(LCDCMD_CLEARDISPLAY);
    DELAY_ms(40);
}
   
      
/******************************************************************************
 * Function:        void LCDErase(void)
 *
 * PreCondition:    LCDInit() must have been called once
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Clears LCDText[] and the LCD's internal display buffer
 *
 * Note:            None
 *****************************************************************************/
void LCD_Erase(void)
{
	// Clear display
	LCDWriteCmd(LCDCMD_CLEARDISPLAY);;    
	DELAY_ms(2);

	// Clear local copy
	memset(LCDText, ' ', 32);
}

void LCDBacklightON(void)
{
    LCD_BKLT = 1;
}
    
void LCDBacklightOFF(void)
{
    LCD_BKLT = 0;
}


void LCD_Update(void)
{
    uint8_t i;

    LCDWriteCmd(LCDCMD_SET_DDRAM_ADDRESS | LCD_FIRSTLINE_ADDRESS);
    
    for(i = 0; i < 16; i++)
    {
        if( LCDText[i] == 0 )
        {
            LCDWriteData(' ');
        }
        else
        {
            LCDWriteData(LCDText[i]);
        }
    }
    
    LCDWriteCmd(LCDCMD_SET_DDRAM_ADDRESS | LCD_SECONDLINE_ADDRESS);
    
    for(i = 16; i < 32; i++)
    {
        if( LCDText[i] == 0 )
        {
            LCDWriteData(' ');
        }
        else
        {
            LCDWriteData(LCDText[i]);
        }
    }
}
   
 
/*********************************************************************
 * Function:        void LCD_Display(char *text, uint8_t value, bool delay)
 *
 * PreCondition:    LCD has been initialized
 *
 * Input:           text - text message to be displayed on LCD
 *                  value - the text message allows up to one byte 
 *                          of variable in the message
 *                  delay - whether need to display the message for
 *                          2 second without change
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function display the text message on the LCD, 
 *                  including up to one uint8_t variable, if applicable.
 *
 * Note:            This routine is only effective if Explorer16 or
 *                  PIC18 Explorer demo boards are used
 ********************************************************************/
void LCDDisplay(char *text, uint8_t value, bool delay)
{
    LCD_Erase();
    sprintf((char *)LCDText, (char*)text, value); 
    LCD_Update();
    
    // display the message for 2 seconds
    if( delay )
    {
        uint8_t i;
        for(i = 0; i < 8; i++)
        {
            DELAY_ms(250);
        }
    }
}


void LCDTRXCount(uint8_t txCount, uint8_t rxCount)
{
    LCD_Erase();
    sprintf((char *)LCDText, (char*)"TX Messages: %3d", txCount);
    sprintf((char *)&(LCDText[16]), (char*)"RX Messages: %3d", rxCount);
    LCD_Update();

}


void UserInterruptHandler(void)
{
//    if( PIR3bits.SSP2IF )
//    {
//        PIR3bits.SSP2IF = 0;
//        DELAY_ms(5);
//    }
}
