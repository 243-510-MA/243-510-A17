/*
 * File:   RXmain.c
 * Author: pic18fxx.blogspot.com
 * 
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f46k22.h>
#include "spi_pic18f.h"
#include "bit_settings.h"
#include "nrf24l01.h"
#include "hd44780.h"
#include "time_delay.h"

void main(void) {
unsigned char bufferRX[32];
OSCCON = 0b01110000;
OSCTUNEbits.PLLEN = 1; // turn on the PLL 64 MHz
ANSELA = 0; ANSELB = 0; ANSELC = 0; ANSELD = 0; ANSELE = 0;
PORTA  = 0; PORTB  = 0; PORTC  = 0; PORTD  = 0; PORTE  = 0;
TRISA  = 0; TRISB  = 0; TRISC  = 0; TRISD  = 0; TRISE  = 0;
SPI1_Close();
SPI1_Init();

NRF24L01_Init(RX_MODE, 0x40); 
while(1)
  {
   while(NRF24L01_DataReady()){}
   NRF24L01_ReadData(bufferRX);
   PORTB = bufferRX[0];
  }
while(1){}
}