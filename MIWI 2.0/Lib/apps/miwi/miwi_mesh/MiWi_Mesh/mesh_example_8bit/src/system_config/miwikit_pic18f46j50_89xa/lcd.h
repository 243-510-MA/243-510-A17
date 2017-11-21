#ifndef _LCD_H_

#define _LCD_H_

#ifdef __XC8
    extern uint8_t LCDText[];
    extern uint8_t value[];
    void LCDUpdate(void);
    void LCDBacklightON(void);
    void LCDBacklightOFF(void);

    #define LinePrint(index,format, arg)    sprintf(&LCDText[index],format,arg); \
                                            LCDUpdate()

    #define LinePrint2(index,format, arg1,arg2)    sprintf(&LCDText[index],format,arg1,arg2); \
                                            LCDUpdate()

    #define LinePrint3(index,format, arg1,arg2,arg3)    sprintf(&LCDText[index],format,arg1,arg2,arg3); \
                                            LCDUpdate()
#else
    #define LinePrint(a,b,c)
    #define LinePrint2(a,b,c,d)
    #define LinePrint3(a,b,c,d,e)
#endif

#endif