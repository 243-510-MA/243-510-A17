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
#include "system.h"
/************************ DEFINITIONS ******************************/
/**
  Section: Included Files
 */

#include <xc.h>
//#include "tmr1.h"   MCC_TMR

/**
  Section: Global Variable Definitions
 */
volatile uint16_t timer1ReloadVal;

/************************ FUNCTION PROTOTYPES **********************/

/************************ VARIABLES ********************************/

volatile uint8_t timerExtension1,timerExtension2;

/************************ FUNCTIONS ********************************/

/*********************************************************************
* Function:         void InitSymbolTimer()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    TMR0 for PIC18 is configured for calculating
*                   the correct symbol times.  TMR2/3 for PIC24/dsPIC
*                   is configured for calculating the correct symbol
*                   times
*
* Overview:		    This function will configure the UART for use at 
*                   in 8 bits, 1 stop, no flowcontrol mode
*
* Note:			    The timer interrupt is enabled causing the timer
*                   roll over calculations.  Interrupts are required
*                   to be enabled in order to extend the timer to
*                   4 bytes in PIC18.  PIC24/dsPIC version do not 
*                   enable or require interrupts
********************************************************************/
void InitSymbolTimer()
{
    
    T1CON = 0b00111000;
    
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    T1CONbits.TMR1ON = 1;
    
/*
    TMR_CON = 0b00111000;
    TMR_IF = 0;
    TMR_IE = 1;
    TMR_ON = 1;
*/
    timerExtension1 = 0;
    timerExtension2 = 0;

}


/*********************************************************************
* Function:         void InitSymbolTimer()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    MIWI_TICK - the current symbol time
*
* Side Effects:	    PIC18 only: the timer interrupt is disabled
*                   for several instruction cycles while the 
*                   timer value is grabbed.  This is to prevent a
*                   rollover from incrementing the timer extenders
*                   during the read of their values
*
* Overview:		    This function returns the current time
*
* Note:			    PIC18 only:
*                   caution: this function should never be called 
*                   when interrupts are disabled if interrupts are 
*                   disabled when this is called then the timer 
*                   might rollover and the byte extension would not 
*                   get updated.
********************************************************************/
MIWI_TICK MiWi_TickGet(void)
{
    MIWI_TICK currentTime;
    

    uint8_t failureCounter;

    /* disable the timer to prevent roll over of the lower 16 bits while before/after reading of the extension */
    PIE1bits.TMR1IE = 0;

    /* copy the byte extension */
    currentTime.byte.b2 = 0;
    currentTime.byte.b3 = 0;

    failureCounter = 0;
    /* read the timer value */
    do
    {
        currentTime.byte.b0 = TMR1L;
        currentTime.byte.b1 = TMR1H;
    } while( currentTime.word.w0 == 0xFFFF && failureCounter++ < 3 );

    //if an interrupt occured after IE = 0, then we need to figure out if it was
    //before or after we read TMR0L
    if(PIR1bits.TMR1IF)
    {
        //if(currentTime.byte.b0<10)
        {
            //if we read TMR0L after the rollover that caused the interrupt flag then we need
            //to increment the 3rd byte
            currentTime.byte.b2++;  //increment the upper most
            if(timerExtension1 == 0xFF)
            {
                currentTime.byte.b3++;
            }
        }
    }

    /* copy the byte extension */
    currentTime.byte.b2 += timerExtension1;
    currentTime.byte.b3 += timerExtension2;

    /* enable the timer*/
    PIE1bits.TMR1IE = 1;

    return currentTime;
}
/***********************************MCC_TMR*****************************************/
uint16_t TMR1_ReadTimer(void) {
    uint16_t readVal;
    uint8_t readValHigh;
    uint8_t readValLow;

    readValLow = TMR1L;
    readValHigh = TMR1H;

    readVal = ((uint16_t) readValHigh << 8) | readValLow;

    return readVal;
}
void TMR1_Initialize(void) {
    //Set the Timer to the options selected in the GUI

    //T1OSCEN disabled; T1RD16 disabled; T1CKPS 1:2; TMR1CS FOSC; T1SYNC synchronize; TMR1ON disabled; 
    T1CON = 0x38;

    //T1GVAL disabled; T1GSPM disabled; T1GSS T1G; T1GTM disabled; T1GPOL low; TMR1GE disabled; T1GGO done; 
    T1GCON = 0x00;

    //TMR1H 0; 
    TMR1H = 0x00;

    //TMR1L 0; 
    TMR1L = 0x00;

    // Load the TMR value to reload variable
    timer1ReloadVal = TMR1;

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR1IF = 0;

    // Enabling TMR1 interrupt.
    PIE1bits.TMR1IE = 1;

    // Start TMR1
    TMR1_StartTimer();
}




/**************************************************************************************/