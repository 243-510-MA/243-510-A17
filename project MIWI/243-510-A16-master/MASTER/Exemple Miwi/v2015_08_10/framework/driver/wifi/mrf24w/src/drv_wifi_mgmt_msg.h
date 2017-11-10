/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    drv_wifi_mgmt_msg.h

  Summary:
    Module for Microchip TCP/IP Stack
    -Provides access to MRF24W WiFi controller
    -Reference: MRF24W Data sheet, IEEE 802.11 Standard

  Description:
    MRF24W Driver Management messages

 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) <2014> released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef __DRV_WF_MGMT_MSG_H_
#define __DRV_WF_MGMT_MSG_H_

/*----------------------------------------------*/
/* Management Message Request/Response Subtypes */
/*----------------------------------------------*/
typedef enum
{
    /* Misc subtypes */
    WF_SCAN_SUBTYPE                             = 1,
    WF_JOIN_SUBTYPE                             = 2,
    WF_AUTH_SUBTYPE                             = 3,
    WF_ASSOC_SUBTYPE                            = 4,
    WF_DISCONNECT_SUBTYPE                       = 5,
    WF_DISASOCC_SUBTYPE                         = 6,
    WF_SET_POWER_MODE_SUBTYPE                   = 7,
    WF_SET_PM_KEY_SUBTYPE                       = 8,
    WF_SET_WEP_MAP_SUBTYPE                      = 9,
    WF_SET_WEP_KEY_SUBTYPE                      = 10,
    WF_SET_TEMP_KEY_SUBTYPE                     = 11,
    WF_CALC_PSK_KEY_SUBTYPE                     = 12,
    WF_SET_WEP_KEY_ID_SUBTYPE                   = 13,
    WF_CONFIG_KEY_SPACE_SUBTYPE                 = 14,
    WF_SET_PARAM_SUBTYPE                        = 15,
    WF_GET_PARAM_SUBTYPE                        = 16,
    WF_ADHOC_CONNECT_SUBTYPE                    = 17,
    WF_ADHOC_START_SUBTYPE                      = 18,

    /* Connection Profile Message Subtypes */
    WF_CP_CREATE_PROFILE_SUBTYPE                = 21,
    WF_CP_DELETE_PROFILE_SUBTYPE                = 22,
    WF_CP_GET_ID_LIST_SUBTYPE                   = 23,
    WF_CP_SET_ELEMENT_SUBTYPE                   = 24,
    WF_CP_GET_ELEMENT_SUBTYPE                   = 25,

    /* Connection Algorithm Message Subtypes */
    WF_CA_SET_ELEMENT_SUBTYPE                   = 26,
    WF_CA_GET_ELEMENT_SUBTYPE                   = 27,

    /* Connnection Manager Message Subtypes */
    WF_CM_CONNECT_SUBYTPE                       = 28,
    WF_CM_DISCONNECT_SUBYTPE                    = 29,
    WF_CM_GET_CONNECTION_STATUS_SUBYTPE         = 30,

    WF_SCAN_START_SUBTYPE                       = 31,
    WF_SCAN_GET_RESULTS_SUBTYPE                 = 32,

    WF_CM_INFO_SUBTYPE                          = 33,

    WF_SCAN_FOR_IE_SUBTYPE                      = 34,  /* not yet supported */
    WF_SCAN_IE_GET_RESULTS_SUBTYPE              = 35,  /* not yet supported */

    WF_CM_GET_CONNECTION_STATISTICS_SUBYTPE     = 36,  /* not yet supported so moved here for now */
    WF_NUM_REQUEST_SUBTYPES

} tMgmtMsgSubtypes;

#if defined(MRF24WG)
    /*-------------------------------------------------------------*/
    /* Connection Profile Element ID�s                             */
    /* Used in conjunction with the WF_CP_SET_ELEMENT_SUBTYPE and  */
    /* WF_CP_GET_ELEMENT_SUBTYPE message subtypes                  */
    /*-------------------------------------------------------------*/
    typedef enum
    {
        WF_CP_ELEMENT_ALL               = 0,  /* sends all elements in CP struct */
        WF_CP_ELEMENT_SSID              = 1,
        WF_CP_ELEMENT_BSSID             = 2,
        WF_CP_ELEMENT_SECURITY          = 3,
        WF_CP_ELEMENT_NETWORK_TYPE      = 4,
        WF_CP_ELEMENT_ADHOC_BEHAVIOR    = 5,
        WF_CP_ELEMENT_WEP_KEY_INDEX     = 6,
        WF_CP_ELEMENT_SSID_TYPE         = 7,
        WF_CP_ELEMENT_WEPKEY_TYPE       = 8,
        WF_CP_ELEMENT_UPDATE_PMK        = 9,
        WF_CP_ELEMENT_READ_WPS_CRED     = 10
    } tCPElementIds;
#else /* MRF24WB */
    /*-------------------------------------------------------------*/
    /* Connection Profile Element ID�s                             */
    /* Used in conjunction with the WF_CP_SET_ELEMENT_SUBTYPE and  */
    /* WF_CP_GET_ELEMENT_SUBTYPE message subtypes                  */
    /*-------------------------------------------------------------*/
    typedef enum
    {
        WF_CP_ELEMENT_ALL               = 0,  /* sends all elements in CP struct */
        WF_CP_ELEMENT_SSID              = 1,
        WF_CP_ELEMENT_BSSID             = 2,
        WF_CP_ELEMENT_SECURITY          = 3,
        WF_CP_ELEMENT_NETWORK_TYPE      = 4,
        WF_CP_ELEMENT_ADHOC_BEHAVIOR    = 5,
        WF_CP_ELEMENT_WEP_KEY_INDEX     = 6,
        WF_CP_ELEMENT_SSID_TYPE         = 7,
        WF_CP_ELEMENT_WEPKEY_TYPE       = 8
    } tCPElementIds;
#endif

#if defined(MRF24WG)
    /*-------------------------------------------------------------*/
    /* Connection Algorithm Element ID�s                           */
    /* Used in conjunction with the WF_CA_SET_ELEMENT_SUBTYPE and  */
    /* WF_CA_GET_ELEMENT_SUBTYPE message subtypes                  */
    /*-------------------------------------------------------------*/
    typedef enum
    {
        WF_CA_ELEMENT_ALL                          = 0,
        WF_CA_ELEMENT_SCANTYPE                     = 1,
        WF_CA_ELEMENT_RSSI                         = 2,
        WF_CA_ELEMENT_CP_LIST                      = 3,
        WF_CA_ELEMENT_LIST_RETRY_COUNT             = 4,
        WF_CA_ELEMENT_EVENT_NOTIFICATION_ACTION    = 5,
        WF_CA_ELEMENT_BEACON_TIMEOUT_ACTION        = 6,
        WF_CA_ELEMENT_DEAUTH_ACTION                = 7,
        WF_CA_ELEMENT_CHANNEL_LIST                 = 8,
        WF_CA_ELEMENT_LISTEN_INTERVAL              = 9,
        WF_CA_ELEMENT_BEACON_TIMEOUT               = 10,
        WF_CA_ELEMENT_SCAN_COUNT                   = 11,
        WF_CA_ELEMENT_MIN_CHANNEL_TIME             = 12,
        WF_CA_ELEMENT_MAX_CHANNEL_TIME             = 13,
        WF_CA_ELEMENT_PROBE_DELAY                  = 14,
        WF_CA_ELEMENT_DTIM_INTERVAL                = 15,
        WF_CA_ELEMENT_BEACON_PERIOD                = 16
    } tCAElementIds;

#else /* MRF24WB */
    /*-------------------------------------------------------------*/
    /* Connection Algorithm Element ID�s                           */
    /* Used in conjunction with the WF_CA_SET_ELEMENT_SUBTYPE and  */
    /* WF_CA_GET_ELEMENT_SUBTYPE message subtypes                  */
    /*-------------------------------------------------------------*/
    typedef enum
    {
        WF_CA_ELEMENT_ALL                          = 0,
        WF_CA_ELEMENT_SCANTYPE                     = 1,
        WF_CA_ELEMENT_RSSI                         = 2,
        WF_CA_ELEMENT_CP_LIST                      = 3,
        WF_CA_ELEMENT_LIST_RETRY_COUNT             = 4,
        WF_CA_ELEMENT_EVENT_NOTIFICATION_ACTION    = 5,
        WF_CA_ELEMENT_BEACON_TIMEOUT_ACTION        = 6,
        WF_CA_ELEMENT_DEAUTH_ACTION                = 7,
        WF_CA_ELEMENT_CHANNEL_LIST                 = 8,
        WF_CA_ELEMENT_LISTEN_INTERVAL              = 9,
        WF_CA_ELEMENT_BEACON_TIMEOUT               = 10,
        WF_CA_ELEMENT_SCAN_COUNT                   = 11,
        WF_CA_ELEMENT_MIN_CHANNEL_TIME             = 12,
        WF_CA_ELEMENT_MAX_CHANNEL_TIME             = 13,
        WF_CA_ELEMENT_PROBE_DELAY                  = 14
    } tCAElementIds;
#endif

#if defined(WF_CM_DEBUG)
/*-------------------------------------------------------------*/
/* CM INFO ID�s                                                */
/* Used in conjunction with the WF_CM_INFO_SUBTYPE             */
/*-------------------------------------------------------------*/
typedef enum
{
    WF_CM_INFO_GET_FSM_STATS                   = 0,
    WF_CM_INFO_CLEAR_FSM_STATS                 = 1
} tCMInfoID;
#endif

#if defined(MRF24WG)
    /* tWFParam - Names (ID's) of WF MAC configurable parameters. */
    typedef enum
    {

        PARAM_MAC_ADDRESS                   = 1,       /* the device MAC address (6 bytes)                              */
        PARAM_REGIONAL_DOMAIN               = 2,       /* the device Regional Domain (1 byte)                           */
        PARAM_RTS_THRESHOLD                 = 3,       /* the RTS byte threshold 256 - 2347 (2 bytes)                   */
        PARAM_LONG_FRAME_RETRY_LIMIT        = 4,       /* the long Frame Retry limit  (1 byte)                          */
        PARAM_SHORT_FRAME_RETRY_LIMIT       = 5,       /* the short Frame Retry limit (1 byte)                          */
        PARAM_TX_LIFETIME_TU                = 6,       /* the Tx Request lifetime in TU's 0 - 4194303 (4 bytes)         */
        PARAM_RX_LIFETIME_TU                = 7,       /* the Rx Frame lifetime in TU's 0 - 4194303 (4 bytes)           */
        PARAM_SUPPLICANT_ON_OFF             = 8,       /* boolean 1 = on 0 = off (1 byte)                               */
        PARAM_CONFIRM_DATA_TX_REQ           = 9,       /* boolean 1 = on 0 = off (1 byte)                               */
        PARAM_MASTER_STATE                  = 10,      /* master state of the MAC using enumerated values (1 byte)      */
        PARAM_HOST_ALERT_BITS               = 11,      /* a bit field which enables/disables various asynchronous       */
                                                       /*   indications from the MAC to the host (2 bytes)              */
        PARAM_NUM_MISSED_BEACONS            = 12,      /* number of consecutive beacons MAC can miss before it          */
                                                       /*   considers the network lost (1 byte)                         */
        PARAM_DIFS_AND_EIFS                 = 13,      /* delay intervals in usec DIFS and EIFS ( 2 * 2 bytes)          */
        PARAM_TX_POWER                      = 14,      /* max and min boundaries for Tx power (2 * 2 bytes)             */
        PARAM_DEFAULT_DEST_MAC_ADDR         = 15,      /* stores a persistant destination MAC address for small         */
                                                       /*   Tx Requests (6 bytes)                                       */
        PARAM_WPA_INFO_ELEMENT              = 16,      /* stores a WPA info element (IE) in 802.11 IE format.  Used     */
                                                       /*   in Assoc Request and Supplicant exchange (3 - 258 bytes)    */
        PARAM_RSN_INFO_ELEMENT              = 17,      /* stores a RSN info element (IE) in 802.11 IE format.  Used     */
                                                       /*   in Assoc Request and Supplicant exchange (3 - 258 bytes)    */
        PARAM_ON_OFF_RADIO                  = 18,      /* bool to force a radio state change 1 = on 0 = off (1 byte)    */
        PARAM_COMPARE_ADDRESS               = 19,      /* A MAC address used to filter received frames                  */
                                                       /*   (sizeof(tAddressFilterInput) = 8 bytes)                     */
        PARAM_SUBTYPE_FILTER                = 20,      /* Bitfield used to filter received frames based on type and     */
                                                       /* sub-type (sizeof(tAddressFilterInput) = 4 bytes)              */
        PARAM_ACK_CONTROL                   = 21,      /* Bitfield used to control the type of frames that cause ACK    */
                                                       /*   responses (sizeof(tAckControlInput) = 4 bytes)              */
        PARAM_STAT_COUNTERS                 = 22,      /* Complete set of statistics counters that are maintained by    */
                                                       /*   the MAC                                                     */
        PARAM_TX_THROTTLE_TABLE             = 23,      /* Custom Tx Rate throttle table to be used to control tx Rate   */
        PARAM_TX_THROTTLE_TABLE_ON_OFF      = 24,      /* A boolean to enable/disable use of the throttle Table and a   */
                                                       /*   tx rate to use if the throttle table is disabled            */
        PARAM_TX_CONTENTION_ARRAY           = 25,      /* Custom Retry contention ladder used for backoff calculation   */
                                                       /*   prior to a Tx attempt                                       */
        PARAM_SYSTEM_VERSION                = 26,      /* 2 byte representation of a version number for the ROM and     */
                                                       /*  Patch                                                        */
        PARAM_STATUE_INFO                   = 27,      /* MAC State information                                         */
        PARAM_SECURITY_CONTROL              = 28,      /* 2 byte data structure to enable/disable encryption            */
        PARAM_FACTORY_SET_TX_MAX_POWER      = 29,      /* gets the factory-set tx max power level                       */
        PARAM_MRF24WB0M                     = 30,      /* a set enables MRF24WB0M Mode, a get gets the version          */
        PARAM_CONNECT_CONTEXT               = 31,      /* gets current connection status                                */
        PARAM_WPA_TIMEOUT                   = 33,      /* configure 4 way handshaking transaction timeout               */
        PARAM_TX_MODE                       = 34,      /* choose tx mode                                                */
        PARAM_ENABLE_DEBUG_PRINT            = 35,      /* enable WPS / P2P debug prints                                 */
        PARAM_LINK_DOWN_THRESHOLD           = 37,      /* sets link down threshold                                      */
        PARAM_STACK_VERSION                 = 38,      /* notifies stack version to RF FW                               */
        PARAM_SET_PSK                       = 39,      /* set psk                                                       */
        PARAM_YIELD_PASSPHRASE_TOHOST       = 40,      /* forces module FW to send passphrase to host in WPS            */
        PARAM_USE_SW_MULTICAST_FILTER       = 41       /* force to use sw multicast filter instead of hw filter         */
#if (defined(STACK_USE_AUTOUPDATE_HTTPSERVER) || defined(STACK_USE_AUTOUPDATE_TCPCLIENT)|| defined(STACK_USE_AUTOUPDATE_UART)) && defined(WF_CS_TRIS)
        ,
        PARAM_FLASH_VERIFY                  = 118,     //Checksum verify
        PARAM_FLASH_update                  = 119,     //AutoUpdate
        PARAM_FLASH_READ                    = 120,     //AutoUpdate
        PARAM_FLASH_WRITE                   = 121      //AutoUpdate
#endif
    } tWFParam;
#if (defined(STACK_USE_AUTOUPDATE_HTTPSERVER) || defined(STACK_USE_AUTOUPDATE_TCPCLIENT)|| defined(STACK_USE_AUTOUPDATE_UART)) && defined(WF_CS_TRIS)
#define UPDATE_SECURITY_KEY 0x0A0B0C            //Security Key
#define UPDATE_SECURITY_KEY_1 0x0A
#define UPDATE_SECURITY_KEY_2 0x0B
#define UPDATE_SECURITY_KEY_3 0x0C
#define UPDATE_CMD_ERASE0   0x0             //Erase  Bank 0
#define UPDATE_CMD_ERASE1   0x1             //Erase  bank 1
#define UPDATE_CMD_CPY_0TO1 0x2             //Copy Bank0 to  bank1
#define UPDATE_CMD_CPY_1TO0 0x3             //Copy Bank1 to  bank0
#define UPDATE_CMD_CPY_CALIBRATION 0x4      //Copy last 128K bytes from Bank1 to Bank0
#define UPDATE_CMD_CMPARE   0x5             //Compare Bank0 and Bnak1
#endif

#else /* MRF24WB */
    /* tWFParam - Names (ID's) of WF MAC configurable parameters. */
    typedef enum
    {

        PARAM_MAC_ADDRESS                   = 1,       /* the device MAC address (6 bytes)                            */
        PARAM_REGIONAL_DOMAIN               = 2,       /* the device Regional Domain (1 byte)                         */
        PARAM_RTS_THRESHOLD                 = 3,       /* the RTS byte threshold 256 - 2347 (2 bytes)                 */
        PARAM_LONG_FRAME_RETRY_LIMIT        = 4,       /* the long Frame Retry limit  (1 byte)                        */
        PARAM_SHORT_FRAME_RETRY_LIMIT       = 5,       /* the short Frame Retry limit (1 byte)                        */
        PARAM_TX_LIFETIME_TU                = 6,       /* the Tx Request lifetime in TU's 0 - 4194303 (4 bytes)       */
        PARAM_RX_LIFETIME_TU                = 7,       /* the Rx Frame lifetime in TU's 0 - 4194303 (4 bytes)         */
        PARAM_SUPPLICANT_ON_OFF             = 8,       /* boolean 1 = on 0 = off (1 byte)                             */
        PARAM_CONFIRM_DATA_TX_REQ           = 9,       /* boolean 1 = on 0 = off (1 byte)                             */
        PARAM_MASTER_STATE                  = 10,      /* master state of the MAC using enumerated values (1 byte)    */
        PARAM_HOST_ALERT_BITS               = 11,      /* a bit field which enables/disables various asynchronous     */
                                                       /*   indications from the MAC to the host (2 bytes)            */
        PARAM_NUM_MISSED_BEACONS            = 12,      /* number of consecutive beacons MAC can miss before it        */
                                                       /*   considers the network lost (1 byte)                       */
        PARAM_DIFS_AND_EIFS                 = 13,      /* delay intervals in usec DIFS and EIFS ( 2 * 2 bytes)        */
        PARAM_TX_POWER                      = 14,      /* max and min boundaries for Tx power (2 * 2 bytes)           */
        PARAM_DEFAULT_DEST_MAC_ADDR         = 15,      /* stores a persistant destination MAC address for small       */
                                                       /*   Tx Requests (6 bytes)                                     */
        PARAM_WPA_INFO_ELEMENT              = 16,      /* stores a WPA info element (IE) in 802.11 IE format.  Used   */
                                                       /*   in Assoc Request and Supplicant exchange (3 - 258 bytes)  */
        PARAM_RSN_INFO_ELEMENT              = 17,      /* stores a RSN info element (IE) in 802.11 IE format.  Used   */
                                                       /*   in Assoc Request and Supplicant exchange (3 - 258 bytes)  */
        PARAM_ON_OFF_RADIO                  = 18,      /* bool to force a radio state change 1 = on 0 = off (1 byte)  */
        PARAM_COMPARE_ADDRESS               = 19,      /* A MAC address used to filter received frames                */
                                                       /*   (sizeof(tAddressFilterInput) = 8 bytes)                   */
        PARAM_SUBTYPE_FILTER                = 20,      /* Bitfield used to filter received frames based on type and   */
                                                       /* sub-type (sizeof(tAddressFilterInput) = 4 bytes)            */
        PARAM_ACK_CONTROL                   = 21,      /* Bitfield used to control the type of frames that cause ACK  */
                                                       /*   responses (sizeof(tAckControlInput) = 4 bytes)            */
        PARAM_STAT_COUNTERS                 = 22,      /* Complete set of statistics counters that are maintained by  */
                                                       /*   the MAC                                                   */
        PARAM_TX_THROTTLE_TABLE             = 23,      /* Custom Tx Rate throttle table to be used to control tx Rate */
        PARAM_TX_THROTTLE_TABLE_ON_OFF      = 24,      /* A boolean to enable/disable use of the throttle Table and a */
                                                       /*   tx rate to use if the throttle table is disabled          */
        PARAM_TX_CONTENTION_ARRAY           = 25,      /* Custom Retry contention ladder used for backoff calculation */
                                                       /*   prior to a Tx attempt                                     */
        PARAM_SYSTEM_VERSION                = 26,      /* 2 byte representation of a version number for the ROM and   */
                                                       /*  Patch                                                      */
        PARAM_STATUE_INFO                   = 27,      /* MAC State information                                       */
        PARAM_SECURITY_CONTROL              = 28,      /* 2 byte data structure to enable/disable encryption          */
        PARAM_FACTORY_SET_TX_MAX_POWER      = 29,      /* gets the factory-set tx max power level                     */
        PARAM_MRF24WB0M                     = 30,      /* a set enables MRF24WB0M Mode, a get gets the version        */
        PARAM_AGGRESSIVE_PS                 = 32,      /* Allows to turn off RF power quicker                         */
        PARAM_CONNECT_CONTEXT               = 33,      /* gets current connection status                              */
        PARAM_DEFERRED_POWERSAVE            = 34,      /* delay power start after dhcp done                           */
    PARAM_LINK_DOWN_THRESHOLD               = 35       /* sets link down threshold */
    } tWFParam;
#endif    /* MRF24WG */

/* used in byte 2 of WF_CONNECTION_LOST_EVENT_SUBTYPE */
#define WF_CONNECTION_TEMPORARILY_LOST  ((uint8_t)0)
#define WF_CONNECTION_PERMANENTLY_LOST  ((uint8_t)1)

#define WF_FLASH_UPDATE_NOT_SUCCESSFUL ((uint8_t)0)
#define WF_FLASH_UPDATE_SUCCESSFUL     ((uint8_t)1)

#define WF_MAX_TX_MGMT_MSG_SIZE         (128)

#define DO_NOT_FREE_MGMT_BUFFER             (0)
#define FREE_MGMT_BUFFER                    (1)

#define MGMT_RESP_1ST_DATA_BYTE_INDEX       (4)  /* first data byte of Mgmt response starts at index 4 */

#if defined(WF_DEBUG)
/* this block of defines is used to check illegal reentry when in WF API functions */
#define WF_ENTERING_FUNCTION    (1)
#define WF_LEAVING_FUNCTION     (0)

/* bit masks for functions that need to be tracked when they are called */
#define WF_PROCESS_EVENT_FUNC   ((uint8_t)0x01)

#endif /* WF_DEBUG */

/*==========================================================================*/
/*                                  TYPEDEFS                                */
/*==========================================================================*/

/* This structure describes the format of the first four bytes of all */
/* mgmt response messages received from the MRF24W                 */
typedef struct mgmtRxHdrStruct
{
    uint8_t  type;          /* always 0x02                  */
    uint8_t  subtype;       /* mgmt msg subtype             */
    uint8_t  result;        /* 1 if success, else failure   */
    uint8_t  macState;      /* not used                     */

} tMgmtMsgRxHdr;

typedef struct mgmtIndicateHdrStruct
{
    uint8_t type;       /* always WF_MGMT_INDICATE_MSG_TYPE (2) */
    uint8_t subType;    /* event type                           */
} tMgmtIndicateHdr;

typedef struct mgmtIndicatePassphraseReady
{
    uint8_t keyLen;
    uint8_t key[64];
    uint8_t ssidLen;
    uint8_t ssid[32];
} tMgmtIndicatePassphraseReady;

typedef struct mgmtIndicateSoftAPEvent
{
    uint8_t event;
    uint8_t reason;
    uint8_t address[6];
} tMgmtIndicateSoftAPEvent;

typedef struct PassphraseReady
{
    bool valid;
    tMgmtIndicatePassphraseReady passphrase;
} tPassphraseReady;

/*==========================================================================*/
/*                                  FUNCTION PROTOTYPES                     */
/*==========================================================================*/

void WaitForMgmtResponseAndReadData(uint8_t expectedSubtype,
                                    uint8_t numDataBytes,
                                    uint8_t startIndex,
                                    uint8_t *p_data);

void SendMgmtMsg(uint8_t *p_header,
                 uint8_t headerLength,
                 uint8_t *p_data,
                 uint8_t dataLength);

void   WaitForMgmtResponse(uint8_t expectedSubtype, uint8_t freeAction);

void SendGetParamMsg(uint8_t paramType, uint8_t *p_paramData, uint8_t paramDataLength);

void SendSetParamMsg(uint8_t paramType, uint8_t *p_paramData, uint8_t paramDataLength);

void WFProcessMgmtIndicateMsg(void);

void SignalMgmtConfirmReceivedEvent(void);

void WFSetConnectionState(uint8_t connectionState);

uint8_t WFGetConnectionState(void);

void WF_SetTxDataConfirm(uint8_t state);     /* WF_ENABLED or WF_DISABLED */
void WF_GetTxDataConfirm(uint8_t *p_state);  /* WF_ENABLED or WF_DISABLED */
bool WFisTxMgmtReady(void);

void WFFreeMgmtTx(void);

void SendRAWManagementFrame(uint16_t bufLen);

void WFEnableMRF24WB0MMode(void);

//void WFGetMRF24WB0MVersion(uint8_t *p_version);  // Use alternative WF_GetDeviceInfo

void WFEnableBroadcastProbeResponse(void);
void WFEnableDeferredPowerSave(void);

/* When asserts are enabled, call this function.  When asserts are not enabled compile it out */
#if defined(WF_DEBUG)
    /* if asserts enabled this function will be called */
    void WFSetFuncState(uint8_t func, uint8_t state);
#else
    /* if asserts disabled the function is no-op'ed */
    #define WFSetFuncState(x,y)
#endif

#endif /* __DRV_WF_MGMT_MSG_H_ */
