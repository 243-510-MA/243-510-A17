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
#include "spi.h"
/*******************************************************************************
 * Function: void SPI_Initialize(void)                                                 
 * 
 * Summary:
 *   Function to initialize the SPI as master in polled mode
 *
 * Description:
 *   This function will initialize the SPI module as master in polled mode
 *
 * Input: None
 *                                                                       
 * Output: None                                                          
 *                                                                       
 ******************************************************************************/
void SPI_Initialize ( )
{
    SPISTAT = 0 ;
    SPICON = ( SPIM_PPRE | SPIM_SPRE ) ;
    SPICONbits.MSTEN = 1 ;
    SPICON2 = 0 ;
    SPICONbits.MODE16 = SPIM_MODE8 ;
    SPICONbits.CKE = SPIM_CKE ;
    SPICONbits.CKP = SPIM_CKP ;
    SPICONbits.SMP = SPIM_SMP ;
    SPIINTENbits.SPIIE = 0 ;
    SPIINTFLGbits.SPIIF = 0 ;
    SPISTATbits.SPIEN = 1 ;
}
/*******************************************************************************
 * Function: unsigned int SPI_PutData(unsigned int)
 *
 * Summary:
 *   Function to send data
 *
 * Description:
 *   In the non-blocking option, this function sends the byte over SPI bus and
 *   checks for Write Collision. In the blocking option, this function waits
 *   for a free transmission buffer.
 *
 * Preconditions:
 *   'SPI_Initialize' should have been called.
 *
 * Input:
 *   None
 *
 * Output:
 *   This function returns ‘0’  on proper initialization of transmission and
 *   ‘SPIM_STS_WRITE_COLLISION’ on occurrence of the Write Collision error.
 *
 ******************************************************************************/
unsigned int SPI_PutData ( unsigned int Data )
{
#ifndef SPIM_BLOCKING_FUNCTION

    if (SPISTATbits.SPITBF)
        return SPIM_STS_WRITE_COLLISION ;
    SPI1BUF = Data ;
    return 0 ;

#else

    // Wait for a data byte reception
    while (SPISTATbits.SPITBF) ;
    SPI1BUF = Data ;
    SPI1STATbits.SPIROV = 0 ;
    IFS0bits.SPI1IF = 0 ;
    Data = SPI1BUF ;

    return 0 ;

#endif
}
/*******************************************************************************
 * Function: unsigned int SPI_Transmit_IsComplete(void)
 *
 * Summary:
 *   Function to check if transmission of data is complete
 *
 * Description:
 *   In the non-blocking option, this function checks whether the transmission
 *   of the byte is completed. In the blocking option, this function waits till
 *   the transmission of the byte is completed.
 *
 * Preconditions:
 *   ‘SPI_PutData’ should have been called.
 *
 * Input:
 *    None
 *
 * Output:
 *    This function returns nothing in the blocking option and returns ‘0’ in the
 *    non-blocking option. It returns SPIM_STS_TRANSMIT_NOT_OVER if the
 *    transmission is not yet over.
 *
 ******************************************************************************/
unsigned SPI_Transmit_IsComplete ( )
{
#ifndef  SPIM_BLOCKING_FUNCTION

    if (SPISTATbits.SPIRBF == 0)
        return SPIM_STS_TRANSMIT_NOT_OVER ;
    return 0 ;

#else

    // Wait for a data byte reception

    while (SPISTATbits.SPIRBF == 0) ;
    return 0 ;

#endif
}


