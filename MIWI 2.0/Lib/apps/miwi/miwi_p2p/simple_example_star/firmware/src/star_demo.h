/* 
 * File:   star_demo.h
 * Author: C17143
 *
 * Created on January 3, 2016, 8:40 PM
 */

#ifndef STAR_DEMO_H
#define	STAR_DEMO_H


/*********************************************************************
* Function: bool Process_RxMessage(void)
*
* Overview: Process a Received Message
*
* PreCondition: MiApp_ProtocolInit
*
* Input:  None
*
* Output: true : If user data :: false : If cmd Data
*
********************************************************************/
bool Process_RxMessage(void);


/*********************************************************************
* Function: void run_star_demo(void)
*
* Overview: Starts the star demo
*
* PreCondition: System Initialization and Miwi Protocol Init
*
* Input:  None
*
* Output: None
*
********************************************************************/
void run_star_demo(void);

#endif	/* STAR_DEMO_H */

