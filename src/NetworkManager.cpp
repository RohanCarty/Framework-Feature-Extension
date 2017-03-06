#include "NetworkManager.h"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "Building.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "Unit.h"
#include "UnitManager.h"

NetworkManager::NetworkManager()
{
	if (SDLNet_Init() == -1) 
	{
		std::cout << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
	}

	m_bNetworkEnabled = true;
	m_bIsConnected = false;
	m_bIsClient = false;

	localIP.host = NULL;
	localIP.port = 2120;

	remoteIP.host = NULL;
	remoteIP.port = 2120;

	m_kUDPSocket = NULL;
	m_pkLocalPacket = SDLNet_AllocPacket(1024);
	m_pkRemotePacket = SDLNet_AllocPacket(1024);
}

NetworkManager::~NetworkManager()
{
	if (!m_kUDPSocket)
	{
		SDLNet_UDP_Close(m_kUDPSocket);
		m_kUDPSocket = NULL;
	}

	SDLNet_FreePacket(m_pkLocalPacket);
	SDLNet_FreePacket(m_pkRemotePacket);

	SDLNet_Quit();
}

bool NetworkManager::GetIsConnected()
{
	return m_bIsConnected;
}


//Attempt to start a server on the specified port, returns true if succesful
bool NetworkManager::StartServer(unsigned int a_uiPort)
{
	m_uiPort = a_uiPort;

	std::cout << "Starting Server on port: " << a_uiPort << std::endl;

	m_kUDPSocket = SDLNet_UDP_Open(m_uiPort);
	
	if (!m_kUDPSocket)
	{
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
	}

	m_bIsConnected = false;
	m_bIsClient = false;

	return true;
}

//TODO
bool NetworkManager::ConnectToServer(std::string a_szIpAddress, unsigned int a_uiPort)
{
	m_uiPort = a_uiPort;

	m_kUDPSocket = SDLNet_UDP_Open(0);
	
	if (!m_kUDPSocket)
	{
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
	}

	SDLNet_ResolveHost(&remoteIP, a_szIpAddress.c_str(), a_uiPort);

	m_bIsConnected = true;
	m_bIsClient = true;

	SDLNet_UDP_Bind(m_kUDPSocket, 1, &remoteIP);
	printf("SDLNet_UDP_Bind: %s\n", SDLNet_GetError());

	return true;
}

bool NetworkManager::Update(float a_fDeltaTime)
{
    //Return true if network isn't enabled
    if(!m_bNetworkEnabled)
    {
        return true;
    }

	if (m_kUDPSocket != NULL)
	{
		if (m_bIsClient == true)
		{
			std::cout << "Sending Packet to: " << ConvertIPaddressToString(remoteIP) << std::endl;
			std::string szTemp = std::to_string(a_fDeltaTime);
			for (int iDx = 0; iDx < szTemp.size(); iDx++)
			{
				m_pkLocalPacket->data[iDx] = (Uint8)szTemp.c_str()[iDx];
			}

			m_pkLocalPacket->len = szTemp.size();
			SDLNet_UDP_Send(m_kUDPSocket, 1, m_pkLocalPacket);
		}
		else
		{
			if (SDLNet_UDP_Recv(m_kUDPSocket, m_pkRemotePacket))
			{
				m_bIsConnected = true;

				std::cout << "Packet Data: ";
				for (int iDx = 0; iDx < m_pkRemotePacket->len; iDx++)
				{
					std::cout<< m_pkRemotePacket->data[iDx];
				}
				std::cout << std::endl;
			}
		}
	}
    
	return true;
}

void NetworkManager::AddCommand(stCommandPacket* a_pkCommandPacket)
{
    
}

void NetworkManager::ProcessCommand(stCommandPacket* a_pkCommandPacket)
{
    
}

void NetworkManager::FullUpdate(stConnectionInfo a_stConnectionInfo)
{

}

IPaddress NetworkManager::GetLocalIPAddress()
{
	return localIP;
}

std::string NetworkManager::GetLocalIPAddressString()
{
	std::string szTemp;

	char cpString[100];

	unsigned char bytes[4];
	bytes[0] = localIP.host & 0xFF;
	bytes[1] = (localIP.host >> 8) & 0xFF;
	bytes[2] = (localIP.host >> 16) & 0xFF;
	bytes[3] = (localIP.host >> 24) & 0xFF;

	snprintf(cpString, 100, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);

	return szTemp;
}

IPaddress NetworkManager::GetRemoteIPAddress()
{
	return remoteIP;
}

std::string NetworkManager::GetRemoteIPAddressString()
{
	std::string szTemp;

	char cpString[100];

	unsigned char bytes[4];
	bytes[0] = remoteIP.host & 0xFF;
	bytes[1] = (remoteIP.host >> 8) & 0xFF;
	bytes[2] = (remoteIP.host >> 16) & 0xFF;
	bytes[3] = (remoteIP.host >> 24) & 0xFF;

	snprintf(cpString, 100, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);

	szTemp = cpString;

	return szTemp;
}

IPaddress NetworkManager::ConvertStringToIPaddress(std::string a_szString)
{

}

std::string NetworkManager::ConvertIPaddressToString(IPaddress a_pkIPAddress)
{
	std::string szTemp;

	char cpString[100];

	unsigned char bytes[4];
	bytes[0] = a_pkIPAddress.host & 0xFF;
	bytes[1] = (a_pkIPAddress.host >> 8) & 0xFF;
	bytes[2] = (a_pkIPAddress.host >> 16) & 0xFF;
	bytes[3] = (a_pkIPAddress.host >> 24) & 0xFF;

	snprintf(cpString, 100, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);

	szTemp = cpString;

	return szTemp;
}