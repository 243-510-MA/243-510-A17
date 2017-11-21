/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#ifndef UART1_BAUD_RATE
#define UART1_BAUD_RATE 9600
#pragma message "This module requires a definition for the desired baud rate.  The require definition was not detected so this module is defaulting to 9600 baud.  Define value if this is not the desired baud rate."
#endif

#ifndef SYSTEM_PERIPHERAL_CLOCK
#define SYSTEM_PERIPHERAL_CLOCK (7370000/2)
#pragma message "This module requires a definition for the peripheral clock frequency.  Assuming 3.685MHz Fcy (7.37MHz Fosc from FRC).  Define value if this is not correct."
#endif

#define CLOCK_DIVIDER TIMER_PRESCALER_1
#define PR3_SETTING (SYSTEM_PERIPHERAL_CLOCK/1000/1)

#if (PR3_SETTING > 0xFFFF)
#undef CLOCK_DIVIDER
#undef PR3_SETTING
#define CLOCK_DIVIDER TIMER_PRESCALER_8
#define PR3_SETTING (SYSTEM_PERIPHERAL_CLOCK/1000/8)
#endif

#if (PR3_SETTING > 0xFFFF)
#undef CLOCK_DIVIDER
#undef PR3_SETTING
#define CLOCK_DIVIDER TIMER_PRESCALER_64
#define PR3_SETTING (SYSTEM_PERIPHERAL_CLOCK/1000/64)
#endif

#if (PR3_SETTING > 0xFFFF)
#undef CLOCK_DIVIDER
#undef PR3_SETTING
#define CLOCK_DIVIDER TIMER_PRESCALER_256
#define PR3_SETTING (SYSTEM_PERIPHERAL_CLOCK/1000/256)
#endif

#include <xc.h>
#include <stdint.h>

void UART1_Initialize(void)
{
    RPINR18bits.U1RXR = 5;      /* U1RX is on RP5 */
    RPOR3bits.RP6R = 0b00011;   /* U1TX is on RP6 */
    
    // Set the UART1 module to the options selected in the user interface.

    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; RXINV disabled; UEN TX_RX; 
    U1MODE = 0x8008;

    // OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; UTXISEL0 TX_ONE_CHAR; UTXINV disabled; 
    U1STA = 0x0000;

    U1BRG = (SYSTEM_PERIPHERAL_CLOCK/4/UART1_BAUD_RATE)-1;

    U1STAbits.UTXEN = 1;
}


uint8_t UART1_Read(void)
{
    while(!(U1STAbits.URXDA == 1))
    {
    }

    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }

    return U1RXREG;
}

void UART1_Write(uint8_t txData)
{
    while(U1STAbits.UTXBF == 1)
    {
    }

    U1TXREG = txData;    // Write the data byte to the USART.
}