/*
 * File:   nrf24l01.h
 * Author: pic18fxx.blogspot.com
 *
 */

#ifndef NRF24L01_H
#define	NRF24L01_H

//==============================================================================
// Connection.
//==============================================================================
// VCC  to 3.3V
// GND  to GND
// SCK  to SCK
// MOSI to SDO
// MISO to SDI
// CE You can set a different conection for this pin. 
// CSN You can set a different conection for this pin.
//==============================================================================
#define NRF24L01_CE         LATDbits.LATD0
#define NRF24L01_TRIS_CE    TRISDbits.TRISD0

#define NRF24L01_CSN        LATDbits.LATD1
#define NRF24L01_TRIS_CSN   TRISDbits.TRISD1

#define PAYLOAD_BYTES       1 // Sets the bytes to send or read.

//==============================================================================
// Register Map.
//==============================================================================
#define RX_MODE             1
#define TX_MODE             2
#define R_REGISTER          0x00  
#define W_REGISTER          0x20  
#define R_RX_PAYLOAD        0x61  
#define W_TX_PAYLOAD        0xA0  
#define FLUSH_TX            0xE1  
#define FLUSH_RX            0xE2  
#define REUSE_TX_PL         0xE3  

#define CONFIG              0x00  
#define EN_AA               0x01  
#define EN_RXADDR           0x02  
#define SETUP_AW            0x03  
#define SETUP_RETR          0x04  
#define RF_CH               0x05  
#define RF_SETUP            0x06  
#define STATUS              0x07  
#define OBSERVE_TX          0x08  
#define CD                  0x09  
#define RX_ADDR_P0          0x0A  
#define RX_ADDR_P1          0x0B  
#define RX_ADDR_P2          0x0C  
#define RX_ADDR_P3          0x0D  
#define RX_ADDR_P4          0x0E  
#define RX_ADDR_P5          0x0F  
#define TX_ADDR             0x10  
#define RX_PW_P0            0x11  
#define RX_PW_P1            0x12  
#define RX_PW_P2            0x13  
#define RX_PW_P3            0x14  
#define RX_PW_P4            0x15  
#define RX_PW_P5            0x16  
#define FIFO_STATUS         0x17  

//==============================================================================
// Function Prototypes.
//==============================================================================
void NRF24L01_WriteRegister(unsigned char Mnemonic, unsigned char value);
unsigned char NRF24L01_ReadRegister(unsigned char Mnemonic);
void NRF24L01_WriteBuffer(unsigned char data, unsigned char *buffer, unsigned char bytes);
void NRF24L01_ReadBuffer(unsigned char data, unsigned char *buffer, unsigned char bytes);
void NRF24L01_Init(unsigned char mode, unsigned char rf_channel);
void NRF24L01_SetMode(unsigned char mode);
void NRF24L01_SendData(unsigned char *buffer);
unsigned char NRF24L01_DataReady(void);
void NRF24L01_ReadData(unsigned char *buffer);
void NRF24L01_SetChannel(unsigned char rf_channel);
unsigned char NRF24L01_GetChannel(void);
void NRF24L01_StandbyI(void);
void NRF24L01_Flush(void);

#endif	/* NRF24L01_H */

