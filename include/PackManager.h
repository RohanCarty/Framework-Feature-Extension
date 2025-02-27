//
//  PackManager.h
//  Pegasus Feather
//
//  Created by Rohan Carty on 21/07/14.
//  Copyright (c) 2014 Dancing Dinosaur Games Pty Ltd. All rights reserved.
//

#ifndef _PACKMANAGER_H_
#define _PACKMANAGER_H_

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
#include <string>
#include <vector>

// File system will search handed path for file using normal fstream, if not found will go to a pak file named the same as the first folder

// Pak file format ripped straight from quake and quake 2

struct stPakHeader
{
	char caID[4]; // Should always be "PACK"
	int iOffset; // Index to the beginning of the file table
	int iSize; // Size of the file table
	std::string szNameOfPak; // Filename of this package.
};

struct stPakFileEntry
{
	char caName[56]; // Filename inside pack file
	int iOffset; // The offset from the beginning of the file to the start of the contents
	int iSize; // Size of the contents
	std::string szNameOfPakHeader; // Filename of the package that contains this resource.
};

struct stLoadedBlobInfo
{
	std::string m_szFileName;
	char* m_apkLoadedBlob;
    unsigned int uiSize;
};

class PackManager
{
public:
    PackManager();
    ~PackManager();

    static int GetSizeOfFile(std::string a_szNameOfFile);

    static char* LoadResource(std::string a_svFileToLoad);

    static void LoadPackageHeaders(std::string a_szNameOfPackage);

    static char* LoadFromPackage(std::string a_szFileToLoad);
private:
    static std::vector<stPakFileEntry> m_astPakFileEntrys;
    static std::vector<stLoadedBlobInfo> m_astLoadedBlobs;
};

#endif /* defined(_PACKMANAGER_H_) */
