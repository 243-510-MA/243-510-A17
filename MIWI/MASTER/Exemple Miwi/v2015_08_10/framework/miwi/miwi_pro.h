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

#ifndef __MIWI_PRO_H_
#define __MIWI_PRO_H_

#include "miwi_config.h"
#include "system.h"

#if defined(PROTOCOL_MIWI_PRO)
/************************ HEADERS **********************************/
#include "miwi_config_pro.h"
#include "SymbolTime.h"
#include "drv_mrf_miwi.h"

#define INPUT
#define OUTPUT
#define IOPUT

#define MAX_HOPS    (NUM_COORDINATOR+1)

#if !defined(ENABLE_NETWORK_FREEZER)
    #error "Network Freezer feature must be enabled for MiWi PRO protocol. Check ConfigApp.h"
#endif

/************************ FUNCTION PROTOTYPES **********************/
void MiWiPROTasks(void);	
uint8_t findNextNetworkEntry(void);
void ClearNetworkTable(void);
uint8_t SearchForLongAddress(void);
uint8_t SearchForShortAddress(void);
void SendIndirectPacket(uint8_t *Address, uint8_t *AltAddress, uint8_t isAltAddress);
uint8_t AddNodeToNetworkTable(void);
void DiscoverNodeByEUI(void);
void OpenSocket(void);
bool isSameAddress(INPUT uint8_t *Address1, INPUT uint8_t *Address2);
void DumpConnection(uint8_t index);


/************************ DEFINITIONS ******************************/
#define FRAME_TYPE_BEACON   0x00
#define FRAME_TYPE_DATA     0x01
#define FRAME_TYPE_ACK      0x02
#define FRAME_TYPE_COMMAND  0x03

#define MAC_COMMAND_ASSOCIATION_REQUEST             0x01
#define MAC_COMMAND_ASSOCIATION_RESPONSE            0x02
    #define ASSOCIATION_SUCCESSFUL                  0x00
    #define ASSOCIATION_PAN_FULL                    0x01
    #define ASSOCIATION_ACCESS_DENIED               0x02
#define MAC_COMMAND_DISASSOCIATION_NOTIFICATION     0x03
#define MAC_COMMAND_DATA_REQUEST                    0x04
#define MAC_COMMAND_PAN_ID_CONFLICT_NOTIFICATION    0x05
#define MAC_COMMAND_ORPHAN_NOTIFICATION             0x06
#define MAC_COMMAND_BEACON_REQUEST                  0x07
#define MAC_COMMAND_COORDINATOR_REALIGNMENT         0x08


#define MAC_COMMAND_TIME_SYNC_DATA_PACKET               0x8A
#define MAC_COMMAND_TIME_SYNC_COMMAND_PACKET            0x8B


#define MIWI_PRO_PROTOCOL_ID 0x5D
#define MIWI_PROTOCOL_ID 0x4D
#define MIWI_PRO_VERSION_NUM 0x41               //v4.1
#define MIWI_PRO_ACK_REQ 0x04

#define PAYLOAD_START       11

#define FAMILY_NONE         0
#define FAMILY_PARENT       0x01
#define FAMILY_CHILD        0x02
#define FAMILY_SIBLING      0x03


#define ROLE_FFD_END_DEVICE 0x00
#define ROLE_COORDINATOR    0x01
#define ROLE_PAN_COORDINATOR 0x02

#define DATA_REQUEST_ASSOCIATION_RESPONSE 0x00
#define DATA_REQUEST_SHORT_ADDRESSES 0x01


#define MIWI_PRO_HEADER_LEN     11
/* Report type and ID definitions */
/* as a user you are able to use Report types 0x10 - 0xFF */

#define MIWI_PRO_STACK_REPORT_TYPE              0x00
    #define OPEN_SOCKET_REQUEST             0x10
    #define OPEN_SOCKET_RESPONSE            0x11
    #define EUI_ADDRESS_SEARCH_REQUEST      0x20
    #define EUI_ADDRESS_SEARCH_RESPONSE     0x21
    #define ACK_REPORT_TYPE                 0x30
    
    #define CHANNEL_HOPPING_REQUEST         0x40
    #define RESYNCHRONIZATION_REQUEST       0x41
    #define RESYNCHRONIZATION_RESPONSE      0x42
    
    #define COORDINATOR_REPORT              0xA0
    #define COORDINATOR_RESPONSE            0xA1
    #define FAMILY_TREE_REQUEST             0xA2
    #define FAMILY_TREE_REPORT              0xA3
    #define ROUTING_TABLE_REPORT            0xA4
    
    #define FA_COORDINATOR_REQUEST          0xB0
    #define FA_ENERGY_SCAN_REQUEST          0xB1
    #define FA_SUGGEST_CHANNEL              0xB2
    #define FA_DISPUTE_CHANNEL              0xB3

#define COORDINATOR_REPORT_STATUS_JOIN      0x00
#define COORDINATOR_REPORT_STATUS_LEAVE     0x01

/************************ DATA TYPE *******************************/
typedef union _MIWI_PRO_STATE_MACHINE
{
    uint8_t Val;
    struct {
        uint8_t searchingForNetwork    :1;
        uint8_t memberOfNetwork        :1;
        uint8_t RxHasUserData          :1;
        uint8_t MiWiPROAckInProgress   :1;
        uint8_t EUISearching           :1;
        uint8_t DataRequesting         :1;
        uint8_t Resynning              :1;
        uint8_t Sleeping               :1;
    } bits;
} MIWI_PRO_STATE_MACHINE;

typedef union _MIWI_PRO_STATUS
{
    uint8_t Val;
    struct
    {
        uint8_t COMM_RANDOM_SEND       :1;
        uint8_t COMM_FAMILY_TREE       :3;
        uint8_t COMM_ROUTING_TABLE     :3;
        uint8_t COMM_FREQ_HOPPING      :1;
    } bits;
} MIWI_PRO_STATUS;


typedef struct _OPEN_SOCKET
{
    union _OPEN_SOCKET_STATUS
    {
        struct _OPEN_SOCKET_STATUS_bits
        {
            uint8_t matchFound :1;
            uint8_t requestIsOpen :1;
            uint8_t itIsMe         :1;
        } bits;
        uint8_t Val;
    } status;
    uint8_t socketHandle;
    API_UINT16_UNION ShortAddress1;
    uint8_t LongAddress1[MY_ADDRESS_LENGTH];
    #if ADDITIONAL_NODE_ID_SIZE > 0
        uint8_t AdditionalNodeID1[ADDITIONAL_NODE_ID_SIZE];
    #endif
    #ifdef NWK_ROLE_COORDINATOR
        API_UINT16_UNION ShortAddress2;
        uint8_t LongAddress2[MY_ADDRESS_LENGTH];
    #endif
    MIWI_TICK socketStart;
} OPEN_SOCKET;



/******************************************************************
 * Overview: The structure to store indirect messages for devices turn off 
 * radio when idle
 *****************************************************************/
typedef struct 
{
    MIWI_TICK        TickStart;      // start time of the indirect message. Used for checking 
                                // indirect message time out
    #if defined(IEEE_802_15_4)                                
        API_UINT16_UNION    DestPANID;      // the PAN identifier for the destination node
    #endif
    uint8_t        DestAddress[MY_ADDRESS_LENGTH];             // unicast destination long address
    union 
    {
        uint8_t    Val;                        // value for the flags
        struct 
        {
            uint8_t    isValid         : 1;    // if this indirect message is valid
            uint8_t    isBroadcast     : 1;    // if this indirect message is for broadcasting
            uint8_t    isCommand       : 1;    // if this indirect message a command
            uint8_t    isSecured       : 1;    // if this indirect message requires encryption
            uint8_t    isAltAddr       : 1;    // if the destination address a short address
        } bits;                             // bit map of the flags
    } flags;                                // flags for indirect message
    uint8_t        PayLoadSize;                // the indirect message pay load size
    #if (TX_BUFFER_SIZE + MIWI_PRO_HEADER_LEN) > 112
        uint8_t        PayLoad[112];
    #else
        uint8_t        PayLoad[TX_BUFFER_SIZE+MIWI_PRO_HEADER_LEN];    // the indirect message pay load
    #endif
} INDIRECT_MESSAGE;


/************************ EXTERNAL VARIABLES **********************/

extern MIWI_PRO_STATE_MACHINE MiWiPROStateMachine;
extern uint8_t TxBuffer[];
extern uint8_t TxData;
extern API_UINT16_UNION myPANID;
extern API_UINT16_UNION myShortAddress;
extern uint8_t myParent;
extern uint8_t tempLongAddress[MY_ADDRESS_LENGTH];
extern API_UINT16_UNION tempShortAddress;
extern OPEN_SOCKET openSocketInfo;

/************************ MACROS **********************************/
#define MAC_FlushTx() {TxData = 0;}
 
#define MSK_SRC_PRSNT           0x01
#define MSK_DST_PRSNT           0x02
#define MSK_ALT_SRC_ADDR        0x04
#define MSK_ALT_DST_ADDR        0x08

#define PACKET_TYPE_BEACON      0x03

#define MICRO_SECOND_PER_COUNTER_TICK   (1000000ul / ( COUNTER_CRYSTAL_FREQ / 8 ))
    
#endif

#endif

    
    
