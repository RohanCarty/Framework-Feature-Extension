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

#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>

#include "PerformanceTimer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "Vector.h"
#include "Unit.h"

int main (int argc, char *argv[]) 
{
#ifdef _WIN32
#ifdef _DEBUG
	std::cout<<"Setting windows specific memory leak detection"<<std::endl;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
#endif
    std::cout<<"Main Thread Started"<<std::endl;

    srand((unsigned int)time(NULL));

    Timer* pTimer = new Timer;

	SceneManager::InitialiseSceneManager(argc, argv);

    /*if(!SceneManager::GetNetworkManager()->m_bIsClient)
    {
        for( int iDx = 0; iDx < 2; iDx++)
        {
            Vector vRandomVector(0,0,0);
            SceneManager::GetUnitManager()->GetUnitList()[SceneManager::GetUnitManager()->SpawnNewUnit(0)]->SetHardLocation(vRandomVector);
            
            //Send Destination
            stCommandPacket stTempCommand;
            stTempCommand.m_iOrder = eOrderSpawn; //Send the spawn command, will get sent when a client actually connects
            stTempCommand.m_iUnit = 0;//Unit Type?
            stTempCommand.m_iClient = 0;
            stTempCommand.m_vFirstVector = vRandomVector;
            SceneManager::GetNetworkManager()->AddCommand(&stTempCommand);
        }
    }*/
    
    std::cout<<"Startup Time: "<<pTimer->GetDT()<<std::endl;

    while(SceneManager::Update((float)pTimer->GetDT()))
    {

    }

	SceneManager::CleanupSceneManager();
	
	delete pTimer;

    std::cout<<"Exited Cleanly."<<std::endl;

    return 0;
}
