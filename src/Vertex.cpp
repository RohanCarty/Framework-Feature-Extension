#include "Vertex.h"

#include <iostream>

//Vertex Functions
Vertex::Vertex()
{
    m_pLocation = new Vector;
}

Vertex::~Vertex()
{
	//std::cout<<"Deleting Vertex Location, Pointer: "<<this<<std::endl;
    delete m_pLocation;
	//std::cout<<"Deleted Vertex, Pointer: "<<this<<std::endl;
}

Vector* Vertex::GetLocation()
{
    return m_pLocation;
}

void Vertex::SetLocation(Vector a_vNewLocation)
{
	*m_pLocation = a_vNewLocation;
}