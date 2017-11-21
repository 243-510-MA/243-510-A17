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

#ifndef _spi_h_
#define _spi_h_

#define SPIM_PIC24
#define SPIM_BLOCKING_FUNCTION
#define SPIM_MODE16 1
#define SPIM_MODE8 0
#define SPIM_SMP 1
#define SPIM_CKE 0
#define SPIM_CKP 1
#define SPIM_PPRE (((unsigned)0b11)<<0)     /* 0b11 = 1:1 */
#define SPIM_SPRE (((unsigned)0b111)<<2)    /* 0b111 = 1:1 */


/*******************************************************************************
*                                                                       
* This section defines names of control registers of SPI Module.        
* Names depends of processor type and module number.                    
*                                                                       
*******************************************************************************/

    #define SPIBUF  SPI1BUF
    #define SPISTAT SPI1STAT
    #define SPIBUFbits  SPI1BUFbits
    #define SPISTATbits SPI1STATbits
    #define SPIINTEN IEC0 
    #define SPIINTFLG IFS0
    #define SPIINTENbits IEC0bits
    #define SPIINTFLGbits IFS0bits
    #define SPIIF SPI1IF
    #define SPIIE SPI1IE
    #define SPICON SPI1CON1
    #define SPICONbits SPI1CON1bits
    #define SPICON2 SPI1CON2
    #define SPICON2bits SPI1CON2bits


/******************************************************************************
* Error and Status Flags                                                
* SPIM_STS_WRITE_COLLISION - indicates that Write collision has occurred
* while trying to transmit the byte.                                    
*                                                                           
* SPIM_STS_TRANSMIT_NOT_OVER - indicates that the transmission is
* not yet over. This is to be checked only when non-blocking
* option is opted.                                                      
*                                                                           
* SPIM_STS_DATA_NOT_READY - indicates that reception SPI buffer is empty
* and there's no data avalable yet.                                      
*                                                                           
*******************************************************************************/
#define SPIM_STS_WRITE_COLLISION    1
#define SPIM_STS_TRANSMIT_NOT_OVER  2  
#define SPIM_STS_DATA_NOT_READY     3  

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
extern void SPI_Initialize();
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
extern unsigned int  SPI_PutData(unsigned int Data);
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
extern unsigned int SPI_Transmit_IsComplete();

#endif
