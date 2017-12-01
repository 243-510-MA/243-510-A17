#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "spi.h"
#include "gtypedefs.h"
#include "system.h"
#include "miwi_config.h"
#include "NetworkManager.h"
#include "NetworkBridge.h"
#include "driver/mrf_miwi/drv_mrf_miwi_mesh.h"
#include "miwi/miwi_mesh_api.h"
#include "Bridge.h"
#include "symbol.h"
//#include "Transceivers.h"
#ifdef __PIC32MX__
    #include <plib.h>
#endif

#ifdef ENABLE_SECURITY
    //used for security and network discovery
    CONNECTION_ENTRY    ConnectionTable[CONNECTION_SIZE];
#endif

#ifdef ENABLE_ACTIVE_SCAN
    ACTIVE_SCAN_RESULT ActiveScanResults[ACTIVE_SCAN_RESULT_SIZE];
    uint8_t ActiveScanChannel;
#endif
    
MAC_RECEIVED_PACKET  MACRxPacket;
RECEIVED_MESSAGE rxMessage;
addr_t srcAddress;
uint8_t TxBuff[RADIO_BUFFER_SIZE];//Used by MiMac
uint8_t TxIndex;
uint8_t TxBuffer[RADIO_BUFFER_SIZE];//Used by MiApp
uint8_t TxData;
bool UserMessageAvailable=false;
uint8_t miapp_currentChannel=0;
extern DeviceType_t myDeviceType;

void radioReceivedPacket(addr_t source, uint16_t PANID, uint8_t*data,uint16_t size,uint8_t rssi, bool isBroadcast, bool isSecure);
#ifdef ENABLE_SECURITY
    uint16_t AddNodeToNetworkTable(addr_t address, uint16_t PANID);
#endif
#ifdef IEEE_802_15_4
uint16_t myShortAddress=0x01;
#endif

#if MY_ADDRESS_LENGTH == 8
    uint8_t myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3, EUI_4, EUI_5,EUI_6,EUI_7};
#elif MY_ADDRESS_LENGTH == 7
    uint8_t myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3, EUI_4, EUI_5,EUI_6};
#elif MY_ADDRESS_LENGTH == 6
    uint8_t myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3, EUI_4, EUI_5};
#elif MY_ADDRESS_LENGTH == 5
    uint8_t myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3, EUI_4};        
#elif MY_ADDRESS_LENGTH == 4
    uint8_t myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2,EUI_3};  
#elif MY_ADDRESS_LENGTH == 3
    uint8_t myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1,EUI_2};    
#elif MY_ADDRESS_LENGTH == 2
    uint8_t myLongAddress[MY_ADDRESS_LENGTH] = {EUI_0,EUI_1};    
#endif
uint16_t myPANID=MY_PAN_ID;

bool SendPacket(bool Broadcast,uint8_t *DestinationAddress, uint16_t destinationPANID, bool isCommand,bool SecurityEnabled)
{
    MAC_TRANS_PARAM tParam;
    bool status;
    
    tParam.flags.Val = 0;
    tParam.flags.bits.packetType = (isCommand) ? PACKET_TYPE_COMMAND : PACKET_TYPE_DATA;
    tParam.flags.bits.ackReq = (Broadcast) ? 0 : 1;
    tParam.flags.bits.broadcast = Broadcast;
    tParam.flags.bits.secEn = SecurityEnabled;
    #if defined(IEEE_802_15_4)
        tParam.altSrcAddr = 0;
        tParam.altDestAddr = (Broadcast) ? true : false;
    #endif
    
    #if defined(INFER_DEST_ADDRESS)
        tParam.flags.bits.destPrsnt = 0;
    #else
        tParam.flags.bits.destPrsnt = 1;//(Broadcast) ? 0:1;
    #endif
    
    #if defined(SOURCE_ADDRESS_ABSENT)
        if( tParam.flags.bits.packetType == PACKET_TYPE_COMMAND )
        {
            tParam.flags.bits.sourcePrsnt = 1;
        }
        else
        {
            tParam.flags.bits.sourcePrsnt = 0;
        }
    #else
        tParam.flags.bits.sourcePrsnt = 1;
    #endif
    
    tParam.DestAddress = DestinationAddress;
    tParam.DestPANID.Val = destinationPANID;

    status = MiMAC_SendPacket(tParam, TxBuff, TxIndex); 
    TxIndex = 0;
    
    return status;
}

bool radioBroadcast(uint8_t *data, uint8_t datasize, uint16_t PANID, bool isSecure)
{
    static addr_t addr;
    uint8_t i;
    
    //addr.bytes[0]=0x12;
    //addr.bytes[1]=0x11;
    //radioUnicast(addr,data,datasize);
    //return(true);
    
    memset(addr.bytes,0xff,sizeof(addr));
    TxIndex=0;
    for(i=0;i<datasize;i++)
    {
        TxBuff[TxIndex++]=data[i];
    }
    return SendPacket(true,addr.bytes,PANID,false,isSecure);
}

bool radioUnicast(addr_t dest, uint16_t PANID, uint8_t *data, uint8_t datasize, bool isSecure)
{
    static addr_t addr;
    uint8_t i;
    addr=dest;
    TxIndex=0;
    for(i=0;i<datasize;i++)
    {
        TxBuff[TxIndex++]=data[i];
    }
    return SendPacket(false,addr.bytes,PANID,false,isSecure);
}

//User code must implement this function; releases the received packet when it has been processed
void radioDiscardPacket(void)
{
    MiMAC_DiscardPacket();
}

bool MiApp_ProtocolInit(bool bNetworkFreezer)
{
    bool res;
    MACINIT_PARAM initValue;
    initValue.PAddress = (uint8_t *) myLongAddress;
    initValue.actionFlags.bits.CCAEnable = 1;
    initValue.actionFlags.bits.PAddrLength = MY_ADDRESS_LENGTH;
    initValue.actionFlags.bits.RepeaterMode = 0;
#ifdef ENABLE_SECURITY
    memset(ConnectionTable,0x00,sizeof(ConnectionTable));
#endif
#ifdef IEEE_802_15_4
    MiMAC_SetAltAddress(myLongAddress,(uint8_t*)&myPANID);
#endif
    res=(MiMAC_Init(initValue));
    MiApp_SetAddressPan(myLongAddress, myPANID);
    NetworkSetDeviceType(myDeviceType);
    return(res);
}

bool MiApp_SetChannel(uint8_t Channel)
{
    bool res=(MiMAC_SetChannel(Channel,0));
    if(res==true)
    {
        miapp_currentChannel=Channel;
    }
    return(res);
}

bool MiApp_StartConnection( uint8_t Mode, uint8_t ScanDuration, uint32_t ChannelMap)
{
    addr_t src;
    memcpy(src.bytes,myLongAddress,MY_ADDRESS_LENGTH);
    NetworkInit(src, myPANID, DeviceCoordinator);
    return(true);
}

uint8_t MiApp_EstablishConnection(uint8_t ActiveScanIndex, uint8_t Mode)
{
    addr_t src;
    DeviceType_t dev=DeviceRFD;
    
    memcpy(src.bytes,myLongAddress,MY_ADDRESS_LENGTH);
    if(ActiveScanIndex<ACTIVE_SCAN_RESULT_SIZE)
    {
        if(ActiveScanResults[ActiveScanIndex].Flags!=0)
        {
            MiApp_SetAddressPan(src.bytes, ActiveScanResults[ActiveScanIndex].PANID.Val);
            MiApp_SetChannel(ActiveScanResults[ActiveScanIndex].Channel);            
        }
    }
    if(myDeviceType!=DeviceCoordinator)
    {
        //If not coordinator, then use the type user selected
        dev=myDeviceType;
    }
    
    myDeviceType=dev;    
    NetworkInit(src, myPANID, dev);
    return(0);
}

void MiApp_SetAddressPan(uint8_t *address, uint16_t panid)
{
    addr_t addr;
    memcpy(myLongAddress,address,MY_ADDRESS_LENGTH);
    memcpy(addr.bytes,address,MY_ADDRESS_LENGTH);    
    myPANID=panid;
#ifdef IEEE_802_15_4
    MiMAC_SetAltAddress((uint8_t*)myLongAddress,(uint8_t*)&myPANID);
#endif
    NetworkSetAddressPAN(addr,panid);
}


bool MiApp_UnicastAddress(uint8_t *DestinationAddress, uint8_t* Origin, bool SecEn)
{
    addr_t src;
    memcpy(src.bytes,DestinationAddress,MY_ADDRESS_LENGTH);
    return(NetworkUnicastData(src, TxBuffer, TxData,SecEn));
}

bool MiApp_BroadcastPacket(bool SecEn)
{
    return(NetworkBroadcastData(TxBuffer, TxData,SecEn));
}

bool MiApp_MessageAvailable(void)
{
    addr_t src;
    static Tick tStart={0};
    
    if(TickGetDiffMs(TickGetMs(),tStart)>=1)
    {
        tStart=TickGetMs();
        TimeTickISR();
    }
    NetworkManagerTask();
    if(MiMAC_ReceivedPacket()==true)
    {
        memcpy(src.bytes,MACRxPacket.SourceAddress,sizeof(src));
        radioReceivedPacket(src,MACRxPacket.SourcePANID,MACRxPacket.Payload,MACRxPacket.PayloadLen,MACRxPacket.RSSIValue,MACRxPacket.flags.bits.broadcast,MACRxPacket.flags.bits.secEn);
    }
#ifdef ENABLE_SECURITY
    if(UserMessageAvailable==true)
    {
        memcpy(src.bytes,rxMessage.SourceAddress,sizeof(src));
        AddNodeToNetworkTable(src,rxMessage.SourcePANID.Val);
    }
#endif
    return(UserMessageAvailable);
}

void MiApp_DiscardMessage(void)
{
    UserMessageAvailable=false;
    NetworkReleaseUserData();
}

void NetworkDataArrivedEvent(addr_t source, uint8_t *data, uint16_t size, bool isBroadcast)
{
    srcAddress=source;
    memset(&rxMessage,0x00,sizeof(rxMessage));
    rxMessage.Payload=data;
    rxMessage.PayloadSize=size;
    rxMessage.SourceAddress=srcAddress.bytes;
    rxMessage.SourcePANID.Val=myPANID;
    rxMessage.flags.bits.broadcast=isBroadcast;
    rxMessage.flags.bits.srcPrsnt=1;    
    UserMessageAvailable=true;
}

bool MiApp_IsMemberOfNetwork(void)
{
    return(IsMemberOfNetwork());
}

addr_t MiApp_GetParentAddress(void)
{
    return(NetworkGetParentAddress());
}
#ifdef ENABLE_SECURITY
uint16_t AddNodeToNetworkTable(addr_t address, uint16_t PANID)
{
    uint16_t i;
    uint16_t ind=0xffff;
    for(i=0;i<CONNECTION_SIZE;i++)
    {
        if( (isSameAddress(ConnectionTable[i].Address,address)==true) 
                && (ConnectionTable[i].PANID.Val == PANID)
                && (ConnectionTable[i].status.bits.isValid==1) )
        {
            break;
        }else if( (ConnectionTable[i].status.bits.isValid==0) && (ind==0xffff) )
        {
            ind=i;
        }
    }
    if(i>=CONNECTION_SIZE)
    {
        i=ind;
        if(i!=0xffff)
        {
            memset(&ConnectionTable[i],0x00,sizeof(CONNECTION_ENTRY));
            memcpy(ConnectionTable[i].Address.bytes,address.bytes,sizeof(address));
            ConnectionTable[i].PANID.Val=PANID;
            ConnectionTable[i].status.bits.isValid=true;
#ifdef ENABLE_SECURITY
            ConnectionTable[i].IncomingFrameCounter=0;
#endif
        }
    }
    return(i);
}
#endif
    
void    MiApp_RemoveConnection(uint8_t ConnectionIndex)
{
    //NA
}

void MiApp_ConnectionMode(uint8_t Mode)
{
    if(NetworkGetDeviceType()==DeviceCoordinator)
    {
        //Only applies to coordinator
        if(Mode==ENABLE_ALL_CONN)
        {
            
        }else if(Mode==DISABLE_ALL_CONN)
        {
            
        }
    }
}
uint8_t MiApp_SearchConnection(uint8_t ScanDuartion, uint32_t ChannelMap)
{
    uint8_t res=0xff;
    uint8_t i;
    Tick tStart;
    uint32_t mask=(1<<1);
    uint16_t msDuration=((uint16_t)ScanDuartion)<<5;//32ms per count
    memset(ActiveScanResults,0x00,sizeof(ActiveScanResults));
    for(i=1;i<32;i++)
    {
        if((mask & ChannelMap)>0)
        {
            ActiveScanChannel=i;
            MiApp_SetChannel(ActiveScanChannel);
            NetworkSendDiscoverRequest();
            tStart=TickGetMs();
            do
            {
                if(MiApp_MessageAvailable())
                {
                    MiApp_DiscardMessage();
                }
                ClrWdt();
            }while(TickGetDiffMs(TickGetMs(),tStart)<msDuration);
        }
        mask<<=1;
    }
    for(i=0;i<ACTIVE_SCAN_RESULT_SIZE;i++)
    {
        if(ActiveScanResults[i].Flags==0)
        {
            break;
        }
    }
    if(i>0)
    {
        res=i;
    }
    return(res);
}
volatile uint16_t pcnt1=0;
void NetworkDiscoveredEvent(uint16_t PANID, uint16_t rssi)
{
    uint16_t i;
    uint16_t empty=0xffff;
    pcnt1++;
    for(i=0;i<ACTIVE_SCAN_RESULT_SIZE;i++)
    {
        if( (ActiveScanResults[i].PANID.Val==PANID) && (ActiveScanResults[i].Channel==ActiveScanChannel) )
        {
            if(ActiveScanResults[i].RSSIValue<rssi)
            {
                ActiveScanResults[i].RSSIValue=rssi;
            }
            break;
        }
        if( (ActiveScanResults[i].Flags==0) && (empty==0xffff) )
        {
            empty=i;
        }
    }
    if(i>=ACTIVE_SCAN_RESULT_SIZE)
    {
        if(empty!=0xffff)
        {
            i=empty;
            
            ActiveScanResults[i].Flags=1;
            ActiveScanResults[i].PANID.Val=PANID;
            ActiveScanResults[i].RSSIValue=rssi;
            ActiveScanResults[i].Channel=ActiveScanChannel;
        }
    }
}

uint8_t MiApp_TransceiverPowerState(uint8_t Mode)
{
    uint8_t res;
    if(Mode==POWER_STATE_SLEEP)
    {
        res=MiMAC_PowerState(POWER_STATE_DEEP_SLEEP);
    }else
    {
        res=MiMAC_PowerState(POWER_STATE_OPERATE);    
    }
    return(res);
}

void MiApp_RequestData(void)
{
    NetworkIssueDataRequest();
}

