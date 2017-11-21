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
#include <xc.h>
#include <stdint.h>

#include "leds_rgb.h"

#define LED_RGB_LD3_RED_LAT     LATAbits.LATA3
#define LED_RGB_LD3_GREEN_LAT   LATBbits.LATB12
#define LED_RGB_LD3_BLUE_LAT    LATAbits.LATA2

#define LED_RGB_LD3_RED_TRIS    TRISAbits.TRISA3
#define LED_RGB_LD3_GREEN_TRIS  TRISBbits.TRISB12
#define LED_RGB_LD3_BLUE_TRIS   TRISAbits.TRISA2

#define INPUT  1
#define OUTPUT 0

void LED_RGB_Enable(LED_RGB led)
{
    switch(led)
    {
        case LED_RGB_LD3:
            LED_RGB_LD3_RED_TRIS = OUTPUT ;
            LED_RGB_LD3_GREEN_TRIS = OUTPUT ;
            LED_RGB_LD3_BLUE_TRIS = OUTPUT ;
                   
            RPOR2bits.RP12R = 6; //6 = OC2 -> RB12[RP12] (green)
            RPOR0bits.RP3R  = 7; //7 = OC3 -> RA2[RP3] (blue)
                       
            /* Configure red */
            CCP1CON1 = 0;
            CCP1CON2 = 0;
            CCP1CON3 = 0;
            CCP1CON1bits.MOD = 0b0100;      //dual edge compare mode
            CCP1CON2bits.OCDEN = 1;         //enable OCM1D (red)
            CCP1TMR = 0;
            CCP1RA = 0;
            CCP1RB = 0;
            CCP1PR = 0x3FF;
            CCP1CON1bits.ON = 1;
            
            /* Configure green */
            CCP2CON1 = 0;
            CCP2CON2 = 0;
            CCP2CON3 = 0;
            CCP2CON1bits.MOD = 0b0100;      //dual edge compare mode
            CCP2CON2bits.OCAEN = 1;
            CCP2TMR = 0;
            CCP2RA = 0;
            CCP2RB = 0;
            CCP2PR = 0x3FF;
            CCP2CON1bits.ON = 1;
            
            /* Configure blue */
            CCP3CON1 = 0;
            CCP3CON2 = 0;
            CCP3CON3 = 0;
            CCP3CON1bits.MOD = 0b0100;      //dual edge compare mode
            CCP3CON2bits.OCAEN = 1;
            CCP3TMR = 0;
            CCP3RA = 0;
            CCP3RB = 0;
            CCP3PR = 0x3FF;
            CCP3CON1bits.ON = 1;
            break;
        
        default:
            break;
    }
}

void LED_RGB_SetRed(LED_RGB led, uint16_t red)
{
    CCP1RB = ((uint32_t)red);
}
    
void LED_RGB_SetGreen(LED_RGB led, uint16_t green)
{
    CCP2RB = ((uint32_t)green);
}

void LED_RGB_SetBlue(LED_RGB led, uint16_t blue)
{
    CCP3RB = ((uint32_t)blue);
}

void LED_RGB_Set(LED_RGB led, uint16_t red, uint16_t green, uint16_t blue)
{
    LED_RGB_SetRed(led, red);
    LED_RGB_SetGreen(led, green);
    LED_RGB_SetBlue(led, blue);
}
