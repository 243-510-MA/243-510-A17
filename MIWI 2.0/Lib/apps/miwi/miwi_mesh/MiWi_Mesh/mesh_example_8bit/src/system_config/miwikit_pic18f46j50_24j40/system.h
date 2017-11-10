#ifndef _HARDWARE_H_

#define _HARDWARE_H_

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
#else

    #include <stdbool.h>
    #include <stdint.h>

#include "driver/mrf_miwi/drv_mrf_miwi_mesh_24j40.h"

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

#define MAC_nCS             LATDbits.LATD5

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