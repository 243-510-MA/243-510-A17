#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "system.h"

void DelayUS(uint32_t delay)
{
#ifdef __PIC32MX__
    uint32_t targetCount;
    uint32_t bakupCount;

    // backup current count of the core timer.
    bakupCount = ReadCoreTimer();
    // Core timer increments every 2 sys clock cycles.
    // Calculate the counts required to complete "us".
    targetCount = ((SYSTEM_GetPeripheralClock()/1000000)/2) * delay;
    // Restart core timer.
    WriteCoreTimer(0);
    // Wait till core timer completes the count.
    while(ReadCoreTimer() < targetCount);
    // Restore count back.
    WriteCoreTimer(bakupCount + targetCount);
#else
    //#warning Fix Delay
    uint32_t i;
    //delay>>=1;
    for(i=0;i<delay;i++)
    {
        Nop();
    }
#endif
}

void DelayMs(uint32_t ms)
{
    DelayUS(ms*1000);
}

