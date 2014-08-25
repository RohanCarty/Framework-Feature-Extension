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

std::vector<stPakFileEntry> PackManager::m_astPakFileEntrys;

PackManager::PackManager()
{

}

PackManager::~PackManager()
{

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

    //File doesn't exist, return 0 size
    return 0;
}

void* PackManager::LoadResource(std::string a_svFileToLoad)
{
    char* pData = NULL;

    std::ifstream fsFileStream(a_svFileToLoad.c_str(), std::fstream::binary | std::fstream::in);

    if(fsFileStream)
    {
        //Get the length of the file
        fsFileStream.seekg(0, fsFileStream.end);
        int length = (int)fsFileStream.tellg();
        fsFileStream.seekg(0, fsFileStream.beg);

        pData = new char[length]; // create the buffer of the character length of the file.

        //std::cout<<"Reading "<<length<<" Characters for: "<<a_svFileToLoad<<std::endl;

        fsFileStream.read(pData, length);

        fsFileStream.close();
    }
	else
	{
        return LoadFromPackage(a_svFileToLoad);
    }
    return pData;
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

		//std::cout<<"Got File Header Offset:"<<stHeader.iOffset<<", Position: "<<fsFileStream.tellg()<<std::endl;

		fsFileStream.get((char*)pcTempChar, 5);
		stHeader.iSize = *pcTempChar;

		//std::cout<<"Got File Header Size:"<<stHeader.iSize<<", Position: "<<fsFileStream.tellg()<<std::endl;

		// searching for file before actual loading

		stPakFileEntry stFileEntry;

		fsFileStream.seekg(stHeader.iOffset);

        std::cout<<"Number of files in Package "<<a_szNameOfPackage<<" : "<<stHeader.iSize / 64<<std::endl;

		for(int i = 0; i < stHeader.iSize / 64; i++)
		{
			//std::cout<<"Position: "<<fsFileStream.tellg()<<std::endl;

			fsFileStream.get(stFileEntry.caName, 57);

			//Grab the required stuff
			fsFileStream.read((char*)pcTempChar, 4);
			stFileEntry.iOffset = *pcTempChar;

			//std::cout<<"Got File Header Offset:"<<stFileEntry.iOffset<<", Position: "<<fsFileStream.tellg()<<std::endl;

			fsFileStream.read((char*)pcTempChar, 4);
			stFileEntry.iSize = *pcTempChar;

			//std::cout<<"Got File Header Size:"<<stFileEntry.iSize<<", Position: "<<fsFileStream.tellg()<<std::endl;

            stFileEntry.szNameOfPakHeader = a_szNameOfPackage;

			m_astPakFileEntrys.push_back(stFileEntry);
        }

        delete pcTempChar;
    }
}

void* PackManager::LoadFromPackage(std::string a_szFileToLoad)
{
    void* pData = NULL;
    std::string a_szNameOfPackage = a_szFileToLoad.substr(0, a_szFileToLoad.find_first_of("/"));
    a_szNameOfPackage.append(".pak");

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

        //std::cout<<"Reading "<<stFileEntry.iSize<<" Characters for: "<<a_svFileToLoad<<std::endl;

        fsFileStream.read((char*)pData, m_astPakFileEntrys[m_uiPakFileEntry].iSize);


        fsFileStream.close();
	}

	return pData;
}
