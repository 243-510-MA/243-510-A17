/********************************************************************
* FileName:		MCHP_API.h
* Dependencies: 
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.30 or higher
*				Microchip C30 v2.03 or higher	
*               Microchip C32 v1.02 or higher
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
*
* Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute 
* Software only when embedded on a Microchip microcontroller or digital 
* signal controller and used with a Microchip radio frequency transceiver, 
* which are integrated into your product or third party product (pursuant 
* to the terms in the accompanying license agreement).  
*
* You should refer to the license agreement accompanying this Software for 
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED ?AS IS? WITHOUT WARRANTY OF ANY 
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
*********************************************************************
* File Description:
*   This file defines MiApp interfaces 
*
* Change History:
*  Rev   Date         Author    Description
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  2.1   06/20/2009   yfy       Add LCD support
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

#ifndef __MCHP_API
    #define __MCHP_API
    
    #include "miwi_config.h"
    #include "gtypedefs.h"
    #include "NetworkManager.h"


    #define INPUT
    #define OUTPUT
    #define IOPUT

    /***************************************************************************/
    // Status information of the connected peer information
    //
    //      This structure contains the information regarding the status of 
    //      the connected peer device. 
    /***************************************************************************/
    typedef union __CONNECTION_STATUS
    {
        uint8_t Val;
        struct _CONNECTION_STAUTS_bits
        {
            uint8_t RXOnWhenIdle       :1;     //1 = transceiver always on, 0 = transceiver sleeps when idle
            uint8_t directConnection   :1;     //1 = can talk to this device directly, 0 = must route to this device
            uint8_t longAddressValid   :1;     //1 = long address valid, 0 = long address unknown
            uint8_t shortAddressValid  :1;     //1 = short address valid, 0 = short address unknown
            uint8_t FinishJoin         :1;     //1 = already finish joining procedure, 0 = in the process of join
            uint8_t isFamily           :1;     //1 = family member (parent/child), 0 = not family
            uint8_t filler             :1;
            uint8_t isValid            :1;     //1 = this entry is valid, 0 = this entry is not valid
        } bits;   
    } CONNECTION_STATUS;
 

    /***************************************************************************
     * Peer Device Information in Connection Table
     *  
     *      This structure contains device information about the peer device
     *      of current node. It is the element structure for connection table.
     *      Due to the bank limitation in PIC18 MCU architecture, the size of
     *      CONNECTION_ENTRY must be dividable by 256 in case the array is across
     *      the bank. In this case, the user need to make sure that there is no
     *      problem 
     **************************************************************************/
    typedef struct __CONNECTION_ENTRY
    {
        #if defined(ENABLE_SECURITY)
            uint32_t        IncomingFrameCounter;
        #endif
        #if !defined(PROTOCOL_P2P)
            _uint16_VAL    PANID;                      // PAN Identifier of the peer device. May not necessary in P2P protocol
            _uint16_VAL    AltAddress;                 // Alternative address of the peer device. Not necessary in P2P protocol
        #endif
        addr_t        Address;                          // Permanent address of peer device
        
        CONNECTION_STATUS status;
        #if ADDITIONAL_NODE_ID_SIZE > 0
            uint8_t        PeerInfo[ADDITIONAL_NODE_ID_SIZE];  // Additional Node ID information, if defined in application layer
        #endif
    } CONNECTION_ENTRY;

    extern CONNECTION_ENTRY    ConnectionTable[CONNECTION_SIZE];

    extern uint8_t            currentChannel;

    /************************************************************************************
     * Function:
     *      void MiApp_ProtocolInit(bool bNetworkFreezer)
     *
     * Summary:
     *      This function initialize the Microchip proprietary wireless protocol
     *
     * Description:        
     *      This is the primary user interface function to initialize the Microchip
     *      proprietary wireless protocol, which is chosen by the application layer. 
     *      Usually, this function must be called after the hardware initialization,
     *      before any other MiApp interface can be called. 
     *
     * PreCondition:    
     *      Hardware initialization has been done.
     *
     * Parameters:           
     *      bool bNetworkFreezer -  Network Freezer is a feature for Microchip proprietary
     *                              protocol. When this feature is enbaled, all critical 
     *                              network information can be saved in nonvotile memory,
     *                              so that after power cycle, the network can be restored 
     *                              to its original state before the power lost without 
     *                              any communication. When Network Freezer feature is enabled
     *                              and the input parameter of bNetworkFreezer is TRUE, the
     *                              initialization process will restore the network paramaters.
     *                  
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      HardwareInit();
     *      MiApp_ProtocolInit();
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *********************************************************************************/
    bool    MiApp_ProtocolInit(bool bNetworkFreezer);
    
    
    /************************************************************************************
     * Function:
     *      bool MiApp_SetChannel(uint8_t Channel)
     *
     * Summary:
     *      This function set the operating channel for the RF transceiver
     *
     * Description:        
     *      This is the primary user interface function to set the operating
     *      channel for the RF transceiver. Valid channels are from 0 to 31.
     *      Depends on the RF transceiver, its frequency band, data rate and 
     *      other settings, not all channels are available. If input channel 
     *      is not available under the current condition, the current operating
     *      channel will not be change, and the return value will be FALSE. 
     *      Otherwise, the return value will be TRUE.
     *
     * PreCondition:    
     *      Protocol initialization has been done.
     *
     * Parameters:           
     *      uint8_t Channel -  The channel to as the future operating channel.
     *                      Channels higher than 31 are invalid. Valid 
     *                      channels between 0-31 depends on a lot of factors
     *                  
     * Returns: 
     *      a boolean to indicate if channel change has been performed successfully
     *
     * Example:
     *      <code>
     *      if( TRUE == MiApp_SetChannel(15) )
     *      {
     *          // channel changes successfully
     *      }
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *********************************************************************************/        
    bool    MiApp_SetChannel(uint8_t Channel);
    
    
    
    #define START_CONN_DIRECT       0x00
    #define START_CONN_ENERGY_SCN   0x01
    #define START_CONN_CS_SCN       0x02
    /************************************************************************************
     * Function:
     *      bool    MiApp_StartConnection(uint8_t Mode, uint8_t ScanDuration, uint32_t ChannelMap)
     *
     * Summary:
     *      This function start a PAN without connected to any other devices
     *
     * Description:        
     *      This is the primary user interface function for the application layer to 
     *      a PAN. Usually, this fucntion is called by the PAN Coordinator who is the
     *      first in the PAN. The PAN Coordinator may start the PAN after a noise scan
     *      if specified in the input mode. 
     *
     * PreCondition:    
     *      Protocol initialization has been done.
     *
     * Parameters:           
     *      uint8_t Mode - Whether to start a PAN after a noise scan. Possible modes are
     *                  * START_CONN_DIRECT Start PAN directly without noise scan
     *                  * START_CONN_ENERGY_SCN Perform an energy scan first, then
     *                                          start the PAN on the channel with least
     *                                          noise.
     *                  * START_CONN_CS_SCN     Perform a carrier-sense scan first,
     *                                          then start the PAN on the channel with
     *                                          least noise.
     *      uint8_t ScanDuration - The maximum time to perform scan on single channel. The
     *                          value is from 5 to 14. The real time to perform scan can
     *                          be calculated in following formula from IEEE 802.15.4 
     *                          specification: 
     *                              960 * (2^ScanDuration + 1) * 10^(-6) second
     *                          ScanDuration is discarded if the connection mode is
     *                          START_CONN_DIRECT.
     *      uint32_t ChannelMap -  The bit map of channels to perform noise scan. The 32-bit
     *                          double uint16_t parameter use one bit to represent corresponding
     *                          channels from 0 to 31. For instance, 0x00000003 represent to 
     *                          scan channel 0 and channel 1. ChannelMap is discarded if the 
     *                          connection mode is START_CONN_DIRECT.
     *                  
     * Returns: 
     *      a boolean to indicate if PAN has been started successfully.
     *
     * Example:
     *      <code>
     *      // start the PAN on the least noisy channel after scanning all possible channels.
     *      MiApp_StartConnection(START_CONN_ENERGY_SCN, 10, 0xFFFFFFFF);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/    
    bool    MiApp_StartConnection( uint8_t Mode, uint8_t ScanDuration, uint32_t ChannelMap);
    
    
    /***************************************************************************
     * Active Scan result 
     *  
     *      This structure contains information from active scan. Application 
     *      layer will depend on this information to decide the way to establish
     *      connections. 
     **************************************************************************/
    typedef struct
    {
        uint8_t		Channel;                        // Operating Channel of the PAN
        _uint16_VAL    PANID;                        // PAN Identifier
        uint8_t        RSSIValue;                   // RSSI value for the response
        uint8_t        Flags;                    // LQI value for the response
    } ACTIVE_SCAN_RESULT; 
    
    #ifdef ENABLE_ACTIVE_SCAN
        extern ACTIVE_SCAN_RESULT ActiveScanResults[ACTIVE_SCAN_RESULT_SIZE];
    #endif
    
    
    
    /************************************************************************************
     * Function:
     *      uint8_t    MiApp_SearchConnection(uint16_t ScanDuartionMs, uint32_t ChannelMap)
     *
     * Summary:
     *      This function perform an active scan to locate operating PANs in the
     *      neighborhood.
     *
     * Description:        
     *      This is the primary user interface function for the application layer to 
     *      perform an active scan. After this function call, all active scan response
     *      will be stored in the global variable ActiveScanResults in the format of 
     *      structure ACTIVE_SCAN_RESULT. The return value indicates the total number
     *      of valid active scan response in the active scan result array.
     *
     * PreCondition:    
     *      Protocol initialization has been done.
     *
     * Parameters:           
     *      uint16_t ScanDurationMs - The maximum time to perform scan on single channel. The
     *                          value is from 5 to 14. The real time to perform scan can
     *                          be calculated in following formula from IEEE 802.15.4 
     *                          specification 
     *                              960 * (2^ScanDuration + 1) * 10^(-6) second
     *      uint32_t ChannelMap -  The bit map of channels to perform noise scan. The 32-bit
     *                          double uint16_t parameter use one bit to represent corresponding
     *                          channels from 0 to 31. For instance, 0x00000003 represent to 
     *                          scan channel 0 and channel 1. 
     *                  
     * Returns: 
     *      The number of valid active scan response stored in the global variable ActiveScanResults.
     *
     * Example:
     *      <code>
     *      // Perform an active scan on all possible channels
     *      NumOfActiveScanResponse = MiApp_SearchConnection(10, 0xFFFFFFFF);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/
    uint8_t    MiApp_SearchConnection(uint8_t ScanDuration, uint32_t ChannelMap);
    
    #define CONN_MODE_DIRECT        0x00
    #define CONN_MODE_INDIRECT      0x01
    /************************************************************************************
     * Function:
     *      uint8_t    MiApp_EstablishConnection(uint8_t ActiveScanIndex, uint8_t Mode)
     *
     * Summary:
     *      This function establish a connection with one or more nodes in an existing
     *      PAN.
     *
     * Description:        
     *      This is the primary user interface function for the application layer to 
     *      start communication with an existing PAN. For P2P protocol, this function
     *      call can establish one or more connections. For network protocol, this 
     *      function can be used to join the network, or establish a virtual socket
     *      connection with a node out of the radio range. There are multiple ways to
     *      establish connection(s), all depends on the input parameters.
     *
     * PreCondition:    
     *      Protocol initialization has been done. If only to establish connection with
     *      a predefined device, an active scan must be performed before and valid active
     *      scan result has been saved.
     *
     * Parameters:           
     *      uint8_t ActiveScanIndex -  The index of the target device in the ActiveScanResults
     *                              array, if a predefined device is targeted. If the 
     *                              value of ActiveScanIndex is 0xFF, the protocol stack will
     *                              try to establish a connection with any device.
     *      uint8_t Mode - The mode to establish a connection. This parameter is generally valid in
     *                  a network protocol. The possible modes are:
     *                  * CONN_MODE_DIRECT      Establish a connection within radio range.
     *                  * CONN_MODE_INDIRECT    Establish a virtual connection with a device 
     *                                          that may be in or out of the radio range. This
     *                                          mode sometimes is called cluster socket, which
     *                                          is only valid for network protocol. The PAN 
     *                                          Coordinator will be involved to establish a 
     *                                          virtual indirect socket connection.
     *                  
     * Returns: 
     *      The index of the peer device on the connection table.
     *
     * Example:
     *      <code>
     *      // Establish one or more connections with any device
     *      PeerIndex = MiApp_EstablishConnection(0xFF, CONN_MODE_DIRECT);
     *      </code>
     *
     * Remarks:    
     *      If more than one connections have been established through this function call, the
     *      return value points to the index of one of the peer devices.
     *
     *  Note:  In mesh network this function will join any available network on current channel
     *       if ActiveScanIndex=0xff or if ActiveScanResult[ActiveScanIndex] is not valid
     *****************************************************************************************/    
    uint8_t    MiApp_EstablishConnection(uint8_t ActiveScanIndex, uint8_t Mode);
    
    /*********************************************************************
     * Function:
     *      void MiApp_RemoveConnection(uint8_t ConnectionIndex)
     *
     * Summary:
     *      This function remove connection(s) in connection table
     *
     * Description:        
     *      This is the primary user interface function to disconnect connection(s).
     *      For a P2P protocol, it simply remove the connection. For a network protocol,
     *      if the device referred by the input parameter is the parent of the device
     *      calling this function, the calling device will get out of network along with
     *      its children. If the device referred by the input parameter is children of
     *      the device calling this function, the target device will get out of network.
     * 
     * PreCondition:    
     *      Transceiver has been initialized. Node has establish
     *      one or more connections
     *
     * Parameters:           
     *      uint8_t ConnectionIndex -  The index of the connection in the
     *                              connection table to be removed
     *                  
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      MiApp_RemoveConnection(0x00);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     ********************************************************************/
    void    MiApp_RemoveConnection(uint8_t ConnectionIndex);
    
    #define ENABLE_ALL_CONN         0x00
    #define ENABLE_PREV_CONN        0x01
    #define ENABLE_ACTIVE_SCAN_RSP  0x02
    #define DISABLE_ALL_CONN        0x03
    /************************************************************************************
     * Function:
     *      void    MiApp_ConnectionMode(uint8_t Mode)
     *
     * Summary:
     *      This function set the current connection mode.
     *
     * Description:        
     *      This is the primary user interface function for the application layer to 
     *      configure the way that the host device accept connection request.
     *
     * PreCondition:    
     *      Protocol initialization has been done. 
     *
     * Parameters:           
     *      uint8_t Mode -     The mode to accept connection request. The privilege for those modes
     *                      decreases gradually as defined. The higher privilege mode has all the 
     *                      rights of the lower privilege modes.
     *                      The possible modes are
     *                      * ENABLE_ALL_CONN       Enable response to all connection request
     *                      * ENABLE_PREV_CONN      Enable response to connection request
     *                                              from device already in the connection
     *                                              table.
     *                      * ENABLE_ACTIVE_SCAN_RSP    Enable response to active scan only
     *                      * DISABLE_ALL_CONN      Disable response to connection request, including
     *                                              an acitve scan request.
     *
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      // Enable all connection request
     *      MiApp_ConnectionMode(ENABLE_ALL_CONN);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    void    MiApp_ConnectionMode(uint8_t Mode);
    
    
    /************************************************************************************
     * Function:
     *      MiApp_FlushTx()
     *
     * Description:        
     *      This macro reset the pointer of the TX buffer. This function is usually called 
     *      before filling application payload.
     *
     * PreCondition:    
     *      Protocol initialization has been done. 
     *
     * Parameters:           
     *      None
     *
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      MiApp_FlushTx();
     *      MiApp_WriteData(AppPayload[0]);
     *      MiApp_WriteData(AppPayload[1]);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    #define MiApp_FlushTx()   {TxData = PAYLOAD_START;}
    
    /************************************************************************************
     * Function:
     *      MiApp_WriteData()
     *
     * Description:        
     *      This macro writes one uint8_t of application payload to the TX buffer.
     *
     * PreCondition:    
     *      Protocol initialization has been done. MiApp_FlushTx() has been called before
     *      calling the first MiApp_WriteData for the first time.
     *
     * Parameters:           
     *      uint8_t a - One uint8_t of application payload to be written to the TX buffer
     *
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      MiApp_FlushTx();
     *      MiApp_WriteData(AppPayload[0]);
     *      MiApp_WriteData(AppPayload[1]);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    #define MiApp_WriteData(a) TxBuffer[TxData++] = a
    
    /************************************************************************************
     * Function:
     *      bool MiApp_BroadcastPacket(bool SecEn )
     *
     * Summary:
     *      This function broadcast a message in the TxBuffer.
     *
     * Description:        
     *      This is the primary user interface function for the application layer to 
     *      broadcast a message. The application payload is filled in the global char
     *      array TxBuffer.
     *
     * PreCondition:    
     *      Protocol initialization has been done. 
     *
     * Parameters:           
     *      bool SecEn -    The boolean indicates if the application payload needs to be
     *                      secured before transmission.
     *
     * Returns: 
     *      A boolean to indicates if the broadcast procedure is succcessful.
     *
     * Example:
     *      <code>
     *      // Secure and then broadcast the message stored in TxBuffer
     *      MiApp_BroadcastPacket(TRUE);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/ 
    bool MiApp_BroadcastPacket(bool SecEn );
    
    /************************************************************************************
     * Function:
     *      bool MiApp_UnicastConnection(uint8_t ConnectionIndex, bool SecEn)
     *
     * Summary:
     *      This function unicast a message in the TxBuffer to the device with the input 
     *      ConnectionIndex in the connection table. 
     *
     * Description:        
     *      This is one of the primary user interface functions for the application layer to 
     *      unicast a message. The destination device is in the connection table specified by 
     *      the input parameter ConnectionIndex. The application payload is filled in the 
     *      global char array TxBuffer.
     *
     * PreCondition:    
     *      Protocol initialization has been done. The input parameter ConnectionIndex points to
     *      a valid peer device in the connection table.
     *
     * Parameters: 
     *      uint8_t ConnectionIndex -  The index of the destination device in the connection table.
     *      bool SecEn -    The boolean indicates if the application payload needs to be
     *                      secured before transmission.
     *
     * Returns: 
     *      A boolean to indicates if the unicast procedure is succcessful.
     *
     * Example:
     *      <code>
     *      // Secure and then unicast the message stored in TxBuffer to the first device in 
     *      // the connection table
     *      MiApp_UnicastConnection(0, TRUE);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/     
    bool MiApp_UnicastConnection(uint8_t ConnectionIndex, bool SecEn);
    

    /************************************************************************************
     * Function:
     *      bool MiApp_UnicastAddress(uint8_t *DestinationAddress, bool PermanentAddr, bool SecEn)
     *
     * Summary:
     *      This function unicast a message in the TxBuffer to the device with DestinationAddress 
     *
     * Description:        
     *      This is one of the primary user interface functions for the application layer to 
     *      unicast a message. The destination device is specified by the input parameter 
     *      DestinationAddress. The application payload is filled in the global char array TxBuffer.
     *
     * PreCondition:    
     *      Protocol initialization has been done. 
     *
     * Parameters: 
     *      uint8_t * DestinationAddress - The destination address of the unicast
     *      uint8_t * origin PermanentAddr -    The boolean to indicate if the destination address above is a
     *      uint8_t * origin - origin address. Null to use current device addr
     *      bool SecEn -    The boolean indicates if the application payload needs to be
     *                      secured before transmission.
     *
     * Returns: 
     *      A boolean to indicates if the unicast procedure is succcessful.
     *
     * Example:
     *      <code>
     *      // Secure and then broadcast the message stored in TxBuffer to the permanent address
     *      // specified in the input parameter.
     *      MiApp_UnicastAddress(DestAddress, TRUE, TRUE);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/      
    bool MiApp_UnicastAddress(uint8_t *DestinationAddress, uint8_t* Origin, bool SecEn);

    #define BROADCAST_ALL   0x01
    #define BROADCAST_RFD   0x02

    /***************************************************************************
     * Received Message information
     *  
     *      This structure contains information about the received application
     *      message.
     **************************************************************************/
    typedef struct
    {
        union
        {
            uint8_t Val;
            struct
            {
                uint8_t		broadcast:  2;      // 1: broadcast message
                uint8_t		ackReq:     1;      // 1: sender request acknowledgement in MAC.
                uint8_t		secEn:      1;      // 1: application payload has been secured
                uint8_t		repeat:     1;      // 1: message received through a repeater
                uint8_t		command:    1;      // 1: message is a command frame
                uint8_t        srcPrsnt:   1;      // 1: source address present in the packet
                uint8_t        altSrcAddr: 1;      // 1: source address is alternative network address
            } bits;
        }  flags;

        _uint16_VAL      SourcePANID;                // PAN Identifier of the sender
        uint8_t         *SourceAddress;             // pointer to the source address
        uint8_t         *Payload;                   // pointer to the application payload
        uint8_t         PayloadSize;                // application payload length
        uint8_t         PacketRSSI;                 // RSSI value of the receive message
        uint8_t 		PacketLQI;                  // LQI value of the received message
    } RECEIVED_MESSAGE;
    
    
    /************************************************************************************
     * Function:
     *      bool    MiApp_MessageAvailable(void)
     *
     * Summary:
     *      This function return a boolean if a message is available for the application
     *
     * Description:        
     *      This is the primary user interface functions for the application layer to 
     *      call the Microchip proprietary protocol stack to check if a message is available
     *      for the application. The function will call the protocol stack state machine
     *      to keep the stack running. It is expected that this function should be called
     *      periodically in the application. In case a message is available, all information
     *      related to the recevied message will be stored in the global variable RxMessage
     *      in the format of RECEIVED_MESSAGE.
     *
     * PreCondition:    
     *      Protocol initialization has been done. 
     *
     * Parameters: 
     *      None
     *
     * Returns: 
     *      A boolean to indicates if a message is available for application.
     *
     * Example:
     *      <code>
     *      if( TRUE == MiApp_MessageAvailable() )
     *      {
     *          // handle the received message in global variable RxMessage
     *
     *          // discard the received message after processing
     *          MiApp_DiscardMessage();
     *      }
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/      
    bool    MiApp_MessageAvailable(void);


    /************************************************************************************
     * Function:
     *      void    MiApp_DiscardMessage(void)
     *
     * Summary:
     *      This function discard the current message for the application and notify the 
     *      protocol layer that it is ready to receive the next message.
     *
     * Description:        
     *      This is the primary user interface functions for the application layer to 
     *      discard the current active message, release the system resources used and 
     *      ready to receive the next message. It is must be called after finish handling
     *      the message, otherwise, no further message can be received.
     *
     * PreCondition:    
     *      Protocol initialization has been done. A message has been received by the 
     *      application layer.
     *
     * Parameters: 
     *      None
     *
     * Returns: 
     *      None
     *
     * Example:
     *      <code>
     *      if( TRUE == MiApp_MessageAvailable() )
     *      {
     *          // handle the received message in global variable RxMessage
     *
     *          // discard the received message after processing
     *          MiApp_DiscardMessage();
     *      }
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/      
    void    MiApp_DiscardMessage(void);

    #define NOISE_DETECT_ENERGY 0x00
    #define NOISE_DETECT_CS     0x01
    /************************************************************************************
     * Function:
     *      uint8_t MiApp_NoiseDetection(  uint32_t ChannelMap, uint8_t ScanDuration,
     *                                  uint8_t DetectionMode, uint8_t *NoiseLevel)
     *
     * Summary:
     *      This function perform a noise scan and returns the channel with least noise
     *
     * Description:        
     *      This is the primary user interface functions for the application layer to 
     *      perform noise detection on multiple channels.
     *
     * PreCondition:    
     *      Protocol initialization has been done. 
     *
     * Parameters: 
     *      uint32_t ChannelMap -  The bit map of channels to perform noise scan. The 32-bit
     *                          double uint16_t parameter use one bit to represent corresponding
     *                          channels from 0 to 31. For instance, 0x00000003 represent to 
     *                          scan channel 0 and channel 1. 
     *      uint8_t ScanDuration - The maximum time to perform scan on single channel. The
     *                          value is from 5 to 14. The real time to perform scan can
     *                          be calculated in following formula from IEEE 802.15.4 
     *                          specification 
     *                              960 * (2^ScanDuration + 1) * 10^(-6) second
     *      uint8_t DetectionMode -    The noise detection mode to perform the scan. The two possible
     *                              scan modes are
     *                              * NOISE_DETECT_ENERGY   Energy detection scan mode
     *                              * NOISE_DETECT_CS       Carrier sense detection scan mode
     *      uint8_t *NoiseLevel -  The noise level at the channel with least noise level
     *
     * Returns: 
     *      The channel that has the lowest noise level
     *
     * Example:
     *      <code>
     *      uint8_t NoiseLevel;
     *      OptimalChannel = MiApp_NoiseDetection(0xFFFFFFFF, 10, NOISE_DETECT_ENERGY, &NoiseLevel);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/  
    uint8_t    MiApp_NoiseDetection(uint32_t ChannelMap, uint8_t ScanDuration, uint8_t DetectionMode, OUTPUT uint8_t *NoiseLevel);
    
    #define POWER_STATE_SLEEP       0x00
    #define POWER_STATE_WAKEUP      0x01
    #define POWER_STATE_WAKEUP_DR   0x02
    /************************************************************************************
     * Function:
     *      uint8_t    MiApp_TransceiverPowerState(uint8_t Mode)
     *
     * Summary:
     *      This function put the RF transceiver into different power state. i.e. Put the 
     *      RF transceiver into sleep or wake it up.
     *
     * Description:        
     *      This is the primary user interface functions for the application layer to 
     *      put RF transceiver into sleep or wake it up. This function is only available
     *      to those wireless nodes that may have to disable the transceiver to save 
     *      battery power.
     *
     * PreCondition:    
     *      Protocol initialization has been done. 
     *
     * Parameters: 
     *      uint8_t Mode - The mode of power state for the RF transceiver to be set. The possible
     *                  power states are following
     *                  * POWER_STATE_SLEEP     The deep sleep mode for RF transceiver
     *                  * POWER_STATE_WAKEUP    Wake up state, or operating state for RF transceiver
     *                  * POWER_STATE_WAKEUP_DR Put device into wakeup mode and then transmit a 
     *                                          data request to the device's associated device
     *
     * Returns: 
     *      The status of the operation. The following are the possible status
     *      * SUCCESS           Operation successful
     *      * ERR_TRX_FAIL      Transceiver fails to go to sleep or wake up
     *      * ERR_TX_FAIL       Transmission of Data Request command failed. Only available if the
     *                          input mode is POWER_STATE_WAKEUP_DR.
     *      * ERR_RX_FAIL       Failed to receive any response to Data Request command. Only available
     *                          if input mode is POWER_STATE_WAKEUP_DR.
     *      * ERR_INVLAID_INPUT Invalid input mode. 
     *
     * Example:
     *      <code>
     *      // put RF transceiver into sleep
     *      MiApp_TransceiverPowerState(POWER_STATE_SLEEP;
     *
     *      // Put the MCU into sleep
     *      Sleep();    
     *
     *      // wakes up the MCU by WDT, external interrupt or any other means
     *      
     *      // make sure that RF transceiver to wake up and send out Data Request
     *      MiApp_TransceiverPowerState(POWER_STATE_WAKEUP_DR);
     *      </code>
     *
     * Remarks:    
     *      None
     *
     *****************************************************************************************/
    uint8_t    MiApp_TransceiverPowerState(uint8_t Mode);
    #define SUCCESS                 0x00
    #define ERR_TRX_FAIL            0x01
    #define ERR_TX_FAIL             0x02
    #define ERR_RX_FAIL             0x03
    #define ERR_INVALID_INPUT       0xFF

    /*******************************************************************************************
     * Function:
     *      bool MiApp_InitChannelHopping(uint32_t ChannelMap)
     *
     * Summary:
     *      
     *      This function tries to start a channel hopping (frequency agility) procedure
     *
     * Description:        
     *      This is the primary user interface function for the application to do energy 
     *      scan to locate the channel with least noise. If the channel is not current 
     *      operating channel, process of channel hopping will be started.
     *
     * PreCondition:    
     *      Transceiver has been initialized
     *
     * Parameters:      
     *      uint32_t ChannelMap -  The bit map of the candicate channels
     *                          which can be hopped to
     *                  
     * Returns: 
     *                  a boolean to indicate if channel hopping is initiated
     *
     * Example:
     *      <code>
     *      // if condition meets, scan all possible channels and hop 
     *      // to the one with least noise
     *      MiApp_InitChannelHopping(0xFFFFFFFF);
     *      </code>
     *
     * Remark:          The operating channel will change to the optimal 
     *                  channel with least noise
     *
     ******************************************************************************************/
    bool MiApp_InitChannelHopping( uint32_t ChannelMap);
    
    /********************************************************************************************
     * Function:
     *      bool MiApp_ResyncConnection(uint8_t ConnectionIndex, uint32_t ChannelMap)
     *
     * Summary:
     *      This function tries to resynchronize the lost connection with 
     *      peers, probably due to channel hopping
     *
     * Description:        
     *      This is the primary user interface function for the application to resynchronize a 
     *      lost connection. For a RFD device that goes to sleep periodically, it may not 
     *      receive the channel hopping command that is sent when it is sleep. The sleeping 
     *      RFD device depends on this function to hop to the channel that the rest of
     *      the PAN has jumped to. This function call is usually triggered by continously 
     *      communication failure with the peers.
     *
     * PreCondition:    
     *      Transceiver has been initialized
     *
     * Parameters:      
     *      uint32_t ChannelMap -  The bit map of channels to perform noise scan. The 32-bit
     *                          double uint16_t parameter use one bit to represent corresponding
     *                          channels from 0 to 31. For instance, 0x00000003 represent to 
     *                          scan channel 0 and channel 1. 
     *                  
     * Returns: 
     *                  a boolean to indicate if resynchronization of connection is successful
     *
     * Example:
     *      <code>
     *      // Sleeping RFD device resync with its associated device, usually the first peer
     *      // in the connection table
     *      MiApp_ResyncConnection(0, 0xFFFFFFFF);
     *      </code>
     *
     * Remark:          
     *      If operation is successful, the wireless node will be hopped to the channel that 
     *      the rest of the PAN is operating on.
     *
     *********************************************************************************************/    
    bool MiApp_ResyncConnection(uint8_t ConnectionIndex, uint32_t ChannelMap);

    //Sets address and PANID of the device at run time
    void MiApp_SetAddressPan(uint8_t *address, uint16_t panid);
    //Returns true if member of network
    bool MiApp_IsMemberOfNetwork(void);
    //returns address of the parent
    addr_t MiApp_GetParentAddress(void);
    //used by sleeping RFD to get buffered data from network
    void MiApp_RequestData(void);
    //Used by sleeping RFD to send data request to network.
    //If data was buffered by the network it will be delivered following this request
    bool MiApp_SendDataRequest(void);
    
    //Initializes SLEEP RFD Buffer (on coordinator; not used on RFDs).
    //buffer - pointer to byte array
    //bufferSize - Length of buffer
    //rrfdMaxDataSize - Maximum length of data to store for each Sleeping RFD
    //Returns number of Sleeping RFD Buffers allocated.
    uint16_t MiApp_InitSleepRFDBuffers(uint8_t *buffer, uint16_t bufferSize, uint16_t rfdMaxDataSize);

    //MACRO used to define size of buffer
    //RFDCount - Number of Sleeping RFDs to support
    //DATALEN - Maximum length of packet to store for RFD
    //Other overheads are included in this MACRO automatically
    #define MAKE_RFDBUFFLEN(RFDCount,DATALEN)      ((ADDRESS_LEN+3+DATALEN)*RFDCount)
    
    //Function to enable/disable network security
    //User can already enable/disable user level security by using true/falce in MiApp_UnicastAddress() function
    //But network level messages (that user does not normally deal with) can also be secured
    //This function allows for such operation;  care should be taken with this because joining device
    //must already know the key to the network it's trying to join or it won't be able to join it.
    void MiApp_SetNetworkSecure(bool isSecure);
    
    
    extern RECEIVED_MESSAGE rxMessage;
    extern uint8_t TxData;
    extern uint8_t TxBuffer[];
    
    typedef enum MessageType_t_def
    {
        mReserved0,
        mReserved1,
        mReserved2,
        mWBootloader,//required by Stack, do not use
        mReserved4,
        mReserved5,
        mReserved6,
        mReserved7,
        mReserved8,
        mReserved9,
        mUserDataType,//This is user Message Type; user should add to this enum 
                    //if new message types are required;  Reserved Enums should not be used
    }MessageType_t;
    
#endif

