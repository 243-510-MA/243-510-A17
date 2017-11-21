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
#include "miwi/miwi_api.h"
#include "task.h"

#include "p2p_demo.h"

#if defined(PROTOCOL_P2P)

    uint8_t i;
    uint8_t TxSynCount = 0;
    uint8_t TxSynCount2 = 0;
    uint8_t TxNum = 0;
    uint8_t RxNum = 0;
    bool chk_sel_status = true;  // Check _select_Status


    void run_p2p_demo(void)
    {

        if( MiApp_MessageAvailable() )
        {

            /*******************************************************************/
            // If a packet has been received, handle the information available 
            // in rxMessage.
            /*******************************************************************/
            #if defined(ENABLE_CONSOLE)
                DemoOutput_HandleMessage();
            #endif 
            DemoOutput_UpdateTxRx(TxNum, ++RxNum);   
            // Toggle LED2 to indicate receiving a packet.
            LED_2 ^= 1;
            DELAY_ms(2000);
            DemoOutput_Instruction();
            /*******************************************************************/
            // Function MiApp_DiscardMessage is used to release the current 
            //  received packet.
            // After calling this function, the stack can start to process the
            //  next received frame 
            /*******************************************************************/        
            MiApp_DiscardMessage();
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
            switch( PressedButton )
            {
                case 1:
                    /*******************************************************************/ 
                    // Button 1 pressed. We need to send out the bitmap of word "MiWi".
                    // First call MiApp_FlushTx to reset the Transmit buffer. Then fill 
                    // the buffer one byte by one byte by calling function 
                    // MiApp_WriteData
                    /*******************************************************************/
                    MiApp_FlushTx();
                    for(i = 0; i < 21; i++)
                    {
                        // TX buffer User DATA
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
                        MiApp_BroadcastPacket(false);
                    #endif

                    DemoOutput_UpdateTxRx(++TxNum, RxNum);
                    DELAY_ms(1000);
                    DemoOutput_Instruction();
                    break;

                case 2:
                    chk_sel_status = true;
                    uint8_t select_ed =0;
                    bool update_ed = true;
                    while(update_ed == true)
                    {

                        //Peer Device Info 
                       LCD_Erase();
                       LCD_Erase(); 
                        sprintf((char *)LCDText, (char*)"RB0:%02d-%02x%02x%02x",select_ed,ConnectionTable[select_ed].Address[0],
                               ConnectionTable[select_ed].Address[1],ConnectionTable[select_ed].Address[2] );
                        sprintf((char *)&(LCDText[16]), (char*)"RB2: Chnge node");
                        LCD_Update();
                        LCD_Update(); 
                        // Display another Peer Device Address
                        chk_sel_status = true;

                        while(chk_sel_status)
                        {
                            uint8_t switch_val = ButtonPressed();
                            //// While waiting in TX , RX will process if any message was available
                            if (MiApp_MessageAvailable())
                            {
                                // Process Received Packet and Discard the packet
                                MiApp_DiscardMessage();
                            }
                            if(switch_val == 1)
                            {
                                update_ed = false;
                                chk_sel_status = false;

                                TxSynCount2++;
                                MiApp_FlushTx();   
                                for(i = 0; i < 11; i++)
                                {
                                    // Tx Buffer User Data
                                    MiApp_WriteData(DE[(TxSynCount2%6)][i]);
                                }
                                TxSynCount2++; 
                                if( MiApp_UnicastConnection(select_ed, true) == false )
                                {
                                    DemoOutput_UnicastFail();
                                }
                                else
                                {
                                    // Successful Transmission
                                    TxNum++;
                                }
                                // Update the LCD
                                DemoOutput_UpdateTxRx(TxNum, RxNum);
                                DELAY_ms(1000);
                                DemoOutput_Instruction();

                                break;
                            }   // end switch_val = 1

                            else if(switch_val == 2)
                            {
                                if (select_ed > conn_size-2)
                                {
                                    // Last Peer Device 
                                    select_ed = 0;
                                }   
                                else
                                {
                                    // Update the Display
                                    select_ed = select_ed+1;
                                }       
                                chk_sel_status = false;
                            }   // end switch_val = 2
                        }  // end of Peer Device selection

                    } // End of Display


                    break;   
                default:
                    break;
            }

        }

    }

#endif