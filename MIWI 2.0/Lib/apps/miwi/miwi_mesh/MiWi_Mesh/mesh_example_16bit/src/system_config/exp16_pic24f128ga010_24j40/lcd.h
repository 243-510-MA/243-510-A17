#ifndef _LCD_H_

#define _LCD_H_

#if defined(__XC8) || defined(__PIC24F__)
    extern uint8_t LCDText[];
    extern uint8_t value[];
    void LCDUpdate(void);
    void LCDBacklightON(void);
    void LCDBacklightOFF(void);
    void LCDInit(void);

    #define LinePrint(index,format, arg)    sprintf((char*)&LCDText[index],format,arg); \
                                            LCDUpdate()

    #define LinePrint2(index,format, arg1,arg2)    sprintf((char*)&LCDText[index],format,arg1,arg2); \
                                            LCDUpdate()

    #define LinePrint3(index,format, arg1,arg2,arg3)    sprintf((char*)&LCDText[index],format,arg1,arg2,arg3); \
                                            LCDUpdate()
#else
    #define LinePrint(a,b,c)
    #define LinePrint2(a,b,c,d)
    #define LinePrint3(a,b,c,d,e)
    #define LCDInit()
#endif

#endif