#include "ObjectManager.h"

//Make sure to declare the array

std::vector<Object*> ObjectManager::m_apkObjectPassports;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

int ObjectManager::GetPassport(Object* a_pkObject)
{
	m_apkObjectPassports.push_back(a_pkObject);

	return (int)m_apkObjectPassports.size() - 1;
}

Object* ObjectManager::GetObjectID(int a_iObjectPassport)
{
	return m_apkObjectPassports[a_iObjectPassport];
}