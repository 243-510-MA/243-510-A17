#ifndef _TIME_DELAY_H_

#define _TIME_DELAY_H_

void DelayMs(uint32_t ms);
void DelayUS(uint32_t delay);

#define Delay10us(a) DelayUS(10*a)

#endif