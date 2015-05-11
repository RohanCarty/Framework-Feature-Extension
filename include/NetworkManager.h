#ifndef _NETWORKMANAGER_H_
#define _NETWORKMANAGER_H_
//Dem debug things
#ifdef _WIN32
	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <malloc.h>
		#include <crtdbg.h>
		#ifndef DBG_NEW
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			#define new DBG_NEW
	   #endif
	#endif
#endif

#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include "SDL_net.h"
#endif

#include <string>
#include <vector>

#include "Vector.h"

//Pretty much all of this class was created while laying in bed with a MacBook, listening to DVDA and the south park the movie album and making dirty jokes to my partner who's currently slaving away at her graveyard shift.

//Struct that contains all the info needed for a connection
struct stConnectionInfo
{
	IPaddress m_kNetworkAddress;
    TCPsocket m_kClientSocket;
    SDLNet_SocketSet m_kSocketSet;
    int m_iGameStep;
};

//List of things a packet would need for units
//Data Type (Location, Order)
//Unit Owner,
//TODO: First order of business is to take an order from a client for a unit.
//Gamestep
//No of Orders
//Order
//  Unit
//  Client
//  Location X
//  Location Y
//  Location Z
//Send these to clients and to the server to do them there things

//Enum for orders
enum eOrderType
{
    eOrderSpawnUnit = 0,
    eOrderSpawnBuilding,
    eOrderDestroy,
    eOrderSetResources,
    eOrderSetOwnership,
    eOrderSetDestination,
    eOrderSetLocation
};

//Prototype/Draft of what a command packet might look like.
struct stCommandPacket
{
    int m_iOrder; //Will be an enum;
    int m_iUnit; //Index of unit,
    int m_iClient; //Number of Client
    Vector m_vFirstVector; //Vectors which're all used for
    Vector m_vSecondVector;//   filling out dem packets with arguments to the orders
};

class NetworkManager
{
public:
    NetworkManager();
    virtual ~NetworkManager();
    
    bool GetIsConnected();
    
    bool StartServer(unsigned int a_uiPort);
    bool ConnectToServer(std::string a_szIpAddress, unsigned int a_uiPort);

	bool Update(float a_fDeltaTime);
    
    void AddCommand(stCommandPacket* a_pkCommandPacket);
    
    void ProcessCommand(stCommandPacket* a_pkCommandPacket);
    
    void FullUpdate(stConnectionInfo);
    
    bool m_bNetworkEnabled;
    bool m_bIsClient;
private:
    std::vector<stConnectionInfo> m_astConnections;
    std::vector<stCommandPacket> m_astCommands;
	TCPsocket sd, csd; /* Socket descriptor, Client socket descriptor */
	IPaddress ip, *remoteIP;
	float buffer[2048];
    unsigned int m_iGameStep;
    unsigned int m_iLastGameStep;
    //Used to measure the last time since a network packet was received.
    float m_fNetworkDeltaTime;
};

#endif
