/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    drv_wifi_connection_profile.c

  Summary:
    Module for Microchip TCP/IP Stack
    -Provides access to MRF24W WiFi controller
    -Reference: MRF24W Data sheet, IEEE 802.11 Standard

  Description:
    MRF24W Driver Connection Profile

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

/*******************************************************************************
 *                                 INCLUDES
 *******************************************************************************/

#include "tcpip/tcpip.h"

#if defined(WF_CS_TRIS)

#include "drv_wifi_mac.h"

/* used for assertions */
#if defined(WF_DEBUG)
    #define WF_MODULE_NUMBER    WF_MODULE_WF_CONNECTION_PROFILE
#endif

#if !defined(MRF24WG)
#define RAW_MGMT_RX_ID  RAW_RX_ID
#endif

/*******************************************************************************
 *                          LOCAL DATA TYPES
 *******************************************************************************/
/* header format for response to CP Get Element message */
typedef struct cpElementResponseStruct
{
    tMgmtMsgRxHdr  mgmtHdr; /* normal 4-byte hdr for all mgmt responses */
    uint8_t        profileId;
    uint8_t        elementId;
    uint8_t        elementDataLength;
    /* element data follows */
} tCPElementResponseHdr;

/*******************************************************************************
*                         LOCAL FUNCTION PROTOTYPES
********************************************************************************/
static void LowLevel_CPSetElement(uint8_t CpId,
                                  uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength);

static void LowLevel_CPGetElement(uint8_t CpId,
                                  uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength,
                                  uint8_t dataReadAction);

/*******************************************************************************
  Function:
    void WF_CPCreate(uint8_t *p_CpId)

  Summary:
    Creates a Connection Profile on the MRF24W.

  Description:
    Requests the MRF24W to create a Connection Profile (CP), assign it an ID,
    and set all the elements to default values.  The ID returned by this function
    is used in other connection profile functions.  A maximum of 2 Connection
    Profiles can exist on the MRF24W.
    Users are encouraged to use 1 profile ID for MRF24W based on v5 stack SW.
    In v6 stack SW, plan is to have 1 profile ID for MRF24W but stack will be designed to
    have capability to handle multiple profiles.

  Precondition:
    MACInit must be called first.

  Parameters:
    p_CpId - Pointer to where Connection Profile ID will be written.  If
              function fails, the CP ID will be set to 0xff.

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPCreate(uint8_t *p_CpId)
{
    uint8_t  hdr[2];

    *p_CpId = 0xff;

    hdr[0] = WF_MGMT_REQUEST_TYPE;
    hdr[1] = WF_CP_CREATE_PROFILE_SUBTYPE;

    SendMgmtMsg(hdr,
                sizeof(hdr),
                NULL,            /* no data */
                0);              /* no data */

    /* wait for MRF24W management response, read data, free response after read */
    WaitForMgmtResponseAndReadData(WF_CP_CREATE_PROFILE_SUBTYPE,
                                   1,                             /* num data bytes to read          */
                                   MGMT_RESP_1ST_DATA_BYTE_INDEX, /* read starting at index 4        */
                                   p_CpId);                       /* write data here                 */
}

/*******************************************************************************
  Function:
    void WF_CPDelete(uint8_t CpId)

  Summary:
    Deletes a Connection Profile on the MRF24W.

  Description:
    Deletes the specified Connection Profile.  If the Connection Profile was in
    FLASH it will be erased from FLASH.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile to delete.

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPDelete(uint8_t CpId)
{
    uint8_t  hdr[2];

    hdr[0] = WF_MGMT_REQUEST_TYPE;
    hdr[1] = WF_CP_DELETE_PROFILE_SUBTYPE;

    SendMgmtMsg(hdr,
                sizeof(hdr),
                &CpId,           /* input data */
                1);              /* data size  */

    /* wait for mgmt response, free it after it comes in (no data needed) */
    WaitForMgmtResponse(WF_CP_DELETE_PROFILE_SUBTYPE, FREE_MGMT_BUFFER);
}

/*******************************************************************************
  Function:
    void WF_CPGetIds(uint8_t *p_cpIdList)

  Summary:
    Retrieves the CP ID bit mask.

  Description:
    Returns a list of all Connection Profile IDs that have been created on the
    MRF24W.  This is not to be confused with the Connection Algorithm's
    connectionProfileList.  This function returns a bit mask corresponding to a
    list of all Connection Profiles that have been created (whether they are in
    the connectionProfileList or not).  Any Connection Profiles that have been
    saved to FLASH will be included.
    Users are encouraged to use 1 profile ID for MRF24W based on v5 stack SW.
    In v6 stack SW, plan is to have 1 profile ID for MRF24W but stack will be designed to
    have capability to handle multiple profiles.

  Precondition:
    MACInit must be called first.

  Parameters:
    p_cpIdList - Pointer to value representing the bit mask where each bit
                  index (plus 1) corresponds to a Connection Profile ID that has
                  been created.  For example, if this value is 0x03, then
                  Connection Profile ID�s 1 and and 2 have been created.

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPGetIds(uint8_t *p_cpIdList)
{
    uint8_t  hdr[2];

    hdr[0] = WF_MGMT_REQUEST_TYPE;
    hdr[1] = WF_CP_GET_ID_LIST_SUBTYPE;

    SendMgmtMsg(hdr,
                sizeof(hdr),
                NULL,
                0);

    /* wait for mgmt response, read data, free after read */
    WaitForMgmtResponseAndReadData(WF_CP_GET_ID_LIST_SUBTYPE,
                                   1,                                /* num data bytes to read          */
                                   MGMT_RESP_1ST_DATA_BYTE_INDEX,    /* only used if num data bytes > 0 */
                                   p_cpIdList);                      /* only used if num data bytes > 0 */
}

#if defined(WF_USE_GROUP_SET_GETS)
#if !defined(MRF24WG)
extern bool gRFModuleVer1209orLater;
#endif
/*******************************************************************************
  Function:
    void WF_CPSetElements(uint8_t CpId, tWFCPElements *p_elements)

  Summary:
    Writes out data for a specific connection profile element.

  Description:
    Sets all Connection Profile elements.  If the Host CPU does not have enough
    memory to create a structure of this size then call the individual set
    functions.

  Precondition:
    MACInit must be called.

  Parameters:
    CpId - Connectino Profile ID.
    p_elements - Pointer to Connection Profile elements structure.

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPSetElements(uint8_t CpId, tWFCPElements *p_elements)
{
    WF_ASSERT(p_elements->ssidLength <= WF_MAX_SSID_LENGTH);

#if !defined(MRF24WG)
    LowLevel_CPSetElement(CpId,                   /* CP ID                        */
                          WF_CP_ELEMENT_ALL,      /* Element ID                   */
                          (uint8_t *)p_elements,    /* pointer to element data      */
                          gRFModuleVer1209orLater ? sizeof(tWFCPElements)
                                                  : sizeof(tWFCPElements)-2);
                                                /* number of element data bytes */
#else
    LowLevel_CPSetElement(CpId,                   /* CP ID                        */
                          WF_CP_ELEMENT_ALL,      /* Element ID                   */
                          (uint8_t *)p_elements,    /* pointer to element data      */
                          sizeof(tWFCPElements)); /* number of element data bytes */
#endif
}

/*******************************************************************************
  Function:
    void WF_CPGetElements(uint8_t CpId, tWFCPElements *p_elements)

  Summary:
    Reads the Connection Profile elements for the specified ID.

  Description:
    Gets all Connection Profile elements for the specified CP ID.  If the Host
    CPU does not have enough memory to create a structure of this size then call
    the individual get functions.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connectino Profile ID.
    p_elements - Pointer to Connection Profile elements structure.

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPGetElements(uint8_t CpId, tWFCPElements *p_elements)
{
    LowLevel_CPGetElement(CpId,                   /* CP ID                        */
                          WF_CP_ELEMENT_ALL,      /* Element ID                   */
                         (uint8_t *)p_elements,     /* pointer to element data      */
                          sizeof(tWFCPElements),  /* number of element data bytes */
                          true);                  /* read data and free buffer    */
}
#endif /* WF_USE_GROUP_SET_GETS */

#if defined(WF_USE_INDIVIDUAL_SET_GETS)
/*******************************************************************************
  Function:
    void WF_CPSetSsidType(uint8_t CpId, uint8_t hidden)

  Summary:
    Sets the SSID type for the specified Connection Profile ID.

  Description:
    Sets the SSID type element in the Connection Profile.
    This is valid only when we create AdHoc network.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId -- Connection Profile ID
    hidden -- True or False

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPSetSsidType(uint8_t CpId, uint8_t hidden)
{
    LowLevel_CPSetElement(CpId,                   /* CP ID                        */
                          WF_CP_ELEMENT_SSID_TYPE,     /* Element ID                   */
                         (uint8_t *)&hidden,         /* pointer to element data      */
                          1);            /* number of element data bytes */

}

/*******************************************************************************
  Function:
    void WF_CPGetSsidType(uint8_t CpId, uint8_t *hidden)

  Summary:
    Gets the SSID type for the specified Connection Profile ID.

  Description:
    Gets the SSID type element in the Connection Profile.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    hidden - Pointer to the SSID type

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPGetSsidType(uint8_t CpId, uint8_t *hidden)
{
     LowLevel_CPGetElement(CpId,                       /* conn. profile ID       */
                          WF_CP_ELEMENT_SSID_TYPE, /* element ID             */
                          hidden,              /* element data pointer   */
                          1,                          /* read one byte          */
                          true);                      /* read data, free buffer */
}

/*******************************************************************************
  Function:
    void WF_CPSetSsid(uint8_t CpId, uint8_t *p_ssid, uint8_t *p_ssidLength)

  Summary:
    Sets the SSID for the specified Connection Profile ID.

  Description:
    Sets the SSID and SSID Length elements in the Connection Profile.  Note that
    an Access Point can have either a visible or hidden SSID.  If an Access Point
    uses a hidden SSID then an active scan must be used (see scanType field in the
    Connection Algorithm).

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    p_ssid - Pointer to the SSID string
    ssidLength - Number of bytes in the SSID

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPSetSsid(uint8_t CpId, uint8_t *p_ssid,  uint8_t ssidLength)
{
    WF_ASSERT(ssidLength <= WF_MAX_SSID_LENGTH);
    LowLevel_CPSetElement(CpId,                   /* CP ID                        */
                          WF_CP_ELEMENT_SSID,     /* Element ID                   */
                         (uint8_t *)p_ssid,         /* pointer to element data      */
                          ssidLength);            /* number of element data bytes */

}

/*******************************************************************************
  Function:
    void WF_CPGetSsid(uint8_t CpId, uint8_t *p_ssid, uint8_t *p_ssidLength)

  Summary:
    Gets the SSID for the specified Connection Profile ID.

  Description:
    Gets the SSID and SSID Length elements in the Connection Profile.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    p_ssid - Pointer to the SSID string
    ssidLength - Pumber of bytes in the SSID

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPGetSsid(uint8_t CpId, uint8_t *p_ssid, uint8_t *p_ssidLength)
{
    tCPElementResponseHdr  mgmtHdr;

    /* Request SSID, but don't have this function read data or free response buffer.       */
    LowLevel_CPGetElement(CpId,                   /* Connection Profile ID                 */
                          WF_CP_ELEMENT_SSID,     /* Element ID                            */
                          NULL,                   /* ptr to element data (not used here    */
                          0,                      /* num data bytes to read (not used here */
                          false);                 /* no read, leave response mounted       */

    /* At this point, management response is mounted and ready to be read.                 */
    /* Set raw index to 0, read normal 4 byte header plus the next 3 bytes, these will be: */
    /*   profile id             [4]                                                        */
    /*   element id             [5]                                                        */
    /*   element data length    [6]                                                        */
    RawRead(RAW_MGMT_RX_ID, 0, sizeof(tCPElementResponseHdr), (uint8_t *)&mgmtHdr);

    /* extract SSID length and write to caller */
    *p_ssidLength = mgmtHdr.elementDataLength;

    /* copy SSID name to callers buffer */
    RawRead(RAW_MGMT_RX_ID, sizeof(tCPElementResponseHdr), *p_ssidLength, p_ssid);

    /* free management buffer */
    DeallocateMgmtRxBuffer();
}

/*******************************************************************************
  Function:
    void WF_CPSetBssid(uint8_t CpId, uint8_t *p_bssid)

  Summary:
    Sets the BSSID for the specified Connection Profile ID.

  Description:
    Sets the BSSID element in a Connection Profile.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    p_bssid - Pointer to the BSSID

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPSetBssid(uint8_t CpId, uint8_t *p_bssid)
{
    LowLevel_CPSetElement(CpId,                  /* CP ID                        */
                          WF_CP_ELEMENT_BSSID,   /* Element ID                   */
                          p_bssid,               /* pointer to element data      */
                          WF_BSSID_LENGTH);      /* number of element data bytes */
}

/*******************************************************************************
  Function:
    void WF_CPGetBssid(uint8_t CpId, uint8_t *p_bssid)

  Summary:
    Gets the BSSID for the specified Connection Profile ID.

  Description:
    Gets the BSSID element in a Connection Profile.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    p_bssid - Pointer to the BSSID

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPGetBssid(uint8_t CpId, uint8_t *p_bssid)
{
    LowLevel_CPGetElement(CpId,                  /* CP ID                             */
                          WF_CP_ELEMENT_BSSID,   /* Element ID                        */
                          p_bssid,               /* pointer to element data           */
                          WF_BSSID_LENGTH,       /* number of element data bytes      */
                          true);                 /* read data, free buffer after read */
}

/*******************************************************************************
  Function:
    void WF_CPSetNetworkType(uint8_t CpId, uint8_t networkType)

  Summary:
    Sets the network type for the specified Connection Profile ID.

  Description:
    Sets the Network Type element for the Connection Profile.  Allowable values are:
    * WF_INFRASTRUCTURE
    * WF_ADHOC
    * WF_P2P
    * WF_SOFT_AP

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    networkType - Type of network to create (infrastructure or adhoc or p2p or softAP)

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPSetNetworkType(uint8_t CpId, uint8_t networkType)
{
    LowLevel_CPSetElement(CpId,                         /* CP ID                        */
                          WF_CP_ELEMENT_NETWORK_TYPE,   /* Element ID                   */
                          &networkType,                 /* pointer to element data      */
                          1);                           /* number of element data bytes */
}

/*******************************************************************************
  Function:
    void WF_CPGetNetworkType(uint8_t CpId, uint8_t networkType)

  Summary:
    Gets the network type for the specified Connection Profile ID.

  Description:
    Gets the Network Type element of the Connection Profile.  Allowable values are:
    * WF_INFRASTRUCTURE
    * WF_ADHOC
    * WF_P2P
    * WF_SOFT_AP

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    networkType - Type of network to create (infrastructure or adhoc or p2p or softAP)

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPGetNetworkType(uint8_t CpId, uint8_t *p_networkType)
{
    LowLevel_CPGetElement(CpId,                       /* conn. profile ID       */
                          WF_CP_ELEMENT_NETWORK_TYPE, /* element ID             */
                          p_networkType,              /* element data pointer   */
                          1,                          /* read one byte          */
                          true);                      /* read data, free buffer */
}

/*******************************************************************************
  Function:
    void WF_CPSetWepKeyType(uint8_t CpId, uint8_t wepKeyType)

  Summary:
    Sets the WEP key type for the specified Connection Profile ID.

  Description:
     Sets the WEP key type (WF_SECURITY_WEP_SHAREDKEY or WF_SECURITY_WEP_OPENKEY)
     for the specified Connection Profile ID. Default WEP key type is
     WF_SECURITY_WEP_OPENKEY.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId       -- Connection Profile ID
    wepKeyType -- WF_SECURITY_WEP_SHAREDKEY or WF_SECURITY_WEP_OPENKEY (default)

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPSetWepKeyType(uint8_t CpId, uint8_t wepKeyType)
{
    LowLevel_CPSetElement(CpId,                        /* CP ID                        */
                          WF_CP_ELEMENT_WEPKEY_TYPE,   /* Element ID                   */
                          &wepKeyType,                 /* pointer to element data      */
                          1);                          /* number of element data bytes */
}

/*******************************************************************************
  Function:
    void WF_CPGetWepKeyType(uint8_t CpId, uint8_t *p_keyType)

  Summary:
    Gets the WEP key type for the specified Connection Profile ID.

  Description:
    Gets the WEP key type element for the specified Connection Profile.  Allowable values are:
    * WF_SECURITY_WEP_SHAREDKEY (0)
    * WF_SECURITY_WEP_OPENKEY (1) - Default

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId -- Connection Profile ID
    networkType -- type of key for Wep security (WF_SECURITY_WEP_SHAREDKEY or WF_SECURITY_WEP_OPENKEY)

  Returns:
    None.

  Remarks:
    Before MRF24W is connected to the AP/Router, calling this function will always return
    WF_SECURITY_WEP_OPENKEY (1) as the default.
  *****************************************************************************/
void WF_CPGetWepKeyType(uint8_t CpId, uint8_t *p_wepKeyType)
{
    LowLevel_CPGetElement(CpId,                       /* conn. profile ID       */
                          WF_CP_ELEMENT_WEPKEY_TYPE, /* element ID             */
                          p_wepKeyType,              /* element data pointer   */
                          1,                          /* read one byte          */
                          true);                      /* read data, free buffer */
}

#if defined(MRF24WG)
/*******************************************************************************
  Function:
    void WF_CPGetWPSCredentials(uint8_t CpId, tWFWpsCred *p_cred)

  Summary:
    Gets the WPS credentials for the specified Connection Profile ID for MRF24WG0MA/B.

  Description:
    Applicable for MRF24WG0M only. Gets the WPS credentials after WPS completed for MRF24WG0MA/B.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    p_cred - Pointer to the credentials

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPGetWPSCredentials(uint8_t CpId, tWFWpsCred *p_cred)
{
    LowLevel_CPGetElement(CpId,                  /* CP ID                             */
                          WF_CP_ELEMENT_READ_WPS_CRED,   /* Element ID                        */
                          (uint8_t *)p_cred,               /* pointer to element data           */
                          sizeof(*p_cred),       /* number of element data bytes      */
                          true);                 /* read data, free buffer after read */
}
#endif /* MRF24WG */

/*******************************************************************************
  Function:
    void WF_CPSetSecurity(uint8_t CpId,
                          uint8_t securityType,
                          uint8_t wepKeyIndex,
                          uint8_t *p_securityKey,
                          uint8_t securityKeyLength)

  Summary:
    Sets the security for the specified Connection Profile.

  Description:
    Configures security for a Connection Profile.

    <table>
    Security                               Key      Length
    --------                               ---      ------
    WF_SECURITY_OPEN                       N/A      N/A
    WF_SECURITY_WEP_40                     hex      4, 5 byte keys
    WF_SECURITY_WEP_104                    hex      4, 13 byte keys
    WF_SECURITY_WPA_AUTO_WITH_KEY          hex      32 bytes
    WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE  ascii    8-63 ascii characters
    WF_SECURITY_WPS_PUSH_BUTTON
    WF_SECURITY_WPS_PIN
    </table>

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    securityType - Value corresponding to the security type desired.
    wepKeyIndex - Only index 0 is valid. (Applicable for WF_SECURITY_WEP_40 or
                   WF_SECURITY_WEP_104)
    p_securityKey - Binary key or passphrase (not used if security is
                     WF_SECURITY_OPEN)
    securityKeyLength - Number of bytes in p_securityKey (not used if security
                         is WF_SECURITY_OPEN)

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WF_CPSetSecurity(uint8_t CpId,
                      uint8_t securityType,
                      uint8_t wepKeyIndex,
                      uint8_t *p_securityKey,
                      uint8_t securityKeyLength)
{
    uint8_t  hdrBuf[7];
    uint8_t  *p_key;

    /* Write out header portion of msg */
    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;           /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CP_SET_ELEMENT_SUBTYPE;      /* mgmt request subtype            */
    hdrBuf[2] = CpId;                           /* Connection Profile ID           */
    hdrBuf[3] = WF_CP_ELEMENT_SECURITY;         /* Element ID                      */

    /* Next to header bytes are really part of data, but need to put them in header      */
    /* bytes in order to prepend to security key                                         */
    hdrBuf[5] = securityType;
    hdrBuf[6] = wepKeyIndex;

    /* if security is open (no key) or WPS push button method */
    if (securityType == WF_SECURITY_OPEN
        || securityType == WF_SECURITY_WPS_PUSH_BUTTON)
    {
        hdrBuf[4]         = 2; /* Only data is security type and wep index */
        p_key             = NULL;
        securityKeyLength = 0;

    }
    /* else security is selected, so need to send key */
    else
    {
        hdrBuf[4] = 2 + securityKeyLength; /* data is security type + wep index + key */
        p_key     = p_securityKey;
    }

    SendMgmtMsg(hdrBuf,              /* msg header which includes the security type and WEP index)    */
                sizeof(hdrBuf),      /* msg header length                                             */
                p_key,               /* msg data (security key), can be NULL                          */
                securityKeyLength);  /* msg data length (will be 0 if p_securityKey is NULL)          */

    /* wait for mgmt response, free after it comes in, don't need data bytes */
    WaitForMgmtResponse(WF_CP_SET_ELEMENT_SUBTYPE, FREE_MGMT_BUFFER);
}

/*******************************************************************************
  Function:
    void WF_CPGetSecurity(uint8_t CpId,
                          uint8_t securityType,
                          uint8_t wepKeyIndex,
                          uint8_t *p_securityKey,
                          uint8_t securityKeyLength)

  Summary:
    Gets the security for the specified Connection Profile.

  Description:
    Configures security for a Connection Profile.

    <table>
    Security                               Key      Length
    --------                               ---      ------
    WF_SECURITY_OPEN                       N/A      N/A
    WF_SECURITY_WEP_40                     hex      4, 5 byte keys
    WF_SECURITY_WEP_104                    hex      4, 13 byte keys
    WF_SECURITY_WPA_AUTO_WITH_KEY          hex      32 bytes
    WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE  ascii    8-63 ascii characters
    WF_SECURITY_WPS_PUSH_BUTTON
    WF_SECURITY_WPS_PIN
    </table>

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    securityType - Value corresponding to the security type desired.
    wepKeyIndex -  Only index 0 is valid. (Applicable for WF_SECURITY_WEP_40 or
                   WF_SECURITY_WEP_104)
    p_securityKey - Binary key or passphrase (not used if security is
                     WF_SECURITY_OPEN)
    securityKeyLength - Number of bytes in p_securityKey (not used if security
                         is WF_SECURITY_OPEN)

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WF_CPGetSecurity(uint8_t CpId,
                      uint8_t *p_securityType,
                      uint8_t *p_wepKeyIndex,
                      uint8_t *p_securityKey,
                      uint8_t *p_securityKeyLength)
{
    tCPElementResponseHdr mgmtHdr;
    uint8_t keyLength;

    /* send request, wait for mgmt response, do not read and do not free up response buffer */
    LowLevel_CPGetElement(CpId,
                          WF_CP_ELEMENT_SECURITY, /* Element ID      */
                          NULL,                   /* do not read     */
                          0,                      /* do not read     */
                          false);                 /* do not read, do not free mgmt buffer */

    /* at this point, management response is mounted and ready to be read */

    /* At this point, management response is mounted and ready to be read.                 */
    /* Set raw index to 0, read normal 4 byte header plus the next 3 bytes, these will be: */
    /*   profile id             [4]                                                        */
    /*   element id             [5]                                                        */
    /*   element data length    [6]                                                        */
    RawRead(RAW_MGMT_RX_ID, 0, sizeof(tCPElementResponseHdr), (uint8_t *)&mgmtHdr);

    RawRead(RAW_MGMT_RX_ID,                     /* raw Id                     */
            sizeof(tCPElementResponseHdr) + 0,  /* index of security type [7] */
            1,                                  /* read one byte              */
            p_securityType);                    /* copy that byte here        */

    RawRead(RAW_MGMT_RX_ID,                     /* raw Id                     */
            sizeof(tCPElementResponseHdr) + 1 , /* index of WEP key index [8] */
            1,                                  /* read one byte              */
            p_wepKeyIndex);                     /* copy that byte here        */

    /* determine security key length and read if it is present */
    keyLength = mgmtHdr.elementDataLength - 2;
    if (keyLength > 0)
    {
        *p_securityKeyLength = keyLength;

        RawRead(RAW_MGMT_RX_ID,                     /* raw Id                  */
                sizeof(tCPElementResponseHdr) + 2,  /* index of first key byte */
                keyLength,                          /* number of bytes to read */
                p_securityKey);                     /* copy bytes here         */

    }
    /* no security key, so set key length param to 0 */
    else
    {
        *p_securityKeyLength = 0;
    }

    /* free management buffer */
    DeallocateMgmtRxBuffer();
}

/*******************************************************************************
  Function:
    void WF_CPSetDefaultWepKeyIndex(uint8_t CpId, uint8_t defaultWepKeyIndex)

  Summary:
    Set up the WEP key index. Only WEP key index 0 is used in RF module FW.

  Description:
    Only applicable if the Connection Profile security type is either
    WF_SECURITY_WEP_40 or WF_SECURITY_WEP_104. Since only WEP
    key index 0 is supported, this function is no longer needed.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    defaultWepKeyIndex - Use WEP key index 0. No longer supporting 4 WEP key indexes (0 - 3).

  Returns:
    None.

  Remarks:
      Note that only WEP  key index 0 amongst AP manufacturers is typically used.  Using any
    of the other three keys may be unpredictable from brand to brand.
  *****************************************************************************/
void WF_CPSetDefaultWepKeyIndex(uint8_t CpId, uint8_t defaultWepKeyIndex)
{
    LowLevel_CPSetElement(CpId,                          /* CP ID                        */
                          WF_CP_ELEMENT_WEP_KEY_INDEX,   /* Element ID                   */
                          &defaultWepKeyIndex,            /* pointer to element data      */
                          1);                            /* number of element data bytes */
}

/*******************************************************************************
  Function:
    void WF_CPGetDefaultWepKeyIndex(uint8_t CpId, uint8_t *p_defaultWepKeyIndex)

  Summary:
    Returns the value of the active WEP keys to use. Only WEP key index 0 is used in RF module FW.

  Description:
    Only applicable if the Connection Profile security type is either
    WF_SECURITY_WEP_40 or WF_SECURITY_WEP_104. Since only WEP
    key index 0 is supported, this function is no longer needed.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    p_defaultWepKeyIndex - Pointer to index of WEP key to use.
                                         No longer supporting 4 WEP key indexes (0 - 3).

  Returns:
    None.

  Remarks:
    Note that only WEP  key index 0 amongst AP manufacturers is typically used.  Using any
    of the other three keys may be unpredictable from brand to brand.
  *****************************************************************************/
void WF_CPGetDefaultWepKeyIndex(uint8_t CpId, uint8_t *p_defaultWepKeyIndex)
{
    LowLevel_CPGetElement(CpId,                        /* conn. profile ID       */
                          WF_CP_ELEMENT_WEP_KEY_INDEX, /* element ID             */
                          p_defaultWepKeyIndex,        /* element data pointer   */
                          1,                           /* read one byte          */
                          true);                       /* read data, free buffer */
}

/*******************************************************************************
  Function:
    void WF_CPSetAdHocBehavior(uint8_t CpId, uint8_t adHocBehavior)

  Summary:
    Selects the desired Ad Hoc behavior

  Description:
    Sets the AdHoc behavior within a Connection Profile.  Allowable values are:
    * WF_ADHOC_CONNECT_THEN_START
    * WF_ADHOC_CONNECT_ONLY
    * WF_ADHOC_START_ONLY

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    adHocBehavior - Value of the adhoc behavior for this connection profile.

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPSetAdHocBehavior(uint8_t CpId, uint8_t adHocBehavior)
{
    LowLevel_CPSetElement(CpId,                          /* CP ID                        */
                          WF_CP_ELEMENT_ADHOC_BEHAVIOR,  /* Element ID                   */
                          &adHocBehavior,                /* pointer to element data      */
                          1);                            /* number of element data bytes */
}

/*******************************************************************************
  Function:
    void WF_CPGetAdHocBehavior(uint8_t CpId, uint8_t *p_adHocBehavior)

  Summary:
    Gets the desired Ad Hoc behavior

  Description:
    Gets the AdHoc behavior within a Connection Profile.  Allowable values are:
    * WF_ADHOC_CONNECT_THEN_START
    * WF_ADHOC_CONNECT_ONLY
    * WF_ADHOC_START_ONLY

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    adHocBehavior - Pointer to location of the adhoc behavior value for this
                     connection profile.

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_CPGetAdHocBehavior(uint8_t CpId, uint8_t *p_adHocBehavior)
{
    LowLevel_CPGetElement(CpId,                         /* conn. profile ID       */
                          WF_CP_ELEMENT_ADHOC_BEHAVIOR, /* element ID             */
                          p_adHocBehavior,              /* element data pointer   */
                          1,                            /* read one byte          */
                          true);                        /* read data, free buffer */
}
#endif /* WF_USE_INDIVIDUAL_SET_GETS */

/*******************************************************************************
  Function:
    static void LowLevel_CPSetElement(uint8_t CpId,
                                  uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength)

  Summary:
    Set an element of the connection profile on the MRF24W.

  Description:
    All Connection Profile 'Set Element' functions call this function to
    construct the management message.  The caller must fix up any endian issues
    prior to calling this function.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    elementId - Element that is being set
    p_elementData - Pointer to element data
    elementDataLength - Number of bytes pointed to by p_elementData

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
static void LowLevel_CPSetElement(uint8_t CpId,
                                  uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength)
{
    uint8_t  hdrBuf[5];

    /* Write out header portion of msg */
    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;       /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CP_SET_ELEMENT_SUBTYPE;  /* mgmt request subtype            */
    hdrBuf[2] = CpId;                       /* Connection Profile ID           */
    hdrBuf[3] = elementId;                  /* Element ID                      */
    hdrBuf[4] = elementDataLength;          /* number of bytes of element data */

    SendMgmtMsg(hdrBuf,              /* msg header        */
                sizeof(hdrBuf),      /* msg header length */
                p_elementData,       /* msg data          */
                elementDataLength);  /* msg data length   */

    /* wait for mgmt response, free after it comes in, don't need data bytes */
    WaitForMgmtResponse(WF_CP_SET_ELEMENT_SUBTYPE, FREE_MGMT_BUFFER);

}

/*******************************************************************************
  Function:
    static void LowLevel_CPGetElement(uint8_t CpId,
                                      uint8_t elementId,
                                      uint8_t *p_elementData,
                                      uint8_t elementDataLength,
                                      uint8_t dataReadAction)

  Summary:
    Get an element of the connection profile on the MRF24W.

  Description:
    All Connection Profile 'Get Element' functions call this function to
    construct the management message.  The caller must fix up any endian issues
    prior to calling this function.

  Precondition:
    MACInit must be called first.

  Parameters:
    CpId - Connection Profile ID
    elementId - Element that is being read
    p_elementData - Pointer to where element data will be written
    elementDataLength - Number of element data bytes that will be read
    dataReadAction - If true then read data per paramters and free mgmt
                      response buffer. If false then return after response
                      received, do not read any data as the caller will do that,
                      and don't free buffer, as caller will do that as well.

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
static void LowLevel_CPGetElement(uint8_t CpId,
                                    uint8_t elementId,
                                    uint8_t *p_elementData,
                                    uint8_t elementDataLength,
                                    uint8_t dataReadAction)    /* true or false */
{
    uint8_t  hdrBuf[4];

    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;          /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CP_GET_ELEMENT_SUBTYPE;  /* mgmt request subtype            */
    hdrBuf[2] = CpId;                       /* Connection Profile ID           */
    hdrBuf[3] = elementId;                  /* Element ID                      */

    SendMgmtMsg(hdrBuf,              /* msg header        */
                sizeof(hdrBuf),      /* msg header length */
                NULL,                /* msg data          */
                0);                  /* msg data length   */

    if (dataReadAction == (uint8_t)true)
    {
        /* wait for mgmt response, read desired data, and then free response buffer */
        WaitForMgmtResponseAndReadData(WF_CP_GET_ELEMENT_SUBTYPE,
                                       elementDataLength,                   /* num data bytes to read                */
                                       sizeof(tCPElementResponseHdr),       /* index of first byte of element data   */
                                       p_elementData);                      /* where to write element data           */
    }
    else
    {
        /* wait for mgmt response, don't read any data bytes, do not release mgmt buffer */
        WaitForMgmtResponse(WF_CP_GET_ELEMENT_SUBTYPE, DO_NOT_FREE_MGMT_BUFFER);
    }
}

#endif /* WF_CS_TRIS */
