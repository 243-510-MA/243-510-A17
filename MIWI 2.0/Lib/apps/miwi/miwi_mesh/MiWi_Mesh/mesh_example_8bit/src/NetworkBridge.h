#ifndef _NETWORK_BRIDGE_H_

#define _NETWORK_BRIDGE_H_

//Implements function to detect when TX is busy
bool IsRadioTxBusy(void);

//These functions should exist in user code
bool radioBroadcast(uint8_t *data, uint8_t datasize, uint16_t PANID, bool isSecure);//true=sent ok
bool radioUnicast(addr_t dest, uint16_t PANID, uint8_t *data, uint8_t datasize, bool isSecure);//true=sent OK

//User code must implement this function; releases the received packet when it has been processed
void radioDiscardPacket(void);

//User code must call this function with incoming packet info
void radioReceivedPacket(addr_t source, uint16_t PANID, uint8_t*data,uint16_t size,uint8_t rssi, bool isBroadcast, bool isSecure);

//User's data arrived event
void NetworkDataArrivedEvent(addr_t src, uint8_t *data, uint16_t size, bool isBroadcast);
//Call this function to release data when done
void NetworkReleaseUserData(void);

//Function used to issue a discovery request;  triggers all devices in radio range to reply with
//network info
void NetworkSendDiscoverRequest(void);
//This event is called with active scan discovery has happened
//User code must implement this function.  User already knows which channel is being tested
void NetworkDiscoveredEvent(uint16_t PANID, uint16_t rssi);

//Used by sleeping RFD to issue data request (receives data stored for device by network)
void NetworkIssueDataRequest(void);

//Sends unicast packet to an address (RFD/Coord)
//Returns true if buffer space is available and all other conditions are valid
//returns false otherwise
bool NetworkUnicastData(addr_t address, uint8_t *data, uint16_t size, bool isSecure);

//Sends a broadcast packet for everyone to receive
bool NetworkBroadcastData(uint8_t *data, uint16_t size, bool isSecure);

//Returns true if rfd is member of network (i.e. network is formed and we joined)
//On Coordinator returns true always
bool IsMemberOfNetwork(void);

void TimeTickISR(void);

bool isNetworkSecurityEnabled(void);

//returns device's PANID
uint16_t NetworkGetPANID(void);
#endif