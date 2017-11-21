#ifndef _MSPI_H_

#define _MSPI_H_

#define HARDWARE_SPI

#ifdef __PIC32MX__
#else
	#define SUPPORT_TWO_SPI
#endif
void SPIPut(uint8_t v);
uint8_t SPIGet(void);
void SPIPut2(uint8_t v);
uint8_t SPIGet2(void);

#endif