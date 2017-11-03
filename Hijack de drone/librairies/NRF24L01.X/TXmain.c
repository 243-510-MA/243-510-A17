/*
 * File:   TXmain.c
 * Author: pic18fxx.blogspot.com
 * 
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f46k22.h>
#include "bit_settings.h"
#include "spi_pic18f.h"
#include "nrf24l01.h"
#include "hd44780.h"
#include "time_delay.h"

void main(void) {
unsigned char i;
char buffer1[20];
unsigned char bufferTX[32];
OSCCON = 0b01110000;
OSCTUNEbits.PLLEN = 1; // turn on the PLL 64 MHz
ANSELA = 0; ANSELB = 0; ANSELC = 0; ANSELD = 0; ANSELE = 0;
PORTA  = 0; PORTB  = 0; PORTC  = 0; PORTD  = 0; PORTE  = 0;
TRISA  = 0; TRISB  = 0; TRISC  = 0; TRISD  = 0; TRISE  = 0;
SPI1_Close();
SPI1_Init();
Lcd_Init();
Lcd_Command(LCD_CLEAR);
Lcd_Command(LCD_CURSOR_OFF);
Lcd_ConstText(1, 1, "nRF24l01 TX Mode");

NRF24L01_Init(TX_MODE, 0x40); 
while(1)
  {
   bufferTX[0] = 1;   
   for(i = 0; i < 8; i++)
      {
       NRF24L01_SendData(bufferTX); 
       sprintf(buffer1, "Sending: %03d", bufferTX[0]);
       Lcd_Text(2, 3, buffer1);
       bufferTX[0] = bufferTX[0] << 1;
       Delay_ms(300);  
      }
   bufferTX[0] = 128;
   for(i = 0; i < 6; i++)
      {
       bufferTX[0] = bufferTX[0] >> 1;
       NRF24L01_SendData(bufferTX); 
       sprintf(buffer1, "Sending: %03d", bufferTX[0]);
       Lcd_Text(2, 3, buffer1);
       Delay_ms(300);  
      }
  }
while(1){}
}