/******************************************************************************
Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the "Company") for its PIC(R) Microcontroller is intended and
supplied to you, the Company's customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
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
*******************************************************************************/

#include <system.h>
#include <uart.h>
#include <stdint.h>

/* Private Definitions ***********************************************/

/* Private Functions *************************************************/

/* Private variables ************************************************/

/*********************************************************************
* Function: bool UART_Initialize(void);
*
* Overview: Initializes the UART.  
*
* PreCondition: none
*
* Input: None
*
* Output: true if initialized, false otherwise
*
********************************************************************/
bool UART_Initialize(void)
{
    LATCbits.LATC12 = 1;    //Make TX pin high by default (idle)
    TRISCbits.TRISC12 = 0;  //Make TX pin output
    
    U1MODE = 0; //Reset to default
    U1STA = 0; //Reset to default (bits that default to 1 are read-only)
    U1STAbits.UTXEN = 1; //Enable TX mode
    U1MODEbits.CLKSEL = 0b10; //FRC clock source;
    U1MODEbits.ON = 1; //Enable module
    
    U1BRG = 0x19;       //19200
    
    UART_ClearScreen();
	
    return true;
}

/*********************************************************************
* Function: void UART_PutString(char* inputString, uint16_t length);
*
* Overview: Puts a string on the UART screen.  Unsupported characters will be
*           discarded.  May block or throw away characters is UART is not ready
*           or buffer space is not available.  Will terminate when either a
*           null terminator character (0x00) is reached or the length number
*           of characters is printed, which ever comes first.
*
* PreCondition: already initialized via UART_Initialize()
*
* Input: char* - string to print
*        uint16_t - length of string to print
*
* Output: None
*
********************************************************************/
void UART_PutString(char* inputString, uint16_t length)
{
    while(length--)
    {
        switch(*inputString)
        {
            case 0x00:
                return;
                
            default:
                UART_PutChar(*inputString++);
                break;
        }
    }
}

/*********************************************************************
* Function: void UART_PutChar(char);
*
* Overview: Puts a character on the UART screen.  Unsupported characters will be
*           discarded.  May block or throw away characters is UART is not ready
*           or buffer space is not available.
*
* PreCondition: already initialized via UART_Initialize()
*
* Input: char - character to print
*
* Output: None
*
********************************************************************/
void UART_PutChar(char inputCharacter)
{
    while(U1STAbits.TRMT == 0)
    {
    }
    
    U1TXREG = inputCharacter;
    
    while(U1STAbits.TRMT == 0)
    {
    }
}

/*********************************************************************
* Function: void UART_ClearScreen(void);
*
* Overview: Clears the screen, if possible.
*
* PreCondition: already initialized via UART_Initialize()
*
* Input: None
*
* Output: None
*
********************************************************************/
void UART_ClearScreen(void)
{
}

/*********************************************************************
* Function: void UART_CursorEnable(bool enable)
*
* Overview: Enables/disables the cursor
*
* PreCondition: None
*
* Input: bool - specifies if the cursor should be on or off
*
* Output: None
*
********************************************************************/
void UART_CursorEnable(bool enable)
{
}
