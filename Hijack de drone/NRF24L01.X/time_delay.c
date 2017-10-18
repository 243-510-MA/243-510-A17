/*
 * File:   time_delay.c
 * Author: http://pic18fxx.blogspot.com
 */
#include <xc.h>
#include "time_delay.h"
#define _XTAL_FREQ 64000000 // Select your XTAL

void Delay_ms(unsigned int count){
unsigned int i;
for(i = 0; i < count; i++){
__delay_ms(1);
}
}

void Delay_us(unsigned int count){
unsigned int i;
for(i = 0; i < count; i++){
__delay_us(1);
}
}