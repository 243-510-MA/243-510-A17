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

#include <xc.h>
#include <stdbool.h>
#include <buttons.h>

/*** Button Definitions *********************************************/
//      S1  is MCLR button
#define S3_PORT  PORTAbits.RA7
#define S6_PORT  PORTBbits.RB13       
#define S5_PORT  PORTBbits.RB3       //Overlaps with D10
#define S4_PORT  PORTBbits.RB1

#define S3_TRIS  TRISAbits.TRISA7
#define S6_TRIS  TRISBbits.TRISB13
#define S5_TRIS  TRISBbits.TRISB3
#define S4_TRIS  TRISBbits.TRISB1

#define BUTTON_PRESSED      0
#define BUTTON_NOT_PRESSED  1
/*********************************************************************
 * Function: bool BUTTON_IsPressed(BUTTON button);
 *
 * Overview: Returns the current state of the requested button
 *
 * PreCondition: button configured via BUTTON_SetConfiguration()
 *
 * Input: BUTTON button - enumeration of the buttons available in
 *        this demo.  They should be meaningful names and not the names
 *        of the buttons on the silkscreen on the board (as the demo
 *        code may be ported to other boards).
 *         i.e. - ButtonIsPressed(BUTTON_SEND_MESSAGE);
 *
 * Output: TRUE if pressed; FALSE if not pressed.
 *
 ********************************************************************/
bool BUTTON_IsPressed ( BUTTON button )
{
    switch (button)
    {
        case BUTTON_S3:
            return ( ( S3_PORT == BUTTON_PRESSED ) ? true : false ) ;

        case BUTTON_S6:
            return ( ( S6_PORT == BUTTON_PRESSED ) ? true : false ) ;

        case BUTTON_S5:
            return ( ( S5_PORT == BUTTON_PRESSED ) ? true : false ) ;

        case BUTTON_S4:
            return ( ( S4_PORT == BUTTON_PRESSED ) ? true : false ) ;

        case BUTTON_DISABLED:
            return false ;
    }

    return false ;
}
/*********************************************************************
 * Function: void BUTTON_Enable(BUTTON button);
 *
 * Overview: Returns the current state of the requested button
 *
 * PreCondition: button configured via BUTTON_SetConfiguration()
 *
 * Input: BUTTON button - enumeration of the buttons available in
 *        this demo.  They should be meaningful names and not the names
 *        of the buttons on the silkscreen on the board (as the demo
 *        code may be ported to other boards).
 *         i.e. - ButtonIsPressed(BUTTON_SEND_MESSAGE);
 *
 * Output: None
 *
 ********************************************************************/
void BUTTON_Enable ( BUTTON button )
{
    switch (button)
    {
        case BUTTON_S3:
            S3_TRIS = 1 ;
            break ;

        case BUTTON_S6:
            S6_TRIS = 1 ;
            break ;

        case BUTTON_S5:
            S5_TRIS = 1 ;
            break ;

        case BUTTON_S4:
            S4_TRIS = 1 ;
            break ;

        case BUTTON_DISABLED:
            break ;
    }
}
