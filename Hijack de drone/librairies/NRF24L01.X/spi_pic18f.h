/*
 * File:   spi_pic18f.h
 * Author: http://pic18fxx.blogspot.com 
 */
#ifndef SPI_PIC18F_H
#define	SPI_PIC18F_H

#define TRIS_SCK1   TRISCbits.TRISC3         
#define TRIS_SDO1   TRISCbits.TRISC5
#define TRIS_SDI1   TRISCbits.TRISC4
#define TRIS_SS1    TRISAbits.TRISA5

void SPI1_Init(void);
void SPI1_Close(void);
void SPI1_Write(unsigned char data);
unsigned char SPI1_Read(void);

#endif	/* SPI_PIC18F_H */