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
********************************************************************/

/******** Include files **********************/
#include <oled.h>
#include <system.h>
#include <stdbool.h>
#include <stdint.h>
#include <usb.h>

#define	oledWR			LATEbits.LATE1
#define	oledWR_TRIS		TRISEbits.TRISE1
#define	oledRD			LATEbits.LATE0
#define	oledRD_TRIS		TRISEbits.TRISE0
#define	oledCS			LATEbits.LATE2
#define	oledCS_TRIS		TRISEbits.TRISE2
#define	oledRESET		LATBbits.LATB4
#define	oledRESET_TRIS	TRISBbits.TRISB4
#define	oledD_C			LATBbits.LATB5
#define	oledD_C_TRIS	TRISBbits.TRISB5

// Color
uint8_t    _color;
// Clipping region control
uint16_t _clipRgn;
// Clipping region borders
uint16_t _clipLeft;
uint16_t _clipTop;
uint16_t _clipRight;
uint16_t _clipBottom;


/*********************************************************************
* Function:  void  DelayMs(uint16_t time)
*
* PreCondition: none
*
* Input: time - delay in ms
*
* Output: none
*
* Side Effects: none
*
* Overview: delays execution on time specified in ms
*
********************************************************************/
#define DELAY_1MS 32000/9

void  DelayMs(uint16_t time)
{
    uint16_t delay;

    //Caller wants to block for a possibly very long time.  If USB is enabled
    //and operated in polling mode, we must call USBDeviceTasks() periodically
    //so we can still process mandatory USB control transfer requests from the
    //host within the time allowed by the USB specs.
    #if defined(USB_POLLING)
        if(U1CONbits.USBEN == 1)
        {
            while(time--)
            {
                for(delay=0; delay < 240u; delay++)
                {
                    USBDeviceTasks();   //Assuming 50 inst cycles/call @ 12 MIPS = ~240 calls/ms
                }
            }
        }
		else
		{
            while(time--)
            {
                for(delay=0; delay<DELAY_1MS; delay++);
            }		
		}
    #else
        while(time--)
        {
            for(delay=0; delay<DELAY_1MS; delay++);
        }
    #endif
}

/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: resets LCD, initializes PMP
*
* Note: none
*
********************************************************************/
void ResetDevice(void){

    unsigned long i;

    oledWR = 0;
    oledWR_TRIS = 0;
    oledRD = 0;
    oledRD_TRIS = 0;
    oledCS = 1;
    oledCS_TRIS = 0;
    oledD_C	= 0;
    oledD_C_TRIS = 0;

    oledRESET = 1;  //1 = not in reset, 0 = in reset
    oledRESET_TRIS = 0;

    //Reset the OLED module
    oledRESET = 0;
    for(i=0;i<100;i++){}
    oledRESET = 1;
    for(i=0;i<100;i++){}

    // Setup Display
    WriteCommand(0xAE);			// turn off the display (AF=ON, AE=OFF)

    WriteCommand(0xDB);			// set  VCOMH
    WriteCommand(0x23);

    WriteCommand(0xD9);			// set  VP
    WriteCommand(0x22);

    //////////////////////////////
    // User Set Up
    //////////////////////////////

    // Re-map
    WriteCommand(0xA1);			// [A0]:column address 0 is map to SEG0
                                                            // [A1]:column address 131 is map to SEG0

    // COM Output Scan Direction
    WriteCommand(0xC8);			// C0 is COM0 to COMn, C8 is COMn to COM0

    // COM Pins Hardware Configuration
    WriteCommand(0xDA);			// set pins hardware configuration
    WriteCommand(0x12);

    // Multiplex Ratio
    WriteCommand(0xA8);			// set multiplex ratio
    WriteCommand(0x3F);			// set to 64 mux

    // Display Clock Divide
    WriteCommand(0xD5);			// set display clock divide
    WriteCommand(0xA0);			// set to 100Hz

    // Contrast Control Register
    WriteCommand(0x81);			// Set contrast control
    WriteCommand(0x60);			// display 0 ~ 127; 2C

    // Display Offset
    WriteCommand(0xD3);			// set display offset
    WriteCommand(0x00);			// no offset

    //Normal or Inverse Display
    WriteCommand(0xA6);			// Normal display

    WriteCommand(0xAD);			// Set DC-DC
    WriteCommand(0x8B);			// 8B=ON, 8A=OFF

    //Clear out the display RAM so that it initializes with all pixels off
	FillDisplay(0x00);      //Turn off all the pixels on the OLED display

    // Display ON/OFF
    WriteCommand(0xAF);			// AF=ON, AE=OFF

    //Startup delay
    DelayMs(150);               //SH1101A datasheet suggests waiting 150ms after turning
                                //on display.  This doesn't appear strictly needed,
                                //but presumably gives the boost circuitry the time
                                //needed to charge to full voltage for normal operation.

    // Entire Display ON/OFF
    WriteCommand(0xA4);			// A4=ON

    // Display Start Line
    WriteCommand(0x40);			// Set display start line

    // Lower Column Address
    WriteCommand(0x00+OFFSET);	// Set lower column address

    // Higher Column Address
    WriteCommand(0x10);			// Set higher column address
}

/*********************************************************************
* Macros:  WriteCommand()
*
* Overview: Writes command word to the display controller. A delay
*			is inserted at the end to meet the controller requirements
*			on selected commands.
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
void WriteCommand(uint8_t cmd)
{
    TRISD = 0x00;
    LATD  = cmd;
    oledRD = 1;
    oledWR = 1;
    oledD_C	= 0;
    oledCS = 0;
    Nop();
    oledWR = 0;
    Nop();
    oledWR = 1;
    Nop();
    oledCS = 1;
    Nop();
    TRISD = 0xFF;
}

/*********************************************************************
* Function:  void WriteTwoByteCommand(BYTE firstByte, BYTE secondByte)
*
* Overview: Writes a two byte command to the display controller, without
*           de-asserting chip select in between bytes.
*
* PreCondition: I/O pins correctly pre-configured for interfacing with the module
*
* Input: BYTE firstByte: The first byte value that will be sent to the device
*        BYTE secondByte: The second byte value that will be sent to the device
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void WriteTwoByteCommand(uint8_t firstByte, uint8_t secondByte)
{
    TRISD = 0x00;
    LATD  = firstByte;
    oledRD = 1;
    oledWR = 1;
    oledD_C	= 0;
    oledCS = 0;
    Nop();          //Extra setup time
    oledWR = 0;
    Nop();
    oledWR = 1;
    Nop();          //Extra hold time
    LATD  = secondByte;
    Nop();          //Extra setup time
    oledWR = 0;
    Nop();
    oledWR = 1;
    Nop();          //Extra hold time
    oledCS = 1;
    Nop();
    TRISD = 0xFF;
}



/*********************************************************************
* Macros:  WriteData()
*
* Overview: Writes data byte to the display controller. A delay
*			is inserted at the end to meet the controller requirements
*			on selected commands.
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
void WriteData(uint8_t data)
{
    TRISD = 0x00;
    LATD  = data;
    oledRD = 1;
    oledWR = 1;
    oledD_C	= 1;
    oledCS = 0;
    Nop();
    oledWR = 0;
    Nop();
    oledWR = 1;
    Nop();
    oledCS = 1;
    Nop();
    TRISD = 0xFF;
}

void FillDisplay(unsigned char data)
{
	unsigned char i,j;

    for(i=0xB0;i<0xB8;i++)		// Go through all 8 pages (which represent 8-pixel tall rows on the screen)
	{
		WriteCommand(i);		// Set page
		WriteCommand(0x00);		// Set lower column address
		WriteCommand(0x10);		// Set upper column address

		for(j = 0; j < SCREEN_CONTROLLER_MAX_X_WIDTH; j++)			// Write to all 132 bytes
		{
			WriteData(data);
		}
	}
	return;
}

void oledWriteChar1x(char letter, unsigned char page, unsigned char column, bool invert)
{
	letter -= ' ';                          //Adjust character to table that starts at 0x20 (for proper indexing into the character/font bitmap array)
	column += OFFSET;                       //Shift over to right by 2 pixels
	WriteCommand(page);                     //Select the 8-pixel tall row of interest
	WriteCommand(0x00+(column&0x0F));       //Set column address command lower nibble
	WriteCommand(0x10+((column>>4)&0x0F));  //Set column address command upper nibble

    if(invert)
    {
        //Write one ASCII character to the screen (8 pixels vertical x 6 pixels horizontal), but with inversion (ex: black chars on white backgnd)
    	WriteData(~g_pucFont[letter][0]);	// Write first column
    	WriteData(~g_pucFont[letter][1]);	// Write second column
    	WriteData(~g_pucFont[letter][2]);	// Write third column
    	WriteData(~g_pucFont[letter][3]);	// Write fourth column
    	WriteData(~g_pucFont[letter][4]);	// Write fifth column
    	WriteData(~g_pucFont[letter][5]);	// Write sixth column
    }
    else
    {
        //Write one ASCII character to the screen (8 pixels vertical x 6 pixels horizontal), no inversion (ex: white chars on black background)
    	WriteData(g_pucFont[letter][0]);	// Write first column
    	WriteData(g_pucFont[letter][1]);	// Write second column
    	WriteData(g_pucFont[letter][2]);	// Write third column
    	WriteData(g_pucFont[letter][3]);	// Write fourth column
    	WriteData(g_pucFont[letter][4]);	// Write fifth column
    	WriteData(g_pucFont[letter][5]);	// Write sixth column
    }
	return;
}

void oledPutROMStringInvertOption(const unsigned char *ptr, unsigned char page, unsigned char col, bool invert)
{
    unsigned char i;

    i=col;
    page = page + 0xB1;
    while(*ptr)
    {
        //Write a 8x6 pixel (vertical x horizontal) character to the screen
        oledWriteChar1x(*ptr,page,i,invert);
        ptr++;
        i+=6;
    }
}

void oledPutStringInvertOption(unsigned char *ptr,unsigned char page, unsigned char col, bool invert)
{
    unsigned char i;

    i=col;
    page = page + 0xB1;
    while(*ptr)
    {
        //Write a 8x6 pixel (vertical x horizontal) character to the screen
        oledWriteChar1x(*ptr,page,i,invert);
        ptr++;
        i+=6;
    }
}


void oledPutImage(const unsigned char *ptr, unsigned char sizex, unsigned char sizey, unsigned char startx, unsigned char starty)
{
	unsigned char i,j,mask;
	unsigned int count = 0;
	
	startx += OFFSET;
	for(i=0xB0+starty;i<(0xB0+sizey);i++)
	{
		WriteCommand(i);                            //Set the page (8 pixel tall row) to write to
		WriteCommand(startx&0x0F);                  //Set the lower nibble of the column address
		WriteCommand(0x10 | ((startx>>4)&0x0F));    //Set the upper nibble of the column address

		for(j=0;j<sizex;j++)
		{
			count ++;
			WriteData(*ptr++);

		}
	}
	return;
}

	


//////////////////////////////////////
//////////////////////////////////////
// OLED ROM CONSTANT DATA ////////////
//////////////////////////////////////
//////////////////////////////////////
const uint8_t g_pucFont[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // " " 0x20
    { 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00 }, // !   0x21
    { 0x00, 0x07, 0x00, 0x07, 0x00, 0x00 }, // "   0x22
    { 0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00 }, // #   0x23
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00 }, // $   0x24
    { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 }, // %   0x25
    { 0x36, 0x49, 0x55, 0x22, 0x50, 0x00 }, // &   0x26
    { 0x00, 0x05, 0x03, 0x00, 0x00, 0x00 }, // '   0x27
    { 0x00, 0x1c, 0x22, 0x41, 0x00, 0x00 }, // (   0x28
    { 0x00, 0x41, 0x22, 0x1c, 0x00, 0x00 }, // )   0x29
    { 0x14, 0x08, 0x3e, 0x08, 0x14, 0x00 }, // *   0x2A
    { 0x08, 0x08, 0x3e, 0x08, 0x08, 0x00 }, // +   0x2B
    { 0x00, 0x50, 0x30, 0x00, 0x00, 0x00 }, // ,   0x2C
    { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 }, // -   0x2D
    { 0x00, 0x60, 0x60, 0x00, 0x00, 0x00 }, // .   0x2E
    { 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 }, // /   0x2F
    { 0x3e, 0x51, 0x49, 0x45, 0x3e, 0x00 }, // 0   0x30
    { 0x00, 0x42, 0x7f, 0x40, 0x00, 0x00 }, // 1   0x31
    { 0x42, 0x61, 0x51, 0x49, 0x46, 0x00 }, // 2   0x32
    { 0x21, 0x41, 0x45, 0x4b, 0x31, 0x00 }, // 3   0x33
    { 0x18, 0x14, 0x12, 0x7f, 0x10, 0x00 }, // 4   0x34
    { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 }, // 5   0x35
    { 0x3c, 0x4a, 0x49, 0x49, 0x30, 0x00 }, // 6   0x36
    { 0x01, 0x71, 0x09, 0x05, 0x03, 0x00 }, // 7   0x37
    { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 }, // 8   0x38
    { 0x06, 0x49, 0x49, 0x29, 0x1e, 0x00 }, // 9   0x39
    { 0x00, 0x36, 0x36, 0x00, 0x00, 0x00 }, // :   0x3A
    { 0x00, 0x56, 0x36, 0x00, 0x00, 0x00 }, // ;   0x3B
    { 0x08, 0x14, 0x22, 0x41, 0x00, 0x00 }, // <   0x3C
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 }, // =   0x3D
    { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 }, // >   0x3E
    { 0x02, 0x01, 0x51, 0x09, 0x06, 0x00 }, // ?   0x3F
    { 0x32, 0x49, 0x79, 0x41, 0x3e, 0x00 }, // @   0x40
    { 0x7e, 0x11, 0x11, 0x11, 0x7e, 0x00 }, // A   0x41
    { 0x7f, 0x49, 0x49, 0x49, 0x36, 0x00 }, // B   0x42
    { 0x3e, 0x41, 0x41, 0x41, 0x22, 0x00 }, // C   0x43
    { 0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00 }, // D   0x44
    { 0x7f, 0x49, 0x49, 0x49, 0x41, 0x00 }, // E   0x45
    { 0x7f, 0x09, 0x09, 0x09, 0x01, 0x00 }, // F   0x46
    { 0x3e, 0x41, 0x49, 0x49, 0x7a, 0x00 }, // G   0x47
    { 0x7f, 0x08, 0x08, 0x08, 0x7f, 0x00 }, // H   0x48
    { 0x00, 0x41, 0x7f, 0x41, 0x00, 0x00 }, // I   0x49
    { 0x20, 0x40, 0x41, 0x3f, 0x01, 0x00 }, // J   0x4A
    { 0x7f, 0x08, 0x14, 0x22, 0x41, 0x00 }, // K   0x4B
    { 0x7f, 0x40, 0x40, 0x40, 0x40, 0x00 }, // L   0x4C
    { 0x7f, 0x02, 0x0c, 0x02, 0x7f, 0x00 }, // M   0x4D
    { 0x7f, 0x04, 0x08, 0x10, 0x7f, 0x00 }, // N   0x4E
    { 0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00 }, // O   0x4F
    { 0x7f, 0x09, 0x09, 0x09, 0x06, 0x00 }, // P   0X50
    { 0x3e, 0x41, 0x51, 0x21, 0x5e, 0x00 }, // Q   0X51
    { 0x7f, 0x09, 0x19, 0x29, 0x46, 0x00 }, // R   0X52
    { 0x46, 0x49, 0x49, 0x49, 0x31, 0x00 }, // S   0X53
    { 0x01, 0x01, 0x7f, 0x01, 0x01, 0x00 }, // T   0X54
    { 0x3f, 0x40, 0x40, 0x40, 0x3f, 0x00 }, // U   0X55
    { 0x1f, 0x20, 0x40, 0x20, 0x1f, 0x00 }, // V   0X56
    { 0x3f, 0x40, 0x38, 0x40, 0x3f, 0x00 }, // W   0X57
    { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 }, // X   0X58
    { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00 }, // Y   0X59
    { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 }, // Z   0X5A
    { 0x00, 0x7f, 0x41, 0x41, 0x00, 0x00 }, // [   0X5B
    { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 }, // "\" 0X5C
    { 0x00, 0x41, 0x41, 0x7f, 0x00, 0x00 }, // ]   0X5D
    { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 }, // ^   0X5E
    { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 }, // _   0X5F
    { 0x00, 0x01, 0x02, 0x04, 0x00, 0x00 }, // `   0X60
    { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00 }, // a   0X61
    { 0x7f, 0x48, 0x44, 0x44, 0x38, 0x00 }, // b   0X62
    { 0x38, 0x44, 0x44, 0x44, 0x20, 0x00 }, // c   0X63
    { 0x38, 0x44, 0x44, 0x48, 0x7f, 0x00 }, // d   0X64
    { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 }, // e   0X65
    { 0x08, 0x7e, 0x09, 0x01, 0x02, 0x00 }, // f   0X66
    { 0x0c, 0x52, 0x52, 0x52, 0x3e, 0x00 }, // g   0X67
    { 0x7f, 0x08, 0x04, 0x04, 0x78, 0x00 }, // h   0X68
    { 0x00, 0x44, 0x7d, 0x40, 0x00, 0x00 }, // i   0X69
    { 0x20, 0x40, 0x44, 0x3d, 0x00, 0x00 }, // j   0X6A
    { 0x7f, 0x10, 0x28, 0x44, 0x00, 0x00 }, // k   0X6B
    { 0x00, 0x41, 0x7f, 0x40, 0x00, 0x00 }, // l   0X6C
    { 0x7c, 0x04, 0x18, 0x04, 0x78, 0x00 }, // m   0X6D
    { 0x7c, 0x08, 0x04, 0x04, 0x78, 0x00 }, // n   0X6E
    { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 }, // o   0X6F
    { 0x7c, 0x14, 0x14, 0x14, 0x08, 0x00 }, // p   0X70
    { 0x08, 0x14, 0x14, 0x18, 0x7c, 0x00 }, // q   0X71
    { 0x7c, 0x08, 0x04, 0x04, 0x08, 0x00 }, // r   0X72
    { 0x48, 0x54, 0x54, 0x54, 0x20, 0x00 }, // s   0X73
    { 0x04, 0x3f, 0x44, 0x40, 0x20, 0x00 }, // t   0X74
    { 0x3c, 0x40, 0x40, 0x20, 0x7c, 0x00 }, // u   0X75
    { 0x1c, 0x20, 0x40, 0x20, 0x1c, 0x00 }, // v   0X76
    { 0x3c, 0x40, 0x30, 0x40, 0x3c, 0x00 }, // w   0X77
    { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 }, // x   0X78
    { 0x0c, 0x50, 0x50, 0x50, 0x3c, 0x00 }, // y   0X79
    { 0x44, 0x64, 0x54, 0x4c, 0x44, 0x00 }, // z   0X7A
    { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 }, // {   0X7B
    { 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00 }, // |   0X7C
    { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 }, // }   0X7D
    { 0x02, 0x01, 0x02, 0x04, 0x02, 0x00 }, // ~   0X7E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // blank 0x7F
    //----------------------------------------------------------//
    //  custom characters                                       //
    //----------------------------------------------------------//
    
    { 0x00, 0x7f, 0x45, 0x45, 0x45, 0x47 }, //     0X80 - left half of folder
    { 0x44, 0x44, 0x44, 0x44, 0x7c, 0x00 }, //     0x81 - right half of folder
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 }, //     0x82 - LED on
    { 0xFF, 0x81, 0x81, 0x81, 0xFF, 0x00 }, //     0x83 - LED off
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //     0x84 - cell blank
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, //     0x85 - cell full
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 }, //     0x86 - left bar
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF }, //     0x87 - right bar
    { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 }, //     0x88 - bottom bar
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }, //     0x89 - top bar
    { 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01 }, //     0x8A - top left corner
    { 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80 }, //     0x8B - bottom left corner
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF }, //     0x8C - top right corner
    { 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF }, //     0x8D - bottom right corner
};

const unsigned char folder_image[] = {0x80, 0x81, 0x00};
