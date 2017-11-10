/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

/************************ HEADERS ****************************************/
#include <stdio.h>
#include "miwi/miwi_api.h"
#include "task.h"
#include "star_demo.h"




#if defined(PROTOCOL_STAR)
    uint8_t i;
    uint8_t TxSynCount = 0;    // Maintain the Count on TX's Done
    uint8_t TxSynCount2 = 0; // Maintain the Count on TX's Done
    uint8_t TxNum = 0;         // Maintain the Count on TX's Done
    uint8_t RxNum = 0;         // Maintain the Count on RX's Done
    // check for selections made by USER
    bool chk_sel_status;
    
    bool display_connections;
    MIWI_TICK t1 , t2;
    


    void run_star_demo(void)
    {
        t1 = MiWi_TickGet ();
        display_connections = true;
        while(display_connections)
        {

            if(MiApp_MessageAvailable())
            {   

                /*******************************************************************/
                // If a packet has been received, handle the information available 
                // in rxMessage.
                /*******************************************************************/
                #if defined(ENABLE_CONSOLE)
                    DemoOutput_HandleMessage();
                #endif

                DemoOutput_UpdateTxRx(TxNum, ++RxNum);
                DELAY_ms(2000);
                // Toggle LED2 to indicate receiving a packet.
                LED_2 ^= 1;
                // Display the Src End Device Info on reception msg
                // We do not display if it is a PAN CO or if the message
                // received was a broadcast packet
                if (role == EndDevice && !rxMessage.flags.bits.broadcast)
                {
                    Source_END_DEVICE_INFO(rxMessage.Payload);
                }

                /*******************************************************************/
                // Function MiApp_DiscardMessage is used to release the current 
                //  received packet.
                // After calling this function, the stack can start to process the
                //  next received frame 
                /*******************************************************************/        
                MiApp_DiscardMessage();
                STAR_DEMO_OPTIONS_MESSAGE ();

            }      
            else
            {   

                /*******************************************************************/
                // If no packet received, now we can check if we want to send out
                // any information.
                // Function ButtonPressed will return if any of the two buttons
                // has been pushed.
                /*******************************************************************/
                uint8_t PressedButton = ButtonPressed();
                if ( PressedButton == 1 || PressedButton == 2)
                {

                    if (role == PanCoordinator)
                    {   
                        /*******************************************************************/ 
                        // Button 1 pressed. We need to send out the bitmap of word "MiWi".
                        // First call MiApp_FlushTx to reset the Transmit buffer. Then fill 
                        // the buffer one byte by one byte by calling function 
                        // MiApp_WriteData
                        /*******************************************************************/

                        MiApp_FlushTx();
                        for(i = 0; i < 21; i++)
                        { //Tx_Buffer User Data
                            MiApp_WriteData(MiWi[(TxSynCount%6)][i]);
                        }
                        TxSynCount++;


                        /*******************************************************************/
                        // Function MiApp_BroadcastPacket is used to broadcast a message
                        //    The only parameter is the boolean to indicate if we need to
                        //       secure the frame
                        /*******************************************************************/
                        #if defined(ENABLE_SECURITY)
                            MiApp_BroadcastPacket(true);
                        #else
                            MiApp_BroadcastPacket (false);
                        #endif

                        DemoOutput_UpdateTxRx(++TxNum, RxNum);

                    }
                    else
                    {

                        uint8_t select_ed = 0;
                        chk_sel_status = true;
                        bool update_ed = true;
                        while(update_ed == true)
                        {   
                            //User Selected Change end device
                            LCD_Erase();
                            if (myConnectionIndex_in_PanCo  == select_ed)
                            {   // if END_device displays itself , "me" is added in display to denote itself 
                                sprintf((char *)LCDText, (char*)"RB0:%02d-%02x%02x%02x-me",END_DEVICES_Short_Address[select_ed].connection_slot,END_DEVICES_Short_Address[select_ed].Address[0],
                                        END_DEVICES_Short_Address[select_ed].Address[1],END_DEVICES_Short_Address[select_ed].Address[2] );
                            }
                            else
                            {
                                sprintf((char *)LCDText, (char*)"RB0:%02d-%02x%02x%02x",END_DEVICES_Short_Address[select_ed].connection_slot,END_DEVICES_Short_Address[select_ed].Address[0],
                                        END_DEVICES_Short_Address[select_ed].Address[1],END_DEVICES_Short_Address[select_ed].Address[2] );
                            }
                            sprintf((char *)LCDText, (char*)"RB0:%02d-%02x%02x%02x",END_DEVICES_Short_Address[select_ed].connection_slot,END_DEVICES_Short_Address[select_ed].Address[0],
                                        END_DEVICES_Short_Address[select_ed].Address[1],END_DEVICES_Short_Address[select_ed].Address[2] );
                            sprintf((char *)&(LCDText[16]), (char*)"RB2: Change node");
                            LCD_Update();
                            chk_sel_status = true;

                            while(chk_sel_status)
                                {
                                    // Check for Button Press on Dev Kits
                                    uint8_t switch_val = ButtonPressed();
                                    // While waiting for TX Input, RX will process if any message was available
                                    if (MiApp_MessageAvailable())
                                    {
                                        // Process received Packets and Discard them after processing
                                        MiApp_DiscardMessage();
                                    }
                                    bool sw_layer_ack_status , mac_ack_status;
                                    if(switch_val == 1)   // Send Packet
                                    {
                                        update_ed = false;    // No Display of peer Device Info 
                                        chk_sel_status = false;   // Selection Mode off
                                        TxSynCount2++;
                                        if (myConnectionIndex_in_PanCo == select_ed)
                                        {
                                            MiApp_FlushTx();
                                            for (i = 0 ; i < 21 ; i++)
                                            {
                                                MiApp_WriteData(MiWi[(TxSynCount%6)][i]);
                                            }

                                            // IF on the demo , a END_Device displays its own Connection Detail
                                            // We unicast data packet to just PAN COR , No forwarding
                                            #if defined(ENABLE_SECURITY)
                                                mac_ack_status = MiApp_UnicastConnection (0, true);
                                            #else
                                                mac_ack_status = MiApp_UnicastConnection (0, false);
                                            #endif
                                                TxNum++;
                                        }

                                        else
                                        { 
                                            // Data can be sent at a time from one END_DEVICE_TO_ANOTHER 
                                            // Edx --> Pan CO --> EDy
                                            // To forward a Packet from one ED to another ED , Destinational End Device
                                            // Connection Index must be known
                                            // This is available in the END_DEVICES_Unique_Short_Address structure 
                                            MiApp_FlushTx();
                                            for (i = 0 ; i < 25 ; i++)
                                            {
                                                MiApp_WriteData(MiWi[(TxSynCount%6)][i-4]);
                                            }
                                            #if defined(ENABLE_SECURITY)
                                                sw_layer_ack_status = MiApp_UnicastStar (select_ed , true);
                                            #else
                                                sw_layer_ack_status = MiApp_UnicastStar (select_ed, false);
                                            #endif


                                            #if defined(ENABLE_APP_LAYER_ACK)
                                                if (sw_layer_ack_status)
                                                {
                                                    TxNum++;     // Tx was successful
                                                }
                                                else
                                                {
                                                    LCDDisplay((char *)"Data_Sending_Fail!!", 0, false); 
                                                }
                                            #else    
                                                TxNum++;
                                            #endif

                                        }
                                        DemoOutput_UpdateTxRx(TxNum, RxNum);
                                        DELAY_ms(1500);  // Delay for Display
                                        STAR_DEMO_OPTIONS_MESSAGE ();
                                    }
                                    else if(switch_val == 2)   // SWITCH to new peer Device Info
                                    {
                                        if (select_ed > end_nodes-1)  // Reset Peer Device Info
                                        {    
                                            // If end of Peer Device Info reset the count
                                            select_ed = 0;    
                                        }
                                        else
                                        {
                                            // New device Information 
                                            select_ed = select_ed+1;
                                        }
                                        chk_sel_status = false;

                                    }  // end of SWITCH to new peer Device INFO
                                } // end of Selection of Peer Device 

                        } // END of Display of PEER DEVICE Info         

                    }// end of END_DEVICE send packet option
                } // end of options on button press
            } // end of TX DEMO for PAN CO and END DEVICE

            // If a End devices looses connection with PAN Co

            if (lost_connection && role == EndDevice)
            {
                MiApp_EstablishConnection(0xFF, CONN_MODE_DIRECT);
                lost_connection = false;

            }
            
            t2 = MiWi_TickGet ();
            // Display the no of End devices in Network
            if( MiWi_TickGetDiff(t2, t1) > (SHARE_PEER_DEVICE_INFO_TIMEOUT) && role == PanCoordinator)
            {
                LCD_Erase();
                sprintf((char *)&(LCDText), (char*)"END_DEVICES :%02d",conn_size);
                LCD_Update();
                DELAY_ms(2000);
                display_connections = false;
                STAR_DEMO_OPTIONS_MESSAGE ();  // Pan Co

            }

        }
    }
#endif   