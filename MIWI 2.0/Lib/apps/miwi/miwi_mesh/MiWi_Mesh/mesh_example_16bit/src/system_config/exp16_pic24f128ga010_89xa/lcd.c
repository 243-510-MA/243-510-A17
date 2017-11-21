#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "spi.h"
#include "system.h"
#include "delay.h"

#ifdef __PIC24F__
#define SAMSUNG_S6A0032		// This LCD driver chip has a different means of entering 4-bit mode.  
uint8_t LCDText[16*2+1];
static void LCDWrite(uint8_t RS, uint8_t Data)
    {
    	#if defined(LCD_DATA_TRIS)
    		LCD_DATA_TRIS = 0x00;
    	#else
    		LCD_DATA0_TRIS = 0;
    		LCD_DATA1_TRIS = 0;
    		LCD_DATA2_TRIS = 0;
    		LCD_DATA3_TRIS = 0;
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_TRIS = 0;
    		LCD_DATA5_TRIS = 0;
    		LCD_DATA6_TRIS = 0;
    		LCD_DATA7_TRIS = 0;
    		#endif
    	#endif
    	LCD_RS_TRIS = 0;
    	LCD_RD_WR_TRIS = 0;
    	LCD_RD_WR_IO = 0;
    	LCD_RS_IO = RS;
    
    #if defined(FOUR_BIT_MODE)
    	#if defined(LCD_DATA_IO)
    		LCD_DATA_IO = Data>>4;
    	#else
    		LCD_DATA0_IO = Data & 0x10;
    		LCD_DATA1_IO = Data & 0x20;
    		LCD_DATA2_IO = Data & 0x40;
    		LCD_DATA3_IO = Data & 0x80;
    	#endif
    	Nop();					// Wait Data setup time (min 40ns)
    	Nop();
    	LCD_E_IO = 1;
    	Nop();					// Wait E Pulse width time (min 230ns)
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	LCD_E_IO = 0;
    #endif
    
    	#if defined(LCD_DATA_IO)
    		LCD_DATA_IO = Data;
    	#else
    		LCD_DATA0_IO = ((Data & 0x01) == 0x01);
    		LCD_DATA1_IO = ((Data & 0x02) == 0x02);
    		LCD_DATA2_IO = ((Data & 0x04) == 0x04);
    		LCD_DATA3_IO = ((Data & 0x08) == 0x08);
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_IO = ((Data & 0x10) == 0x10);
    		LCD_DATA5_IO = ((Data & 0x20) == 0x20);
    		LCD_DATA6_IO = ((Data & 0x40) == 0x40);
    		LCD_DATA7_IO = ((Data & 0x80) == 0x80);
    		#endif
    	#endif
    	Nop();					// Wait Data setup time (min 40ns)
    	Nop();
    	LCD_E_IO = 1;
    	Nop();					// Wait E Pulse width time (min 230ns)
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	Nop();
    	LCD_E_IO = 0;
    
    	#if defined(LCD_DATA_TRIS)
    		LCD_DATA_TRIS = 0xFF;
    	#else
    		LCD_DATA0_TRIS = 1;
    		LCD_DATA1_TRIS = 1;
    		LCD_DATA2_TRIS = 1;
    		LCD_DATA3_TRIS = 1;
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_TRIS = 1;
    		LCD_DATA5_TRIS = 1;
    		LCD_DATA6_TRIS = 1;
    		LCD_DATA7_TRIS = 1;
    		#endif
    	#endif
    	LCD_RS_TRIS = 1;
    	LCD_RD_WR_TRIS = 1;
    }

 void LCDInit(void)
    {
    	uint8_t i;
    
    	memset(LCDText, ' ', sizeof(LCDText)-1);
    	LCDText[sizeof(LCDText)-1] = 0;
    
    	// Setup the I/O pins
    	LCD_E_IO = 0;
    	LCD_RD_WR_IO = 0;
    
    
    	#if defined(LCD_DATA_TRIS)
    		LCD_DATA_TRIS = 0x00;
    	#else
    		LCD_DATA0_TRIS = 0;
    		LCD_DATA1_TRIS = 0;
    		LCD_DATA2_TRIS = 0;
    		LCD_DATA3_TRIS = 0;
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_TRIS = 0;
    		LCD_DATA5_TRIS = 0;
    		LCD_DATA6_TRIS = 0;
    		LCD_DATA7_TRIS = 0;
    		#endif
    	#endif
    	LCD_RD_WR_TRIS = 0;
    	LCD_RS_TRIS = 0;
    	LCD_E_TRIS = 0;
    
    
    	// Wait the required time for the LCD to reset
		DelayMs(40);
    
    	// Set the default function
    	// Go to 8-bit mode first to reset the instruction state machine
    	// This is done in a loop 3 times to absolutely ensure that we get 
    	// to 8-bit mode in case if the device was previously booted into 
    	// 4-bit mode and our PIC got reset in the middle of the LCD 
    	// receiving half (4-bits) of an 8-bit instruction
    	LCD_RS_IO = 0;
    	#if defined(LCD_DATA_IO)
    		LCD_DATA_IO = 0x03;
    	#else
    		LCD_DATA0_IO = 1;
    		LCD_DATA1_IO = 1;
    		LCD_DATA2_IO = 0;
    		LCD_DATA3_IO = 0;
    		#if !defined(FOUR_BIT_MODE)
    		LCD_DATA4_IO = 0;
    		LCD_DATA5_IO = 0;
    		LCD_DATA6_IO = 0;
    		LCD_DATA7_IO = 0;
    		#endif
    	#endif
    	Nop();					// Wait Data setup time (min 40ns)
    	Nop();
    	for(i = 0; i < 3; i++)
    	{
    		LCD_E_IO = 1;
			Delay10us(1);			// Wait E Pulse width time (min 230ns)
    		LCD_E_IO = 0;
			DelayMs(2);
    	}
    	
    #if defined(FOUR_BIT_MODE)
    	#if defined(SAMSUNG_S6A0032)
    		// Enter 4-bit mode (requires only 4-bits on the S6A0032)
    		#if defined(LCD_DATA_IO)
    			LCD_DATA_IO = 0x02;
    		#else
    			LCD_DATA0_IO = 0;
    			LCD_DATA1_IO = 1;
    			LCD_DATA2_IO = 0;
    			LCD_DATA3_IO = 0;
    		#endif
    		Nop();					// Wait Data setup time (min 40ns)
    		Nop();
    		LCD_E_IO = 1;
			Delay10us(1);			// Wait E Pulse width time (min 230ns)
    		LCD_E_IO = 0;
    	#else
    		// Enter 4-bit mode with two lines (requires 8-bits on most LCD controllers)
    		LCDWrite(0, 0x28);
    	#endif
    #else
    	// Use 8-bit mode with two lines
    	LCDWrite(0, 0x38);
    #endif
		Delay10us(5);
    	
    	// Set the entry mode
    	LCDWrite(0, 0x06);	// Increment after each write, do not shift
		Delay10us(5);
    
    	// Set the display control
    	LCDWrite(0, 0x0C);		// Turn display on, no cusor, no cursor blink
		Delay10us(5);
    
    	// Clear the display
    	LCDWrite(0, 0x01);	
		DelayMs(2);
    	
    }
 void LCDUpdate(void)
    {
    	uint8_t i, j;
    
    	// Go home
    	LCDWrite(0, 0x02);
		DelayMs(2);
    
    	// Output first line
    	for(i = 0; i < 16u; i++)
    	{
    		// Erase the rest of the line if a null char is 
    		// encountered (good for printing strings directly)
    		if(LCDText[i] == 0u)
    		{
    			for(j=i; j < 16u; j++)
    			{
    				LCDText[j] = ' ';
    			}
    		}
    		LCDWrite(1, LCDText[i]);
			Delay10us(5);
    	}
    	
    	// Set the address to the second line
    	LCDWrite(0, 0xC0);
		Delay10us(5);
    
    	// Output second line
    	for(i = 16; i < 32u; i++)
    	{
    		// Erase the rest of the line if a null char is 
    		// encountered (good for printing strings directly)
    		if(LCDText[i] == 0u)
    		{
    			for(j=i; j < 32u; j++)
    			{
    				LCDText[j] = ' ';
    			}
    		}
    		LCDWrite(1, LCDText[i]);
			Delay10us(5);
    	}
    }

 void LCDWriteLine(uint16_t number, char *line)
    {
        uint8_t    i;
        uint8_t    j;
    
        j = 0;
        i = 0;
            
        if(number == 2)
        {
            while ((LCDText[j] != 0) && (j < 16))
            {
                j++;
            }
        }    
    
        do 
        {
            LCDText[j++] = line[i++];
        }    
        while ((LCDText[j-1] != 0) && (j < 31));
        
        LCDUpdate();
    }
    
#else
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
    #define	LCDCMD_CONTRASTSET_HIGHuint8_t	    0x54
    #define LCDCMD_CONTRASTSET_LOWuint8_t	    0x70    // 0x70	-	Set Contrast low byte				 -> 0x00
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

        LCD_WriteCmd(LCDCMD_CONTRASTSET_LOWuint8_t);
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
    #endif//XC8

#endif