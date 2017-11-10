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

/************************ HEADERS **********************************/

#include "console.h"
//#include "eusart2.h"  MCC_UART
#if defined(ENABLE_CONSOLE)
 


/**
  Section: Macro Declarations
 */
#define EUSART2_TX_BUFFER_SIZE 8   // MCC_UART
#define EUSART2_RX_BUFFER_SIZE 8   // MCC_UART

/**
  Section: Global Variables
 */



/************************ VARIABLES ********************************/
unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    
//DEFINITIONS


#define USART_USE_BRGH_HIGH
#if defined(USART_USE_BRGH_LOW)
    #define SPBRG_VAL   ( ((SYS_CLK_FrequencySystemGet()/BAUD_RATE)/64) - 1)
#else
    #define SPBRG_VAL   ( ((SYS_CLK_FrequencySystemGet()/BAUD_RATE)/16) - 1)
#endif

#if SPBRG_VAL > 255
    #error "Calculated SPBRG value is out of range for current SYS_CLK_FrequencySystemGet()."
#endif


//FUNCTIONS
    
/*********************************************************************
* Function:         void CONSOLE_Initialize(void)
*
* PreCondition:     none
*
* Input:            none
*
* Output:           none
*
* Side Effects:	    UART is configured
*
* Overview:         This function will configure the UART for use at
*                   in 8 bits, 1 stop, no flowcontrol mode
*
* Note:             None
********************************************************************/
void CONSOLE_Initialize(void)
{
    
    
    #if defined(USART_USE_BRGH_HIGH)
        TSTA = 0x24;
    #else
        TSTA = 0x20;
    #endif
    RSTA = 0x90;
    UART_SPBRG = SPBRG_VAL;
    PIE1bits.RCIE = 1;
}
void EUSART2_Initialize(void) {
    // disable interrupts before changing states
   // PIE3bits.RC2IE = 0;
   // PIE3bits.TX2IE = 0;
    // Added to the code usually present in MCC pinmanager_Init()
    //TX2_TRIS = 0;  // MCC_UART
    //RX2_TRIS = 1; // MCC_UART
    UART_RX_PIN = 1;
    UART_TX_PIN = 0;
    // Set the EUSART2 module to the options selected in the user interface.

    // ABDEN disabled; WUE enabled; RCIDL idle; ABDOVF no_overflow; CKTXP async_noninverted_sync_fallingedge; BRG16 16bit_generator; DTRXP not_inverted; 
    BAUDCON2 = 0x4A;

    // ADDEN disabled; RX9 8-bit; RX9D 0x0; FERR no_error; CREN enabled; SPEN enabled; SREN disabled; OERR no_error; 
    RCSTA2 = 0x90;

    // CSRC slave_mode; TRMT TSR_empty; TXEN enabled; BRGH hi_speed; SYNC asynchronous; SENDB sync_break_complete; TX9D 0x0; TX9 8-bit; 
    TXSTA2 = 0x24;

    // Baud Rate = 19200; SPBRGL 103; 
    SPBRG2 = 0x67;

    // Baud Rate = 19200; SPBRGH 0; 
    SPBRGH2 = 0x00;


    
    // enable receive interrupt
   // PIE3bits.RC2IE = 1;
}
/*********************************************************************
* Function:         void CONSOLE_PutString(char* str)
*
* PreCondition:     none
*
* Input:            str - string that needs to be printed
*
* Output:           none
*
* Side Effects:	    str is printed to the console
*
* Overview:         This function will print the inputed ROM string
*
* Note:             Do not power down the microcontroller until
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void CONSOLE_PutString(char* str)
{
    uint8_t c;

    while( c = *str++ )
        EUSART2_Write(c);
}



/*********************************************************************
* Function:         void CONSOLE_Put(uint8_t c)
*
* PreCondition:     none
*
* Input:            c - character to be printed
*
* Output:           none
*
* Side Effects:	    c is printed to the console
*
* Overview:         This function will print the inputed character
*
* Note:             Do not power down the microcontroller until
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void CONSOLE_Put(uint8_t c)
{
    while( !CONSOLE_IsPutReady() );
        TREG = c;
    
}

uint8_t EUSART2_Read(void) {
    RCREG2 = 0x00;
    while (!PIR3bits.RC2IF) {
    }
    
    if (1 == RCSTA2bits.OERR) {
        // EUSART2 error - restart

        RCSTA2bits.CREN = 0;
        RCSTA2bits.CREN = 1;
    }

    return RCREG2;
}

void EUSART2_Write(uint8_t txData) {
    while (0 == PIR3bits.TX2IF) {
    }

    TXREG2 = txData; // Write the data byte to the USART.
}
/*********************************************************************
* Function:         uint8_t CONSOLE_Get(void)
*
* PreCondition:     none
*
* Input:            none
*
* Output:           one byte received by UART
*
* Side Effects:	    none
*
* Overview:         This function will receive one byte from UART
*
* Note:             Do not power down the microcontroller until
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
uint8_t CONSOLE_Get(void)
{
    
    // Clear overrun error if it has occured
    // New bytes cannot be received if the error occurs and isn't cleared
    if(RSTA_OERR)
    {
        RSTA_CREN = 0;   // Disable UART receiver
        RSTA_CREN = 1;   // Enable UART receiver
    }
    //while (!PIR1bits.RCIF);
       return RREG;  
}

void CONSOLE_Read_Text(uint8_t *Output, uint8_t length)
{
  uint8_t i;
  for(i=0;i<length;i++)
      Output[i] = EUSART2_Read();
      //Output[i] = CONSOLE_Get();
}

/*********************************************************************
* Function:         void CONSOLE_PrintHex(uint8_t toPrint)
*
* PreCondition:     none
*
* Input:            toPrint - character to be printed
*
* Output:           none
*
* Side Effects:	    toPrint is printed to the console
*
* Overview:         This function will print the inputed char to
*                   the console in hexidecimal form
*
* Note:             Do not power down the microcontroller until
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void CONSOLE_PrintHex(uint8_t toPrint)
{
    uint8_t PRINT_VAR;
    PRINT_VAR = toPrint;
    toPrint = (toPrint>>4)&0x0F;
    CONSOLE_Put(CharacterArray[toPrint]);
    toPrint = (PRINT_VAR)&0x0F;
    CONSOLE_Put(CharacterArray[toPrint]);
    return;
}

/*********************************************************************
* Function:         void CONSOLE_PrintDec(uint8_t toPrint)
*
* PreCondition:     none
*
* Input:            toPrint - character to be printed. Range is 0-99
*
* Output:           none
*
* Side Effects:	    toPrint is printed to the console in decimal
*                   
*
* Overview:         This function will print the inputed char to
*                   the console in decimal form
*
* Note:             Do not power down the microcontroller until
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void CONSOLE_PrintDec(uint8_t toPrint)
{
    if( toPrint >= 100 )
        CONSOLE_Put(CharacterArray[toPrint/100]);
    if( toPrint >= 10 )
        CONSOLE_Put(CharacterArray[(toPrint%100)/10]);
    CONSOLE_Put(CharacterArray[toPrint%10]);
}




#endif  //ENABLE_CONSOLE
