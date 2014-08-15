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

PackManager::PackManager()
{
    
}

PackManager::~PackManager()
{
    
}

void* PackManager::LoadResource(std::string a_svFileToLoad, int *a_out_sizeOfFile)
{
    char* pData = NULL;
    
    std::ifstream fsFileStream(a_svFileToLoad.c_str(), std::fstream::binary | std::fstream::in);

	//*a_out_sizeOfFile = 0;
    
    if(fsFileStream)
    {
        //Get the length of the file
        fsFileStream.seekg(0, fsFileStream.end);
        int length = (int)fsFileStream.tellg();
        fsFileStream.seekg(0, fsFileStream.beg);
        
        pData = new char[length]; // create the buffer of the character length of the file.
        
        //std::cout<<"Reading "<<length<<" Characters for: "<<a_svFileToLoad<<std::endl;
        
        fsFileStream.read(pData, length);
        
        *a_out_sizeOfFile = length;
        
        fsFileStream.close();
    }
	else
	{
		std::string svPackageName;
		svPackageName = a_svFileToLoad.substr(0, a_svFileToLoad.find_first_of("/"));
		svPackageName.append(".pak");

		//Remove package name from file to load
		a_svFileToLoad = a_svFileToLoad.substr(a_svFileToLoad.find_first_of("/") + 1);

		//std::cout<<"File not found, looking in Pack: "<<svPackageName<<" for "<<a_svFileToLoad<<std::endl;

		std::ifstream fsFileStream(svPackageName.c_str(), std::fstream::binary | std::fstream::in);

		if(fsFileStream)
		{
			stPakHeader stHeader; // Instantiate a header
			fsFileStream.get(stHeader.caID, 5); //Remember that fstream.get is n - 1 characters

			if(strcmp(stHeader.caID, "PACK") != 0)
			{
				std::cout<<"File is not a Pak file: "<<stHeader.caID<<std::endl;

				*a_out_sizeOfFile = 0;

				return pData;
			}

			//std::cout<<stHeader.caID<<std::endl;

			//std::cout<<"File is type Pak, Position: "<<fsFileStream.tellg()<<std::endl;

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

			for(int i = 0; i < stHeader.iSize / 64; i++)
			{
				//std::cout<<"Position: "<<fsFileStream.tellg()<<std::endl;

				fsFileStream.get(stFileEntry.caName, 57);

				if(strcmp(a_svFileToLoad.c_str(), stFileEntry.caName) != 0)
				{
					//std::cout<<"Didn't find File, it is: "<<stFileEntry.caName<<std::endl;
					fsFileStream.seekg(fsFileStream.tellg() += 8); //Need to keep moving the file forward
					continue;
				}

				//Grab the required stuff
				fsFileStream.read((char*)pcTempChar, 4);
				stFileEntry.iOffset = *pcTempChar;

				//std::cout<<"Got File Header Offset:"<<stFileEntry.iOffset<<", Position: "<<fsFileStream.tellg()<<std::endl;

				fsFileStream.read((char*)pcTempChar, 4);
				stFileEntry.iSize = *pcTempChar;

				//std::cout<<"Got File Header Size:"<<stFileEntry.iSize<<", Position: "<<fsFileStream.tellg()<<std::endl;

				fsFileStream.seekg(stFileEntry.iOffset);

				pData = new char[stFileEntry.iSize]; // create the buffer of the character length of the file.
        
				std::cout<<"Reading "<<stFileEntry.iSize<<" Characters for: "<<a_svFileToLoad<<std::endl;
        
				fsFileStream.read(pData, stFileEntry.iSize);
        
				*a_out_sizeOfFile = stFileEntry.iSize;

				break;
        
			}

			if(*a_out_sizeOfFile == 0)
			{
				std::cout<<"File not found in pak file"<<std::endl;
			}

			//delete pcTempChar; // Clean that shit up

			fsFileStream.close();

			return pData;
		}
		else
		{
			std::cout<<"Package "<<svPackageName<<" Not Found."<<std::endl;

			*a_out_sizeOfFile = 0;

			return pData;
		}		
	}
    
    return pData;
}