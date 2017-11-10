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

#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <stdbool.h>

#define OLED_NUMBER_OF_COLUMNS 21
#define OLED_NUMBER_OF_ROWS    8

/*********************************************************************
* Function: bool OLED_Initialize()
*
* Overview: Initializes the print operation
*
* PreCondition: none
*
* Input: None
*
* Output: true if successful, false if not configured or doesn?t
*         exist for this board.
*
********************************************************************/
bool OLED_Initialize();

/*********************************************************************
* Function: void OLED_PutString(char* string, uint16_t length)
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
void OLED_PutString(char* string, uint16_t length);

/*********************************************************************
* Function: void OLED_PutChar(char charToPrint)
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
void OLED_PutChar(char charToPrint);

/*********************************************************************
* Function: void OLED_ClearScreen()
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
void OLED_ClearScreen(void);

#endif // OLED_H
