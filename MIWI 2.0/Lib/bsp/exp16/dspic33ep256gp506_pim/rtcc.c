/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

/* NOTE: This device doesn't have an actual RTCC.  We will be emulating the
 * functionality of the RTCC using the TIMER_1MS BSP.  Please include
 * the timer_1ms.c/.h files in your project and give enough clients
 * to that module for your application, plus one for this module.
 * 
 * Additionally make sure the TIMER_1MS module is initialized before
 * Initializing this module.
 */
#include <xc.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "rtcc.h"
#include "timer_1ms.h"

static uint8_t RTCC_DecToBCD (uint8_t value);
static uint8_t RTCC_BCDToDec (uint8_t value);
static void SecondEventHandler ( void );

static volatile RTCC_DATETIME datetime;
static volatile bool modified;

void RTCC_BuildTimeGet(RTCC_DATETIME *dateTime)
{
    uint8_t weekday;
    uint8_t month;
    uint8_t y;
    uint8_t dateTable[] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

    dateTime->second =  (((__TIME__[6]) & 0x0f) << 4) | ((__TIME__[7]) & 0x0f);
    dateTime->minute =  (((__TIME__[3]) & 0x0f) << 4) | ((__TIME__[4]) & 0x0f);
    dateTime->hour = (((__TIME__[0]) & 0x0f) << 4) | ((__TIME__[1]) & 0x0f);
    dateTime->day =  (((__DATE__[4]) & 0x0f) << 4) | ((__DATE__[5]) & 0x0f);
    dateTime->year = (((__DATE__[9]) & 0x0f) << 4) | ((__DATE__[10]) & 0x0f);

    //Set the month
    switch(__DATE__[0])
    {
        case 'J':
            //January, June, or July
            switch(__DATE__[1])
            {
                case 'a':
                    //January
                    month = 0x01;
                    break;
                case 'u':
                    switch(__DATE__[2])
                    {
                        case 'n':
                            //June
                            month = 0x06;
                            break;
                        case 'l':
                            //July
                            month = 0x07;
                            break;
                    }
                    break;
            }
            break;
        case 'F':
            month = 0x02;
            break;
        case 'M':
            //March,May
            switch(__DATE__[2])
            {
                case 'r':
                    //March
                    month = 0x03;
                    break;
                case 'y':
                    //May
                    month = 0x05;
                    break;
            }
            break;
        case 'A':
            //April, August
            switch(__DATE__[1])
            {
                case 'p':
                    //April
                    month = 0x04;
                    break;
                case 'u':
                    //August
                    month = 0x08;
                    break;
            }
            break;
        case 'S':
            month = 0x09;
            break;
        case 'O':
            month = 0x10;
            break;
        case 'N':
            month = 0x11;
            break;
        case 'D':
            month = 0x12;
            break;
    }

    dateTime->month = month;

    // Start with weekday = 6.  This value is valid for this algorithm for this century.
    weekday = 6;
    // y = year
    y = ((dateTime->year >> 4) * 10) + (dateTime->year & 0x0f);
    // Weekday = base day + year + x number of leap days
    weekday += y + (y / 4);
    // If the current year is a leap year but it's not March, subtract 1 from the date
    if (((y % 4) == 0) && (month < 3))
    {
        weekday -= 1;
    }
    // Add an offset based on the current month
    weekday += dateTable[month - 1];
    // Add the current day in the month
    weekday += ((dateTime->day >> 4) * 10) + (dateTime->day & 0x0f);
    weekday = weekday % 7;

    dateTime->weekday = weekday;
    
    if (!dateTime->bcdFormat)
    {
        dateTime->year = RTCC_BCDToDec (dateTime->year);
        dateTime->month = RTCC_BCDToDec (dateTime->month);
        dateTime->day = RTCC_BCDToDec (dateTime->day);
        dateTime->weekday = RTCC_BCDToDec (dateTime->weekday);
        dateTime->hour = RTCC_BCDToDec (dateTime->hour);
        dateTime->minute = RTCC_BCDToDec (dateTime->minute);
        dateTime->second = RTCC_BCDToDec (dateTime->second);
    }
}

void RTCC_Initialize (RTCC_DATETIME * value)
{    
    if (value->bcdFormat)
    {
        datetime.year = RTCC_BCDToDec (value->year);
        datetime.month = RTCC_BCDToDec (value->month);
        datetime.day = RTCC_BCDToDec (value->day);
        datetime.weekday = RTCC_BCDToDec (value->weekday);
        datetime.hour = RTCC_BCDToDec (value->hour);
        datetime.minute = RTCC_BCDToDec (value->minute);
        datetime.second = RTCC_BCDToDec (value->second);
    }
    else
    {
        datetime.weekday = value->weekday;
        datetime.day = value->day;
        datetime.month = value->month;
        datetime.year = value->year;
        datetime.hour = value->hour;
        datetime.minute = value->minute;
        datetime.second = value->second;
    }
    datetime.bcdFormat = false;
    
    while(TIMER_RequestTick(&SecondEventHandler, 1000) == false)
    {
    }
}

void RTCC_TimeGet (RTCC_DATETIME * value)
{
    RTCC_DATETIME currentTime;
        
    do
    {
        modified = false;        
        currentTime.bcdFormat = datetime.bcdFormat;
        currentTime.weekday = datetime.weekday;
        currentTime.day = datetime.day;
        currentTime.month = datetime.month;
        currentTime.year = datetime.year;
        currentTime.hour = datetime.hour;
        currentTime.minute = datetime.minute;
        currentTime.second = datetime.second;
    } while(modified == true);
          
    if (value->bcdFormat == true)
    {
        value->year = RTCC_DecToBCD (currentTime.year);
        value->month = RTCC_DecToBCD (currentTime.month);
        value->day = RTCC_DecToBCD (currentTime.day);
        value->weekday = RTCC_DecToBCD (currentTime.weekday);
        value->hour = RTCC_DecToBCD (currentTime.hour);
        value->minute = RTCC_DecToBCD (currentTime.minute);
        value->second = RTCC_DecToBCD (currentTime.second);
    }
    else
    {
        value->year = currentTime.year;
        value->month = currentTime.month;
        value->day = currentTime.day;
        value->weekday = currentTime.weekday;
        value->hour = currentTime.hour;
        value->minute = currentTime.minute;
        value->second = currentTime.second;
    }
}

// Note : value must be < 100
static uint8_t RTCC_DecToBCD (uint8_t value)
{
    return (((value / 10)) << 4) | (value % 10);
}

static uint8_t RTCC_BCDToDec (uint8_t value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}

void inline __attribute__((deprecated)) BSP_RTCC_Initialize (BSP_RTCC_DATETIME * value)
{
    RTCC_Initialize(value);
}

void inline __attribute__((deprecated)) BSP_RTCC_TimeGet (BSP_RTCC_DATETIME * value)
{
    RTCC_TimeGet(value);
}

static void SecondEventHandler ( void )
{   
    modified = true;

    /* update seconds */
    if ( datetime.second < 59 )
    {
        datetime.second++ ;
        return;
    }
    datetime.second = 0x00 ;
    
    if ( datetime.minute < 59 )
    {
        datetime.minute++ ;
        return;
    }
    datetime.minute = 0x00 ;
    
    if ( datetime.hour < 23 )
    {
        datetime.hour ++ ;
        return;
    }
    datetime.hour = 0x00 ;
    
    datetime.weekday++;
    if(datetime.weekday == 7 )
    {
        datetime.weekday = 0;
    }
    
    switch(datetime.month)
    {
        /* February */
        case 2:
            if(datetime.day < 29)
            {
                /* 29 is okay on leap years ('00, '04, '08, etc.) */
                if((datetime.year%4) == 0)
                {
                    datetime.day++;
                    return;
                }
            }
            break;
            
        /* April, June, September, November */
        case 4:
        case 6:
        case 9:
        case 11:
            if(datetime.day < 30)
            {
                datetime.day++;
                return;
            }
            break;
            
        /* January, March, May, July, August, October, December */
        default:
            if(datetime.day < 31)
            {
                datetime.day++;
                return;
            }
            break;
    }
    
    /* If we made it here, then we have rolled over on days. */
    datetime.day = 1;
    
    if(datetime.month < 12)
    {
        datetime.month++;
        return;
    }
    datetime.month = 1;
    
    datetime.year++;    
}


