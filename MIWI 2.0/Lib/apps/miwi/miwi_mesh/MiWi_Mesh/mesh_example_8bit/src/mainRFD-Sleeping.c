/* 1. GRANT OF LICENSE. Viper Design LLC hereby grants you a limited, non-exclusive license to use the 
   code on twenty five (25) network devices (code is limited to operate with 25 or fewer devices). 
   If you need to use ViperLink on more than twenty five (25) devices at a time, you may request such
   permission from us via email:  info@viper-design.com
2. DISCLAIMER OF WARRANTIES: The code is provided ?as is.? to the full extent permitted by applicable local
   law, Viper Design disclaim all other warranties, representations and conditions, express or implied, 
   including, but not limited to, the implied warranties, representations and conditions of merchantability,
   fitness for a particular purpose and non-infringement.
   copyright 2017 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "system.h"
#include "gtypedefs.h"
#include "spi.h"
#include "symbol.h"
#include "delay.h"
#include "miwi_config.h"
#include "miwi/miwi_mesh_api.h"
#include "lcd.h"
#include "NetworkManager.h"


#define VER_MAJOR  2
#define VER_MINOR  0

//Can only be used on structures, due to address misalignment issues on 16bit and 32bit platforms
#define u16(a)  *(uint16_t*)(&a)

uint8_t currentChannel=25;
addr_t lastSource={0};
addr_t myAddr;
DeviceType_t myDeviceType;

addr_t Read_MAC_Address(void);

typedef enum devState_t_def
{
    devPOR,
    devWaitJoin,
    devAwake,
    devSleep,
}devState_t;

devState_t devState=devPOR;

int main(void)
{
    addr_t src;
    addr_t src1;
    Tick tStart;
    bool pButtonPressed_0=false;
    bool pButtonPressed_1=false;
    bool SendPacket=false;
    bool RequestData=false;
    uint8_t counter=0;
    bool pMemberOfNetwork=false;

    //Sets device type for the current device
    myDeviceType=DeviceSleepingRFD;
    
    HardwareInit();
    InitSymbolTimer();

    myAddr=Read_MAC_Address();
   
    currentChannel = 25;
    MiApp_SetAddressPan(myAddr.bytes, 0x5533);
    MiApp_ProtocolInit(false);

    LED0Off();
    LED1Off();
    LED2Off();   
    LinePrint(0,"a:%04X p:FFFF",u16(myAddr));
    
    //Join any network on current channel
    //MiApp_SetChannel(currentChannel);
    //MiApp_EstablishConnection(0xff,0);

    //Perform active scan first and then join the first network found
    while(MiApp_SearchConnection(14,(0x0full<<25))==0xff);
    //Enable network level security (does not impact User Level Security)
    //MiApp_SetNetworkSecure(true);
    MiApp_EstablishConnection(0,0);    

    while(1)
    {
        ButtonTask();
        LEDManagerTask();
        switch(devState)
        {
            case devPOR:
                tStart=TickGetMs();
                SendPacket=false;
                RequestData=false;
                devState=devWaitJoin;
                break;
                
            case devWaitJoin:
                if(MiApp_IsMemberOfNetwork()==true) 
                {
                    devState=devAwake;
                    tStart=TickGetMs();
                }
                if(MiApp_MessageAvailable()==true)
                {
                    MiApp_DiscardMessage();
                }
                break;
                
            case devAwake:
                if(TickGetDiffMs(TickGetMs(),tStart)<MS(300))
                {
                    if(MiApp_MessageAvailable()==true)
                    {
                        LinePrint2(17,"i:%04X=%02X      ",*((uint16_t*)(rxMessage.SourceAddress)),rxMessage.Payload[1]);
                        LEDCommand(LEDGreen,lCMD_Toggle);
                        MiApp_DiscardMessage();
                        tStart=TickGetMs();//Stay away another 300ms
                        MiApp_RequestData();//Keep requesting data, so that all buffered
                                    //packets are delivered, not just the first one
                    }
                }else
                {
                    MiApp_TransceiverPowerState(POWER_STATE_SLEEP);
                    devState=devSleep;
                }
                if(MiApp_IsMemberOfNetwork()==false) 
                {
                    devState=devWaitJoin;
                }
                break;     
            
            case devSleep: 
                //Micro can sleep in this state and wake up on interrupts
                //this example simulates this without sleeping; SendPacket and RequestPacket 
                //Flags are set elsewhere to wake the process up
                //Radio is asleep in this state
                if(SendPacket==true)
                {
                    MiApp_TransceiverPowerState(POWER_STATE_WAKEUP);
                    addr_t coord={0};
                    memset(coord.bytes,0x00,sizeof(coord));
                    MiApp_FlushTx();
                    MiApp_WriteData(mUserDataType);
                    MiApp_WriteData(counter);
                    if(MiApp_UnicastAddress(coord.bytes, NULL, false)==true)
                    {
                        //Only increment if able to send data (i.e. network OK, and 
                        //buffer is not full)
                        LinePrint(17,"o:%02X           ",counter++);
                    }
                    tStart=TickGetMs();
                    SendPacket=false;
                    devState=devAwake;
                }
                if(RequestData==true)
                {
                    MiApp_TransceiverPowerState(POWER_STATE_WAKEUP);
                    MiApp_RequestData();
                    tStart=TickGetMs();
                    RequestData=false;
                    devState=devAwake;
                }
                break;
        }
        
        if(isButtonPressed(0)==true)
        {
            if(pButtonPressed_0==false)
            {
                pButtonPressed_0=true;
                SendPacket=true;
            }
        }else
        {
            pButtonPressed_0=false;
        }
        if(isButtonPressed(1)==true)
        {
            if(pButtonPressed_1==false)
            {
                pButtonPressed_1=true;
                RequestData=true;
            }
        }else
        {
            pButtonPressed_1=false;
        }
        
        if(MiApp_IsMemberOfNetwork()==true)
        {
            src1=MiApp_GetParentAddress();
            if( (pMemberOfNetwork==false) || (memcmp(src.bytes,src1.bytes,MY_ADDRESS_LENGTH)!=0))
            {
                pMemberOfNetwork=true;
                src=MiApp_GetParentAddress();
                LinePrint(7,"p:%04X",u16(src));
            }
        }else
        {
            if(pMemberOfNetwork==true)
            {
                pMemberOfNetwork=false;
                src=MiApp_GetParentAddress();
                LinePrint(7,"p:%s","FFFF");
            }        
        }
    }
}

//EEPROM access functions
#define sSPI_WRT_STATUS  0x01
#define sSPI_WRITE       0x02
#define sSPI_READ        0x03
#define sSPI_DIS_WRT     0x04
#define sSPI_RD_STATUS   0x05
#define sSPI_EN_WRT      0x06

#define EEPROM_MAC_ADDR 0xFA

void EEPROMRead(uint8_t *dest, uint8_t addr, uint8_t count)
{
    MAC_nCS = 0;
    SPIPut2(sSPI_READ);
    SPIPut2(addr);
    while( count )
    {
        *dest++ = SPIGet2();
        count--;
    }
    MAC_nCS = 1;
} 

addr_t Read_MAC_Address(void)
{
    uint8_t addr8[6];
    addr_t address;
    uint8_t *pAddress=(uint8_t*)&address;
    uint8_t i;
    uint8_t j=0;
    for(i=0;i<6;i++)
    {
        EEPROMRead(&(addr8[6-1-i]), EEPROM_MAC_ADDR+i, 1);
    }
	if( MY_ADDRESS_LENGTH > 6 )
	{
	    for(i = 0; i < 3; i++)
	    {
	        pAddress[j++]=addr8[i];
	    }
	    if( MY_ADDRESS_LENGTH > 7 )
	    {
	        pAddress[j++] = 0xFE;
	    }
	    pAddress[j++] = 0xFF;
	    for(i = 0; i < 3; i++)
	    {
	        pAddress[j++]=addr8[i+3];
	    }
	}
	else
	{
        memcpy(pAddress,addr8,MY_ADDRESS_LENGTH);
	}
    return address;
}
