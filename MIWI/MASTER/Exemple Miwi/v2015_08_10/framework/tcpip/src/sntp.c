/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    sntp.c

  Summary:
    Module for Microchip TCP/IP Stack
     -Locates an NTP Server from public site using DNS
     -Requests UTC time using SNTP and updates SNTPTime structure
      periodically, according to NTP_QUERY_INTERVAL value
    - Reference: RFC 1305

  Description:
    Simple Network Time Protocol (SNTP) Client Version 3

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

#define __SNTP_C_

#include "tcpip/tcpip.h"

#if defined(STACK_USE_SNTP_CLIENT)

// Defines how frequently to resynchronize the date/time (default: 10 minutes)
#define NTP_QUERY_INTERVAL (10ull*60ull * TICK_SECOND)

// Defines how long to wait to retry an update after a failure.
// Updates may take up to 6 seconds to fail, so this 14 second delay is actually only an 8-second retry.
#define NTP_FAST_QUERY_INTERVAL (14ull * TICK_SECOND)

// Port for contacting NTP servers
#define NTP_SERVER_PORT (123ul)

// Reference Epoch to use.  (default: 01-Jan-1970 00:00:00)
#define NTP_EPOCH (86400ul * (365ul * 70ul + 17ul))

// Defines how long to wait before assuming the query has failed
#define NTP_REPLY_TIMEOUT (6ul*TICK_SECOND)

// These are normally available network time servers.
// The actual IP returned from the pool will vary every
// minute so as to spread the load around stratum 1 timeservers.
// For best accuracy and network overhead you should locate the
// pool server closest to your geography, but it will still work
// if you use the global pool.ntp.org address or choose the wrong
// one or ship your embedded device to another geography.
#define NTP_SERVER  "pool.ntp.org"
//#define NTP_SERVER  "europe.pool.ntp.org"
//#define NTP_SERVER  "asia.pool.ntp.org"
//#define NTP_SERVER  "oceania.pool.ntp.org"
//#define NTP_SERVER  "north-america.pool.ntp.org"
//#define NTP_SERVER  "south-america.pool.ntp.org"
//#define NTP_SERVER  "africa.pool.ntp.org"

// Defines the structure of an NTP packet
typedef struct {

    struct {
        uint8_t mode : 3; // NTP mode
        uint8_t versionNumber : 3; // SNTP version number
        uint8_t leapIndicator : 2; // Leap second indicator
    } flags; // Flags for the packet

    uint8_t stratum; // Stratum level of local clock
    int8_t poll; // Poll interval
    int8_t precision; // Precision (seconds to nearest power of 2)
    uint32_t root_delay; // Root delay between local machine and server
    uint32_t root_dispersion; // Root dispersion (maximum error)
    uint32_t ref_identifier; // Reference clock identifier
    uint32_t ref_ts_secs; // Reference timestamp (in seconds)
    uint32_t ref_ts_fraq; // Reference timestamp (fractions)
    uint32_t orig_ts_secs; // Origination timestamp (in seconds)
    uint32_t orig_ts_fraq; // Origination timestamp (fractions)
    uint32_t recv_ts_secs; // Time at which request arrived at sender (seconds)
    uint32_t recv_ts_fraq; // Time at which request arrived at sender (fractions)
    uint32_t tx_ts_secs; // Time at which request left sender (seconds)
    uint32_t tx_ts_fraq; // Time at which request left sender (fractions)
} NTP_PACKET;

// Seconds value obtained by last update
static uint32_t dwSNTPSeconds = 0;

// Tick count of last update
static uint32_t dwLastUpdateTick = 0;

static enum {
    SM_HOME = 0,
    SM_UDP_IS_OPENED,
    //SM_NAME_RESOLVE,
    //SM_ARP_START_RESOLVE,
    //SM_ARP_RESOLVE,
    //SM_ARP_START_RESOLVE2,
    //SM_ARP_RESOLVE2,
    //SM_ARP_START_RESOLVE3,
    //SM_ARP_RESOLVE3,
    //SM_ARP_RESOLVE_FAIL,
    SM_UDP_SEND,
    SM_UDP_RECV,
    SM_SHORT_WAIT,
    SM_WAIT,
} SNTPState = SM_HOME;

/*****************************************************************************
  Function:
    void SNTPInit(void)

  Summary:
    SNTP module initialization function.

  Description:
    Initialize the SNTP module state machine.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
 ***************************************************************************/
void SNTPInit(void)
{
    SNTPState = SM_HOME;
}

/*****************************************************************************
  Function:
    void SNTPClient(void)

  Summary:
    Periodically checks the current time from a pool of servers.

  Description:
    This function periodically checks a pool of time servers to obtain the
    current date/time.

  Precondition:
    UDP is initialized.

  Parameters:
    None

  Returns:
    None

  Remarks:
    This function requires once available UDP socket while processing, but
    frees that socket when the SNTP module is idle.
 ***************************************************************************/
void SNTPClient(void)
{
    NTP_PACKET pkt;
    uint16_t w;
    //static NODE_INFO Server;
    static uint32_t dwTimer;
    static UDP_SOCKET MySocket = INVALID_UDP_SOCKET;

    switch (SNTPState) {
    case SM_HOME:
        if (MySocket == INVALID_UDP_SOCKET) {
            MySocket = UDPOpenEx((uint32_t) (PTR_BASE) NTP_SERVER, UDP_OPEN_ROM_HOST, 0, NTP_SERVER_PORT);
            SNTPState++;
        } else {
            UDPClose(MySocket);
            MySocket = INVALID_UDP_SOCKET;
        }
        break;

    case SM_UDP_IS_OPENED:
        if (UDPIsOpened(MySocket) == true) {
            SNTPState = SM_UDP_SEND;
        }
        /*  else
            {
                UDPClose(MySocket);
                SNTPState = SM_HOME;
                MySocket = INVALID_UDP_SOCKET;
            }
         */
        break;

#if 0
        // Obtain ownership of the DNS resolution module
        if (!DNSBeginUsage())
            break;

        // Obtain the IP address associated with the server name
        DNSResolveROM((ROM uint8_t *) NTP_SERVER, DNS_TYPE_A);
        dwTimer = TickGet();
        SNTPState = SM_NAME_RESOLVE;
        break;

    case SM_NAME_RESOLVE:
        // Wait for DNS resolution to complete
        if (!DNSIsResolved(&Server.IPAddr)) {
            if ((TickGet() - dwTimer) > (5 * TICK_SECOND)) {
                DNSEndUsage();
                dwTimer = TickGetDiv64K();
                SNTPState = SM_SHORT_WAIT;
            }
            break;
        }

        // Obtain DNS resolution result
        if (!DNSEndUsage()) {
            // No valid IP address was returned from the DNS
            // server.  Quit and fail for a while if host is not valid.
            dwTimer = TickGetDiv64K();
            SNTPState = SM_SHORT_WAIT;
            break;
        }
        SNTPState = SM_ARP_START_RESOLVE;
        // No need to break

    case SM_ARP_START_RESOLVE:
    case SM_ARP_START_RESOLVE2:
    case SM_ARP_START_RESOLVE3:
        // Obtain the MAC address associated with the server's IP address
        ARPResolve(&Server.IPAddr);
        dwTimer = TickGet();
        SNTPState++;
        break;

    case SM_ARP_RESOLVE:
    case SM_ARP_RESOLVE2:
    case SM_ARP_RESOLVE3:
        // Wait for the MAC address to finish being obtained
        if (!ARPIsResolved(&Server.IPAddr, &Server.MACAddr)) {
            // Time out if too much time is spent in this state
            if (TickGet() - dwTimer > 1 * TICK_SECOND) {
                // Retransmit ARP request by going to next SM_ARP_START_RESOLVE state or fail by going to SM_ARP_RESOLVE_FAIL state.
                SNTPState++;
            }
            break;
        }
        SNTPState = SM_UDP_SEND;
        break;

    case SM_ARP_RESOLVE_FAIL:
        // ARP failed after 3 tries, abort and wait for next time query
        dwTimer = TickGetDiv64K();
        SNTPState = SM_SHORT_WAIT;
        break;
#endif
        // case SM_UDP_IS_OPENED:
    case SM_UDP_SEND:
        // Open up the sending UDP socket
        //MySocket = UDPOpen(0, &Server, NTP_SERVER_PORT);
#if 0

        MySocket = UDPOpenEx(NTP_SERVER, UDP_OPEN_ROM_HOST, 0, NTP_SERVER_PORT);
        if (MySocket == INVALID_UDP_SOCKET)
            break;
#endif

        // Make certain the socket can be written to
        if (!UDPIsPutReady(MySocket)) {
            UDPClose(MySocket);
            SNTPState = SM_HOME;
            MySocket = INVALID_UDP_SOCKET;
            break;
        }

        // Transmit a time request packet
        memset(&pkt, 0, sizeof (pkt));
        pkt.flags.versionNumber = 3; // NTP Version 3
        pkt.flags.mode = 3; // NTP Client
        pkt.orig_ts_secs = swapl(NTP_EPOCH);
        UDPPutArray((uint8_t *) & pkt, sizeof (pkt));
        UDPFlush();

        dwTimer = TickGet();
        SNTPState = SM_UDP_RECV;
        break;

    case SM_UDP_RECV:
        // Look for a response time packet
        if (!UDPIsGetReady(MySocket)) {
            if ((TickGet()) - dwTimer > NTP_REPLY_TIMEOUT) {
                // Abort the request and wait until the next timeout period
                UDPClose(MySocket);
                //dwTimer = TickGetDiv64K();
                //SNTPState = SM_SHORT_WAIT;
                SNTPState = SM_HOME;
                MySocket = INVALID_UDP_SOCKET;
                break;
            }
            break;
        }

        // Get the response time packet
        w = UDPGetArray((uint8_t *) & pkt, sizeof (pkt));
        UDPClose(MySocket);
        dwTimer = TickGetDiv64K();
        SNTPState = SM_WAIT;
        MySocket = INVALID_UDP_SOCKET;

        // Validate packet size
        if (w != sizeof (pkt)) {
            break;
        }

        // Set out local time to match the returned time
        dwLastUpdateTick = TickGet();
        dwSNTPSeconds = swapl(pkt.tx_ts_secs) - NTP_EPOCH;
        // Do rounding.  If the partial seconds is > 0.5 then add 1 to the seconds count.
        if (((uint8_t *) & pkt.tx_ts_fraq)[0] & 0x80)
            dwSNTPSeconds++;
        break;

    case SM_SHORT_WAIT:
        // Attempt to requery the NTP server after a specified NTP_FAST_QUERY_INTERVAL time (ex: 8 seconds) has elapsed.
        if (TickGetDiv64K() - dwTimer > (NTP_FAST_QUERY_INTERVAL / 65536ull)) {
            SNTPState = SM_HOME;
            MySocket = INVALID_UDP_SOCKET;
        }
        break;

    case SM_WAIT:
        // Requery the NTP server after a specified NTP_QUERY_INTERVAL time (ex: 10 minutes) has elapsed.
        if (TickGetDiv64K() - dwTimer > (NTP_QUERY_INTERVAL / 65536ull)) {
            SNTPState = SM_HOME;
            MySocket = INVALID_UDP_SOCKET;
        }

        break;
    }
}

/*****************************************************************************
  Function:
    uint32_t SNTPGetUTCSeconds(void)

  Summary:
    Obtains the current time from the SNTP module.

  Description:
    This function obtains the current time as reported by the SNTP module.
    Use this value for absolute time stamping.  The value returned is (by
    default) the number of seconds since 01-Jan-1970 00:00:00.

  Precondition:
    None

  Parameters:
    None

  Returns:
    The number of seconds since the Epoch.  (Default 01-Jan-1970 00:00:00)

  Remarks:
    Do not use this function for time difference measurements.  The Tick
    module is more appropriate for those requirements.
 ***************************************************************************/
uint32_t SNTPGetUTCSeconds(void)
{
    uint32_t dwTickDelta;
    uint32_t dwTick;

    // Update the dwSNTPSeconds variable with the number of seconds
    // that has elapsed
    dwTick = TickGet();
    dwTickDelta = dwTick - dwLastUpdateTick;
    while (dwTickDelta > TICK_SECOND) {
        dwSNTPSeconds++;
        dwTickDelta -= TICK_SECOND;
    }

    // Save the tick and residual fractional seconds for the next call
    dwLastUpdateTick = dwTick - dwTickDelta;

    return dwSNTPSeconds;
}

#endif // if defined(STACK_USE_SNTP_CLIENT)
