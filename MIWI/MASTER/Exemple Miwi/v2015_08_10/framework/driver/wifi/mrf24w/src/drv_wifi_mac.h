/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    drv_wifi_mac.h

  Summary:
    Module for Microchip TCP/IP Stack
    -Provides access to MRF24W WiFi controller
    -Reference: MRF24W Data sheet, IEEE 802.11 Standard

  Description:
     MRF24W Driver

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

#ifndef __DRV_WF_MAC_H_
#define __DRV_WF_MAC_H_

/*******************************************************************************
 *                            INCLUDE FILES
 *******************************************************************************/

#if defined(WF_CS_TRIS)
    #include "driver/wifi/mrf24w/drv_wifi_api.h"
    #include "driver/wifi/mrf24w/src/drv_wifi_mgmt_msg.h"
    #include "driver/wifi/mrf24w/src/drv_wifi_debug_strings.h"

    #if defined(MRF24WG)
        #include "driver/wifi/mrf24w/src/drv_wifi_priv_24g.h"
        #include "driver/wifi/mrf24w/src/drv_wifi_raw_24g.h"
    #else /* MRF24WB */
        #include "driver/wifi/mrf24w/src/drv_wifi_priv.h"
        #include "driver/wifi/mrf24w/src/drv_wifi_raw.h"
    #endif

#endif /* WF_CS_TRIS */

#endif /* __DRV_WF_MAC_H_ */
