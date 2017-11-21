#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __PIC32MX__
    #include <plib.h>
#endif
#include "system.h"
#include "symbol.h"


#ifdef __PIC32MX__

uint32_t upperTick=0;

void InitSymbolTimer()
{
    CloseTimer2();
    WriteTimer2(0x00);
    WriteTimer3(0x00);
    WritePeriod3(0xFFFF);
    upperTick=0;
    OpenTimer2((T2_ON|T2_32BIT_MODE_ON|T2_PS_1_1),0xFFFFFFFF);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_5 | T3_INT_SUB_PRIOR_2);
}

uint64_t MiWi_TickGet(void)
{
    uint64_t currentTime;
    uint32_t uTick,puTick;
    uint64_t tickNow;

    do
    {
        uTick=upperTick;
        currentTime=TMR2;
        puTick=upperTick;
    }while(uTick!=puTick);
    currentTime|=((uint64_t)uTick<<32);
    tickNow=currentTime;
    return tickNow;
}
volatile Tick glbTmrNowms;
MIWI_TICK MiWi_TickGetMs(void) //Returns ms, not ticks!  Done for backwards compat.
{
    Tick tNow;
    tNow.Val=MiWi_TickGet();
    tNow.Val/=(uint64_t)ms2Ticks(1);
    glbTmrNowms.Val=tNow.Val;
    return tNow;
}

uint32_t safeMiWi_TickGetMs(void)
{
    uint64_t currentTime;
    uint32_t uTick,puTick;

    do
    {
        uTick=upperTick;
        currentTime=TMR2;
        puTick=upperTick;
    }while(uTick!=puTick);
    currentTime|=((uint64_t)uTick<<32);
    return(currentTime/(uint64_t)ms2Ticks(1));
}

void __ISR(_TIMER_3_VECTOR, ipl5auto) T3Interrupt ( void)
{
    upperTick++;
    INTClearFlag(INT_T3);
}


void TimerDisable(void)
{
    T2CONbits.TON=0;
}

void TimerEnable(void)
{
    T2CONbits.TON=1;
}
#elif defined(__PIC24F__)

uint32_t upperTick=0;

void InitSymbolTimer()
{
    upperTick=0;
    T2CON = 0b0000000000001000;
    T2CONbits.TON = 1;
}

uint64_t MiWi_TickGet(void)
{
    uint64_t currentTime;
    uint32_t uTick,puTick;
    uint64_t tickNow;
    uint16_t tTick,ptTick;

    do
    {
        uTick=upperTick;
        tTick=TMR3;
        currentTime=TMR2 | ((uint32_t)(TMR3)<<16);
        ptTick=TMR3;
        puTick=upperTick;
    }while( (uTick!=puTick) || (tTick!=ptTick) );
    currentTime|=((uint64_t)uTick<<32);
    tickNow=currentTime;
    return tickNow;
}
volatile Tick glbTmrNowms;
MIWI_TICK MiWi_TickGetMs(void) //Returns ms, not ticks!  Done for backwards compat.
{
    Tick tNow;
    tNow.Val=MiWi_TickGet();
    tNow.Val/=(uint64_t)ms2Ticks(1);
    glbTmrNowms.Val=tNow.Val;
    return tNow;
}

uint32_t safeMiWi_TickGetMs(void)
{
    uint64_t currentTime;
    uint32_t uTick,puTick;

    do
    {
        uTick=upperTick;
        currentTime=TMR2;
        puTick=upperTick;
    }while(uTick!=puTick);
    currentTime|=((uint64_t)uTick<<32);
    return(currentTime/(uint64_t)ms2Ticks(1));
}

void _ISRFAST __attribute__((interrupt, auto_psv)) _T2Interrupt(void)
{
    upperTick++;
    _T2IF=0;
}


void TimerDisable(void)
{
    T2CONbits.TON=0;
}

void TimerEnable(void)
{
    T2CONbits.TON=1;
}


    
#else

#define TMR_IF          INTCONbits.TMR0IF
#define TMR_IE          INTCONbits.TMR0IE
#define TMR_IP          INTCON2bits.TMR0IP
#define TMR_ON          T0CONbits.TMR0ON
#define TMR_CON         T0CON
#define TMR_L           TMR0L
#define TMR_H           TMR0H
#define TMR_            TMR0



uint32_t t1extension=0;
volatile bool t1fired=false;
MIWI_TICK currentTime;

void InitSymbolTimer()
{
    TMR_CON = 0x00;//prescaler 1:256
    TMR_IP = 1;
    TMR_IF = 0;
    TMR_IE = 1;
    TMR_ON = 1;

    t1extension=0;
}

uint32_t safeMiWi_TickGetMs(void)
{
    
    return(0);
}

MIWI_TICK MiWi_TickGetMs(void) //Returns ms, not ticks!  Done for backwards compat.
{
    uint32_t tnow=MiWi_TickGet();
    //tnow<<=8;
    //tnow/=12000ull;
    currentTime.Val=tnow;
    return(currentTime);
}

uint64_t MiWi_TickGet(void)
{
    uint32_t tmrval=0;
    uint32_t t1ext;
    
    do
    {
        t1fired=false;
        //tmrval=TMR_;
        t1ext=t1extension;
    } while(t1fired==true);
    
    tmrval|=(((uint32_t)t1ext));
    return(tmrval);
}

void TimerDisable(void)
{
    TMR_IE=0;
}
void TimerEnable(void)
{
    TMR_IE=1;
}

void TimerEnterCritical(void)
{
    TimerDisable();
}

void TimerExitCritical(void)
{
    TimerEnable();
}
#endif
