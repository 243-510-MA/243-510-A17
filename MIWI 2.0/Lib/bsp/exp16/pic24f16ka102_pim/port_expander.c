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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <xc.h>

#define PEXA        0x14    //Latch for port A of Port Expander
#define PEXB        0x15    //Latch for port B of Port Expander
#define PEXIOCON    0x05    //IOCON Register of Port Expander
#define PEXDIRA     0x00    //Port Expander Direction Register A
#define PEXDIRB     0x01    //Port Expander Direction Register A
/*****************************************************************************
 * Function: SPI_Write
 *
 * Preconditions: None.
 *
 * Overview: This function handles SPI write to peripheral.
 *
 * Input: None.
 *
 * Output: None.
 *
 ******************************************************************************/
void SPI_Write ( unsigned char tempData )
{
    SPI1BUF = tempData ;                //shift the data into the SPI buffer
    while (!IFS0bits.SPI1IF)            //wait for transmission to complete
        tempData = SPI1BUF ;                //do a dummy read to clear buffer full flag
    IFS0bits.SPI1IF = 0 ;           //clear the SPI interrupt
}
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
void PortExpander_ByteWrite ( unsigned char port , unsigned char data )
{
    PORTAbits.RA6 = 0 ;     //Set CS for Port Expander
    Nop ( ) ;
    SPI_Write ( 0x40 ) ;            //Write Command
    SPI_Write ( port ) ;            //Output port of Port Expander
    SPI_Write ( data ) ;            //Send the data
    PORTAbits.RA6 = 1 ;     //Return CS to signifiy end of transfer
    Nop ( ) ;
}
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

void PortExpander_Initialize ( void )
{
  
   // extern unsigned char PortEx_B_State ;
    //extern unsigned char PortEx_LED_State ;

    IFS0bits.SPI1IF = 0 ;
    TRISAbits.TRISA6 = 0 ;
    PORTAbits.RA6 = 1 ;     //set CS high for port expander
    Nop ( ) ;
    Nop ( ) ;

    PortExpander_ByteWrite ( PEXIOCON , 0x20 ) ;    //setup port expander
    PortExpander_ByteWrite ( PEXDIRA , 0x00 ) ; //set port A of expander to all outputs
    PortExpander_ByteWrite ( PEXDIRB , 0x00 ) ; //set port B of expander to all outputs
}

