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
    m_bNetworkEnabled = false;
    m_bIsClient = false;
    
	if (SDLNet_Init() < 0)
	{
		std::cout<<"SDLNet_Init: "<< SDLNet_GetError()<<std::endl;
	}
    
    m_fNetworkDeltaTime = 0.0f;
    
    m_iGameStep = 0;
    m_iLastGameStep = m_iGameStep;
    
    std::cout<<"Game Step: "<<m_iGameStep<<std::endl;
    
	csd = NULL;
}

NetworkManager::~NetworkManager()
{
    //TODO: Finish handling the cleanup properly.
    //TODO: Call disconnects
    //TODO: Unallocate parts
    if(m_bNetworkEnabled)
    {
        SDLNet_TCP_Close(sd);
    }
	SDLNet_Quit();
}

bool NetworkManager::GetIsConnected()
{
    return m_bNetworkEnabled;
}


//Attempt to start a server on the specified port, returns true if succesful
bool NetworkManager::StartServer(unsigned int a_uiPort)
{
    /* Resolving the host using NULL make network interface to listen */
	if (SDLNet_ResolveHost(&ip, NULL, a_uiPort) < 0)
	{
		std::cout<<"SDLNet_ResolveHost: "<< SDLNet_GetError()<<std::endl;
        return false;
	}
    
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		std::cout<<"SDLNet_TCP_Open: "<< SDLNet_GetError()<<std::endl;
        return false;
	}
    
   
    
    m_bNetworkEnabled = true;
    m_bIsClient = false;
    
    return true;
}

//TODO
bool NetworkManager::ConnectToServer(std::string a_szIpAddress, unsigned int a_uiPort)
{
    /* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&ip, a_szIpAddress.c_str(), a_uiPort) < 0)
	{
        std::cout<<"SDLNet_ResolveHost: "<<SDLNet_GetError()<<std::endl;
        return false;
	}
    
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
        std::cout<<"SDLNet_TCP_Open: "<<SDLNet_GetError()<<std::endl;
        return false;
	}
    
    stConnectionInfo stTempInfo;
    stTempInfo.m_kSocketSet = SDLNet_AllocSocketSet(1);
    stTempInfo.m_kClientSocket = sd;
    
    SDLNet_TCP_AddSocket(stTempInfo.m_kSocketSet, stTempInfo.m_kClientSocket);
    /* Get the remote address */
    if ((remoteIP = SDLNet_TCP_GetPeerAddress(stTempInfo.m_kClientSocket)))
    {
        /* Print the address, converting in the host format */
        //Need to work on spliting it to it's octets
        printf("Connected To Host: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
        stTempInfo.m_kNetworkAddress = *remoteIP;
    }
    else
    {
        fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
        return false;
    }
    
    //Remember: TCP Connections are automatically reciprocated
    buffer[0] = m_iGameStep;
    
    //Add the connection info to the vector
    m_astConnections.push_back(stTempInfo);
    
    m_bNetworkEnabled = true;
    m_bIsClient = true;
    return true;
}

bool NetworkManager::Update(float a_fDeltaTime)
{
    //Return true if network isn't enabled
    if(!m_bNetworkEnabled)
    {
        return true;
    }
    
    //Loop through all connections we have logged in the vector
    for(unsigned int iDx = 0; iDx < m_astConnections.size(); iDx++)
    {
        //TODO: Change all this shit back to TCP, slow is better than dealing with this shit.
        
        //TODO: Make it so this works with multiple clients.
        
        //TODO: Send step packets when nothing else to send.
        
        
        buffer[0] = m_iGameStep;
        //buffer[1] = m_astCommands.size();
        buffer[1] = 0;
        
        if(m_astCommands.size() > 0)
        {
            std::cout<<"Commands in queue to be sent."<<std::endl;
            buffer[1] = 1;
            buffer[2] = m_astCommands[0].m_iOrder;
            buffer[3] = m_astCommands[0].m_iUnit;
            buffer[4] = m_astCommands[0].m_vFirstVector.x;
            buffer[5] = m_astCommands[0].m_vFirstVector.y;
            buffer[6] = m_astCommands[0].m_vFirstVector.z;
            buffer[7] = m_astCommands[0].m_vSecondVector.x;
            buffer[8] = m_astCommands[0].m_vSecondVector.y;
            buffer[9] = m_astCommands[0].m_vSecondVector.z;
        }

        //Do a for loop adding the commands to the packet (limit only the first 10 commands to be added to the packet due to assumption of aggressive MTU sizes)
        
        //iDx is for number of commands, iDy is for offset in buffer
        /*unsigned int iDy = 2;
        for(unsigned int iDx = 0; iDx < m_astCommands.size(); iDx++)
        {
            buffer[iDy] = m_astCommands[iDx].m_iOrder;
            buffer[iDy + 1] = m_astCommands[iDx].m_iUnit;
            buffer[iDy + 2] = m_astCommands[iDx].m_vFirstVector.x;
            buffer[iDy + 3] = m_astCommands[iDx].m_vFirstVector.y;
            buffer[iDy + 4] = m_astCommands[iDx].m_vFirstVector.z;
            buffer[iDy + 5] = m_astCommands[iDx].m_vSecondVector.x;
            buffer[iDy + 6] = m_astCommands[iDx].m_vSecondVector.y;
            buffer[iDy + 7] = m_astCommands[iDx].m_vSecondVector.z;
            iDy += 8;
        }*/
        
        
        //TODO: reliable lock-step system for transmitting commands, e.g. server sends step number, clients reply with orders relevent to that step or just an acknowledgement, game step increses one.
        
        //Clear out commands once done
        if(m_astCommands.size() * sizeof(float) * 8 > 10) //Waiting for next Round of packets
        {
            std::cout<<"Multiple packets waiting"<<std::endl;
        }
        if(m_astCommands.size() > 0)
        {
            m_astCommands.erase(m_astCommands.begin());
        }
        //Do it after you've determined the length based on size, you idiot
        //m_astCommands.erase(m_astCommands.begin(), m_astCommands.end());
        
        if(SDLNet_TCP_Send(m_astConnections[iDx].m_kClientSocket, buffer,10) < 10)
        {
            std::cout<<"SDLNet_TCP_Send "<<SDLNet_GetError()<<std::endl;
        }

        m_fNetworkDeltaTime += a_fDeltaTime;
        
        //If there is any activity on the socket (returns -1 for no sockets, 0 for no data
        //TODO: need to handle -1 as it'll be an empty connection entry, just sucking resources
        if(SDLNet_CheckSockets(m_astConnections[iDx].m_kSocketSet, 0) > 0)
        {
            //When the bound socket has data to be received, pull some data out of the socket
            if (SDLNet_TCP_Recv(m_astConnections[iDx].m_kClientSocket, buffer, 10) > 0)
            {
                if(m_bIsClient)
                {
                    m_iGameStep = buffer[0];
                    
                    int m_iNumberOfOrders = (int)buffer[1];
                    
                    if(m_iNumberOfOrders > 0)
                    {
                        std::cout<<"Client Received Orders: "<<m_iNumberOfOrders<<std::endl;
                        
                        //for(int iDx = 2; (iDx < m_iNumberOfOrders * 8) + 2; iDx += 8)
                        //{
                            stCommandPacket stTempCommand;
                            stTempCommand.m_iOrder = buffer[2];
                            
                            stTempCommand.m_iUnit = buffer[3];
                            
                            stTempCommand.m_vFirstVector = Vector(buffer[4], buffer[ 5], buffer[6]);
                            stTempCommand.m_vSecondVector = Vector(buffer[7], buffer[ 8], buffer[9]);
                            
                            ProcessCommand(&stTempCommand);
                        //}
                    }
                }
                //Terminate the connection if the gamestep -1 got received
                if(buffer[0] == -1)	/* Terminate this connection */
                {
                    printf("Terminate connection\n");
                    // delete the socket from the socket set
                    SDLNet_TCP_DelSocket(m_astConnections[iDx].m_kSocketSet, m_astConnections[iDx].m_kClientSocket);
                    /* Close the client socket */
                    SDLNet_TCP_Close(m_astConnections[iDx].m_kClientSocket);
                    //do the same for the server socket if it exists
                    //Free the socket set
                    SDLNet_FreeSocketSet(m_astConnections[iDx].m_kSocketSet);
                    //Erase the current connection info
                    m_astConnections.erase(m_astConnections.begin() + iDx);
                    //Rewind the counter for next round
                    iDx--;
                }
            }
        }
    }

	if(csd == NULL && !m_bIsClient)	// If a server, wait for a connection
	{
		if ((csd = SDLNet_TCP_Accept(sd)))
		{
            //Client has tried to connect to this server, start setting up the full connection
            stConnectionInfo stTempInfo;
            stTempInfo.m_kSocketSet = SDLNet_AllocSocketSet(1);
            stTempInfo.m_kClientSocket = csd;
			SDLNet_TCP_AddSocket(stTempInfo.m_kSocketSet, stTempInfo.m_kClientSocket);
			/* Get the remote address */
			if ((remoteIP = SDLNet_TCP_GetPeerAddress(stTempInfo.m_kClientSocket)))
				/* Print the address, converting in the host format */
                //Need to work on spliting it to it's octets
				printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
			else
				fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
            
            //Remember: TCP Connections are automatically reciprocated
            
            stTempInfo.m_kNetworkAddress = *remoteIP;
            
		std::cout<<stTempInfo.m_kNetworkAddress.host<<":"<<stTempInfo.m_kNetworkAddress.port<<std::endl;
			
            //Add the connection info to the vector
            m_astConnections.push_back(stTempInfo);
            
            //Send all current info on game to client (sync to current spot)
            NetworkManager::FullUpdate(m_astConnections[m_astConnections.size() - 1]);
            
            //Clear Client Socket Descriptor to allow a new connection to be created.
            csd = NULL;
		}
	}

	return true;
}

void NetworkManager::AddCommand(stCommandPacket* a_pkCommandPacket)
{
    if(m_astConnections.size() == 0 || m_bIsClient) //if client or no connection, no orders to be sent (clients are just spectators at the moment)
    {
        return; //if there's no connection ignore the packet
    }
    
    stCommandPacket stTempCommand;
    stTempCommand.m_iOrder = a_pkCommandPacket->m_iOrder;
    stTempCommand.m_iUnit = a_pkCommandPacket->m_iUnit;
    stTempCommand.m_iClient = a_pkCommandPacket->m_iClient;
    stTempCommand.m_vFirstVector = a_pkCommandPacket->m_vFirstVector;
    stTempCommand.m_vSecondVector = a_pkCommandPacket->m_vSecondVector;
    
    std::cout<<"Command added to queue: "<<stTempCommand.m_iOrder<<std::endl;
    
    m_astCommands.push_back(stTempCommand);
}

void NetworkManager::ProcessCommand(stCommandPacket* a_pkCommandPacket)
{
    switch(a_pkCommandPacket->m_iOrder)
    {
        int iTempUnitIndex;
        case eOrderSpawnUnit:
            /*iTempUnitIndex = SceneManager::GetUnitManager()->SpawnNewUnit(a_pkCommandPacket->m_iUnit);
            SceneManager::GetUnitManager()->GetUnitList().back()->SetHardLocation(a_pkCommandPacket->m_vFirstVector, false);*/
        case eOrderSpawnBuilding:
            iTempUnitIndex = SceneManager::GetUnitManager()->SpawnNewBuilding(a_pkCommandPacket->m_vFirstVector, a_pkCommandPacket->m_vSecondVector.x);
        case eOrderDestroy:
            break;
        case eOrderSetResources:
            break;
        case eOrderSetOwnership:
            break;
        case eOrderSetDestination:
            SceneManager::GetUnitManager()->GetUnitList()[a_pkCommandPacket->m_iUnit]->SetDestination(a_pkCommandPacket->m_vFirstVector, false);
            break;
        case eOrderSetLocation:
            SceneManager::GetUnitManager()->GetUnitList()[a_pkCommandPacket->m_iUnit]->SetLocation(a_pkCommandPacket->m_vFirstVector, false);
            break;
        default:
            std::cout<<"Network Manager Process Command Default"<<std::endl;
    }
}

void NetworkManager::FullUpdate(stConnectionInfo a_stConnectionInfo)
{
    for(unsigned int iDx = 0; iDx < SceneManager::GetUnitManager()->GetBuildingList().size(); iDx++)
    {
        stCommandPacket stTempCommand;
        
        //Send Current Location;
        stTempCommand.m_iOrder = eOrderSpawnBuilding;
        stTempCommand.m_iUnit = iDx;
        stTempCommand.m_iClient = 0;
        stTempCommand.m_vFirstVector = *SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetLocation();
        stTempCommand.m_vSecondVector = *SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetLocation(); // Fill the entire command fuckhead
        stTempCommand.m_vSecondVector.x = SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetBuildingType();
        AddCommand(&stTempCommand);
    }
    
    for(unsigned int iDx = 0; iDx < SceneManager::GetUnitManager()->GetUnitList().size(); iDx++)
    {
        stCommandPacket stTempCommand;
        
        //Send Current Location;
        stTempCommand.m_iOrder = eOrderSpawnUnit;
        stTempCommand.m_iUnit = iDx;
        stTempCommand.m_iClient = 0;
        stTempCommand.m_vFirstVector = *SceneManager::GetUnitManager()->GetUnitList()[iDx]->GetLocation();
        stTempCommand.m_vSecondVector = *SceneManager::GetUnitManager()->GetUnitList()[iDx]->GetLocation(); // Fill the entire command fuckhead
        AddCommand(&stTempCommand);
    }
}