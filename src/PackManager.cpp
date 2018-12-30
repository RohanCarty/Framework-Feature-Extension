//
//  PackManager.cpp
//  Pegasus Feather
//
//  Created by Rohan Carty on 21/07/14.
//  Copyright (c) 2014 Dancing Dinosaur Games Pty Ltd. All rights reserved.
//

#include "PackManager.h"
#include <fstream>
#include <cstring>

#include <unistd.h>

std::vector<stPakFileEntry> PackManager::m_astPakFileEntrys;
std::vector<stLoadedBlobInfo> PackManager::m_astLoadedBlobs;

PackManager::PackManager()
{

}

PackManager::~PackManager()
{
    while(m_astLoadedBlobs.size() > 0)
	{
		delete m_astLoadedBlobs.back().m_apkLoadedBlob;
		m_astLoadedBlobs.pop_back();
	}
}

int PackManager::GetSizeOfFile(std::string a_szNameOfFile)
{
    std::string a_szNameOfPackage = a_szNameOfFile.substr(0, a_szNameOfFile.find_first_of("/"));
    a_szNameOfPackage.append(".pak");

    a_szNameOfFile = a_szNameOfFile.substr(a_szNameOfFile.find_first_of("/") + 1);

    for(unsigned int iDx = 0; iDx< m_astPakFileEntrys.size(); iDx++)
    {
        if(strcmp(a_szNameOfFile.c_str(), m_astPakFileEntrys[iDx].caName) == 0 && strcmp(a_szNameOfPackage.c_str(), m_astPakFileEntrys[iDx].szNameOfPakHeader.c_str()) == 0)
        {
            return m_astPakFileEntrys[iDx].iSize;
        }
    }

    //Haven't found the file yet
    LoadPackageHeaders(a_szNameOfPackage);

    for(unsigned int iDx = 0; iDx < m_astPakFileEntrys.size(); iDx++)
    {
        if(strcmp(a_szNameOfFile.c_str(), m_astPakFileEntrys[iDx].caName) == 0 && strcmp(a_szNameOfPackage.c_str(), m_astPakFileEntrys[iDx].szNameOfPakHeader.c_str()) == 0)
        {
            return m_astPakFileEntrys[iDx].iSize;
        }
    }
    
    //TODO: File size of files that aren't inside a package to use for over-rides

    //File doesn't exist, return 0 size
    return 0;
}

char* PackManager::LoadResource(std::string a_svFileToLoad)
{
    char* pData = NULL;
    
    //Check to see if we already have a copy in memory to cut down on disk reads and maybe save on memory
    
    for(unsigned int uiDx = 0; uiDx < m_astLoadedBlobs.size(); uiDx++)
	{
		if(m_astLoadedBlobs[uiDx].m_szFileName == a_svFileToLoad)
		{
            //std::cout<<"Found previous resource: "<<a_svFileToLoad<<std::endl;
			return m_astLoadedBlobs[uiDx].m_apkLoadedBlob;
		}
	}

    std::ifstream fsFileStream(a_svFileToLoad.c_str(), std::fstream::binary | std::fstream::in);

    stLoadedBlobInfo stTemp;

    if(fsFileStream)
    {
        //Get the length of the file
        fsFileStream.seekg(0, fsFileStream.end);
        int iLength = (int)fsFileStream.tellg();
        fsFileStream.seekg(0, fsFileStream.beg);

        pData = new char[iLength]; // create the buffer of the character length of the file.

        std::cout<<"Reading "<<iLength<<" Characters for: "<<a_svFileToLoad<<"From External File"<<std::endl;

        fsFileStream.read(pData, iLength);

        fsFileStream.close();
        
        stTemp.m_apkLoadedBlob = pData;
        stTemp.m_szFileName = a_svFileToLoad;
        stTemp.uiSize = iLength;
        m_astLoadedBlobs.push_back(stTemp);

        std::cout<<"Keeping Resource in Memory: "<<a_svFileToLoad<<" at Index: "<<m_astLoadedBlobs.size()<<" Size: "<<iLength / 1024 <<"kb/"<<iLength<<" bytes"<<std::endl;

        return stTemp.m_apkLoadedBlob;
    }
	else
	{
        return LoadFromPackage(a_svFileToLoad);
    }
    
    return NULL; //something broke
}



void PackManager::LoadPackageHeaders(std::string a_szNameOfPackage)
{
    std::ifstream fsFileStream(a_szNameOfPackage.c_str(), std::fstream::binary | std::fstream::in);

	if(fsFileStream)
	{
		stPakHeader stHeader; // Instantiate a header
		fsFileStream.get(stHeader.caID, 5); //Remember that fstream.get is n - 1 characters

		if(strcmp(stHeader.caID, "PACK") != 0)
		{
			std::cout<<"File is not a Pak file: "<<stHeader.caID<<std::endl;
		}

		int* pcTempChar = new int; // For getting int's using atoi

		*pcTempChar = 0;

		fsFileStream.get((char*)pcTempChar, 5);
		stHeader.iOffset = *pcTempChar;
#ifdef _DEBUG
		std::cout<<"Got File Header Offset:"<<stHeader.iOffset<<", Position: "<<fsFileStream.tellg()<<std::endl;
#endif //_DEBUG
		fsFileStream.get((char*)pcTempChar, 5);
		stHeader.iSize = *pcTempChar;
#ifdef _DEBUG
		std::cout<<"Got File Header Size:"<<stHeader.iSize<<", Position: "<<fsFileStream.tellg()<<std::endl;
#endif //_DEBUG
		// searching for file before actual loading

		stPakFileEntry stFileEntry;

		fsFileStream.seekg(stHeader.iOffset);

        std::cout<<"Number of files in Package "<<a_szNameOfPackage<<" : "<<stHeader.iSize / 64<<std::endl;

		for(int i = 0; i < stHeader.iSize / 64; i++)
		{
#ifdef _DEBUG
			std::cout<<"Position: "<<fsFileStream.tellg()<<std::endl;
#endif //_DEBUG

			fsFileStream.get(stFileEntry.caName, 57);

			//Grab the required stuff
			fsFileStream.read((char*)pcTempChar, 4);
			stFileEntry.iOffset = *pcTempChar;
#ifdef _DEBUG
			std::cout<<"Got File Header Offset:"<<stFileEntry.iOffset<<", Position: "<<fsFileStream.tellg()<<std::endl;
#endif //_DEBUG
			fsFileStream.read((char*)pcTempChar, 4);
			stFileEntry.iSize = *pcTempChar;
#ifdef _DEBUG
			std::cout<<"Got File Header Size:"<<stFileEntry.iSize<<", Position: "<<fsFileStream.tellg()<<std::endl;
#endif //_DEBUG
            stFileEntry.szNameOfPakHeader = a_szNameOfPackage;
				
			m_astPakFileEntrys.push_back(stFileEntry);
        }
#ifdef _DEBUG
		std::cout << "Finished filling out pak file headers" << std::endl;
#endif //_DEBUG
    }
}

char* PackManager::LoadFromPackage(std::string a_szFileToLoad)
{
    char* pData = NULL;
    std::string a_szNameOfPackage = a_szFileToLoad.substr(0, a_szFileToLoad.find_first_of("/"));
    a_szNameOfPackage.append(".pak");
    std::string szFullResourcePath = a_szFileToLoad;

    a_szFileToLoad = a_szFileToLoad.substr(a_szFileToLoad.find_first_of("/") + 1);

    unsigned int m_uiPakFileEntry = -1;

    for(unsigned int iDx = 0; iDx< m_astPakFileEntrys.size(); iDx++)
    {
        if(strcmp(a_szFileToLoad.c_str(), m_astPakFileEntrys[iDx].caName) == 0)
        {
            m_uiPakFileEntry = iDx;
        }
    }

    if(m_uiPakFileEntry == (unsigned int)-1)
    {
        LoadPackageHeaders(a_szNameOfPackage);

        for(unsigned int iDx = 0; iDx< m_astPakFileEntrys.size(); iDx++)
        {
            if(strcmp(a_szFileToLoad.c_str(), m_astPakFileEntrys[iDx].caName) == 0)
            {
                m_uiPakFileEntry = iDx;
            }
        }
    }


    std::ifstream fsFileStream(m_astPakFileEntrys[m_uiPakFileEntry].szNameOfPakHeader.c_str(), std::fstream::binary | std::fstream::in);

    if(fsFileStream)
    {

        fsFileStream.seekg(m_astPakFileEntrys[m_uiPakFileEntry].iOffset);

        pData = new char[m_astPakFileEntrys[m_uiPakFileEntry].iSize]; // create the buffer of the character length of the file.

        //std::cout<<"Reading "<<m_astPakFileEntrys[m_uiPakFileEntry].iSize<<" Characters for: "<<a_szFileToLoad<<" with resource index of: "<<m_uiPakFileEntry<<std::endl;

        fsFileStream.read((char*)pData, m_astPakFileEntrys[m_uiPakFileEntry].iSize);

        fsFileStream.close();

        stLoadedBlobInfo stTemp;

        stTemp.m_apkLoadedBlob = pData;
        stTemp.uiSize = m_astPakFileEntrys[m_uiPakFileEntry].iSize;
        stTemp.m_szFileName = szFullResourcePath;
        m_astLoadedBlobs.push_back(stTemp);
        std::cout<<"Keeping Resource in Memory: "<<a_szFileToLoad<<" at Index: "<<m_astLoadedBlobs.size()<<" Size: "<<m_astPakFileEntrys[m_uiPakFileEntry].iSize / 1024 <<"kb/"<<m_astPakFileEntrys[m_uiPakFileEntry].iSize<<" bytes"<<std::endl;
        return stTemp.m_apkLoadedBlob;
	}
    
    return NULL; //Something broke
}
