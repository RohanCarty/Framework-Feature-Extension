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

}

NetworkManager::~NetworkManager()
{

}

bool NetworkManager::GetIsConnected()
{
	return false;
}


//Attempt to start a server on the specified port, returns true if succesful
bool NetworkManager::StartServer(unsigned int a_uiPort)
{
	return true;
}

//TODO
bool NetworkManager::ConnectToServer(std::string a_szIpAddress, unsigned int a_uiPort)
{
	return true;
}

bool NetworkManager::Update(float a_fDeltaTime)
{
    //Return true if network isn't enabled
    if(!m_bNetworkEnabled)
    {
        return true;
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