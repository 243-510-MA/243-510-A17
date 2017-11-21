#ifndef _HARDWARE_H_

#define _HARDWARE_H_

#include "miwi_config.h"

    
#define SW1             1
#define SW2             2	

    #define SYSTEM_GetSystemClock()             12000000ULL
    #define SYSTEM_GetInstructionClock()        SYSTEM_GetSystemClock ()
    #define SYSTEM_GetPeripheralClock()         SYSTEM_GetSystemClock ()
    #define _XTAL_FREQ                          SYSTEM_GetSystemClock()

    #define SetPeriodMS(a)                      (a*SYSTEM_GetSystemClock()/1000)
    //#define MS(a)                               ((uint32_t)a*SYSTEM_GetSystemClock()/1000UL)
    #define ms2Ticks(a)                         ((uint32_t)a*SYSTEM_GetSystemClock()/1000UL)
    #define us2Ticks(a)                         ((uint32_t)a*SYSTEM_GetSystemClock()/1000000ULL)
    
// There are three ways to use NVM to store data: External EPROM, Data EEPROM and
// programming space, with following definitions:
//      #define USE_EXTERNAL_EEPROM
//      #define USE_DATA_EEPROM
//      #define USE_PROGRAMMING_SPACE
// Each demo board has defined the method of using NVM, as
// required by Network Freezer feature.
#define USE_EXTERNAL_EEPROM

#define SUPPORT_TWO_SPI

// Define EEPROM_SHARE_SPI if external EEPROM shares the SPI
// bus with RF transceiver
//#define EEPROM_SHARE_SPI

//MRF89XA DEFINITIONS
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
#define PHY_IRQ1            INTCON3bits.INT1IF
#define PHY_IRQ1_En         INTCON3bits.INT1IE
#define PHY_RESETn          LATDbits.LATD0
#define PHY_RESETn_TRIS     TRISDbits.TRISD0

    // Switch and LED Pin Definitions
    #define SW0_PORT            PORTBbits.RB1
    #define SW0_TRIS            TRISBbits.TRISB1
    #define SW1_PORT            PORTBbits.RB2
    #define SW1_TRIS            TRISBbits.TRISB2



// EEProm Pin Definitions
#define RF_EEnCS            LATDbits.LATD5
#define RF_EEnCS_TRIS	    TRISDbits.TRISD5

// SPI1 Pin Definitions
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



#define LED0                LATCbits.LATC2
#define LED0_TRIS           TRISCbits.TRISC2
#define LED1                LATAbits.LATA6
#define LED1_TRIS           TRISAbits.TRISA6
#define LED2                LATAbits.LATA7
#define LED2_TRIS           TRISAbits.TRISA7

// Viper 
    #define LED0On()            LED0 = 1
    #define LED0Off()           LED0 = 0
    #define LED0Toggle()        LED0 ^= 1

    #define LED1On()            LED1 = 1
    #define LED1Off()           LED1 = 0
    #define LED1Toggle()        LED1 ^= 1

    #define LED2On()            LED2 = 1
    #define LED2Off()           LED2 = 0
    #define LED2Toggle()        LED2 ^= 1

    #define LEDxOn(a)            if(a==0){LED0On();}else if(a==1){LED1On();}else{LED2On();}
    #define LEDxOff(a)           if(a==0){LED0Off();}else if(a==1){LED1Off();}else{LED2Off();}
    #define LEDxToggle(a)        if(a==0){LED0Toggle();}else if(a==1){LED1Toggle();}else{LED2Toggle();}

// External EEPROM Pin Definitions
//#define EE_nCS_TRIS         TRISDbits.TRISD5
#define MAC_nCS             LATDbits.LATD5
#define TMRL                TMR0L

//External SST Serial Flash Definitions
#define EE_nCS              LATEbits.LATE2
#define EE_nCS_TRIS         TRISEbits.TRISE2

// LCD Pin Definitions
#define LCD_CS_TRIS         TRISDbits.TRISD7
#define LCD_CS              LATDbits.LATD7
#define LCD_RS_TRIS         TRISDbits.TRISD3
#define LCD_RS              LATDbits.LATD3
#define LCD_RESET_TRIS      TRISEbits.TRISE0
#define LCD_RESET           LATEbits.LATE0
#define LCD_BKLT_TRIS       TRISEbits.TRISE1
#define LCD_BKLT            LATEbits.LATE1


// Viper
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
    //bool isButtonPressed(unsigned char index);
    void ButtonTask(void);
    void LEDCommand(LED_t led, LEDCmd_t cmd);
    void LEDManagerTask(void);
#endif