//
//  PackManager.h
//  Pegasus Feather
//
//  Created by Rohan Carty on 21/07/14.
//  Copyright (c) 2014 Dancing Dinosaur Games Pty Ltd. All rights reserved.
//

#ifndef __Pegasus_Feather__PackManager__
#define __Pegasus_Feather__PackManager__

#include <iostream>
#include <string>

// File system will search handed path for file using normal fstream, if not found will go to a pak file named the same as the first folder

// Pak file format ripped straight from quake and quake 2

struct stPakHeader
{
	char caID[4]; // Should always be "PACK"
	int iOffset; // Index to the beginning of the file table
	int iSize; // Size of the file table
};

struct stPakFileEntry
{
	char caName[56]; // Filename inside pack file
	int iOffset; // The offset from the beginning of the file to the start of the contents
	int iSize; // Size of the contents
};

class PackManager
{
public:
    PackManager();
    ~PackManager();
    
    static void* LoadResource(std::string a_svFileToLoad, int* a_out_sizeOfFile);
};

#endif /* defined(__Pegasus_Feather__PackManager__) */
