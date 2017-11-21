#ifndef _G_TYPEDEFS_H_

#define _G_TYPEDEFS_H_

#include <stdint.h>
#include <stdbool.h>


#define ROM     const

#ifdef __PIC32MX__
    #define _PACKED __attribute__ ((packed))
    #define _extension_ __extension__
#else
    #define _extension_
    #define uint64_t uint32_t
    #define _PACKED
#endif

typedef union
{
    uint16_t Val;
    uint8_t v[2];
    struct
    {
        uint8_t LB;
        uint8_t HB;
    } uint8_t;
    struct
    {
        _extension_ uint8_t b0:1;
        _extension_ uint8_t b1:1;
        _extension_ uint8_t b2:1;
        _extension_ uint8_t b3:1;
        _extension_ uint8_t b4:1;
        _extension_ uint8_t b5:1;
        _extension_ uint8_t b6:1;
        _extension_ uint8_t b7:1;
        _extension_ uint8_t b8:1;
        _extension_ uint8_t b9:1;
        _extension_ uint8_t b10:1;
        _extension_ uint8_t b11:1;
        _extension_ uint8_t b12:1;
        _extension_ uint8_t b13:1;
        _extension_ uint8_t b14:1;
        _extension_ uint8_t b15:1;
    } bits;
} _uint16_VAL, uint16_BITS;

typedef union
{
    uint32_t Val;
    uint16_t w[2];
} uint32_VAL;

#define WORD_VAL    uint16_VAL

#ifdef __XC32
    #define ClrWdt()    ClearWDT()
    #define __delay_ms      DelayMs
#endif

uint32_t krnd(void);
void krndInit(uint16_t value);

//typedef struct addr_t_def
//{
//    uint8_t bytes[MY_ADDRESS_LENGTH];
//}addr_t;

#endif