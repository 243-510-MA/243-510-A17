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

#include <stdbool.h>
#include <print_ram_buffer.h>
#include <stdint.h>
#include <string.h>

/* User build options *********************************************/
#ifndef PRINT_BUFFER_SIZE
    #define PRINT_BUFFER_SIZE 32
#endif

/* Variables ******************************************************/
static char printBuffer[PRINT_BUFFER_SIZE];
static char* current;

/*********************************************************************
* Function: bool PRINT_SetConfiguration(PRINT_CONFIGURATION configuration);
*
* Overview: Configures the print configuration
*
* PreCondition: none
*
* Input: configuration - the print configuration to use.  Some boards
*         may have more than one print configuration enabled
*
* Output: TRUE if successful, FALSE if LED not configured or doesn?t 
*         exist for this board.
*
********************************************************************/
bool PRINT_SetConfiguration(PRINT_CONFIGURATION configuration)
{
    PRINT_ClearScreen();

    return true;
}

/*********************************************************************
* Function: void PRINT_String(char* string, uint16_t length)
*
* Overview: Prints a string until a null terminator is reached or the
*           specified string length is printed.
*
* PreCondition: none
*
* Input: char* string - the string to print.
*        uint16_t length - the length of the string.
*
* Output: None
*
********************************************************************/
void PRINT_String(char* string, uint16_t length)
{
    while(*string != 0)
    {
        if(length == 0)
        {
            return;
        }

        PRINT_Char(*string++);
        length--;
    }
}

/*********************************************************************
* Function: void PRINT_Char(char charToPrint)
*
* Overview: Prints a character
*
* PreCondition: none
*
* Input: char charToPrint - the character to print
*
* Output: None
*
********************************************************************/
void PRINT_Char(char charToPrint)
{
    *current = charToPrint;
    
    if(current == (&printBuffer[PRINT_BUFFER_SIZE - 1]))
    {
        current = printBuffer;
    }
    else
    {
        current++;
    }
}

/*********************************************************************
* Function: void PRINT_ClearStreen()
*
* Overview: Clears the screen, if possible
*
* PreCondition: none
*
* Input: None
*
* Output: None
*
********************************************************************/
void PRINT_ClearScreen(void)
{
    memset(printBuffer, 0, sizeof(printBuffer));
    current = printBuffer;
}


