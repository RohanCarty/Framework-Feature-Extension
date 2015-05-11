#include "TextLibrary.h"
#include "Texture.h"
#include "Vector.h"
#include "Vertex.h"
#include "Mesh.h"

#include "OpenGLDisplayManager.h"
#include "PackManager.h"

TextLibrary::TextLibrary(DisplayManager* a_kDisplayManager)
{
	std::cout<<"TextLibrary Initialized."<<std::endl;
}

TextLibrary::~TextLibrary()
{
	
}

//Prints Strings passed in through a string, takes a position and a character size.
void TextLibrary::PrintString(std::string& sString, double x, double y, unsigned int CharacterSize)
{
	
}

void TextLibrary::PrintHUDString(std::string& sString, double x, double y, unsigned int CharacterSize)
{
	
}

Vector TextLibrary::GetStringSize(std::string& sString, unsigned int CharacterSize)
{
	return Vector(0,0,0);
}
