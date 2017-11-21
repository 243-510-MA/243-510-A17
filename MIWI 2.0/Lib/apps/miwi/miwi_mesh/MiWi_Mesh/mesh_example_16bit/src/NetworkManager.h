#ifndef _NETWORK_MANAGER_H_

#define _NETWORK_MANAGER_H_

#ifdef MRF89XA_COMPILE
    #define ADDRESS_LEN     2
#endif

#ifdef MRF24J40_COMPILE
    #define ADDRESS_LEN         8
#endif

#if !defined(ADDRESS_LEN)
    //#define ADDRESS_LEN     2
    #error Define Address Len!!!
#endif

//Used to identify current device type on network
typedef enum DeviceType_t_def
{
    DeviceCoordinator,
    DeviceRFD,
    DeviceSleepingRFD,
    DeviceUnknown=0x07,
}DeviceType_t;

typedef union addr_t_def
{
    uint8_t bytes[ADDRESS_LEN];
    uint16_t even_addr_pack;    //Dummy, do not use;  Without this XC16 compiler places this 
                            //structure at odd address, which can cause address errors
}addr_t;

//Sets the address size (max 8)
void NetworkAddressSizeSet(uint8_t value);
uint8_t NetworkAddressSizeGet(void);

//Returns the device type that was set at network init
DeviceType_t NetworkGetDeviceType(void);

//Call to initialize network
void NetworkInit(addr_t address, uint16_t PANID, DeviceType_t dev);
//Sets/updates address/PAN of device
void NetworkSetAddressPAN(addr_t address, uint16_t PANID);
//Set Device Type
void NetworkSetDeviceType(DeviceType_t dev);

//Main network task function
void NetworkManagerTask(void);

//Function that can be used to compare addresses by user code
bool isSameAddress(addr_t a1, addr_t a2);

//Returns address of current parent (all 0xffs = No Valid Parent!)
addr_t NetworkGetParentAddress(void);

//returns true if route to coordinator exists (always true on coord)
bool NetworkHasRoute(void);

//Used by coordinator to reply to RFDs' discovery request
void NetworkSendDiscoverResponse(void);
#endif