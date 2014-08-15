#include "Mesh.h"
#include "DisplayManager.h"
#include "Vertex.h"
#include "Texture.h"
#include "PackManager.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

//Texture Functions
Mesh::Mesh()
{
	m_iNumberOfVertices = 0;
	m_pakVertices = NULL;
	m_pakOriginalVertices = NULL;
	m_pafU = NULL;
	m_pafV = NULL;
}

Mesh::~Mesh()
{
	delete[] m_pakVertices;
	delete[] m_pakOriginalVertices;
	delete[] m_pafU;
	delete[] m_pafV;
}

bool Mesh::LoadMesh(std::string a_szFileName)
{
	if(m_pakVertices != NULL)
	{
		delete[] m_pakVertices;
		m_pakVertices = NULL;
	}
	if(m_pakOriginalVertices != NULL)
	{
		delete[] m_pakOriginalVertices;
		m_pakOriginalVertices = NULL;
	}
	if(m_pafU != NULL)
	{
		delete[] m_pafU;
		m_pafU = NULL;
	}
	if(m_pafV != NULL)
	{
		delete[] m_pafV;
		m_pafV = NULL;
	}
	SetElementCounts(a_szFileName);

	int iCurrentCount = 0;

	Vector vTemp;

	std::string sTempString;
	std::string sTempNodeName;

    xmlNodePtr pCurNode;//, pParentNode;
    xmlDocPtr pDoc;
    xmlChar* pXmlBuff;	

	int iSizeOfFile;

	std::string szTempString = (char*)PackManager::LoadResource(a_szFileName,&iSizeOfFile);

	pDoc = xmlReadMemory(szTempString.c_str(),iSizeOfFile,"", NULL, 0);

	//pDoc = xmlReadMemory((char*)PackManager::LoadResource(a_szFileName,iSizeOfFile),*iSizeOfFile,"Mesh", NULL, 0);

    pCurNode = xmlDocGetRootElement(pDoc);

    if(pCurNode == NULL)
    {
		std::cout<<"Mesh load error "<<a_szFileName<<std::endl;
        return false;
    }

    pCurNode = pCurNode->children;
    pCurNode = pCurNode->next;

    bool bFinished = false;

    while(bFinished == false)
    {
        if(strcmp((const char*) pCurNode->name, "text") != 0)
        {
            sTempString.clear();
			sTempNodeName.clear();

            pXmlBuff = xmlNodeGetContent(pCurNode);

            sTempString.append((const char *) pXmlBuff);
			sTempNodeName.append((const char*) pCurNode->name);

			if(sTempNodeName == "Vertex")
			{
				//Do Vertex Stuff
				//std::cout<<"Vertex Data: "<<sTempString<<std::endl;
				std::size_t sFound;

				sFound = sTempString.find(",");
				vTemp.x = atof(sTempString.substr(0, sFound).c_str());
				sTempString.erase(0, sFound + 1);
				sFound = sTempString.find(",");
				vTemp.y = atof(sTempString.substr(0, sFound ).c_str());
				sTempString.erase(0, sFound + 1);
				sFound = sTempString.at(0);
				vTemp.z = atof(sTempString.substr(0, sFound).c_str());

				//std::cout<<"Processed Vertex Data: "<<vTemp.x<<", "<<vTemp.y<<", "<<vTemp.z<<std::endl;

				m_pakVertices[iCurrentCount].SetLocation(vTemp);
				m_pakOriginalVertices[iCurrentCount].SetLocation(vTemp);
			}

			if(sTempNodeName == "UV")
			{
				//Do UV stuff
				//std::cout<<"UV Data: "<<sTempString<<std::endl;
				std::size_t sFound = sTempString.find(",");
				m_pafU[iCurrentCount] = atof(sTempString.substr(0, sFound ).c_str());
				sTempString.erase(0, sFound + 1);
				sFound = sTempString.at(0);
				m_pafV[iCurrentCount] = atof(sTempString.substr(0, sFound).c_str());

				//std::cout<<"Processed UV Data: "<<m_paiU[iCurrentCount]<<", "<<m_paiV[iCurrentCount]<<std::endl;

				iCurrentCount++;
			}
        }

        pCurNode = pCurNode->next;
        if(pCurNode == NULL)
        {
            bFinished = true;
        }
    }

	return true;
}

void Mesh::SetElementCounts(std::string a_szFileName)
{
	m_iNumberOfVertices = 0;

	std::string sTempString;
	std::string sTempNodeName;

    xmlNodePtr pCurNode;//, pParentNode;
    xmlDocPtr pDoc;
    xmlChar* pXmlBuff;
	
	int iSizeOfFile;

	std::string szTempString = (char*)PackManager::LoadResource(a_szFileName,&iSizeOfFile);

	pDoc = xmlReadMemory(szTempString.c_str(),iSizeOfFile,"", NULL, 0);
	//pDoc = xmlParseMemory((char*)PackManager::LoadResource(a_szFileName,iSizeOfFile),*iSizeOfFile);

    pCurNode = xmlDocGetRootElement(pDoc);

    if(pCurNode == NULL)
    {
		std::cout<<"Mesh load error "<<a_szFileName<<std::endl;
        return;
    }

    pCurNode = pCurNode->children;
    pCurNode = pCurNode->next;

    bool bFinished = false;

    while(bFinished == false)
    {
        if(strcmp((const char*) pCurNode->name, "text") != 0)
        {
            sTempString.clear();
			sTempNodeName.clear();

            pXmlBuff = xmlNodeGetContent(pCurNode);

            sTempString.append((const char *) pXmlBuff);
			sTempNodeName.append((const char*) pCurNode->name);

			if(sTempNodeName == "Vertex")
			{
				//Increase count
				m_iNumberOfVertices++;
			}
        }

        pCurNode = pCurNode->next;
        if(pCurNode == NULL)
        {
            bFinished = true;
        }
    }

	//Now allocate the arrays
	if(m_pakVertices != NULL)
	{
		//delete m_pakVertices;
	}
	if(m_pakOriginalVertices != NULL)
	{
		//delete m_pakOriginalVertices;
	}
	if(m_pafU != NULL)
	{
		//delete m_paiU;
	}
	if(m_pafV != NULL)
	{
		//delete m_paiV;
	}

	//std::cout<<"Number of Vertices: "<<m_iNumberOfVertices<<std::endl;

	m_pakVertices = new Vertex[m_iNumberOfVertices];
	m_pakOriginalVertices = new Vertex[m_iNumberOfVertices];
	m_pafU = new float[m_iNumberOfVertices];
	m_pafV = new float[m_iNumberOfVertices];

	return;
}

void Mesh::Update()
{
	if(m_pTexture->GetIsAnimated())
	{
		m_pafU[0] = m_pTexture->m_fMinU;
		m_pafU[1] = m_pTexture->m_fMaxU;
		m_pafU[2] = m_pTexture->m_fMaxU;
		m_pafU[3] = m_pTexture->m_fMinU;

		m_pafV[0] = m_pTexture->m_fMinV;
		m_pafV[1] = m_pTexture->m_fMinV;
		m_pafV[2] = m_pTexture->m_fMaxV;
		m_pafV[3] = m_pTexture->m_fMaxV;
	}

	for(int iDx = 0; iDx < m_iNumberOfVertices; iDx++)
	{
		m_pakVertices[iDx].SetLocation(*m_pakOriginalVertices[iDx].GetLocation());
	}
}

Vertex* Mesh::GetVertexArray()
{
	return m_pakVertices;
}

float* Mesh::GetTextureUArray()
{
	return m_pafU;
}

float* Mesh::GetTextureVArray()
{
	return m_pafV;
}

int Mesh::GetNumberOfVertices()
{
	return m_iNumberOfVertices;
}

void Mesh::SetTexture(Texture* a_pTexture)
{
	m_pTexture = a_pTexture;
}