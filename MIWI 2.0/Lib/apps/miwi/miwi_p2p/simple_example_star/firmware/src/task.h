/* 
 * File:   task.h
 * Author: C17143
 *
 * Created on January 3, 2016, 8:40 PM
 */

#ifndef TASK_H
#define	TASK_H

#include "system.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "demo_output.h"
#include "miwi/miwi_nvm.h"


/*********************************************************************
* Function: void freezer_feature(void)
*
* Overview: Wait for User Inputs from Switches on Board 
*
* PreCondition: Hardware_Init()
*
* Input:  None
*
* Output:Boolean True : New Network --False: Restore the Old Network Stored on EEPROM
 *                                     // False : Using Network Freezer Feature
*
********************************************************************/
bool freezer_feature(void);
/*********************************************************************
* Function: void Initialize_Demo(void)
*
* Overview: MAC Layer Protocol Initialization , Ch selection and Network Roles 
 *                  Pan Co or End Device In case of Star
 *              
*
* PreCondition: Hardware_Init()
*
* Input:  None
*
* Output:None 
*           
********************************************************************/
void Initialize_Demo(bool);
/*********************************************************************
* Function: void Run_Demo(void)
*
* Overview: Start P2p or Star Demo depending on the protocol choosen in 
 *                  miwi_config.h
*
* PreCondition: Hardware_Init && Demo_Init
*
* Input:  None
*
* Output:None 
*           
********************************************************************/
void Run_Demo(void);

/*********************************************************************
* Function: void Run_Demo(void)
*
* Overview: Display the PAN ID , channel and its 3 byte long address 
*
* PreCondition: None 
*
* Input:  None
*
* Output:None 
*           
********************************************************************/
void dev_info(void);
/*********************************************************************
* Function: bool Time_out(MIWI_TICK a, MIWI_TICK b , no_of_seconds);
*
* Overview: User Defined Time out
*
* PreCondition: hardware_init 
*
* Input:  Time a and b , no_of_seconds :TimeDifference > how many seconds ?
*
* Output:true : if Timeout , false : if Timeout condition not met  
*           
********************************************************************/
bool Time_out(MIWI_TICK, MIWI_TICK , uint8_t);

#endif	/* TASK_H */

