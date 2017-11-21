/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

#include "miwi_config.h"

#ifdef __PIC32MX__

    #define SYSTEM_GetSystemClock()             48000000ULL
    #define SYSTEM_GetInstructionClock()        SYSTEM_GetSystemClock ()
    #define SYSTEM_GetPeripheralClock()         SYSTEM_GetSystemClock ()

    #define SetPeriodMS(a)                      (a*SYSTEM_GetSystemClock()/1000)
    //#define MS(a)                               ((uint64_t)a*SYSTEM_GetSystemClock()/1000UL)
    #define ms2Ticks(a)                         ((uint64_t)a*SYSTEM_GetSystemClock()/1000UL)
    #define us2Ticks(a)                         ((uint64_t)a*SYSTEM_GetSystemClock()/1000000ULL)

    #define IRQ1_INT_PIN        PORTAbits.RA2
    #define IRQ1_INT_TRIS       TRISAbits.TRISA2
    #if defined(_COORD) && !defined(USE_RFD_HW)
        #define PHY_IRQ1_CLR()      (IFS0CLR = _IFS0_INT1IF_MASK)
        #define PHY_IRQ1_En_SET()   (IEC0SET = _IEC0_INT1IE_MASK)
        #define PHY_IRQ1_En_CLR()   (IEC0CLR = _IEC0_INT1IE_MASK)

        #define PHY_IRQ1            IFS0bits.INT1IF
        #define PHY_IRQ1_En         IEC0bits.INT1IE
    #else
        #define PHY_IRQ1_En_SET()   (IEC0SET = _IEC0_INT2IE_MASK)
        #define PHY_IRQ1_En_CLR()   (IEC0CLR = _IEC0_INT2IE_MASK)
        #define PHY_IRQ1_CLR()      (IFS0CLR = _IFS0_INT2IF_MASK)
        #define PHY_IRQ1            IFS0bits.INT2IF
        #define PHY_IRQ1_En         IEC0bits.INT2IE
    #endif

    #define Config_nCS          LATAbits.LATA1
    #define Config_nCS_TRIS     TRISAbits.TRISA1
    #define Data_nCS            LATAbits.LATA0
    #define Data_nCS_TRIS       TRISAbits.TRISA0
    #define IRQ0_INT_PIN        PORTBbits.RB3
    #define IRQ0_INT_TRIS       TRISBbits.TRISB3
    #define PHY_RESETn          LATBbits.LATB7
    #define PHY_RESETn_TRIS     TRISBbits.TRISB7

    #define RF_EEnCS_TRIS       TRISBbits.TRISB0
    #define RF_EEnCS            LATBbits.LATB0

    #define PHY_IRQ0            IFS0bits.INT1IF //dummy interrupt
    #define PHY_IRQ0_En         IFS0bits.INT1IF

    #define SDI_TRIS            TRISBbits.TRISB13
    #define SDO_TRIS            TRISBbits.TRISB2
    #define SCK_TRIS            TRISBbits.TRISB15

    #define LED0_TRIS           TRISBbits.TRISB14
    #define LED0_IO             LATBbits.LATB14

    #define LED1_TRIS           TRISBbits.TRISB8
    #define LED1_IO             LATBbits.LATB8


    #define LED0On()            LED0_IO=1
    #define LED0Off()           LED0_IO=0
    #define LED0Toggle()        LED0_IO^=1

    #define LED1On()            LED1_IO=1
    #define LED1Off()           LED1_IO=0
    #define LED1Toggle()        LED1_IO^=1

    #define LED2Off()           

    #define LEDxOn(a)            if(a==0){LED0On();}else{LED1On();}
    #define LEDxOff(a)           if(a==0){LED0Off();}else{LED1Off();}
    #define LEDxToggle(a)        if(a==0){LED0Toggle();}else{LED1Toggle();}
#elif defined(__PIC24F__)
    #define SYSTEM_GetSystemClock()             32000000ULL
    #define SYSTEM_GetInstructionClock()        (SYSTEM_GetSystemClock ()/2)
    #define SYSTEM_GetPeripheralClock()         (SYSTEM_GetSystemClock ()/2)

    #define SetPeriodMS(a)                      (a*SYSTEM_GetPeripheralClock()/1000)
    //#define MS(a)                               ((uint64_t)a*SYSTEM_GetPeripheralClock()/1000UL)
    #define ms2Ticks(a)                         ((uint64_t)a*SYSTEM_GetPeripheralClock()/1000UL)
    #define us2Ticks(a)                         ((uint64_t)a*SYSTEM_GetPeripheralClock()/1000000ULL)

    #define IRQ1_INT_PIN        PORTEbits.RE9
    #define IRQ1_INT_TRIS       TRISEbits.TRISE9
    #if defined(_COORD) && !defined(USE_RFD_HW)
        #define PHY_IRQ1            IFS1bits.INT2IF
        #define PHY_IRQ1_En         IEC1bits.INT2IE

        #define PHY_IRQ1_CLR()      (PHY_IRQ1 = 0)
        #define PHY_IRQ1_En_SET()   (PHY_IRQ1_En = 1)
        #define PHY_IRQ1_En_CLR()   (PHY_IRQ1_En = 0)
    #else
        #define PHY_IRQ1            IFS1bits.INT2IF
        #define PHY_IRQ1_En         IEC1bits.INT2IE

        #define PHY_IRQ1_CLR()      (PHY_IRQ1 = 0)
        #define PHY_IRQ1_En_SET()   (PHY_IRQ1_En = 1)
        #define PHY_IRQ1_En_CLR()   (PHY_IRQ1_En = 0)
    #endif


    // MAC_EEPROM 
    #define MAC_nCS                     LATFbits.LATF1
    #define MAC_nCS_TRIS                 TRISFbits.TRISF1
    #define Config_nCS          LATBbits.LATB1
    #define Config_nCS_TRIS     TRISBbits.TRISB1
    #define Data_nCS            LATBbits.LATB2
    #define Data_nCS_TRIS       TRISBbits.TRISB2
    #define IRQ0_INT_PIN        PORTEbits.RE8 
    #define IRQ0_INT_TRIS       TRISEbits.TRISE8
    #define PHY_RESETn          LATGbits.LATG2
    #define PHY_RESETn_TRIS     TRISGbits.TRISG2

    #define RF_EEnCS_TRIS       TRISDbits.TRISD12
    #define RF_EEnCS            LATDbits.LATD12

    #define PHY_IRQ0            IFS3bits.INT3IF //dummy interrupt on MRF89XA
    #define PHY_IRQ0_En         IFS3bits.INT3IF

    #define SDI_TRIS            TRISFbits.TRISF7
    #define SDO_TRIS            TRISFbits.TRISF8
    #define SPI_SDO             LATFbits.LATF8
    #define SCK_TRIS            TRISFbits.TRISF6 
    #define SPI_SCK             LATFbits.LATF6 

    #define LED0_TRIS           TRISAbits.TRISA7
    #define LED0_IO             LATAbits.LATA7

    #define LED1_TRIS           TRISAbits.TRISA6
    #define LED1_IO             LATAbits.LATA6


    #define LED0On()            LED0_IO=1
    #define LED0Off()           LED0_IO=0
    #define LED0Toggle()        LED0_IO^=1

    #define LED1On()            LED1_IO=1
    #define LED1Off()           LED1_IO=0
    #define LED1Toggle()        LED1_IO^=1

    #define LED2Off()           

    #define LEDxOn(a)            if(a==0){LED0On();}else{LED1On();}
    #define LEDxOff(a)           if(a==0){LED0Off();}else{LED1Off();}
    #define LEDxToggle(a)        if(a==0){LED0Toggle();}else{LED1Toggle();}


    // Switch and LED Pin Definitions
    #define SW0_PORT            PORTDbits.RD6
    #define SW0_TRIS            TRISDbits.TRISD6
    #define SW1_PORT            PORTDbits.RD7
    #define SW1_TRIS            TRISDbits.TRISD7


    #define PHY_CS_TRIS         Data_nCS_TRIS
    #define PHY_CS              Data_nCS
    #define PHY_WAKE            LATDbits.LATD3
    #define PHY_WAKE_TRIS       TRISDbits.TRISD3

    #define TMRL    TMR2

    #ifdef MRF89XA
        #ifndef RF_INT_PIN
            #define RF_INT_PIN  IRQ1_INT_PIN
            #define RF_INT_TRIS IRQ1_INT_TRIS
        #endif
        #ifndef RFIE
            #define RFIE        PHY_IRQ1_En
            #define RFIF        PHY_IRQ1
        #endif
    #endif
    #ifdef MRF24J40
            #define RF_INT_PIN  PORTEbits.RE8
            #define RF_INT_TRIS TRISEbits.TRISE8
            #define RFIE        IEC1bits.INT1IE
            #define RFIF        IFS1bits.INT1IF
    #endif

    //LCD IO below
    #define LCD_DATA0_TRIS		(TRISEbits.TRISE0)		// Multiplexed with LED6
    #define LCD_DATA0_IO		(LATEbits.LATE0)
    #define LCD_DATA1_TRIS		(TRISEbits.TRISE1)
    #define LCD_DATA1_IO		(LATEbits.LATE1)
    #define LCD_DATA2_TRIS		(TRISEbits.TRISE2)
    #define LCD_DATA2_IO		(LATEbits.LATE2)
    #define LCD_DATA3_TRIS		(TRISEbits.TRISE3)		// Multiplexed with LED3
    #define LCD_DATA3_IO		(LATEbits.LATE3)
    #define LCD_DATA4_TRIS		(TRISEbits.TRISE4)		// Multiplexed with LED2
    #define LCD_DATA4_IO		(LATEbits.LATE4)
    #define LCD_DATA5_TRIS		(TRISEbits.TRISE5)
    #define LCD_DATA5_IO		(LATEbits.LATE5)
    #define LCD_DATA6_TRIS		(TRISEbits.TRISE6)
    #define LCD_DATA6_IO		(LATEbits.LATE6)
    #define LCD_DATA7_TRIS		(TRISEbits.TRISE7)
    #define LCD_DATA7_IO		(LATEbits.LATE7)
    #define LCD_RD_WR_TRIS		(TRISDbits.TRISD5)
    #define LCD_RD_WR_IO		(LATDbits.LATD5)
    #define LCD_RS_TRIS			(TRISBbits.TRISB15)
    #define LCD_RS_IO			(LATBbits.LATB15)
    #define LCD_E_TRIS			(TRISDbits.TRISD4)
    #define LCD_E_IO			(LATDbits.LATD4)


#else

    #include <stdbool.h>
    #include <stdint.h>

#include "Microchip/Include/Transceivers/MRF24J40/MRF24J40.h"

    #define SYSTEM_GetSystemClock()             12000000ULL
    #define SYSTEM_GetInstructionClock()        SYSTEM_GetSystemClock ()
    #define SYSTEM_GetPeripheralClock()         SYSTEM_GetSystemClock ()
    #define _XTAL_FREQ                          SYSTEM_GetSystemClock()

    #define SetPeriodMS(a)                      (a*SYSTEM_GetSystemClock()/1000)
    //#define MS(a)                               ((uint32_t)a*SYSTEM_GetSystemClock()/1000UL)
    #define ms2Ticks(a)                         ((uint32_t)a*SYSTEM_GetSystemClock()/1000UL)
    #define us2Ticks(a)                         ((uint32_t)a*SYSTEM_GetSystemClock()/1000000ULL)

    //#define IRQ1_INT_PIN        PORTBbits.RB0
    //#define IRQ1_INT_TRIS       TRISBbits.TRISB0

    //#define Config_nCS          LATAbits.LATA1
    //#define Config_nCS_TRIS     TRISAbits.TRISA1
    //#define Data_nCS            LATAbits.LATA0
    //#define Data_nCS_TRIS       TRISAbits.TRISA0
    //#define IRQ0_INT_PIN        PORTBbits.RB3
    //#define IRQ0_INT_TRIS       TRISBbits.TRISB3
#ifdef RFIF
    #define PHY_RESETn          LATBbits.LATB7
    #define PHY_RESETn_TRIS     TRISBbits.TRISB7
#endif
    //#define RF_EEnCS_TRIS       TRISBbits.TRISB0
    //#define RF_EEnCS            LATBbits.LATB0
    #define RF_EEnCS_TRIS       TRISDbits.TRISD5
    #define RF_EEnCS            LATDbits.LATD5

    #define PHY_IRQ0            INTCONbits.INT0IF
    #define PHY_IRQ0_En         INTCONbits.INT0IE

    //#define SDI_TRIS            TRISBbits.TRISB5
    //#define SDO_TRIS            TRISBbits.TRISB1
    //#define SCK_TRIS            TRISBbits.TRISB2

                #define SPI_SDI             PORTBbits.RB5               
                #define SDI_TRIS            TRISBbits.TRISB5
                #define SPI_SDO             LATCbits.LATC7               
                #define SDO_TRIS            TRISCbits.TRISC7
                #define SPI_SCK             LATBbits.LATB4               
                #define SCK_TRIS            TRISBbits.TRISB4

                // SPI2 Pin Definitions
                #define SPI_SDI2            PORTDbits.RD6               
                #define SDI2_TRIS           TRISDbits.TRISD6
                #define SPI_SDO2            LATDbits.LATD2               
                #define SDO2_TRIS           TRISDbits.TRISD2
                #define SPI_SCK2            LATDbits.LATD4               
                #define SCK2_TRIS           TRISDbits.TRISD4

                #define SPI2SSPIF           PIR3bits.SSP2IF
                #define SPI2WCOL            SSP2CON1bits.WCOL
                #define SPI2SSPBUF          SSP2BUF

    #define LED0_TRIS           TRISCbits.TRISC2
    #define LED0_IO             LATCbits.LATC2

    #define LED1_TRIS           TRISAbits.TRISA6
    #define LED1_IO             LATAbits.LATA6

    #define LED2_TRIS			TRISAbits.TRISA7
    #define LED2_IO				LATAbits.LATA7

    #define LED0On()            LED0_IO=1
    #define LED0Off()           LED0_IO=0
    #define LED0Toggle()        LED0_IO^=1

    #define LED1On()            LED1_IO=1
    #define LED1Off()           LED1_IO=0
    #define LED1Toggle()        LED1_IO^=1

    #define LED2On()            LED2_IO=1
    #define LED2Off()           LED2_IO=0
    #define LED2Toggle()        LED2_IO^=1

    #define LEDxOn(a)            if(a==0){LED0On();}else if(a==1){LED1On();}else{LED2On();}
    #define LEDxOff(a)           if(a==0){LED0Off();}else if(a==1){LED1Off();}else{LED2Off();}
    #define LEDxToggle(a)        if(a==0){LED0Toggle();}else if(a==1){LED1Toggle();}else{LED2Toggle();}

    // Switch and LED Pin Definitions
    #define SW0_PORT            PORTBbits.RB1
    #define SW0_TRIS            TRISBbits.TRISB1
    #define SW1_PORT            PORTBbits.RB2
    #define SW1_TRIS            TRISBbits.TRISB2

#ifndef RFIF 
    	#define Data_nCS            LATAbits.LATA5
    	#define Data_nCS_TRIS       TRISAbits.TRISA5  
    	#define Config_nCS          LATDbits.LATD1
        #define Config_nCS_TRIS     TRISDbits.TRISD1 

        #define IRQ0_INT_PIN        PORTBbits.RB0
        #define IRQ0_INT_TRIS       TRISBbits.TRISB0
    	#define PHY_IRQ0            INTCONbits.INT0IF
    	#define PHY_IRQ0_En         INTCONbits.INT0IE
    	
        #define IRQ1_INT_PIN        PORTCbits.RC6
        #define IRQ1_INT_TRIS       TRISCbits.TRISC6  
        #define PHY_IRQ1_En_SET()   (INTCON3bits.INT1IE=1)
        #define PHY_IRQ1_En_CLR()   (INTCON3bits.INT1IE=0)
        #define PHY_IRQ1_CLR()      (INTCON3bits.INT1IF=0)
        #define PHY_IRQ1            INTCON3bits.INT1IF
        #define PHY_IRQ1_En         INTCON3bits.INT1IE


        //#define RFIF                INTCONbits.INT0IF
        //#define RFIE                INTCONbits.INT0IE
        #define PHY_CS              LATAbits.LATA5
        #define PHY_CS_TRIS         TRISAbits.TRISA5
        //#define RF_INT_PIN          PORTBbits.RB0
        //#define RF_INT_TRIS         TRISBbits.TRISB0
        #define PHY_WAKE            LATDbits.LATD1
        #define PHY_WAKE_TRIS       TRISDbits.TRISD1
        #define PHY_RESETn          LATDbits.LATD0
        #define PHY_RESETn_TRIS     TRISDbits.TRISD0


        // LCD Pin Definitions
        #define LCD_CS_TRIS         TRISDbits.TRISD7
        #define LCD_CS              LATDbits.LATD7
        #define LCD_RS_TRIS         TRISDbits.TRISD3
        #define LCD_RS              LATDbits.LATD3
        #define LCD_RESET_TRIS      TRISEbits.TRISE0
        #define LCD_RESET           LATEbits.LATE0
        #define LCD_BKLT_TRIS       TRISEbits.TRISE1
        #define LCD_BKLT            LATEbits.LATE1

		#define EE_nCS_TRIS         TRISDbits.TRISD5
        #define EE_nCS              LATDbits.LATD5

#ifdef MRF24J40
        #define RFIE        INTCONbits.INT0IE
        #define RFIF        INTCONbits.INT0IF
        #define RF_INT_PIN  PORTBbits.RB0
        #define RF_INT_TRIS TRISBbits.TRISB0
#else
        #define RFIE        PHY_IRQ1_En
        #define RFIF        PHY_IRQ1
        #define RF_INT_PIN  IRQ1_INT_PIN
        #define RF_INT_TRIS IRQ1_INT_TRIS
#endif
        #define TMRL        TMR1

#endif

#endif


    typedef enum LED_t_def
    {
        LEDGreen,
        LEDYellow,
        LEDRed,
        LEDCount,
    }LED_t;

    typedef enum LEDCmd_t_def
    {
        lCMD_None,
        lCMD_Off,
        lCMD_On,
        lCMD_Blink,
        lCMD_Toggle,
    }LEDCmd_t;


    void HardwareInit(void);
    bool isButtonPressed(uint8_t index);
    void ButtonTask(void);
    void LEDCommand(LED_t led, LEDCmd_t cmd);
    void LEDManagerTask(void);
#endif