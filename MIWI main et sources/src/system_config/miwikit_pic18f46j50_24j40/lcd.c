#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "spi.h"
#include "system.h"
#include "delay.h"

#ifdef __XC8

#define	LCD_LINELENGTH				    16    
#define	LCD_FIRSTLINE_ADDRESS	        0x00
#define	LCD_SECONDLINE_ADDRESS	        0x40

#define LCD_WAKEUP                      0x30
#define LCD_FUNCTION_SET                0x39    // 0x39	-	8 bit interface; 2 lines; Instruction Table 1
#define LCD_INT_OSC                     0x14    // 0x14	-	Set Internal OSC and frame frequency -> 183 Hz
#define LCD_PWR_CTRL                    0x56    // 0x56	-	Power Control; Booster ON; Contrast C5=1
#define LCD_FOLLOWER_CTRL               0x6D    // 0x6D	-	Follower Control; Follower On; Amplification = 0b101
#define LCD_ON                          0x0E    // 0x0C	-	Display ON
#define LCD_ENTRY_MODE                  0x06    // 0x06	-	Entry Mode: Left to right
#define	LCDCMD_CLEARDISPLAY			    0x01    // 0x01	-	Clear Display
#define	LCDCMD_CONTRASTSET_HIGHBYTE	    0x54
#define LCDCMD_CONTRASTSET_LOWBYTE	    0x70    // 0x70	-	Set Contrast low byte				 -> 0x00
#define LCDCMD_SET_DDRAM_ADDRESS	    0x80	

uint8_t LCDText[16*2+1];
bool lcdInitialized=false;

void LCD_WriteCmd(uint8_t data)
{
    GIE=0;
    LCD_CS = 0;
    LCD_RS = 0;
    
    SPIPut2(data);
    
    LCD_CS = 1;
    GIE=1;
}

void LCD_WriteData(uint8_t data)
{
    GIE=0;
    LCD_CS = 0;
    LCD_RS = 1;
    
    SPIPut2(data);
    
    LCD_CS = 1;
    GIE=1;
}

void LCDInit(void)
{    
    LCD_BKLT = 0;
    LCD_CS = 1;
    LCD_RS = 0;
    LCD_RESET = 0;
    LCD_BKLT_TRIS = 0;
    LCD_CS_TRIS = 0;
    LCD_RS_TRIS = 0;
    LCD_RESET_TRIS = 0;
    
    LCD_RESET = 0;    
    DelayMs(2);
    LCD_RESET = 1;
    DelayMs(2);
    LCD_WriteCmd(LCD_WAKEUP);
    DelayMs(2);
    LCD_WriteCmd(LCD_WAKEUP);
    DelayMs(2);
    LCD_WriteCmd(LCD_WAKEUP);
    DelayMs(2);
    LCD_WriteCmd(LCD_FUNCTION_SET);
    DelayMs(2);
    LCD_WriteCmd(LCD_INT_OSC);
    DelayMs(2);
    LCD_WriteCmd(LCD_PWR_CTRL);
    DelayMs(2);
    LCD_WriteCmd(LCD_FOLLOWER_CTRL);
    DelayMs(2);
    
    LCD_WriteCmd(LCDCMD_CONTRASTSET_LOWBYTE);
    DelayMs(2);
    LCD_WriteCmd(LCD_ON);
    DelayMs(2);
    LCD_WriteCmd(LCD_ENTRY_MODE);
    DelayMs(2);
    LCD_WriteCmd(LCDCMD_CLEARDISPLAY);
    DelayMs(10);
}
   
void LCDErase(void)
{
	// Clear display
	LCD_WriteCmd(LCDCMD_CLEARDISPLAY);;    
	DelayMs(2);

	// Clear local copy
    memset(LCDText,0x00,sizeof(LCDText));
	memset(LCDText, ' ', sizeof(LCDText)-1);
}

void LCDBacklightON(void)
{
    LCD_BKLT = 1;
}
    
void LCDBacklightOFF(void)
{
    LCD_BKLT = 0;
}

void LCDUpdate(void)
{
    uint8_t i;
    if(lcdInitialized==false)
    {
        lcdInitialized=true;
        LCDInit();
    }
    LCD_WriteCmd(LCDCMD_SET_DDRAM_ADDRESS | LCD_FIRSTLINE_ADDRESS);
    
    for(i = 0; i < 16; i++)
    {
        DelayUS(10);
        if( LCDText[i] == 0 )
        {
            LCD_WriteData(' ');
        }
        else
        {
            LCD_WriteData(LCDText[i]);
        }
    }
    DelayUS(10);
    LCD_WriteCmd(LCDCMD_SET_DDRAM_ADDRESS | LCD_SECONDLINE_ADDRESS);
    
    for(i = 16; i < 32; i++)
    {
        DelayUS(10);
        if( LCDText[i] == 0 )
        {
            LCD_WriteData(' ');
        }
        else
        {
            LCD_WriteData(LCDText[i]);
        }
    }
}

#endif