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


#define PORTEX_SS_PORT      PORTAbits.PORTA5
#define PORTEX_SS_TRIS      TRISAbits.TRISA5
#define PORTEX_SCK_TRIS     TRISBbits.TRISB11
#define PORTEX_SDO_TRIS     TRISBbits.TRISB13
#define PORTEX_SDI_TRIS     TRISBbits.TRISB10

#define PEXA        0x14    //Latch for port A of Port Expander
#define PEXB        0x15    //Latch for port B of Port Expander

/*****************************************************************************
 * Function: PortExpander_ByteWrite
 *
 * Preconditions: None.
 *
 * Overview: This function initiates one complete SPI transfer to Port Expander.
 *
 * Input: None.
 *
 * Output: None.
 *
 ******************************************************************************/

void PortExpander_ByteWrite(unsigned char port, unsigned char data);

/*****************************************************************************
 * Function: PortExpander_Initialize
 *
 * Preconditions: None.
 *
 * Overview: This function initializes the Port Expander.
 *
 * Input: None.
 *
 * Output: None.
 *
 ******************************************************************************/
void PortExpander_Initialize ( void );




