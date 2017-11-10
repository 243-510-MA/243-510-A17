#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __PIC32MX__
    #include <plib.h>
#endif
#include "miwi_config.h"
#include "system.h"

#include "symbol.h"
#ifdef __PIC32MX__
    #if defined(_COORD) && !defined(USE_RFD_HW)
        #pragma config JTAGEN = OFF

        // USERID = No Setting
        #pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
        #pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
        #pragma config FUSBIDIO = OFF           // USB USID Selection (Controlled by Port Function)
        #pragma config FVBUSONIO = OFF          // USB VBUS ON Selection (Controlled by Port Function)

        // DEVCFG2

        #pragma config FPLLIDIV = DIV_1         // PLL Input Divider (3x Divider)
        #pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
        #pragma config UPLLIDIV = DIV_1         // USB PLL Input Divider (3x Divider)

        #pragma config FPLLMUL = MUL_24         // PLL Multiplier (24x Multiplier)
        #pragma config UPLLEN = ON              // USB PLL Enable (Enabled)
        #pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

        // DEVCFG1
        #pragma config FNOSC = PRIPLL //FRCPLL   //PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
        #pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
        #pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
        #pragma config POSCMOD = EC             // Primary Oscillator Configuration (XT osc mode)

        #pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
        #pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
        #pragma config WDTPS = PS1              // Watchdog Timer Postscaler (1:1)
        #pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
        #pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
        //#pragma config FWDTWINSZ = WISZ_25      // Watchdog Timer Window Size (Window Size is 25%)

        // DEVCFG0
        #pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)
        #pragma config PWP = OFF                // Program Flash Write Protect (Disable)
        #pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
        #pragma config CP = ON
    #else

        #pragma config JTAGEN = OFF

        // USERID = No Setting
        #pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
        #pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
        #pragma config FUSBIDIO = OFF           // USB USID Selection (Controlled by Port Function)
        #pragma config FVBUSONIO = OFF          // USB VBUS ON Selection (Controlled by Port Function)

        // DEVCFG2

        #pragma config FPLLIDIV = DIV_2         // PLL Input Divider (3x Divider)
        #pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
        #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (3x Divider)

        #pragma config FPLLMUL = MUL_24         // PLL Multiplier (24x Multiplier)
        #pragma config UPLLEN = ON              // USB PLL Enable (Enabled)
        #pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

        // DEVCFG1
        #pragma config FNOSC = FRCPLL   //PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
        #pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
        #pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
        #pragma config POSCMOD = OFF             // Primary Oscillator Configuration (XT osc mode)

        #pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
        #pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
        #pragma config WDTPS = PS1              // Watchdog Timer Postscaler (1:1)
        #pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
        #pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
        //#pragma config FWDTWINSZ = WISZ_25      // Watchdog Timer Window Size (Window Size is 25%)

        // DEVCFG0
        #pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)
        #pragma config PWP = OFF                // Program Flash Write Protect (Disable)
        #pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
        #pragma config CP = ON
    #endif
#else
    // CONFIG1L
    #pragma config WDTEN = OFF       // Watchdog Timer (Enabled)
    #pragma config PLLDIV = 2       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset (Enabled)
    #pragma config XINST = OFF      // Extended Instruction Set (Disabled)

    // CONFIG1H
    #pragma config CPUDIV = OSC1    // CPU System Clock Postscaler (No CPU system clock divide)
    #pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

    // CONFIG2L
    #pragma config OSC = INTOSCPLL  // Oscillator (INTOSCPLL)
    #pragma config T1DIG = ON       // T1OSCEN Enforcement (Secondary Oscillator clock source may be selected)
    #pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
    #pragma config FCMEN = ON       // Fail-Safe Clock Monitor (Enabled)
    #pragma config IESO = ON        // Internal External Oscillator Switch Over Mode (Enabled)

    // CONFIG2H
    #pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

    // CONFIG3L
    #pragma config DSWDTOSC = INTOSCREF// DSWDT Clock Select (DSWDT uses INTRC)
    #pragma config RTCOSC = INTOSCREF// RTCC Clock Select (RTCC uses INTRC)
    #pragma config DSBOREN = ON     // Deep Sleep BOR (Enabled)
    #pragma config DSWDTEN = ON     // Deep Sleep Watchdog Timer (Enabled)
    #pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

    // CONFIG3H
    #pragma config IOL1WAY = OFF    // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set and cleared as needed)
    #pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

    // CONFIG4L
    #pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
    #pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
    #pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

    // CONFIG4H
    #pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)


#endif

//uint16_t savedSPIBRG=0;

#ifdef __PIC32MX__

void HardwareInit(void)
{
    
    SYSTEMConfigWaitStatesAndPB ( SYSTEM_GetSystemClock ());
    CheKseg0CacheOn ();
    ANSELA=0;
    ANSELB=0;

    SDI2Rbits.SDI2R=3;//RPB13
    RPB2Rbits.RPB2R=4;//SDO2
#if defined(_COORD) && !defined(USE_RFD_HW)
    //Had to reroute the INT line to RPA3 (INT1)
    INT1Rbits.INT1R=0;//INT1 to RPA3
#else
    INT2Rbits.INT2R=0;//INT2 to RPA2
#endif
    Data_nCS = 1;
    Config_nCS = 1;

    Data_nCS_TRIS = 0;
    Config_nCS_TRIS = 0;

    IRQ0_INT_TRIS = 1;
    IRQ1_INT_TRIS = 1;

    //PHY_IRQ0 = 0;           // MRF89XA
    //PHY_IRQ0_En = 1;        // MRF89XA
    PHY_IRQ1 = 0;           // MRF89XA
    PHY_IRQ1_En = 1;        // MRF89XA
    /*******************************************************************/
    // Confiure SPI1
    /*******************************************************************/
    SDI_TRIS = 1;
    SDO_TRIS = 0;
    SCK_TRIS = 0;

    PHY_RESETn = 1;
    PHY_RESETn_TRIS = 0;

    DelayMs(100);

    PHY_RESETn=0;

    DelayMs(100);

#if defined(_COORD) && !defined(USE_RFD_HW)
    INTClearFlag(INT_INT1);
    ConfigINT1(EXT_INT_PRI_5 | RISING_EDGE_INT | EXT_INT_DISABLE);
#else
    INTClearFlag(INT_INT2);
    ConfigINT2(EXT_INT_PRI_5 | RISING_EDGE_INT | EXT_INT_DISABLE);
#endif
    OpenTimer1(T1_ON | T1_PS_1_1 | T1_SOURCE_INT, SetPeriodMS(1));
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_4 | T1_INT_SUB_PRIOR_2);

    SpiChnOpen(SPI_CHANNEL2, SPI_OPEN_MSTEN | SPI_OPEN_CKE_REV | SPI_OPEN_ON, 4);
    SpiChnSetBitRate(SPI_CHANNEL2, SYSTEM_GetPeripheralClock(), 1000000);

    INTEnableSystemMultiVectoredInt ();


    LED0_TRIS=0;
    LED1_TRIS=0;

}

void __ISR(_TIMER_1_VECTOR, ipl4auto) T1Interrupt ( void)
{
    
    INTClearFlag(INT_T1);
}
uint32_t savedSPIBRG;
void HardwareSaveSPIBaudSetValue(uint32_t baud)
{
    savedSPIBRG=SPI2BRG;
    SpiChnSetBitRate(SPI_CHANNEL2, SYSTEM_GetPeripheralClock(), baud);
}

void HardwareRestoreSPIBaud(void)
{
    if(savedSPIBRG!=0)
    {
        SPI2BRG=savedSPIBRG;
        savedSPIBRG=0;
    }
}

void ButtonTask(void)
{

}

bool isButtonPressed(uint8_t index)
{
    return(false);
}

#else


    void HardwareInit(void)
    {
		/*******************************************************************/
		// Primary Internal Oscillator
		/*******************************************************************/
		OSCCON = 0x78;
		OSCTUNEbits.PLLEN = 1;
		WDTCONbits.SWDTEN = 0;

		/*******************************************************************/
		// Configure PPS Related Pins
		/*******************************************************************/
		// Unlock to config PPS
		EECON2 = 0x55;
		EECON2 = 0xAA;
		PPSCONbits.IOLOCK = 0;

		RPINR1 = 17;            // Mapping IRQ1 to RC6(RP17) * Used for MRF89XA
		RPINR21 = 23;			// Mapping SDI2 to RD6(RP23)
		RPOR21 = 10;			// Mapping SCK2 to RD4(RP21)
		RPOR19 = 9;			    // Mapping SDO2 to RD2(RP19)
	
		// Lock System
		EECON2 = 0x55;
		EECON2 = 0xAA;
		PPSCONbits.IOLOCK = 1;

		/*******************************************************************/
		// AN0 & AN1 Analog Pins others Digital Pins
		/*******************************************************************/
		ANCON0 = 0xFC;
		ANCON1 = 0x1F;

		/*******************************************************************/
		// Configure Switch and LED I/O Ports
		/*******************************************************************/
		LED0_TRIS=0;
        LED1_TRIS=0;
        LED2_TRIS=0;
        /*LED_1 = 0;
		LED_2 = 0;
		LED_3 = 0;
		LED1_TRIS = 0;
		LED2_TRIS = 0;
		LED3_TRIS = 0;
            
		SW0_TRIS = 1;
		SW1_TRIS = 1;*/
							
		INTCON2bits.RBPU = 0;   // Enable PORTB Pull-ups for Switches

		/*******************************************************************/
		// Configure the Temp Sensor and VBat port
		/*******************************************************************/	
		TRISAbits.TRISA1 = 1;
		TRISAbits.TRISA0 = 1;
    
		/*******************************************************************/
		// Config RF Radio
		/*******************************************************************/
		#if defined(MRF24J40)
			/*******************************************************************/
			// Config MRF24J40 Pins
			/*******************************************************************/
			PHY_CS = 1;
			PHY_RESETn = 1;    
			PHY_WAKE = 1;
    
			PHY_CS_TRIS = 0;
			PHY_RESETn_TRIS = 0;        
			PHY_WAKE_TRIS = 0;
			RF_INT_TRIS = 1;
             
			// Config INT0 Edge = Falling
			INTCON2bits.INTEDG0 = 0;
    
			RFIF = 0;
			RFIE = 1;
    
			/*******************************************************************/
			// Confiure SPI1
			/*******************************************************************/     
			SDI_TRIS = 1;
			SDO_TRIS = 0;
			SCK_TRIS = 0;
    
			SSP1STAT = 0xC0;
			SSP1CON1 = 0x20;
   
		#elif defined(MRF89XA)
			/*******************************************************************/
			// Config MRF89XA Pins
			/*******************************************************************/ 
			Data_nCS = 1;
			Config_nCS = 1;
    
			Data_nCS_TRIS = 0; 
			Config_nCS_TRIS = 0;   
    
			IRQ0_INT_TRIS = 1;
			IRQ1_INT_TRIS = 1;
    
			// Config IRQ1 Edge = Rising
			INTCON2bits.INTEDG1 = 1;
    
			// Config IRQ0 Edge = Falling
			INTCON2bits.INTEDG0 = 1;
    
			PHY_IRQ0 = 0;           // MRF89XA
			PHY_IRQ0_En = 1;        // MRF89XA  
			PHY_IRQ1 = 0;           // MRF89XA
			PHY_IRQ1_En = 1;        // MRF89XA    
			/*******************************************************************/
			// Confiure SPI1
			/*******************************************************************/   
			SDI_TRIS = 1;
			SDO_TRIS = 0;
			SCK_TRIS = 0;
    
			SSP1STAT = 0xC0;
			SSP1CON1 = 0x22;

            /*******************************************************************/
            // Configure SPI2
            /*******************************************************************/   
            SDI2_TRIS = 1;
            SDO2_TRIS = 0;
            SCK2_TRIS = 0;
            
            RPINR1 = 17;            // Mapping IRQ1 to RC6(RP17) * Used for MRF89XA
            RPINR21 = 23;			// Mapping SDI2 to RD6(RP23)
            RPOR21 = 10;			// Mapping SCK2 to RD4(RP21)
            RPOR19 = 9;			    // Mapping SDO2 to RD2(RP19)

            SSP2STAT = 0x00;
            SSP2CON1 = 0x31;

            PIR3bits.SSP2IF = 0;
            
            
		#endif
    
		/*******************************************************************/    
		// Configure EEProm Pins
		/*******************************************************************/
		RF_EEnCS = 1;
		RF_EEnCS_TRIS = 0;
    
		/*******************************************************************/
		// Configure LCD Pins
		/*******************************************************************/
		LCD_BKLT = 0;
		LCD_CS = 1;
		LCD_RS = 0;
		LCD_RESET = 0;
		LCD_BKLT_TRIS = 0;
		LCD_CS_TRIS = 0;
		LCD_RS_TRIS = 0;
		LCD_RESET_TRIS = 0;
    
		/*******************************************************************/
		// Configure SPI2
		/*******************************************************************/   
		SDI2_TRIS = 1;
		SDO2_TRIS = 0;
		SCK2_TRIS = 0;
    
		SSP2STAT = 0x00;
		SSP2CON1 = 0x32;
        
        EE_nCS_TRIS=0;
        EE_nCS=1;
    
		PIR3bits.SSP2IF = 0; 

		/*******************************************************************/
		// Enable System Interupts
		/*******************************************************************/
		INTCONbits.GIEH = 1;
    }

    void HardwareSaveSPIBaudSetValue(uint32_t baud)
    {
    
    }
    void HardwareRestoreSPIBaud(void)
    {
    
    }

    uint8_t switches=0;
    
    void ButtonTask(void)
    {
        static Tick tStart;
        static uint8_t s0Count=0,s1Count=0;
        static uint8_t pS0=0,pS1=0;
        uint8_t Sw;
        if(TickGetDiffMs(TickGetMs(),tStart)>10)
        {
            Sw=SW0_PORT;
            if(pS0==Sw)
            {
                if(s0Count<5)
                {
                    s0Count++;
                }else
                {
                    if(pS0==0)
                    {
                        switches|=(1<<0);
                    }else
                    {
                        switches&=(~(1<<0));
                    }
                }
            }
            pS0=Sw;
            
            Sw=SW1_PORT;
            if(pS1==Sw)
            {
                if(s1Count<5)
                {
                    s1Count++;
                }else
                {
                    if(pS1==0)
                    {
                        switches|=(1<<1);
                    }else
                    {
                        switches&=(~(1<<1));
                    }
                }
            }
            pS1=Sw;
        }        
    }
    
    bool isButtonPressed(uint8_t index)
    {
        uint8_t mask=(1<<index);
        return((switches & mask)>0);
    }
#endif
    
//LED Manager
typedef enum LEDState_t_def
{
    LED_POR,
    LED_Off,
    LED_Blink,
    LED_DelayActive,
    LED_DelayInactive,
    LED_On,
}LEDState_t;



LEDState_t LEDState[LEDCount]={LED_POR,LED_POR};
LEDCmd_t LEDCmd[LEDCount]={lCMD_None,lCMD_None};

void LEDManagerTask(void)
{
    uint16_t i;
    static LEDState_t lNextState[LEDCount];
    static Tick tStart[LEDCount];

    for(i=0;i<LEDCount;i++)
    {
        switch(LEDState[i])
        {
            case LED_POR:
                LEDxOff(i);
                LEDState[i]=LED_Off;
                break;

            case LED_Off:
                if(LEDCmd[i]==lCMD_On)
                {
                    LEDxOn(i);
                    LEDState[i]=LED_On;
                }else if(LEDCmd[i]==lCMD_Blink)
                {
                    tStart[i]=TickGetMs();
                    LEDState[i]=LED_DelayActive;
                    lNextState[i]=LED_Off;
                    LEDxOn(i);
                }else if(LEDCmd[i]==lCMD_Toggle)
                {
                    LEDxOn(i);
                    LEDState[i]=LED_On;
                }
                LEDCmd[i]=lCMD_None;
                break;

            case LED_DelayActive:
                if(TickGetDiffMs(TickGetMs(),tStart[i])>MS(20))
                {
                    LEDState[i]=LED_DelayInactive;
                    tStart[i]=TickGetMs();
                    if(lNextState[i]==LED_Off)
                    {
                        LEDxOff(i);
                    }else
                    {
                        LEDxOn(i);
                    }
                }
                break;

            case LED_DelayInactive:
                if(TickGetDiffMs(TickGetMs(),tStart[i])>MS(20))
                {
                    LEDState[i]=lNextState[i];
                }
                break;

            case LED_On:
                if(LEDCmd[i]==lCMD_Off)
                {
                    LEDxOff(i);
                    LEDState[i]=LED_Off;
                }else if(LEDCmd[i]==lCMD_Blink)
                {
                    tStart[i]=TickGetMs();
                    LEDState[i]=LED_DelayActive;
                    lNextState[i]=LED_On;
                    LEDxOff(i);
                }else if(LEDCmd[i]==lCMD_Toggle)
                {
                    LEDxOff(i);
                    LEDState[i]=LED_Off;
                }
                LEDCmd[i]=lCMD_None;
                break;
        }
    }
}

void LEDCommand(LED_t led, LEDCmd_t cmd)
{
    if( (LEDCmd[led]!=lCMD_On) && (LEDCmd[led]!=lCMD_Off) )
    {
        LEDCmd[led]=cmd;
    }
}

void BlinkRedLED(void)
{
    LEDCommand(LEDRed,lCMD_Blink);
}
