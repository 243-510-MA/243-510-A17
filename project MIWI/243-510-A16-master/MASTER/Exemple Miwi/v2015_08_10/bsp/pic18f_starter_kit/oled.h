/********************************************************************
  File Information:
    FileName:     	oled.h
    Dependencies:   See INCLUDES section
    Processor:      PIC18F46J50
    Hardware:       PIC18F Starter Kit
    Complier:  	    Microchip C18 (for PIC18)
    Company:        Microchip Technology, Inc.
    
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

  File Description:
    
    Change History:
     Rev   Date         Description
     1.0                Initial release

********************************************************************/
#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <stdbool.h>

/*********************************************/

void WriteCommand(uint8_t cmd);
void WriteTwoByteCommand(uint8_t firstByte, uint8_t secondByte);
void WriteData(uint8_t data);
void FillDisplay(unsigned char data);
void oledPutStringInvertOption(unsigned char *ptr,unsigned char page, unsigned char col, bool invert);
void oledPutROMStringInvertOption(const unsigned char *ptr,unsigned char page, unsigned char col, bool invert);
void oledPutImage(const unsigned char *ptr, unsigned char sizex, unsigned char sizey, unsigned char startx, unsigned char starty);

/*********************************************/
extern const uint8_t g_pucFont[][6];
extern const unsigned char folder_image[];
/*********************************************/

// Define this to implement ClearDevice function in the driver.
#define USE_DRV_CLEARDEVICE

/*********************************************************************
* Overview: Horizontal and vertical screen size.
*********************************************************************/
#ifdef  USE_PORTRAIT
#error THE DRIVER DOES SUPPORT PORTRAIT MODE
#else

// Defines the display offset in x direction. Dependent on the display 
// used and how it is connected.
#define OFFSET				2

// Defines the horizontal screen size. Dependent on the display glass used.
#define SCREEN_HOR_SIZE    128

// Defines the vertical screen size. Dependent on the display glass used.
#define SCREEN_VER_SIZE    64

//Defines the horizontal addressable range supported by the controller of
//the screen (note: may be larger than the physically implemented pixels columns
//of the actual glass).
#define SCREEN_CONTROLLER_MAX_X_WIDTH  132  //132 is max supported by SH1101A controller
#define SCREEN_CONTROLLER_MAX_Y_HEIGHT  64  //64 is max supported by SH1101A controller

#endif
/*********************************************************************
* Overview: Clipping region control codes to be used with SetClip(...)
*           function. 
*********************************************************************/
#define CLIP_DISABLE       0 	// Disables clipping.
#define CLIP_ENABLE        1	// Enables clipping.

/*********************************************************************
* Overview: Screen Saver parameters. 
*			- SSON - Means that screen saver will be enabled when 
*					 ScreenSaver(SSON) function is called with SSON as 
*					 parameter.
*			- SSOFF - Means that screen saver will be disbled when 
*					 ScreenSaver(SSOFF) function is called with SSOFF as 
*					 parameter.
*           
*********************************************************************/
#define SSON				1	// screen saver is turned on
#define	SSOFF				0	// screen saver is turned off

/*********************************************************************
* Overview: Color definitions.
*********************************************************************/

#define BLACK              (uint16_t)0b00000000
#define WHITE              (uint16_t)0b11111111

/*********************************************************************
* Overview: Emboss size set for 3-D effect
*********************************************************************/

#define GOL_EMBOSS_SIZE                 1		



// Memory pitch for line
#define LINE_MEM_PITCH		0x100 

// Definitions for reset pin
#define RST_TRIS_BIT       TRISDbits.TRISD2
#define RST_LAT_BIT        LATDbits.LATD2

// Color
extern uint8_t _color;

/*********************************************************************
* Overview: Clipping region control and border settings.
*
*********************************************************************/

// Clipping region enable control
extern uint16_t _clipRgn;

// Left clipping region border
extern uint16_t _clipLeft;
// Top clipping region border
extern uint16_t _clipTop;
// Right clipping region border
extern uint16_t _clipRight;
// Bottom clipping region border
extern uint16_t _clipBottom;

/*********************************************************************
* Macros:  PMPDelay()
*
* Overview: Delays execution for PMP cycle time. This is dependent on 
*			processor clock. 
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
//#if (SYS_CLK_FrequencySystemGet() == 32000000)
// at 32 MHZ 1 nop is at 32.25 ns. Controller needs 300 ns minimum delay from
// last CS valid to next CS valid. Thus we have the 7 nops (227.5 ns).
// PMP access at this clock frequency is 3 cycles (97.5 ns).
// 		therefore: 227.5 + 97.5 > 300 
//   			   verify: 	300 - 97.5 = 202.5
//				   			202.5/32.5 = 6.23 thus we can use 7 nops.	 				
#define PMPDelay()  Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();  

//#else
//
//#define PMPDelay()  Nop(); 
//
//#endif

/*********************************************************************
* Macros:  PMPWaitBusy()
*
* Overview: Delays execution for PMP cycle time.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
//#ifdef __PIC24F__
#define PMPWaitBusy()  Nop(); 
//#else
//#error CONTROLLER IS NOT SUPPORTED
//#endif

/*********************************************************************
* Macros:  WriteData(writeuint8_t)
*
* Overview: Writes data.
*
* PreCondition: none
*
* Input: writeuint8_t - Data uint8_t to be written.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
//#define WriteData(writeuint8_t) 	PMADDR=0x4001;PMPWaitBusy();PMDIN1=writeuint8_t;PMPDelay();

/*********************************************************************
* Macros:  ReadData(readuint8_t)
*
* Overview: Reads a uint8_t from the display buffer. This assumes that the
*			page value, lower and higher column address pointers are already set.
*
* PreCondition: Page, lower and higher column address pointers are already set.
*
* Input: none.
*
* Output: Readuint8_t - data read from the display buffer.
*
* Side Effects: none
*
********************************************************************/
#define ReadData(readuint8_t)		    TRISD = 0xFF;oledD_C = 1;Nop();oledRD = 0;Nop();oledRD = 1;Nop();oledRD = 0;Nop();readuint8_t = PORTD;

/*********************************************************************
* Macros:  SetAddress(lowerAddr,higherAddr)
*
* Overview: Sets the page and the lower and higher address pointer
*			of the display buffer. All parameters should be pre-calculated.
*
* PreCondition: none
*
* Input: page - Page value for the address.
*		 lowerAddr - Lower column address.
*		 higherAddr - Higher column address.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetAddress(page,lowerAddr,higherAddr)\
	WriteCommand(page);\
	WriteCommand(lowerAddr);\
	WriteCommand(higherAddr);

/*********************************************************************
* Function:  void ResetDevice()
*
* Overview: Initializes LCD module.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void ResetDevice(void);

/*********************************************************************
* Function:  void  DelayMs(uint16_t time)
*
* Overview: Delays execution on time specified in milliseconds.
*           The delay is correct only for 16MIPS.
*
* PreCondition: none
*
* Input: time - Delay in milliseconds.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void DelayMs(uint16_t time);

#define oledPutROMString(ptr,page,col) oledPutROMStringInvertOption(ptr,page,col,false)
#define oledPutString(ptr,page,col) oledPutStringInvertOption(ptr,page,col,false)


#endif // OLED_H
