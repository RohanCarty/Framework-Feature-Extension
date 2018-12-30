#include "Mesh.h"
#include "DisplayManager.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Texture.h"
#include "PackManager.h"

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

	Vector vTemp;
    
    int iCurrentCount = 0;

	std::string sTempString;

	//int iSizeOfFile;

    //iSizeOfFile = PackManager::GetSizeOfFile(a_szFileName);

	char* pTempResource = PackManager::LoadResource(a_szFileName);

	std::string szTempString = (char*)pTempResource;
    
	//Open file
	m_szsStringStream.str(szTempString);
    
	char acTempCstring[1024];
    
    m_szsStringStream.seekg(0);
    m_szsStringStream.clear();
    
	std::string szTempName;
	
	if(!m_szsStringStream.good())
	{
        std::cout<<"File is not good"<<std::endl;
		/*std::string szTempError;
         strerror_s(acTempCstring, errno);
         szTempError = "Error code: "; szTempError += acTempCstring;
         LogString(szTempError);*/
	}
    
    
    while(m_szsStringStream.good()) // While file is good
    {
		m_szsStringStream.getline(acTempCstring, 128);
		szTempString = acTempCstring;
        
		if(szTempString.find_first_of("<") != szTempString.npos) // if doesn't contain a ?
        {
			szTempName = szTempString.substr(szTempString.find_first_of("<") + 1, szTempString.find(">") - szTempString.find_first_of("<") - 1);
			std::string sBuff(szTempString.substr(szTempString.find_first_of(">") + 1, szTempString.find_last_of("<") - szTempString.find_first_of(">") - 1)); //Prune text out
			//std::string sBuff = "";
            //std::cout<<"Name Buffer is: " + szTempName<<std::endl;
            //std::cout<<"Data Buffer is: " + sBuff<<std::endl;
            
            sTempString.clear();
            
            if(szTempName == "Vertex")
			{
				//Do Vertex Stuff
				//std::cout<<"Vertex Data: "<<sTempString<<std::endl;
				std::size_t sFound;
                
                sTempString = sBuff;
                
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
            
			if(szTempName == "UV")
			{
				//Do UV stuff
				//std::cout<<"UV Data: "<<sTempString<<std::endl;
                sTempString = sBuff;
                
				std::size_t sFound = sTempString.find(",");
				m_pafU[iCurrentCount] = (float)atof(sTempString.substr(0, sFound ).c_str());
				sTempString.erase(0, sFound + 1);
				sFound = sTempString.at(0);
				m_pafV[iCurrentCount] = (float)atof(sTempString.substr(0, sFound).c_str());
                
				//std::cout<<"Processed UV Data: "<<m_paiU[iCurrentCount]<<", "<<m_paiV[iCurrentCount]<<std::endl;
                
				iCurrentCount++;
			}
        }
    }

    //DO NOT DELETE PACK MANAGER NOW KEEPS IT IN MEMORY
	//delete pTempResource;

	return true;
}

void Mesh::SetElementCounts(std::string a_szFileName)
{
	m_iNumberOfVertices = 0;

	std::string sTempString;
    
	//int iSizeOfFile;
    
    //iSizeOfFile = PackManager::GetSizeOfFile(a_szFileName);
    
	char* pTempResource = PackManager::LoadResource(a_szFileName);

	std::string szTempString = (char*)pTempResource;
    
	//Open file
	m_szsStringStream.str(szTempString);
    
	char acTempCstring[1024];
    
    m_szsStringStream.seekg(0, m_szsStringStream.beg);
    
	std::string szTempName;
	
	if(!m_szsStringStream.good())
	{
        std::cout<<"File is not good"<<std::endl;
		/*std::string szTempError;
         strerror_s(acTempCstring, errno);
         szTempError = "Error code: "; szTempError += acTempCstring;
         LogString(szTempError);*/
	}
    
    
    while(m_szsStringStream.good()) // While file is good
    {
		m_szsStringStream.getline(acTempCstring, 128);
		szTempString = acTempCstring;
        
		if(szTempString.find_first_of("<") != szTempString.npos) // if doesn't contain a ?
        {
			szTempName = szTempString.substr(szTempString.find_first_of("<") + 1, szTempString.find(">") - szTempString.find_first_of("<") - 1);
			std::string sBuff(szTempString.substr(szTempString.find_first_of(">") + 1, szTempString.find_last_of("<") - szTempString.find_first_of(">") - 1)); //Prune text out
			//std::string sBuff = "";
            //std::cout<<"Name Buffer is: " + szTempName<<std::endl;
            //std::cout<<"Data Buffer is: " + sBuff<<std::endl;
            
            sTempString.clear();
            
            if(szTempName == "Vertex")
			{
                m_iNumberOfVertices++;
			}
        }
    }

	//std::cout<<"Number of Vertices: "<<m_iNumberOfVertices<<std::endl;

	m_pakVertices = new Vertex[m_iNumberOfVertices];
	m_pakOriginalVertices = new Vertex[m_iNumberOfVertices];
	m_pafU = new float[m_iNumberOfVertices];
	m_pafV = new float[m_iNumberOfVertices];
    
    m_szsStringStream.seekg(0);

//DO NOT DELETE PACK MANAGER NOW KEEPS IT IN MEMORY
//	delete pTempResource;

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

void Mesh::GenerateBasicMesh(float a_fSizeX, float a_fSizeY, float a_fSizeZ)
{
	//Half the sizes in order to create a centred mesh
	a_fSizeX /= 2; a_fSizeY /= 2; a_fSizeZ /= 2;

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

	m_pakVertices = new Vertex[4];
	m_pakOriginalVertices = new Vertex[4];
	m_pafU = new float[4];
	m_pafV = new float[4];

	m_iNumberOfVertices = 4;

	Vector vTemp(-a_fSizeX, -a_fSizeY, a_fSizeZ);

	m_pakVertices[0].SetLocation(vTemp);
	m_pakOriginalVertices[0].SetLocation(vTemp);

	m_pafU[0] = 0.0f;
	m_pafV[0] = 0.0f;

	vTemp = Vector(a_fSizeX, -a_fSizeY, a_fSizeZ);

	m_pakVertices[1].SetLocation(vTemp);
	m_pakOriginalVertices[1].SetLocation(vTemp);

	m_pafU[1] = 1.0f;
	m_pafV[1] = 0.0f;

	vTemp = Vector(a_fSizeX, a_fSizeY, a_fSizeZ);

	m_pakVertices[2].SetLocation(vTemp);
	m_pakOriginalVertices[2].SetLocation(vTemp);

	m_pafU[2] = 1.0f;
	m_pafV[2] = 1.0f;

	vTemp = Vector(-a_fSizeX, a_fSizeY, a_fSizeZ);

	m_pakVertices[3].SetLocation(vTemp);
	m_pakOriginalVertices[3].SetLocation(vTemp);

	m_pafU[3] = 0.0f;
	m_pafV[3] = 1.0f;
}

void Mesh::SetTexture(Texture* a_pTexture)
{
	m_pTexture = a_pTexture;
}

void Mesh::SetMatrix(Matrix a_mMatrix)
{
	m_mObjectMatrix = a_mMatrix;
}
Matrix Mesh::GetMatrix()
{
	return m_mObjectMatrix;
}