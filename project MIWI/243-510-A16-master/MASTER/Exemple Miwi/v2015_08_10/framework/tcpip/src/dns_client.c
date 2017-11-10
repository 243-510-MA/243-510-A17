/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    dns_client.c

  Summary:
    Module for Microchip TCP/IP Stack
     -Provides hostname to IP address translation
     -Reference: RFC 1035

  Description:
    Domain Name System (DNS) Client

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

#define __DNS_C_

#include "tcpip/tcpip.h"

#if defined(STACK_USE_DNS_CLIENT)

/***************************************************************************
  Section:
    Constants and Global Variables
 ***************************************************************************/

#define DNS_PORT  53u // Default port for DNS resolutions
#define DNS_TIMEOUT  (TICK_SECOND * 1) // Elapsed time after which a DNS resolution is considered to have timed out

static UDP_SOCKET MySocket = INVALID_UDP_SOCKET; // UDP socket to use for DNS queries
static uint8_t *DNSHostName; // Host name in RAM to look up
static ROM uint8_t *DNSHostNameROM; // Host name in ROM to look up
static uint8_t RecordType; // Record type being queried
static NODE_INFO ResolvedInfo; // Node information about the resolved node

// Semaphore flags for the DNS module
static union {
    uint8_t Val;

    struct {
        unsigned char DNSInUse : 1; // Indicates the DNS module is in use
        unsigned char AddressValid : 1; // Indicates that the address resolution is valid and complete
        unsigned char filler : 6;
    } bits;
} Flags = {0x00};

// State machine for a DNS query
static enum {
    DNS_START = 0, // Initial state to reset client state variables
    DNS_ARP_START_RESOLVE, // Send ARP resolution of DNS server or gateway MAC address
    DNS_ARP_RESOLVE, // Wait for response to ARP request
    DNS_OPEN_SOCKET, // Open UDP socket
    DNS_QUERY, // Send DNS query to DNS server
    DNS_GET_RESULT, // Wait for response from DNS server
    DNS_FAIL, // ARP or DNS server not responding
    DNS_DONE // DNS query is finished
} smDNS = DNS_DONE;

// Structure for the DNS header
typedef struct {
    TCPIP_UINT16_VAL TransactionID;
    TCPIP_UINT16_VAL Flags;
    TCPIP_UINT16_VAL Questions;
    TCPIP_UINT16_VAL Answers;
    TCPIP_UINT16_VAL AuthoritativeRecords;
    TCPIP_UINT16_VAL AdditionalRecords;
} DNS_HEADER;

typedef struct {
    // Response name is first, but it is variable length and must be retrieved using the DNSDiscardName() function
    TCPIP_UINT16_VAL ResponseType;
    TCPIP_UINT16_VAL ResponseClass;
    TCPIP_UINT32_VAL ResponseTTL;
    TCPIP_UINT16_VAL ResponseLen;
} DNS_ANSWER_HEADER;

/***************************************************************************
  Section:
    Function Prototypes
 ***************************************************************************/
static void DNSPutString(uint8_t *String);
static void DNSDiscardName(void);

#if defined(__XC8)
static void DNSPutROMString(ROM uint8_t *String);
#else
// Non-ROM alias for C30/C32
#define DNSPutROMString(a)  DNSPutString((uint8_t *)a)
#endif

/***************************************************************************
  Function:
    bool DNSInit(void)

  Summary:
    DNS module initialization function.

  Description:
    This function does not do anything when the stack is initialized for the
    first time. The real work it does is to clean DNS semaphore during Wi-Fi
    Soft AP redirection. So it is actually a re-initialization function.

  Precondition:
    Stack is initialized.

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None

 ***************************************************************************/
void DNSInit(void)
{
    if (AppConfig.hibernateFlag) {
        DNSBeginUsage();
        DNSDiscardName();
        DNSEndUsage();
    }
}

/***************************************************************************
  Function:
    bool DNSBeginUsage(void)

  Summary:
    Claims access to the DNS module.

  Description:
    This function acts as a semaphore to obtain usage of the DNS module.
    Call this function and ensure that it returns true before calling any
    other DNS APIs.  Call DNSEndUsage when this application no longer
    needs the DNS module so that other applications may make use of it.

  Precondition:
    Stack is initialized.

  Parameters:
    None

  Return Values:
    true - No other DNS resolutions are in progress and the calling
            application has sucessfully taken ownership of the DNS module
    false - The DNS module is currently in use.  Yield to the stack and
            attempt this call again later.

  Remarks:
    Ensure that DNSEndUsage is always called once your application has
    obtained control of the DNS module.  If this is not done, the stack
    will hang for all future applications requiring DNS access.
 ***************************************************************************/
bool DNSBeginUsage(void)
{
    if (Flags.bits.DNSInUse)
        return false;

    Flags.bits.DNSInUse = true;
    return true;
}

/***************************************************************************
  Function:
    bool DNSEndUsage(void)

  Summary:
    Releases control of the DNS module.

  Description:
    This function acts as a semaphore to obtain usage of the DNS module.
    Call this function when this application no longer needs the DNS
    module so that other applications may make use of it.

  Precondition:
    DNSBeginUsage returned true on a previous call.

  Parameters:
    None

  Return Values:
    true - The address to the host name was successfully resolved.
    false - The DNS failed or the address does not exist.

  Remarks:
    Ensure that DNSEndUsage is always called once your application has
    obtained control of the DNS module.  If this is not done, the stack
    will hang for all future applications requiring DNS access.
 ***************************************************************************/
bool DNSEndUsage(void)
{
    if (MySocket != INVALID_UDP_SOCKET) {
        UDPClose(MySocket);
        MySocket = INVALID_UDP_SOCKET;
    }
    smDNS = DNS_DONE;
    Flags.bits.DNSInUse = false;

    return Flags.bits.AddressValid;
}

/***************************************************************************
  Function:
    void DNSResolve(uint8_t *Hostname, uint8_t Type)

  Summary:
    Begins resolution of an address.

  Description:
    This function attempts to resolve a host name to an IP address.  When
    called, it starts the DNS state machine.  Call DNSIsResolved repeatedly
    to determine if the resolution is complete.

    Only one DNS resoultion may be executed at a time.  The Hostname must
    not be modified in memory until the resolution is complete.

  Precondition:
    DNSBeginUsage returned true on a previous call.

  Parameters:
    Hostname - A pointer to the null terminated string specifiying the
        host for which to resolve an IP.
    RecordType - DNS_TYPE_A or DNS_TYPE_MX depending on what type of
        record resolution is desired.

  Returns:
    None

  Remarks:
    This function requires access to one UDP socket.  If none are available,
    MAX_UDP_SOCKETS may need to be increased.
 ***************************************************************************/
void DNSResolve(uint8_t *Hostname, uint8_t Type)
{
    if (StringToIPAddress(Hostname, &ResolvedInfo.IPAddr)) {
        Flags.bits.AddressValid = true;
        smDNS = DNS_DONE;
    } else {
        DNSHostName = Hostname;
        DNSHostNameROM = NULL;
        smDNS = DNS_START;
        RecordType = Type;
        Flags.bits.AddressValid = false;
    }
}

/***************************************************************************
  Function:
    void DNSResolveROM(ROM uint8_t *Hostname, uint8_t Type)

  Summary:
    Begins resolution of an address.

  Description:
    This function attempts to resolve a host name to an IP address.  When
    called, it starts the DNS state machine.  Call DNSIsResolved repeatedly
    to determine if the resolution is complete.

    Only one DNS resoultion may be executed at a time.  The Hostname must
    not be modified in memory until the resolution is complete.

  Precondition:
    DNSBeginUsage returned true on a previous call.

  Parameters:
    Hostname - A pointer to the null terminated string specifiying the
        host for which to resolve an IP.
    RecordType - DNS_TYPE_A or DNS_TYPE_MX depending on what type of
        record resolution is desired.

  Returns:
    None

  Remarks:
    This function requires access to one UDP socket.  If none are available,
    MAX_UDP_SOCKETS may need to be increased.

    This function is aliased to DNSResolve on non-PIC18 platforms.
 ***************************************************************************/
#if defined(__XC8)
void DNSResolveROM(ROM uint8_t *Hostname, uint8_t Type)
{
    if (ROMStringToIPAddress(Hostname, &ResolvedInfo.IPAddr)) {
        Flags.bits.AddressValid = true;
        smDNS = DNS_DONE;
    } else {
        DNSHostName = NULL;
        DNSHostNameROM = Hostname;
        smDNS = DNS_START;
        RecordType = Type;
        Flags.bits.AddressValid = false;
    }
}
#endif

/*****************************************************************************
  Function:
    bool DNSIsResolved(IP_ADDR *HostIP)

  Summary:
    Determines if the DNS resolution is complete and provides the IP.

  Description:
    Call this function to determine if the DNS resolution of an address has
    been completed.  If so, the resolved address will be provided in HostIP.

  Precondition:
    DNSResolve or DNSResolveROM has been called.

  Parameters:
    HostIP - A pointer to an IP_ADDR structure in which to store the
        resolved IP address once resolution is complete.

  Return Values:
    true - The DNS client has obtained an IP, or the DNS process
        has encountered an error.  HostIP will be 0.0.0.0 on error.  Possible
        errors include server timeout (i.e. DNS server not available), hostname
        not in the DNS, or DNS server errors.
    false - The resolution process is still in progress.
 ***************************************************************************/
bool DNSIsResolved(IP_ADDR *HostIP)
{
    static uint32_t StartTime;
    static TCPIP_UINT16_VAL SentTransactionID __attribute__((persistent));
    static uint8_t vARPAttemptCount;
    static uint8_t vDNSAttemptCount;
    uint8_t i;
    TCPIP_UINT16_VAL w;
    DNS_HEADER DNSHeader;
    DNS_ANSWER_HEADER DNSAnswerHeader;

    switch (smDNS) {
    case DNS_START:
        vARPAttemptCount = 0;
        vDNSAttemptCount = 0;
        // No break;

    case DNS_ARP_START_RESOLVE:
        ARPResolve(&AppConfig.PrimaryDNSServer);
        vARPAttemptCount++;
        StartTime = TickGet();
        smDNS = DNS_ARP_RESOLVE;
        break;

    case DNS_ARP_RESOLVE:
        if (!ARPIsResolved(&AppConfig.PrimaryDNSServer, &ResolvedInfo.MACAddr)) {
            if (TickGet() - StartTime > DNS_TIMEOUT)
                smDNS = (vARPAttemptCount >= 3u) ? DNS_FAIL : DNS_ARP_START_RESOLVE;
            break;
        }
        ResolvedInfo.IPAddr.Val = AppConfig.PrimaryDNSServer.Val;
        smDNS = DNS_OPEN_SOCKET;
        // No break: DNS_OPEN_SOCKET is the correct next state

    case DNS_OPEN_SOCKET:
        //MySocket = UDPOpen(0, &ResolvedInfo, DNS_PORT);

        MySocket = UDPOpenEx((uint32_t) (PTR_BASE) & ResolvedInfo, UDP_OPEN_NODE_INFO, 0, DNS_PORT);
        if (MySocket == INVALID_UDP_SOCKET)
            break;

        smDNS = DNS_QUERY;
        // No need to break, we can immediately start resolution

    case DNS_QUERY:
        if (!UDPIsPutReady(MySocket))
            break;

        // Put DNS query here
        SentTransactionID.Val++;
        UDPPut(SentTransactionID.v[1]); // User chosen transaction ID
        UDPPut(SentTransactionID.v[0]);
        UDPPut(0x01); // Standard query with recursion
        UDPPut(0x00);
        UDPPut(0x00); // 0x0001 questions
        UDPPut(0x01);
        UDPPut(0x00); // 0x0000 answers
        UDPPut(0x00);
        UDPPut(0x00); // 0x0000 name server resource records
        UDPPut(0x00);
        UDPPut(0x00); // 0x0000 additional records
        UDPPut(0x00);

        // Put hostname string to resolve
        if (DNSHostName)
            DNSPutString(DNSHostName);
        else
            DNSPutROMString(DNSHostNameROM);

        UDPPut(0x00); // Type: DNS_TYPE_A A (host address) or DNS_TYPE_MX for mail exchange
        UDPPut(RecordType);
        UDPPut(0x00); // Class: IN (Internet)
        UDPPut(0x01);

        UDPFlush();
        StartTime = TickGet();
        smDNS = DNS_GET_RESULT;
        break;

    case DNS_GET_RESULT:
        if (!UDPIsGetReady(MySocket)) {
            if (TickGet() - StartTime > DNS_TIMEOUT)
                smDNS = DNS_FAIL;
            break;
        }

        // Retrieve the DNS header and de-big-endian it
        UDPGet(&DNSHeader.TransactionID.v[1]);
        UDPGet(&DNSHeader.TransactionID.v[0]);

        // Throw this packet away if it isn't in response to our last query
        if (DNSHeader.TransactionID.Val != SentTransactionID.Val) {
            UDPDiscard();
            break;
        }

        UDPGet(&DNSHeader.Flags.v[1]);
        UDPGet(&DNSHeader.Flags.v[0]);
        UDPGet(&DNSHeader.Questions.v[1]);
        UDPGet(&DNSHeader.Questions.v[0]);
        UDPGet(&DNSHeader.Answers.v[1]);
        UDPGet(&DNSHeader.Answers.v[0]);
        UDPGet(&DNSHeader.AuthoritativeRecords.v[1]);
        UDPGet(&DNSHeader.AuthoritativeRecords.v[0]);
        UDPGet(&DNSHeader.AdditionalRecords.v[1]);
        UDPGet(&DNSHeader.AdditionalRecords.v[0]);

        // Remove all questions (queries)
        while (DNSHeader.Questions.Val--) {
            DNSDiscardName();
            UDPGet(&w.v[1]); // Question type
            UDPGet(&w.v[0]);
            UDPGet(&w.v[1]); // Question class
            UDPGet(&w.v[0]);
        }

        // Scan through answers
        while (DNSHeader.Answers.Val--) {
            DNSDiscardName(); // Throw away response name
            UDPGet(&DNSAnswerHeader.ResponseType.v[1]); // Response type
            UDPGet(&DNSAnswerHeader.ResponseType.v[0]);
            UDPGet(&DNSAnswerHeader.ResponseClass.v[1]); // Response class
            UDPGet(&DNSAnswerHeader.ResponseClass.v[0]);
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[3]); // Time to live
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[2]);
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[1]);
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[0]);
            UDPGet(&DNSAnswerHeader.ResponseLen.v[1]); // Response length
            UDPGet(&DNSAnswerHeader.ResponseLen.v[0]);

            // Make sure that this is a 4 byte IP address, response type A or MX, class 1
            // Check if this is Type A or MX
            if (DNSAnswerHeader.ResponseType.Val == 0x0001u &&
                    DNSAnswerHeader.ResponseClass.Val == 0x0001u && // Internet class
                    DNSAnswerHeader.ResponseLen.Val == 0x0004u) {
                Flags.bits.AddressValid = true;
                UDPGet(&ResolvedInfo.IPAddr.v[0]);
                UDPGet(&ResolvedInfo.IPAddr.v[1]);
                UDPGet(&ResolvedInfo.IPAddr.v[2]);
                UDPGet(&ResolvedInfo.IPAddr.v[3]);
                goto DoneSearchingRecords;
            } else {
                while (DNSAnswerHeader.ResponseLen.Val--) {
                    UDPGet(&i);
                }
            }
        }

        // Remove all Authoritative Records
        while (DNSHeader.AuthoritativeRecords.Val--) {
            DNSDiscardName(); // Throw away response name
            UDPGet(&DNSAnswerHeader.ResponseType.v[1]); // Response type
            UDPGet(&DNSAnswerHeader.ResponseType.v[0]);
            UDPGet(&DNSAnswerHeader.ResponseClass.v[1]); // Response class
            UDPGet(&DNSAnswerHeader.ResponseClass.v[0]);
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[3]); // Time to live
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[2]);
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[1]);
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[0]);
            UDPGet(&DNSAnswerHeader.ResponseLen.v[1]); // Response length
            UDPGet(&DNSAnswerHeader.ResponseLen.v[0]);

            // Make sure that this is a 4 byte IP address, response type A or MX, class 1
            // Check if this is Type A
            if (DNSAnswerHeader.ResponseType.Val == 0x0001u &&
                    DNSAnswerHeader.ResponseClass.Val == 0x0001u && // Internet class
                    DNSAnswerHeader.ResponseLen.Val == 0x0004u) {
                Flags.bits.AddressValid = true;
                UDPGet(&ResolvedInfo.IPAddr.v[0]);
                UDPGet(&ResolvedInfo.IPAddr.v[1]);
                UDPGet(&ResolvedInfo.IPAddr.v[2]);
                UDPGet(&ResolvedInfo.IPAddr.v[3]);
                goto DoneSearchingRecords;
            } else {
                while (DNSAnswerHeader.ResponseLen.Val--) {
                    UDPGet(&i);
                }
            }
        }

        // Remove all Additional Records
        while (DNSHeader.AdditionalRecords.Val--) {
            DNSDiscardName(); // Throw away response name
            UDPGet(&DNSAnswerHeader.ResponseType.v[1]); // Response type
            UDPGet(&DNSAnswerHeader.ResponseType.v[0]);
            UDPGet(&DNSAnswerHeader.ResponseClass.v[1]); // Response class
            UDPGet(&DNSAnswerHeader.ResponseClass.v[0]);
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[3]); // Time to live
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[2]);
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[1]);
            UDPGet(&DNSAnswerHeader.ResponseTTL.v[0]);
            UDPGet(&DNSAnswerHeader.ResponseLen.v[1]); // Response length
            UDPGet(&DNSAnswerHeader.ResponseLen.v[0]);

            // Make sure that this is a 4 byte IP address, response type A or MX, class 1
            // Check if this is Type A
            if (DNSAnswerHeader.ResponseType.Val == 0x0001u &&
                    DNSAnswerHeader.ResponseClass.Val == 0x0001u && // Internet class
                    DNSAnswerHeader.ResponseLen.Val == 0x0004u) {
                Flags.bits.AddressValid = true;
                UDPGet(&ResolvedInfo.IPAddr.v[0]);
                UDPGet(&ResolvedInfo.IPAddr.v[1]);
                UDPGet(&ResolvedInfo.IPAddr.v[2]);
                UDPGet(&ResolvedInfo.IPAddr.v[3]);
                goto DoneSearchingRecords;
            } else {
                while (DNSAnswerHeader.ResponseLen.Val--) {
                    UDPGet(&i);
                }
            }
        }

    DoneSearchingRecords:

        UDPDiscard();
        UDPClose(MySocket);
        MySocket = INVALID_UDP_SOCKET;
        smDNS = DNS_DONE;
        // No break, DNS_DONE is the correct step

    case DNS_DONE:
        // Return 0.0.0.0 if DNS resolution failed, otherwise return the
        // resolved IP address
        if (!Flags.bits.AddressValid)
            ResolvedInfo.IPAddr.Val = 0;
        HostIP->Val = ResolvedInfo.IPAddr.Val;
        return true;

    case DNS_FAIL:
        // If 3 attempts or more, quit
        if (vDNSAttemptCount >= 2u) {
            // Return an invalid IP address 0.0.0.0 if we can't finish ARP or DNS query step
            HostIP->Val = 0x00000000;
            return true;
        }
        vDNSAttemptCount++;

        // Swap primary and secondary DNS servers if there is a secondary DNS server programmed
        if (AppConfig.SecondaryDNSServer.Val) {
            AppConfig.PrimaryDNSServer.Val ^= AppConfig.SecondaryDNSServer.Val;
            AppConfig.SecondaryDNSServer.Val ^= AppConfig.PrimaryDNSServer.Val;
            AppConfig.PrimaryDNSServer.Val ^= AppConfig.SecondaryDNSServer.Val;

            // Start another ARP resolution for the secondary server (now primary)
            vARPAttemptCount = 0;
            if (MySocket != INVALID_UDP_SOCKET) {
                UDPClose(MySocket);
                MySocket = INVALID_UDP_SOCKET;
            }
            smDNS = DNS_ARP_START_RESOLVE;
        }

        break;

    }

    return false;
}

/*****************************************************************************
  Function:
    static void DNSPutString(uint8_t *String)

  Summary:
    Writes a string to the DNS socket.

  Description:
    This function writes a string to the DNS socket, ensuring that it is
    properly formatted.

  Precondition:
    UDP socket is obtained and ready for writing.

  Parameters:
    String - the string to write to the UDP socket.

  Returns:
    None
 ***************************************************************************/
static void DNSPutString(uint8_t *String)
{
    uint8_t *RightPtr;
    uint8_t i;
    uint8_t Len;

    RightPtr = String;

    while (1) {
        do {
            i = *RightPtr++;
        } while ((i != 0x00u) && (i != '.') && (i != '/') && (i != ',') && (i != '>'));

        // Put the length and data
        // Also, skip over the '.' in the input string
        Len = (uint8_t) (RightPtr - String - 1);
        UDPPut(Len);
        String += UDPPutArray(String, Len) + 1;

        if (i == 0x00u || i == '/' || i == ',' || i == '>')
            break;
    }

    // Put the string null terminator character (zero length label)
    UDPPut(0x00);
}

/*****************************************************************************
  Function:
    static void DNSPutROMString(ROM uint8_t *String)

  Summary:
    Writes a ROM string to the DNS socket.

  Description:
    This function writes a string to the DNS socket, ensuring that it is
    properly formatted.

  Precondition:
    UDP socket is obtained and ready for writing.

  Parameters:
    String - the string to write to the UDP socket.

  Returns:
    None

  Remarks:
    This function is aliased to DNSPutString on non-PIC18 platforms.
 ***************************************************************************/
#if defined(__XC8)
static void DNSPutROMString(ROM uint8_t *String)
{
    ROM uint8_t *RightPtr;
    uint8_t i;
    uint8_t Len;

    RightPtr = String;

    while (1) {
        do {
            i = *RightPtr++;
        } while ((i != 0x00u) && (i != '.') && (i != '/') && (i != ',') && (i != '>'));

        // Put the length and data
        // Also, skip over the '.' in the input string
        Len = (uint8_t) (RightPtr - String - 1);
        UDPPut(Len);
        String += UDPPutROMArray(String, Len) + 1;

        if (i == 0x00u || i == '/' || i == ',' || i == '>')
            break;
    }

    // Put the string terminator character (zero length label)
    UDPPut(0x00);
}
#endif

/*****************************************************************************
  Function:
    static void DNSDiscardName(void)

  Summary:
    Reads a name string or string pointer from the DNS socket and discards it.

  Description:
    This function reads a name string from the DNS socket.  Each string
    consists of a series of labels.  Each label consists of a length prefix
    byte, followed by the label bytes.  At the end of the string, a zero length
    label is found as termination.  If name compression is used, this function
    will automatically detect the pointer and discard it.

  Precondition:
    UDP socket is obtained and ready for reading a DNS name

  Parameters:
    None

  Returns:
    None
 ***************************************************************************/
static void DNSDiscardName(void)
{
    uint8_t i;

    while (1) {
        // Get first byte which will tell us if this is a 16-bit pointer or the
        // length of the first of a series of labels
        if (!UDPGet(&i))
            return;

        // Check if this is a pointer, if so, get the reminaing 8 bits and return
        if ((i & 0xC0u) == 0xC0u) {
            UDPGet(&i);
            return;
        }

        // Exit once we reach a zero length label
        if (i == 0u)
            return;

        // Discard complete label
        UDPGetArray(NULL, i);
    }
}

#endif // #if defined(STACK_USE_DNS_CLIENT)
