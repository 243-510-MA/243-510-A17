/********************************************************************
* FileName:		MRF24J40.c
* Dependencies:    
* Processor:	PIC18, PIC24, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher	
*               Microchip C32 v1.02 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
*
* Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute 
* Software only when embedded on a Microchip microcontroller or digital 
* signal controller and used with a Microchip radio frequency transceiver, 
* which are integrated into your product or third party product (pursuant 
* to the terms in the accompanying license agreement).   
*
* You should refer to the license agreement accompanying this Software for 
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
*********************************************************************
* File Description:
*
*  This file provides transceiver driver functionality for MRF24J40 
*  IEEE 802.15.4 compatible 2.4GHz transceiver. The transceiver
*  driver interfaces are based on Microchip MAC strategy. The 
*  transceiver driver interfaces works with all Microchip wireless 
*  protocols
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/15/2009    yfy       Initial revision
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  2.1   6/20/2009    yfy       Add LCD support
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "gtypedefs.h"
#include "symbol.h"

#include "miwi_config.h"
#include "miwi/miwi_mesh_api.h"
#include "spi.h"
#include "delay.h"

#if defined(MRF24J40)

    #include "driver/mrf_miwi/drv_mrf_miwi_mesh.h"
    #include "driver/mrf_miwi/drv_mrf_miwi_mesh_24j40.h"
    //#include "Transceivers/Transceivers.h"
    #include "NetworkManager.h"
    //#include "WirelessProtocols/NVM.h"
    extern uint32_t krnd(void);
    extern void krndInit(uint16_t seed);
    extern uint32_t t1extension;
    extern volatile bool t1fired;

  
    /************************ VARIABLES ********************************/
    MACINIT_PARAM   MACInitParams;
    
//#warning Fix FAILURE_COUNTER
    #define FAILURE_COUNTER ((uint16_t)0x20 * 4)
    
    #ifdef ENABLE_SECURITY
        #if defined(__18CXX)
            #pragma romdata securityKey = 0x2E
        #endif
            ROM uint8_t mySecurityKey[16] = {SECURITY_KEY_00, SECURITY_KEY_01, SECURITY_KEY_02, SECURITY_KEY_03, SECURITY_KEY_04,
    	        SECURITY_KEY_05, SECURITY_KEY_06, SECURITY_KEY_07, SECURITY_KEY_08, SECURITY_KEY_09, SECURITY_KEY_10, SECURITY_KEY_11, 
    	        SECURITY_KEY_12, SECURITY_KEY_13, SECURITY_KEY_14, SECURITY_KEY_15}; // The 16-byte security key used in the 
    	                                                                             // security module. 
        #if defined(__18CXX)    	                                                                             
            #pragma romdata
        #endif
        ROM uint8_t mySecurityLevel = SECURITY_LEVEL;             // The security level used in the security module. 
                                                                        // The security level is defined in IEEE 802.15.4 specification
                                                                        // It can be from 1 to 7.
        ROM uint8_t myKeySequenceNumber = KEY_SEQUENCE_NUMBER;    // The sequence number of security key. Used to identify
                                                                        // the security key
    	
    	uint32_t OutgoingFrameCounter;     
        
        addr_t tmpSourceLongAddress;
        uint32_t tmpFrameCounter;  
        
        #include "miwi/miwi_mesh_api.h"
        #include "delay.h"          
    #endif	
    
    #if defined(__18CXX)
        #pragma udata MAC_RX_BUFF
    #endif
	    struct
	    {
	        uint8_t PayloadLen;
	        uint8_t Payload[RX_PACKET_SIZE];
	    } RxBuffer[BANK_SIZE]; 
    #if defined(__18CXX)
        #pragma udata
    #endif
    uint8_t BankIndex = 0xFF;
    
    uint8_t IEEESeqNum;
    volatile uint16_t failureCounter = 0;
    uint8_t MACCurrentChannel;
    
    uint16_t MAC_PANID;
    uint16_t myNetworkAddress;

    volatile MRF24J40_STATUS MRF24J40Status;
    
    void SPIPut(uint8_t v);
    uint8_t SPIGet(void);
    
    /*********************************************************************
     * void PHYSetLongRAMAddr(INPUT uint16_t address, INPUT uint8_t value)
     *
     * Overview:        This function writes a value to a LONG RAM address
     *
     * PreCondition:    Communication port to the MRF24J40 initialized
     *
     * Input:           address - the address of the LONG RAM address
     *                      that you want to write to
     *                  value   - the value that you want to write to
     *                      that register
     *
     * Output:          None
     *
     * Side Effects:    The register value is changed
     *                  Interrupt from radio is turned off before accessing
     *                  the SPI and turned back on after accessing the SPI
     *
     ********************************************************************/
    void PHYSetLongRAMAddr(INPUT uint16_t address, INPUT uint8_t value)
    {
        volatile uint8_t tmpRFIE = RFIE;
        
        RFIE = 0;
        PHY_CS = 0;
        SPIPut((((uint8_t)(address>>3))&0x7F)|0x80);
        SPIPut((((uint8_t)(address<<5))&0xE0)|0x10);
        SPIPut(value);
        PHY_CS = 1;
        RFIE = tmpRFIE;
    }
    
    /*********************************************************************
     * void PHYSetShortRAMAddr(INPUT uint8_t address, INPUT uint8_t value)
     *
     * Overview:        This function writes a value to a short RAM address
     *
     * PreCondition:    Communication port to the MRF24J40 initialized
     *
     * Input:           address - the address of the short RAM address
     *                      that you want to write to.  Should use the
     *                      WRITE_ThisAddress definition in the MRF24J40 
     *                      include file.
     *                  value   - the value that you want to write to
     *                      that register
     *
     * Output:          None
     *
     * Side Effects:    The register value is changed
     *                  Interrupt from radio is turned off before accessing
     *                  the SPI and turned back on after accessing the SPI
     *
     ********************************************************************/
    void PHYSetShortRAMAddr(INPUT uint8_t address, INPUT uint8_t value)
    {
        volatile uint8_t tmpRFIE = RFIE;
        
        RFIE = 0;
        PHY_CS = 0;     
        SPIPut(address);
        SPIPut(value);
        PHY_CS = 1;
        RFIE = tmpRFIE;
    }
    
    /*********************************************************************
     * uint8_t PHYGetShortRAMAddr(INPUT uint8_t address)
     *
     * Overview:        This function reads a value from a short RAM address
     *
     * PreCondition:    Communication port to the MRF24J40 initialized
     *
     * Input:           address - the address of the short RAM address
     *                      that you want to read from.  Should use the
     *                      READ_ThisAddress definition in the MRF24J40 
     *                      include file.
     *
     * Output:          None
     *
     * Side Effects:    Interrupt from radio is turned off before accessing
     *                  the SPI and turned back on after accessing the SPI
     *
     ********************************************************************/
    uint8_t PHYGetShortRAMAddr(INPUT uint8_t address)
    {
        uint8_t toReturn;
        volatile uint8_t tmpRFIE = RFIE;
        
        RFIE = 0;
        PHY_CS = 0;
        SPIPut(address);
        toReturn = SPIGet();
        PHY_CS = 1;
        RFIE = tmpRFIE;
        
        return toReturn;
    }
    
    /*********************************************************************
     * uint8_t PHYGetLongRAMAddr(INPUT uint16_t address)
     *
     * Overview:        This function reads a value from a long RAM address
     *
     * PreCondition:    Communication port to the MRF24J40 initialized
     *
     * Input:           address - the address of the long RAM address
     *                      that you want to read from.  
     *
     * Output:          the value read from the specified Long register
     *
     * Side Effects:    Interrupt from radio is turned off before accessing
     *                  the SPI and turned back on after accessing the SPI
     *
     ********************************************************************/
    uint8_t PHYGetLongRAMAddr(INPUT uint16_t address)
    {
        uint8_t toReturn;
        volatile uint8_t tmpRFIE = RFIE;
        
        RFIE = 0;
        PHY_CS = 0;
        SPIPut(((address>>3)&0x7F)|0x80);
        SPIPut(((address<<5)&0xE0));
        toReturn = SPIGet();
        PHY_CS = 1;
        RFIE = tmpRFIE;
        
        return toReturn;
    }

    void InitMRF24J40(void)
    {
        uint8_t i;
        uint16_t j;
        
        // first perform a hardware reset
        PHY_RESETn = 0;
        for(j=0;j<(uint16_t)300;j++){}
    
        PHY_RESETn = 1;
        for(j=0;j<(uint16_t)300;j++){}
      
        /* do a soft reset */
        PHYSetShortRAMAddr(WRITE_SOFTRST,0x07);
        do
        {
            i = PHYGetShortRAMAddr(READ_SOFTRST);
        }
        while((i&0x07) != (uint8_t)0x00);   
    
        for(j=0;j<(uint16_t)1000;j++){}
     
        /* flush the RX fifo */
        PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01);
        
        /* Program the short MAC Address, 0xffff */
        PHYSetShortRAMAddr(WRITE_SADRL,0xFF);
        PHYSetShortRAMAddr(WRITE_SADRH,0xFF);
        PHYSetShortRAMAddr(WRITE_PANIDL,0xFF);
        PHYSetShortRAMAddr(WRITE_PANIDH,0xFF);
        
        /* Program Long MAC Address*/
        for(i=0;i<(uint8_t)8;i++)
        {
            PHYSetShortRAMAddr(WRITE_EADR0+i*2,MACInitParams.PAddress[i]);
        } 
        
        /* setup */
        PHYSetLongRAMAddr(RFCTRL2,0x80);
        
        #if defined(ENABLE_PA_LNA)
            #if defined(MRF24J40MB)
                // There are special MRF24J40 transceiver output power
                // setting for Microchip MRF24J40MB module.
                #if APPLICATION_SITE == EUROPE
                    // MRF24J40 output power set to be -14.9dB
                    PHYSetLongRAMAddr(RFCTRL3, 0x70);
                #else
                    // MRF24J40 output power set to be -1.9dB
                    PHYSetLongRAMAddr(RFCTRL3, 0x18);
                #endif
            #elif defined(MRF24J40MC)
                // MRF24J40 output power set to be -3.7dB for MRF24J40MB
                PHYSetLongRAMAddr(RFCTRL3, 0x28);
            #else
                // power level set to be 0dBm, must adjust according to
                // FCC/IC/ETSI requirement
                PHYSetLongRAMAddr(RFCTRL3,0x00);
            #endif
        #else
            // power level to be 0dBm
            PHYSetLongRAMAddr(RFCTRL3,0x00);
        #endif
        
        /* program RSSI ADC with 2.5 MHz clock */
        PHYSetLongRAMAddr(RFCTRL6,0x90);
        
        PHYSetLongRAMAddr(RFCTRL7,0x80);
        
        PHYSetLongRAMAddr(RFCTRL8,0x10); 
        
        PHYSetLongRAMAddr(SCLKDIV, 0x21);
    
        /* Program CCA mode using RSSI */
        PHYSetShortRAMAddr(WRITE_BBREG2,0x80);
        /* Enable the packet RSSI */
        PHYSetShortRAMAddr(WRITE_BBREG6,0x40);
        /* Program CCA, RSSI threshold values */
        PHYSetShortRAMAddr(WRITE_RSSITHCCA,0x60);
    
        #if defined(ENABLE_PA_LNA)
            
            #if defined(MRF24J40MC)
                PHYSetShortRAMAddr(WRITE_GPIODIR, 0x08); 
                PHYSetShortRAMAddr(WRITE_GPIO, 0x08);
            #endif
            PHYSetLongRAMAddr(TESTMODE, 0x0F);
            
        #endif
        
        PHYSetShortRAMAddr(WRITE_FFOEN, 0x98);
        PHYSetShortRAMAddr(WRITE_TXPEMISP, 0x95);
    
        // wait until the MRF24J40 in receive mode
        do
        {
            i = PHYGetLongRAMAddr(RFSTATE);
        }
        while((i&0xA0) != 0xA0);
        
        PHYSetShortRAMAddr(WRITE_INTMSK,0xE6);

        #ifdef ENABLE_INDIRECT_MESSAGE
            PHYSetShortRAMAddr(WRITE_ACKTMOUT, 0xB9);
        #endif
        
        // Make RF communication stable under extreme temperatures
        PHYSetLongRAMAddr(RFCTRL0, 0x03);
        PHYSetLongRAMAddr(RFCTRL1, 0x02);
        
        MiMAC_SetChannel(MACCurrentChannel, 0);
                    
        // Define TURBO_MODE if more bandwidth is required
        // to enable radio to operate to TX/RX maximum 
        // 625Kbps
        #ifdef TURBO_MODE
        
            PHYSetShortRAMAddr(WRITE_BBREG0, 0x01);
            PHYSetShortRAMAddr(WRITE_BBREG3, 0x38);
            PHYSetShortRAMAddr(WRITE_BBREG4, 0x5C);
            
            PHYSetShortRAMAddr(WRITE_RFCTL,0x04);
            PHYSetShortRAMAddr(WRITE_RFCTL,0x00);
    
        #endif          
        
    }
        
    
    /************************************************************************************
     * Function:
     *      bool MiMAC_ReceivedPacket(void)
     *
     * Summary:
     *      This function check if a new packet has been received by the RF transceiver
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      check if a packet has been received by the RF transceiver. When a packet has
     *      been received, all information will be stored in the global variable 
     *      MACRxPacket in the format of MAC_RECEIVED_PACKET;
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      None
     *
     * Returns: 
     *      A boolean to indicate if a packet has been received by the RF transceiver.
     *
     * Example:
     *      <code>
     *      if( true == MiMAC_ReceivedPacket() )
     *      {
     *          // handle the raw data from RF transceiver
     * 
     *          // discard the current packet
     *          MiMAC_DiscardPacket();
     *      }
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    bool MiMAC_ReceivedPacket(void)
    {   
        uint8_t i;
        
        //set the interrupt flag just in case the interrupt was missed
        if(RF_INT_PIN == 0)
        {
            RFIF = 1;
        }
        
        //If the stack TX has been busy for a long time then
        //time out the TX because we may have missed the interrupt 
        //and don't want to lock up the stack forever
        if(MRF24J40Status.bits.TX_BUSY)
        {
            if(failureCounter >= FAILURE_COUNTER)
            {
                failureCounter = 0;
                MRF24J40Status.bits.TX_BUSY = 0;
            }
            else
            {
                failureCounter++;
            }
        }
  
        BankIndex = 0xFF;
        for(i = 0; i < BANK_SIZE; i++)
        {
            if( RxBuffer[i].PayloadLen > 0 )
            {
                BankIndex = i;
                break;
            }
        }        
  
        //if(MRF24J40Status.bits.RX_BUFFERED == 1)
        if( BankIndex < BANK_SIZE )
        {
            uint8_t addrMode;
            #ifndef TARGET_SMALL
                bool bIntraPAN = true;
            
                if( (RxBuffer[BankIndex].Payload[0] & 0x40) == 0 )
                {
                    bIntraPAN = false;   
                }
            #endif
            MACRxPacket.flags.Val = 0;
            MACRxPacket.altSourceAddress = false;

            //Determine the start of the MAC payload
            addrMode = RxBuffer[BankIndex].Payload[1] & 0xCC;
            switch(addrMode)
            {
                case 0xC8: //short dest, long source
                    // for P2P only broadcast allows short destination address
                    if( RxBuffer[BankIndex].Payload[5] == 0xFF && RxBuffer[BankIndex].Payload[6] == 0xFF )
                    {
                        MACRxPacket.flags.bits.broadcast = 1;
                    }
                    MACRxPacket.flags.bits.sourcePrsnt = 1;
                    
                    #ifndef TARGET_SMALL
                        if( bIntraPAN ) // check if it is intraPAN
                    #endif
                    {
                        #ifndef TARGET_SMALL
                            memcpy(&MACRxPacket.SourcePANID,&RxBuffer[BankIndex].Payload[3],2);
                            //MACRxPacket.SourcePANID = RxBuffer[BankIndex].Payload[3] | (RxBuffer[BankIndex].Payload[4]<<8);
                        #endif
                        MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[7]);
                        
                        MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 19;
                        MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[15]);   
                    }
                    #ifndef TARGET_SMALL
                        else
                        {
                            memcpy(&MACRxPacket.SourcePANID,&RxBuffer[BankIndex].Payload[7],2);
                            //MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[7];
                            //MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[8];
                            MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[9]);
                            MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 21;
                            MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[17]);
                        }
                    #endif

                    break;
                
                case 0xCC: // long dest, long source
                    MACRxPacket.flags.bits.sourcePrsnt = 1;
                    #ifndef TARGET_SMALL
                        if( bIntraPAN ) // check if it is intraPAN
                    #endif
                    {
                        //rxFrame.flags.bits.intraPAN = 1;
                        #ifndef TARGET_SMALL
                            memcpy(&MACRxPacket.SourcePANID,&RxBuffer[BankIndex].Payload[3],2);
                            //MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[3];
                            //MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[4];
                        #endif
                        MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[13]);
                        MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 25;
                        MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[21]);    
                    } 
                    #ifndef TARGET_SMALL
                        else
                        {
                            memcpy(&MACRxPacket.SourcePANID,&RxBuffer[BankIndex].Payload[13],2);

                            //MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[13];
                            //MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[14];
                            MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[15]);
                            MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 27;
                            MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[23]);
                        }
                    #endif
                    break;
                
                case 0x80:      // short source only. used in beacon
                    {
                        MACRxPacket.flags.bits.broadcast = 1;
                        MACRxPacket.flags.bits.sourcePrsnt = 1;
                        MACRxPacket.altSourceAddress = true;
                        memcpy(&MACRxPacket.SourcePANID,&RxBuffer[BankIndex].Payload[3],2);

                        //MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[3];
                        //MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[4];
                        MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[5]);
                        MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 11;
                        MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[7]);
                    }
                    break;
                        
                case 0x88: // short dest, short source
                    {
                        if( RxBuffer[BankIndex].Payload[5] == 0xFF && RxBuffer[BankIndex].Payload[6] == 0xFF )
                        {
                            MACRxPacket.flags.bits.broadcast = 1;
                        }
                        MACRxPacket.flags.bits.sourcePrsnt = 1;
                        MACRxPacket.altSourceAddress = true;
                        #ifndef TARGET_SMALL
                            if( bIntraPAN == false )
                            {
                                memcpy(&MACRxPacket.SourcePANID,&RxBuffer[BankIndex].Payload[7],2);

                                //MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[7];
                                //MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[8];
                                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[9]);
                                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 15;
                                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[11]);
                            }
                            else
                        #endif
                        {
                            memcpy(&MACRxPacket.SourcePANID,&RxBuffer[BankIndex].Payload[3],2);

                            //MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[3];
                            //MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[4];
                            MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[7]);
                            MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 13;
                            MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[9]);
                        }     
                    }
                    break;
                
                case 0x8C:  // long dest, short source
                    {
                        MACRxPacket.flags.bits.sourcePrsnt = 1;
                        MACRxPacket.altSourceAddress = true;
                        #ifndef TARGET_SMALL
                            if( bIntraPAN ) // check if it is intraPAN
                        #endif
                        {
                            #ifndef TARGET_SMALL
                                memcpy(&MACRxPacket.SourcePANID,&RxBuffer[BankIndex].Payload[3],2);
                                //MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[3];
                                //MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[4];
                            #endif
                            MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[12]);
                            
                            MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 19;
                            MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[15]);   
                        }
                        #ifndef TARGET_SMALL
                            else
                            {
                                memcpy(&MACRxPacket.SourcePANID,&RxBuffer[BankIndex].Payload[12],2);

                                //MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[12];
                                //MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[13];
                                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[14]);
                                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 21;
                                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[17]);
                            }
                        #endif
                    }
                    break;
 
                
                case 0x08: //dest-short, source-none
                    // all other addressing mode will not be supported in P2P
                default:
                    // not valid addressing mode or no addressing info
                    MiMAC_DiscardPacket();
                    return false;             
            }  
            
            #ifdef ENABLE_SECURITY
                if( RxBuffer[BankIndex].Payload[0] & 0x08 )
                {
                    // if security is used, remove the security header and MIC from
                    // the payload size
                    switch(mySecurityLevel)
                    {
                        case 0x02:
                        case 0x05:
                            MACRxPacket.PayloadLen -= 21;
                            break;
                        case 0x03:
                        case 0x06:
                            MACRxPacket.PayloadLen -= 13;
                            break;
                        case 0x04:
                        case 0x07:
                            MACRxPacket.PayloadLen -= 9;
                            break;
                        case 0x01:
                            MACRxPacket.PayloadLen -= 5;
                            break;
                        default:
                            break;    
                    }
                    // remove the security header from the payload 
                    MACRxPacket.Payload = &(MACRxPacket.Payload[5]);
                    MACRxPacket.flags.bits.secEn = 1;
                }
            #else
                if( RxBuffer[BankIndex].Payload[0] & 0x08 )
                {
                    MiMAC_DiscardPacket();
                    return false;
                }
            #endif
    
            // check the frame type. Only the data and command frame type
            // are supported. Acknowledgement frame type is handled in 
            // MRF24J40 transceiver hardware.
            switch( RxBuffer[BankIndex].Payload[0] & 0x07 ) // check frame type
            {
                case 0x01:  // data frame
                    MACRxPacket.flags.bits.packetType = PACKET_TYPE_DATA;
                    break;
                case 0x03:  // command frame
                    MACRxPacket.flags.bits.packetType = PACKET_TYPE_COMMAND;
                    break;
                case 0x00:
                    // use reserved packet type to represent beacon packet
                    MACRxPacket.flags.bits.packetType = PACKET_TYPE_RESERVE;
                    break;
                default:    // not support frame type
                    MiMAC_DiscardPacket();
                    return false;
            }
            #ifndef TARGET_SMALL
                MACRxPacket.LQIValue = RxBuffer[BankIndex].Payload[RxBuffer[BankIndex].PayloadLen-2];
                MACRxPacket.RSSIValue = RxBuffer[BankIndex].Payload[RxBuffer[BankIndex].PayloadLen-1];
            #endif
            
            return true;
        }
        return false;
    }
    
    
    /************************************************************************************
     * Function:
     *      void MiMAC_DiscardPacket(void)
     *
     * Summary:
     *      This function discard the current packet received from the RF transceiver
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      discard the current packet received from the RF transceiver.
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      None
     *
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      if( true == MiMAC_ReceivedPacket() )
     *      {
     *          // handle the raw data from RF transceiver
     * 
     *          // discard the current packet
     *          MiMAC_DiscardPacket();
     *      }
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    void MiMAC_DiscardPacket(void)
    {
        //re-enable the ACKS

        if( BankIndex < BANK_SIZE )
        {
            RxBuffer[BankIndex].PayloadLen = 0;
        }    
    }
    
    
    /************************************************************************************
     * Function:
     *      bool MiMAC_SendPacket(  MAC_TRANS_PARAM transParam, 
     *                              uint8_t *MACPayload, uint8_t MACPayloadLen)
     *
     * Summary:
     *      This function transmit a packet
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      send a packet. Input parameter transParam configure the way
     *      to transmit the packet.
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      MAC_TRANS_PARAM transParam -    The struture to configure the transmission way
     *      uint8_t * MACPaylaod -             Pointer to the buffer of MAC payload
     *      uint8_t MACPayloadLen -            The size of the MAC payload
     *
     * Returns: 
     *      A boolean to indicate if a packet has been received by the RF transceiver.
     *
     * Example:
     *      <code>
     *      MiMAC_SendPacket(transParam, MACPayload, MACPayloadLen);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/    
    bool MiMAC_SendPacket( INPUT MAC_TRANS_PARAM transParam, 
                        INPUT uint8_t *MACPayload, 
                        INPUT uint8_t MACPayloadLen)
    {
        uint8_t headerLength;
        uint8_t loc = 0;
        uint8_t i = 0;
        #ifndef TARGET_SMALL
            bool IntraPAN;
        #endif
        Tick t1, t2;

    
        if( transParam.flags.bits.broadcast )
        {
            transParam.altDestAddr = true;
        }
        
        if( transParam.flags.bits.secEn )
        {
            transParam.altSrcAddr = false;
        }
        
    
        // wait for the previous transmission finish
        #if !defined(VERIFY_TRANSMIT)
            t1 = MiWi_TickGetMs();
            while( MRF24J40Status.bits.TX_BUSY ) 
            {
                if(RF_INT_PIN == 0)
                {
                    RFIF = 1;
                }

                t2 = MiWi_TickGetMs();
                t2.Val = TickGetDiffMs(t2, t1);
                if( t2.Val > TWENTY_MILI_SECOND ) // 20 ms
                {
                    InitMRF24J40();    
                    //MiMAC_SetChannel(MACCurrentChannel, 0);
                    MiMAC_SetAltAddress((uint8_t*)&myNetworkAddress, (uint8_t*)&MAC_PANID);  
                    MRF24J40Status.bits.TX_BUSY = 0;
                }
            }
        #endif
        
        // set the frame control in variable i    
        if( transParam.flags.bits.packetType == PACKET_TYPE_COMMAND )
        {
            i = 0x03;
        }
        else if( transParam.flags.bits.packetType == PACKET_TYPE_DATA)
        {
            i = 0x01;
        }
        

        // decide the header length for different addressing mode
        #ifndef TARGET_SMALL
            if( (transParam.DestPANID.Val == MAC_PANID) && (MAC_PANID != 0xFFFF) ) // this is intraPAN
        #endif
        {
            headerLength = 5;
            i |= 0x40;
            #ifndef TARGET_SMALL
                IntraPAN = true;
            #endif
        }
        
        #ifndef TARGET_SMALL
        else
        {
            headerLength = 7;
            IntraPAN = false;
        }
        #endif
        
        if( transParam.altDestAddr )
        {
            headerLength += 2;
        }
        else
        {
            headerLength += 8;
        }
        
        if( transParam.altSrcAddr )
        {
            headerLength += 2;
        }
        else
        {
            headerLength += 8;
        }
        
        #ifdef ENABLE_SECURITY
            if( transParam.flags.bits.secEn )
            {
                i |= 0x08;   
            }
        #endif
        
        if( transParam.flags.bits.ackReq && transParam.flags.bits.broadcast == false ) 
        {
            i |= 0x20;   
        }
        
        // use PACKET_TYPE_RESERVE to represent beacon. Fixed format for beacon packet
        if( transParam.flags.bits.packetType == PACKET_TYPE_RESERVE)
        {
            i = 0x00;
            headerLength = 7;
            #if !defined(TARGET_SMALL)
                IntraPAN = false;
            #endif
            transParam.altSrcAddr = true;
            transParam.flags.bits.ackReq = false;
        }
    
        // set header length
        PHYSetLongRAMAddr(loc++, headerLength);
        // set packet length
        #ifdef ENABLE_SECURITY
            if( transParam.flags.bits.secEn )
            {
                PHYSetLongRAMAddr(loc++, headerLength + MACPayloadLen + 5);
            }
            else
        #endif
        {
            PHYSetLongRAMAddr(loc++, headerLength + MACPayloadLen);
        }
        
        // set frame control LSB
        PHYSetLongRAMAddr(loc++, i);
        
        // set frame control MSB
        if( transParam.flags.bits.packetType == PACKET_TYPE_RESERVE )
        {
            PHYSetLongRAMAddr(loc++, 0x80);
            // sequence number
            PHYSetLongRAMAddr(loc++, IEEESeqNum++);
        }
        else 
        {
            if( transParam.altDestAddr && transParam.altSrcAddr )
            {
                PHYSetLongRAMAddr(loc++, 0x88);
            }
            else if( transParam.altDestAddr && transParam.altSrcAddr == 0 )
            {
                PHYSetLongRAMAddr(loc++, 0xC8);
            }
            else if( transParam.altDestAddr == 0 && transParam.altSrcAddr == 1 )
            {
                PHYSetLongRAMAddr(loc++, 0x8C);
            }
            else
            {
                PHYSetLongRAMAddr(loc++, 0xCC);
            }
            
            // sequence number
            PHYSetLongRAMAddr(loc++, IEEESeqNum++);
            
            // destination PANID     
            PHYSetLongRAMAddr(loc++, transParam.DestPANID.v[0]);
            PHYSetLongRAMAddr(loc++, transParam.DestPANID.v[1]);
            
            // destination address
            if( transParam.flags.bits.broadcast )
            {
                PHYSetLongRAMAddr(loc++, 0xFF);
                PHYSetLongRAMAddr(loc++, 0xFF);
            }
            else
            {
                if( transParam.altDestAddr )
                {
                    PHYSetLongRAMAddr(loc++, transParam.DestAddress[0]);
                    PHYSetLongRAMAddr(loc++, transParam.DestAddress[1]);
                }
                else
                {
                    for(i = 0; i < 8; i++)
                    {
                        PHYSetLongRAMAddr(loc++, transParam.DestAddress[i]);
                    }
                }
            }
        }
        
        #ifndef TARGET_SMALL
            // source PANID if necessary
            if( IntraPAN == false )
            {
                PHYSetLongRAMAddr(loc++, ((MAC_PANID>>0)&0xff));
                PHYSetLongRAMAddr(loc++, ((MAC_PANID>>8)&0xff));
            }
        #endif
        
        // source address
        if( transParam.altSrcAddr )
        {
            PHYSetLongRAMAddr(loc++, ((myNetworkAddress>>0)&0xff));
            PHYSetLongRAMAddr(loc++, ((myNetworkAddress>>8)&0xff));
        }
        else
        {
            for(i = 0; i < 8; i++)
            {
                PHYSetLongRAMAddr(loc++, MACInitParams.PAddress[i]);
            }
        }
        
        #ifdef ENABLE_SECURITY
            if( transParam.flags.bits.secEn )
            {
                // fill the additional security aux header
                for(i = 0; i < 4; i++)
                {
                    PHYSetLongRAMAddr(loc++, ((OutgoingFrameCounter>>(i<<3))&0xff) );
                }
                OutgoingFrameCounter++;
                #if defined(ENABLE_NETWORK_FREEZER)
                    if( (OutgoingFrameCounter.v[0] == 0) && ((OutgoingFrameCounter.v[1] & 0x03) == 0) )
                    {
                        nvmPutOutFrameCounter(OutgoingFrameCounter.v);
                    }    
                #endif

                PHYSetLongRAMAddr(loc++, myKeySequenceNumber);
                
                // fill the security key
                for(i = 0; i < 16; i++)
                {
                    PHYSetLongRAMAddr(0x280 + i, mySecurityKey[i]);
                }
                
                // set the cipher mode
                PHYSetShortRAMAddr(WRITE_SECCR0, mySecurityLevel);
            }
        #endif
        
        
        // write the payload
        for(i = 0; i < MACPayloadLen; i++)
        {
            PHYSetLongRAMAddr(loc++, MACPayload[i]);
        }
        
        MRF24J40Status.bits.TX_BUSY = 1;
    
        // set the trigger value
        if( transParam.flags.bits.ackReq && transParam.flags.bits.broadcast == false )
        {
            i = 0x05;
            #ifndef TARGET_SMALL
                MRF24J40Status.bits.TX_PENDING_ACK = 1;
            #endif
        }
        else
        {
            i = 0x01;
            #ifndef TARGET_SMALL
                MRF24J40Status.bits.TX_PENDING_ACK = 0;
            #endif
        }
        #ifdef ENABLE_SECURITY
            if( transParam.flags.bits.secEn )
            {
                i |= 0x02;
            }
        #endif
        
        // now trigger the transmission
        PHYSetShortRAMAddr(WRITE_TXNMTRIG, i); 
        
        #ifdef VERIFY_TRANSMIT
            t1 = MiWi_TickGetMs();
            while(1)
            {   
                if( RF_INT_PIN == 0 )
                {
                    RFIF = 1;
                }
                if( MRF24J40Status.bits.TX_BUSY == 0 )
                {
                    if( MRF24J40Status.bits.TX_FAIL )
                    {
                        MRF24J40Status.bits.TX_FAIL = 0;
                        return false;
                    }
                    break;   
                }
                t2 = MiWi_TickGetMs();
                if( TickGetDiffMs(t2, t1) > MS(40) )
                {
                    InitMRF24J40();
                    MiMAC_SetAltAddress((uint8_t*)&myNetworkAddress, (uint8_t*)&MAC_PANID);
                    MRF24J40Status.bits.TX_BUSY = 0;
                    return false;
                }
            }
        #endif
        
        return true;    
        
    }

    
    #if defined(ENABLE_ED_SCAN) 
        /************************************************************************************
         * Function:
         *      uint8_t MiMAC_ChannelAssessment(uint8_t AssessmentMode)
         *
         * Summary:
         *      This function perform the noise detection on current operating channel
         *
         * Description:        
         *      This is the primary MiMAC interface for the protocol layer to 
         *      perform the noise detection scan. Not all assessment modes are supported
         *      for all RF transceivers.
         *
         * PreCondition:    
         *      MiMAC initialization has been done.  
         *
         * Parameters: 
         *      uint8_t AssessmentMode -   The mode to perform noise assessment. The possible 
         *                              assessment modes are
         *                              * CHANNEL_ASSESSMENT_CARRIER_SENSE Carrier sense detection mode
         *                              * CHANNEL_ASSESSMENT_ENERGY_DETECT Energy detection mode
         *
         * Returns: 
         *      A byte to indicate the noise level at current channel.
         *
         * Example:
         *      <code>
         *      NoiseLevel = MiMAC_ChannelAssessment(CHANNEL_ASSESSMENT_CARRIER_SENSE);
         *      </code>
         *
         * Remarks:    
         *      None
         *
         *****************************************************************************************/     
        uint8_t MiMAC_ChannelAssessment(INPUT uint8_t AssessmentMode)
        {
            uint8_t RSSIcheck;
            
            #if defined(ENABLE_PA_LNA)
                PHYSetLongRAMAddr(TESTMODE, 0x08);              // Disable automatic switch on PA/LNA
                #if defined(MRF24J40MC)
                    PHYSetShortRAMAddr(WRITE_GPIODIR, 0x0F);        // Set GPIO direction
                    PHYSetShortRAMAddr(WRITE_GPIO, 0x0C);           // Enable LNA
                #else
                    PHYSetShortRAMAddr(WRITE_GPIODIR, 0x0F);        // Set GPIO direction
                    PHYSetShortRAMAddr(WRITE_GPIO, 0x04);           // Enable LNA
                #endif
            #endif
            
            // calculate RSSI for firmware request
            PHYSetShortRAMAddr(WRITE_BBREG6, 0x80);                                                 
            
            // Firmware Request the RSSI
            RSSIcheck = PHYGetShortRAMAddr (READ_BBREG6);
            while ((RSSIcheck & 0x01) != 0x01)
            {
                RSSIcheck = PHYGetShortRAMAddr (READ_BBREG6);
            }
            
            // read the RSSI
            RSSIcheck = PHYGetLongRAMAddr(0x210);
            
            // enable RSSI attached to received packet again after
            // the energy scan is finished
            PHYSetShortRAMAddr(WRITE_BBREG6, 0x40);
            
            #if defined(ENABLE_PA_LNA)
                #if defined(MRF24J40MC)
                    PHYSetShortRAMAddr(WRITE_GPIODIR, 0x08);
                    PHYSetShortRAMAddr(WRITE_GPIO, 0x08);
                #else
                    PHYSetShortRAMAddr(WRITE_GPIODIR, 0x00);
                    PHYSetShortRAMAddr(WRITE_GPIO, 0);
                #endif
                PHYSetLongRAMAddr(TESTMODE, 0x0F);
            #endif
            
            return RSSIcheck;
        }
    #endif
    

    #if defined(ENABLE_SLEEP)
        /************************************************************************************
         * Function:
         *      bool MiMAC_PowerState(uint8_t PowerState)
         *
         * Summary:
         *      This function puts the RF transceiver into sleep or wake it up
         *
         * Description:        
         *      This is the primary MiMAC interface for the protocol layer to 
         *      set different power state for the RF transceiver. There are minimal 
         *      power states defined as deep sleep and operating mode. Additional
         *      power states can be defined for individual RF transceiver depends
         *      on hardware design.
         *
         * PreCondition:    
         *      MiMAC initialization has been done. 
         *
         * Parameters: 
         *      uint8_t PowerState -   The power state of the RF transceiver to be set to. 
         *                          The minimum definitions for all RF transceivers are
         *                          * POWER_STATE_DEEP_SLEEP RF transceiver deep sleep mode.
         *                          * POWER_STATE_OPERATE RF transceiver operating mode.
         * Returns: 
         *      A boolean to indicate if chaning power state of RF transceiver is successful.
         *
         * Example:
         *      <code>
         *      // Put RF transceiver into sleep
         *      MiMAC_PowerState(POWER_STATE_DEEP_SLEEP);
         *      // Put MCU to sleep
         *      Sleep();
         *      // Wake up the MCU by WDT, external interrupt or any other means
         *
         *      // Wake up the RF transceiver
         *      MiMAC_PowerState(POWER_STATE_OPERATE); 
         *      </code>
         *
         * Remarks:    
         *      None
         *
         *****************************************************************************************/    
        bool MiMAC_PowerState(INPUT uint8_t PowerState)
        {
            switch(PowerState)
            {
                case POWER_STATE_DEEP_SLEEP:
                    {
                        //;clear the WAKE pin in order to allow the device to go to sleep
                        PHY_WAKE = 0;
                        
                        #if defined(ENABLE_PA_LNA)
                            PHYSetLongRAMAddr(TESTMODE, 0x08);              // Disable automatic switch on PA/LNA
                            PHYSetShortRAMAddr(WRITE_GPIODIR, 0x0F);        // Set GPIO direction
                            PHYSetShortRAMAddr(WRITE_GPIO, 0x00);           // Disable PA and LNA
                        #endif
                        
                        // make a power management reset to ensure device goes to sleep
                        PHYSetShortRAMAddr(WRITE_SOFTRST, 0x04);
                        
                        //;write the registers required to place the device in sleep
                        PHYSetShortRAMAddr(WRITE_TXBCNINTL,0x80);
                        PHYSetShortRAMAddr(WRITE_RXFLUSH,0x60);
                        PHYSetShortRAMAddr(WRITE_SLPACK,0x80);
                    }
                    break;
                    
                case POWER_STATE_OPERATE:
                    {
                        #if 1
                            // use wake pin to wake up the radio 
                            // enable the radio to wake up quicker
                            
                            uint8_t results;
                            MIWI_TICK t1, t2;
                            
                            //wake up the device
                            PHY_WAKE = 1;
                        
                            t1 = MiWi_TickGetMs();
                            
                            while(1)
                            {
                                t2 = MiWi_TickGetMs();
                                t2.Val = TickGetDiffMs(t2, t1);
                                
                                // if timeout, assume the device has waken up
                                if( t2.Val > HUNDRED_MILI_SECOND)
                                {
                                    InitMRF24J40();
                                    //MiMAC_SetChannel(MACCurrentChannel);
                                    MiMAC_SetAltAddress((uint8_t*)&myNetworkAddress, (uint8_t*)&MAC_PANID);
                                    break;
                                }
                            
                                results = PHYGetShortRAMAddr(READ_ISRSTS);
                                if((results & 0x40) != 0x00)
                                {
                                    break;
                                }
                            }
                    
                            while(1)
                            {
                                t2 = MiWi_TickGetMs();
                                t2.Val = TickGetDiffMs(t2, t1);
                                
                                // if timeout, assume the device has waken up
                                if( t2.Val > HUNDRED_MILI_SECOND )
                                {
                                    InitMRF24J40();
                                    //MiMAC_SetChannel(MACCurrentChannel);
                                    MiMAC_SetAltAddress((uint8_t*)&myNetworkAddress, (uint8_t*)&MAC_PANID);
                                    break;
                                }
                                
                                results = PHYGetLongRAMAddr(RFSTATE);
                                if( (results & 0xE0) == 0xA0 )
                                {
                                    break;
                                }
                                
                            }
                            
                            PHYSetShortRAMAddr(WRITE_RFCTL, 0x04);  
                            PHYSetShortRAMAddr(WRITE_RFCTL, 0x00);              
                        
                        #else
                        
                            // use reset to wake up the radio is more
                            // reliable
                            InitMRF24J40();
                            MiMAC_SetAltAddress((uint8_t*)&myNetworkAddress, (uint8_t*)&MAC_PANID);
                            
                        #endif
                        
                        #if defined(ENABLE_PA_LNA)
                            #if defined(MRF24J40MC)
                                PHYSetShortRAMAddr(WRITE_GPIODIR, 0x08);
                                PHYSetShortRAMAddr(WRITE_GPIO, 0x08);
                            #else
                                PHYSetShortRAMAddr(WRITE_GPIODIR, 0x00);
                                PHYSetShortRAMAddr(WRITE_GPIO, 0);
                            #endif
                            PHYSetLongRAMAddr(TESTMODE, 0x0F);
                        #endif             
                    }
                    break;
                    
                default:
                    return false;
            }
            return true;
        }
    #endif

    /************************************************************************************
     * Function:
     *      bool MiMAC_SetChannel(uint8_t channel, uint8_t offsetFreq)
     *
     * Summary:
     *      This function set the operating channel for the RF transceiver
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      set the operating frequency of the RF transceiver. Valid channel
     *      number are from 0 to 31. For different frequency band, data rate
     *      and other RF settings, some channels from 0 to 31 might be
     *      unavailable. Paramater offsetFreq is used to fine tune the center
     *      frequency across the frequency band. For transceivers that follow
     *      strict definition of channels, this parameter may be discarded.
     *      The center frequency is calculated as 
     *      (LowestFrequency + Channel * ChannelGap + offsetFreq)
     *
     * PreCondition:    
     *      Hardware initialization on MCU has been done. 
     *
     * Parameters: 
     *      uint8_t channel -  Channel number. Range from 0 to 31. Not all channels
     *                      are available under all conditions.
     *      uint8_t offsetFreq -   Offset frequency used to fine tune the center 
     *                          frequency. May not apply to all RF transceivers
     *
     * Returns: 
     *      A boolean to indicates if channel setting is successful.
     *
     * Example:
     *      <code>
     *      // Set center frequency to be exactly channel 12
     *      MiMAC_SetChannel(12, 0);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/      
    bool MiMAC_SetChannel(INPUT uint8_t channel, INPUT uint8_t offsetFreq)
    {
        if( channel < 11 || channel > 26)
        {
            return false;
        }
        
        #if defined(ENABLE_PA_LNA) && (defined(MRF24J40MB) || defined(MRF24J40MC))
            if( channel == 26 )
            {
                return false;
            }
        #endif
        
        MACCurrentChannel = channel;
        PHYSetLongRAMAddr(RFCTRL0,((channel-11)<<4)|0x03);
        PHYSetShortRAMAddr(WRITE_RFCTL,0x04);
        PHYSetShortRAMAddr(WRITE_RFCTL,0x00);   
        return true;  
    }
    
    
    /************************************************************************************
     * Function:
     *      bool MiMAC_SetPower(uint8_t outputPower)
     *
     * Summary:
     *      This function set the output power for the RF transceiver
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      set the output power for the RF transceiver. Whether the RF
     *      transceiver can adjust output power depends on the hardware
     *      implementation.
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      uint8_t outputPower -  RF transceiver output power. 
     *
     * Returns: 
     *      A boolean to indicates if setting output power is successful.
     *
     * Example:
     *      <code>
     *      // Set output power to be 0dBm
     *      MiMAC_SetPower(TX_POWER_0_DB);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    bool MiMAC_SetPower(INPUT uint8_t outputPower)
    {
        uint8_t reg;
        uint8_t tmp = outputPower % 10;
    
        reg = (outputPower / 10) << 6;
        if( tmp > 5 )
        {
            reg += 0x38;
        }
        else if( tmp > 4 )
        {
            reg += 0x30;
        }
        else if( tmp > 3 )
        {
            reg += 0x28;
        }
        else if( tmp > 2 )
        {
            reg += 0x20;
        }
        else if( tmp > 1 )
        {
            reg += 0x18;
        }
        else if( tmp > 0 )
        {
            reg += 0x10;
        }
    
        PHYSetLongRAMAddr(RFCTRL3,reg);
        return true;
    }
    
    
    /************************************************************************************
     * Function:
     *      bool MiMAC_SetAltAddress(uint8_t *Address, uint8_t *PANID)
     *
     * Summary:
     *      This function set the alternative network address and PAN identifier if
     *      applicable
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      set alternative network address and/or PAN identifier. This function
     *      call applies to only IEEE 802.15.4 compliant RF transceivers. In case
     *      alternative network address is not supported, this function will return
     *      false.
     *
     * PreCondition:    
     *      MiMAC initialization has been done. 
     *
     * Parameters: 
     *      uint8_t * Address -    The alternative network address of the host device.
     *      uint8_t * PANID -      The PAN identifier of the host device
     *
     * Returns: 
     *      A boolean to indicates if setting alternative network address is successful.
     *
     * Example:
     *      <code>
     *      uint16_t NetworkAddress = 0x0000;
     *      uint16_t PANID = 0x1234;
     *      MiMAC_SetAltAddress(&NetworkAddress, &PANID);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    bool MiMAC_SetAltAddress(INPUT uint8_t *Address, INPUT uint8_t *PANID)
    {
        memcpy(&myNetworkAddress,Address,2);
        memcpy(&MAC_PANID,PANID,2);
        
        PHYSetShortRAMAddr(WRITE_SADRL,((myNetworkAddress>>0)&0xff));
        PHYSetShortRAMAddr(WRITE_SADRH,((myNetworkAddress>>8)&0xff));
        PHYSetShortRAMAddr(WRITE_PANIDL,((MAC_PANID>>0)&0xff));
        PHYSetShortRAMAddr(WRITE_PANIDH,((MAC_PANID>>8)&0xff));
        return true;    
    }
    
    
    /************************************************************************************
     * Function:
     *      bool MiMAC_Init(MACINIT_PARAM initValue)
     *
     * Summary:
     *      This function initialize MiMAC layer
     *
     * Description:        
     *      This is the primary MiMAC interface for the protocol layer to 
     *      initialize the MiMAC layer. The initialization parameter is 
     *      assigned in the format of structure MACINIT_PARAM.
     *
     * PreCondition:    
     *      MCU initialization has been done. 
     *
     * Parameters: 
     *      MACINIT_PARAM initValue -   Initialization value for MiMAC layer
     *
     * Returns: 
     *      A boolean to indicates if initialization is successful.
     *
     * Example:
     *      <code>
     *      MiMAC_Init(initParameter);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    bool MiMAC_Init(INPUT MACINIT_PARAM initValue)
    {   
        uint8_t i;
        
        MACInitParams = initValue;

        IEEESeqNum = TMRL;
        
        MACCurrentChannel = 11;
           
        InitMRF24J40();
        
        MRF24J40Status.Val = 0;
        
        for(i = 0; i < BANK_SIZE; i++)
        {
            RxBuffer[i].PayloadLen = 0;
        }        
        #ifdef ENABLE_SECURITY
            #if defined(ENABLE_NETWORK_FREEZER)
                if( initValue.actionFlags.bits.NetworkFreezer )
                {
                    nvmGetOutFrameCounter(OutgoingFrameCounter.v);
                    OutgoingFrameCounter.Val += FRAME_COUNTER_UPDATE_INTERVAL;
                    nvmPutOutFrameCounter(OutgoingFrameCounter.v);
                }
                else
                {
                    OutgoingFrameCounter.Val = 0;
                    nvmPutOutFrameCounter(OutgoingFrameCounter.v);
                    OutgoingFrameCounter.Val = 1;
                }        
            #else
        	    OutgoingFrameCounter = 1;
        	#endif
        #endif 

        uint16_t seed=0;
        for(i=0;i<16;i++)
        {
            seed<<=1;
            seed|=((MiMAC_ChannelAssessment(CHANNEL_ASSESSMENT_CARRIER_SENSE)) & 0x01);
            //_delay(65535);
            DelayMs(10);
        }
        
        krndInit(seed);
        IEEESeqNum = krnd();
                
        return true;
    }
    
    /*********************************************************************
     * void HighISR(void)
     *
     * Overview:        This is the interrupt handler for the MRF24J40 and
     *                  P2P stack. 
     *
     * PreCondition:    None
     *
     * Input:           None
     *
     * Output:          None
     *
     * Side Effects:    Various flags and registers set.
     *
     ********************************************************************/
    #if defined(__XC8)
        void interrupt HighISR(void)
    #elif defined(__dsPIC30F__) || defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24H__)
        void _ISRFAST __attribute__((interrupt, auto_psv)) _INT1Interrupt(void)
    #elif defined(__PIC32MX__)
        void __ISR(_EXTERNAL_2_VECTOR, ipl5auto) _INT2Interrupt(void)
    #else
        void _ISRFAST _INT2Interrupt(void)
    #endif
    {
        if(RFIE && RFIF)
        {  
            uint8_t i;
            uint8_t j;
              
            //clear the interrupt flag as soon as possible such that another interrupt can
            //occur quickly.
            RFIF = 0;

            //create a new scope for the MRF24J40 interrupts so that we can clear the interrupt
            //flag quickly and then handle the interrupt that we have already received
            {
                MRF24J40_IFREG flags;        
    
                //read the interrupt status register to see what caused the interrupt        
                flags.Val=PHYGetShortRAMAddr(READ_ISRSTS);

                if(flags.bits.RF_TXIF)
                {
                    //if the TX interrupt was triggered
                    //clear the busy flag indicating the transmission was complete
                    MRF24J40Status.bits.TX_BUSY = 0;
                    
                    failureCounter = 0;
                    
                    #ifndef TARGET_SMALL
                        //if we were waiting for an ACK
                        if(MRF24J40Status.bits.TX_PENDING_ACK)
                        {
                            uint8_t results;
                            
                            //read out the results of the transmission
                            results = PHYGetShortRAMAddr(READ_TXSR);
                            
                            if((results & 0x01)>0)
                            {
                                //the transmission wasn't successful and the number
                                //of retries is located in bits 7-6 of TXSR
                                MRF24J40Status.bits.TX_FAIL = 1;
                            }

                            //transmission finished
                            //clear that I am pending an ACK, already got it
                            MRF24J40Status.bits.TX_PENDING_ACK = 0;

                        }
                    #endif
                }
                
                if(flags.bits.RF_RXIF)
                {  
                    uint8_t RxBank = 0xFF;
                    
                    for(i = 0; i < BANK_SIZE; i++)
                    {
                        if( RxBuffer[i].PayloadLen == 0 )
                        {
                            RxBank = i;
                            break;
                        }
                    }        
                          
                    //if the RX interrupt was triggered
                    if( RxBank < BANK_SIZE )
                    {
                        #ifdef ENABLE_SECURITY
                            if( MRF24J40Status.bits.RX_SECURITY )
                            {
                                uint8_t DecryptionStatus = PHYGetShortRAMAddr(READ_SECISR);
                                uint8_t FrameControl;
                                
                                MRF24J40Status.bits.RX_SECURITY = 0;
                                if( (DecryptionStatus & 0x02) != 0 )
                                {
                                    PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01);
                                    goto START_OF_SEC_INT;
                                }
                                
                                i = 6;
                                FrameControl = PHYGetLongRAMAddr(0x301);
                                if( (FrameControl & 0x40) == 0 ) // intra PAN?
                                {
                                    i += 2;
                                }
                                
                                FrameControl = PHYGetLongRAMAddr(0x302);
                                if( (FrameControl & 0x0C) == 0x0C )
                                {
                                    i += 8;
                                }
                                else
                                {
                                    i += 2;
                                }
            
                                // get the source address
                                for(j = 0; j < 8; j++)
                                {
                                    tmpSourceLongAddress.bytes[j] = PHYGetLongRAMAddr(0x300 + i + j);
                                }
                                uint8_t* v=(uint8_t*)&tmpFrameCounter;
                                for(j = 0; j < 4; j++)
                                {
                                    v[j] = PHYGetLongRAMAddr(0x308 + i + j);
                                }
                                

                                for(i = 0; i < CONNECTION_SIZE; i++)
                                {
                                    if( (ConnectionTable[i].status.bits.isValid) && 
                                        isSameAddress(ConnectionTable[i].Address, tmpSourceLongAddress) )
                                    {
                                        break;
                                    }
                                }
                                
                                if( i < CONNECTION_SIZE )
                                {
                                    if( ConnectionTable[i].IncomingFrameCounter > tmpFrameCounter )
                                    {
                                        PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01);  
                                        goto START_OF_SEC_INT;
                                    }
                                    else
                                    {
                                        if( tmpFrameCounter == 0xFFFFFFFF )
                                        {
                                            ConnectionTable[i].IncomingFrameCounter = 0;
                                        }
                                        else
                                        {
                                            ConnectionTable[i].IncomingFrameCounter = tmpFrameCounter;
                                        }                                        
                                    }
                                }

                            }
                            else
                        #endif
                        #if defined(ENABLE_SECURITY) && !defined(TARGET_SMALL)
                            if( MRF24J40Status.bits.RX_IGNORE_SECURITY )
                            {
                                MRF24J40Status.bits.RX_IGNORE_SECURITY = 0;
                                PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01);
                                goto START_OF_SEC_INT; 
                            }
                        #endif
                        //If the part is enabled for receiving packets right now
                        //(not pending an ACK)
                        //indicate that we have a packet in the buffer pending to 
                        //be read into the buffer from the FIFO
                        PHYSetShortRAMAddr(WRITE_BBREG1, 0x04);

                        //get the size of the packet
                        //2 more bytes for RSSI and LQI reading 
                        RxBuffer[RxBank].PayloadLen = PHYGetLongRAMAddr(0x300) + 2;
                        if(RxBuffer[RxBank].PayloadLen<RX_PACKET_SIZE)
                        {   
                            //indicate that data is now stored in the buffer
                            MRF24J40Status.bits.RX_BUFFERED = 1;
                            
                            //copy all of the data from the FIFO into the TxBuffer, plus RSSI and LQI
                            for(i=1;i<=RxBuffer[RxBank].PayloadLen+2;i++)
                            {
                                RxBuffer[RxBank].Payload[i-1] = PHYGetLongRAMAddr(0x300+i);
                            }
                            PHYSetShortRAMAddr(WRITE_RXFLUSH, 0x01);
                        }
                        else
                        {
                            //else it was a larger packet than we can receive
                            //flush it
                            PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01); 
                        }
    
                        // enable radio to receive next packet
                        PHYSetShortRAMAddr(WRITE_BBREG1, 0x00);
                    }
                    else
                    {
                        //else if the RX is not enabled then we need to flush this packet
                        //flush the buffer
                        PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01);
                        #if defined(ENABLE_SECURITY)
                            MRF24J40Status.bits.RX_SECURITY = 0;
                            #if !defined(TARGET_SMALL)
                                MRF24J40Status.bits.RX_IGNORE_SECURITY = 0;
                            #endif
                        #endif
                    }//end of RX_BUFFERED check
                        
                } //end of RXIF check
                
START_OF_SEC_INT:                
                if( flags.bits.SECIF )
                {
                    #ifdef ENABLE_SECURITY
                        uint8_t FrameControl;
                        if( MRF24J40Status.bits.TX_BUSY )
                        {
                            MRF24J40Status.bits.RX_IGNORE_SECURITY = 1;
                            PHYSetShortRAMAddr(WRITE_SECCR0, 0x80);
                            PHYSetShortRAMAddr(WRITE_RXFLUSH,0x01);
                            goto END_OF_RF_INT;
                        }
                        
                        #ifndef TARGET_SMALL
                            
                            // all the code below is to check the key sequence
                            // number
                            
                            i = 6;
                            FrameControl = PHYGetLongRAMAddr(0x301);
                            if( (FrameControl & 0x40) == 0 ) // intra PAN?
                            {
                                i += 2;
                            }
                            
                            FrameControl = PHYGetLongRAMAddr(0x302);
                            if( (FrameControl & 0x0C) == 0x0C )
                            {
                                i += 8;
                            }
                            else
                            {
                                i += 2;
                            }
                            
                            // wait until the key sequence number is available
                            for(j = 0; j < i + 10; j++)
                            {
                                PHYGetLongRAMAddr(0x301+j);
                            }
                            
                            // get key sequence number
                            j = PHYGetLongRAMAddr(0x30C + i);
    
                            if( j != myKeySequenceNumber )
                            {
                                PHYSetShortRAMAddr(WRITE_SECCR0, 0x80); // ignore the packet
                                MRF24J40Status.bits.RX_IGNORE_SECURITY = 1;
                            }

                            if( MRF24J40Status.bits.RX_IGNORE_SECURITY == 0 )
                        #endif
                        {
                            // supply the key
                            for(i = 0; i < 16; i++)
                            {
                                PHYSetLongRAMAddr(0x2B0 + i, mySecurityKey[i]);
                            }
                            MRF24J40Status.bits.RX_SECURITY = 1;
    
                            // set security level and trigger the decryption
                            PHYSetShortRAMAddr(WRITE_SECCR0, mySecurityLevel << 3 | 0x40);
                        }
                    #else
                        PHYSetShortRAMAddr(WRITE_SECCR0, 0x80); // ignore the packet
                        //MRF24J40Status.bits.RX_IGNORE_SECURITY = 1;
                    #endif 
                }                
            } //end of scope of RF interrupt handler
        } //end of if(RFIE && RFIF)

END_OF_RF_INT:        
       #if defined(__XC8)
            #if defined USE_IRQ0_AS_INTERRUPT
                {
                    if(PHY_IRQ0 && PHY_IRQ0_En)
                    {
                        PHY_IRQ0 = 0;
                        
                    #if !defined(TARGET_SMALL)                    
                        if(RF_Mode == RF_RECEIVER)
                        {
                            RSSIVal = (RegisterRead(RSTSREG>>8))>>1;        //Capturing the RSSiVal at SYNC/ADRS match
                        }
                    #endif
                    }
                }
            #endif
        
            //check to see if the symbol timer overflowed
            if ((INTCONbits.TMR0IF == 1) && (INTCONbits.TMR0IE == 1))
            {
                uint16_t tmr = TMR0;
                t1extension++;
                tmr += (0xffff - 6000);
                TMR0L = tmr;
                TMR0H = tmr >> 8;
                TMR0L = tmr;
                INTCONbits.TMR0IF = 0;
                t1fired=true;
            }
        
            if(PHY_IRQ0 && PHY_IRQ0_En)
            {
                PHY_IRQ0 = 0;
            } 
            //UserInterruptHandler();
        #endif

        
        return;
        
    } //end of interrupt handler
    
    
    
    #if defined(__18CXX)
        #pragma code highVector=0x08
        void HighVector (void)
        {
            _asm goto HighISR _endasm
        }
        #pragma code /* return to default code section */
        
        #pragma code lowhVector=0x18
        void LowVector (void)
        {
            _asm goto HighISR _endasm
        }
        #pragma code /* return to default code section */
    #endif

#else

    /*******************************************************************
     * C18 compiler cannot compile an empty C file. define following 
     * bogus variable to bypass the limitation of the C18 compiler if
     * a different transceiver is chosen.
     ******************************************************************/
    extern char bogusVariable;
#endif

