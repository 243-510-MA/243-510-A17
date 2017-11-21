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

#include <leds.h>
#include <stdbool.h>
#include <p24FJ256GB106.h>

/*********************************************************************
* Function: void LED_On(LED led);
*
* Overview: Turns requested LED on
*
* PreCondition: LED configured via LED_Configure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_On(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_On(LED led)
{
    switch(led)
    {
        case LED_GREEN:
            TRISG &= ~0x0300;
            break;
        case LED_BLUE:
            TRISG &= ~0x00C0;
            break;
        case LED_RED:
            TRISF &= ~0x0030;
            break;

        case LED_NONE:
            break ;
    }
}

/*********************************************************************
* Function: void LED_Off(LED led);
*
* Overview: Turns requested LED off
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Off(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Off(LED led)
{
    switch(led)
    {
        case LED_GREEN:
            TRISG |= 0x0300;
            break;

        case LED_BLUE:
            TRISG |= 0x00C0;
            break;

        case LED_RED:
            TRISF |= 0x0030;
            break;

        case LED_NONE:
            break ;
    }
}

/*********************************************************************
* Function: void LED_Toggle(LED led);
*
* Overview: Toggles the state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Toggle(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Toggle(LED led)
{
    switch(led)
    {
        case LED_GREEN:
            TRISG ^= 0x0300;
            break;

        case LED_BLUE:
            TRISG ^= 0x00C0;
            break;

        case LED_RED:
            TRISF ^= 0x0030;
            break;

        case LED_NONE:
            break ;
    }
}

/*********************************************************************
* Function: bool LED_Get(LED led);
*
* Overview: Returns the current state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Get(LED_CONNECTION_DETECTED);
*
* Output: true if on, false if off
*
********************************************************************/
bool LED_Get(LED led)
{
    switch(led)
    {
        case LED_GREEN:
            return (TRISG & 0x0300) ? false : true;

        case LED_BLUE:
            return (TRISG & 0x00C0) ? false : true;

        case LED_RED:
            return (TRISF & 0x0030) ? false : true;

        case LED_NONE:
            return false ;
    }
    return false ;
}



/*********************************************************************
* Function: void LED_Enable(LED led);
*
* Overview: Configures the LED for use by the other LED API
*
* PreCondition: none
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*
* Output: none
*
********************************************************************/
void LED_Enable(LED led)
{
    switch(led)
    {
        case LED_GREEN:
            LATG &= ~0x0300;
	    TRISF |= 0x0300;
            break;

        case LED_BLUE:
            LATG &= ~0x00C0;
	    TRISF |= 0x00C0;
            break;

        case LED_RED:
            LATF &= ~0x0030;
            TRISF |= 0x0030;
            break;

        case LED_NONE:
            break ;
    }
}




