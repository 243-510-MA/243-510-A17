#ifndef __CONFIG_APP_H_
#define __CONFIG_APP_H_

#define MEM_ACK_INFO_SIZE   5

#define HARDWARE_SPI

//------------------------------------------------------------------------
// Definition of RF Transceiver. ONLY ONE TRANSCEIVER CAN BE CHOSEN
//------------------------------------------------------------------------

    /*********************************************************************/
    // Definition of MRF24J40 enables the application to use Microchip
    // MRF24J40 2.4GHz IEEE 802.15.4 compliant RF transceiver. Only one
    // RF transceiver can be defined.
    /*********************************************************************/
#ifdef MRF24J40_COMPILE
    #define MRF24J40
#endif
    
    /*********************************************************************/
    // Definition of MRF49XA enables the application to use Microchip
    // MRF49XA subGHz proprietary RF transceiver. Only one RF transceiver
    // can be defined.
    /*********************************************************************/
    //#define MRF49XA
    
    
    /*********************************************************************/
    // Definition of MRF89XA enables the application to use Microchip
    // MRF89XA subGHz proprietary RF transceiver
    /*********************************************************************/
#ifdef MRF89XA_COMPILE
    #define MRF89XA

#endif
    //#define USE_IRQ0_AS_INTERRUPT

#define USE_UNIQUE_MAC_ADDRESS
/*********************************************************************/
// MY_ADDRESS_LENGTH defines the size of wireless node permanent 
// address in byte. This definition is not valid for IEEE 802.15.4
// compliant RF transceivers.
/*********************************************************************/
#ifdef MRF89XA_COMPILE
    #define MY_ADDRESS_LENGTH       2   //(sizeof(addr_t))
#endif

#ifdef MRF24J40_COMPILE
    #define MY_ADDRESS_LENGTH       8   //(sizeof(addr_t))
#endif

/*********************************************************************/
// EUI_x defines the xth byte of permanent address for the wireless
// node
/*********************************************************************/
#define EUI_7 0x11
#define EUI_6 0x22
#define EUI_5 0x33
#define EUI_4 0x44
#define EUI_3 0x55
#define EUI_2 0x66
#define EUI_1 0x77
#define EUI_0 0x04

/*********************************************************************/
// TX_BUFFER_SIZE defines the maximum size of application payload
// which is to be transmitted
/*********************************************************************/
#define TX_BUFFER_SIZE 54
#define RADIO_BUFFER_SIZE   64
/*********************************************************************/
// RX_BUFFER_SIZE defines the maximum size of application payload
// which is to be received
/*********************************************************************/
#define RX_BUFFER_SIZE 90//Used for RX of the data (max packet length)

/*********************************************************************/
// MY_PAN_ID defines the PAN identifier. Use 0xFFFF if prefer a 
// random PAN ID.
/*********************************************************************/
#define MY_PAN_ID                       0x1234

/*********************************************************************/
// ENABLE_PA_LNA enable the external power amplifier and low noise
// amplifier on the RF board to achieve longer radio communication 
// range. To enable PA/LNA on RF board without power amplifier and
// low noise amplifier may be harmful to the transceiver.
/*********************************************************************/
//#define ENABLE_PA_LNA

/*********************************************************************/
// ENABLE_SLEEP will enable the device to go to sleep and wake up 
// from the sleep
/*********************************************************************/
#define ENABLE_SLEEP


/*********************************************************************/
// ENABLE_ED_SCAN will enable the device to do an energy detection scan
// to find out the channel with least noise and operate on that channel
/*********************************************************************/
#define ENABLE_ED_SCAN

/*********************************************************************/
// ENABLE_ED_SCAN will enable the device to do an energy detection scan
// to find out the channel with least noise and operate on that channel
/*********************************************************************/
#define ENABLE_ACTIVE_SCAN

#ifdef ENABLE_ACTIVE_SCAN
    #define ACTIVE_SCAN_RESULT_SIZE 2
#endif


/*********************************************************************/
// ENABLE_SECURITY will enable the device to encrypt and decrypt
// information transferred
/*********************************************************************/
#define ENABLE_SECURITY

#if defined(MRF89XA) && defined(ENABLE_SECURITY)
    #define SOFTWARE_SECURITY
#endif

#define CONNECTION_SIZE     5
#define PAYLOAD_START       0


#if MY_ADDRESS_LENGTH > 8
    #error "Maximum address length is 8"
#endif

#if MY_ADDRESS_LENGTH < 2
    #error "Minimum address length is 2"
#endif

#if defined(MRF24J40)
    #define PROTOCOL_HEADER_SIZE    0
    #define IEEE_802_15_4
    #undef MY_ADDRESS_LENGTH
    #define MY_ADDRESS_LENGTH 8
#endif

#endif
