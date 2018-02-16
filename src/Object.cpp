#include "Object.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Vector.h"
#include "Vertex.h"
#include "Mesh.h"
#include "AnimatedTexture.h"
#include "Matrix.h"

#include <fstream>

Object::Object(Scene* a_pkScene)
{
	//std::cout<<"Object created. Pointer: "<<this<<std::endl;

    m_pkScene = a_pkScene;
    m_pkParent = NULL;
    m_pkChild = NULL;
	m_uiShaderNumber = SceneManager::GetDisplayManager()->GetDefaultShader();
    m_pLocation = new Vector;
    m_pWorldLocation = new Vector;
    stRenderable stTempRenderable;
	//Get default textures and shaders
    stTempRenderable.m_pkTexture = new AnimatedTexture(SceneManager::GetDisplayManager());
	Mesh* TempMesh = new Mesh();
	TempMesh->LoadMesh("Resources/Meshes/Plane.mesh");
	TempMesh->SetTexture(stTempRenderable.m_pkTexture);
    stTempRenderable.m_pkMesh = TempMesh;
    stTempRenderable.m_bIsHidden = false;
    m_apkRenderables.push_back(stTempRenderable);
    m_fRotation = 0.0f;
    m_vSize = Vector(128.0,128.0, 128.0);
	m_fScale = 1.0f;
	m_bHidden = false;
    m_iObjectType = eObject;
}

Object::~Object()
{
    //Delete all of the renderables
    while(m_apkRenderables.size() > 0)
    {
        SceneManager::GetDisplayManager()->UnloadTexture(m_apkRenderables.back().m_pkTexture->GetTextureNumber());
        delete m_apkRenderables.back().m_pkTexture;
        delete m_apkRenderables.back().m_pkMesh;
        m_apkRenderables.pop_back();
    }

    if(m_pkParent != NULL)
    {
        m_pkParent->DetachChild();
    }

    //Store the pointer to the child to delete it in order to make sure it's destroyed;
    Object* TempObject;
    if(m_pkChild != NULL)
    {
        TempObject = m_pkChild;
        DetachChild();
        delete TempObject;
    }
	//std::cout<<"Deleting m_pLocation, Pointer: "<<this<<std::endl;
    delete m_pLocation;
	//std::cout<<"Deleting m_pWorldLocation, Pointer: "<<this<<std::endl;
    delete m_pWorldLocation;
	//std::cout<<"Object Destroyed. Pointer: "<<this<<std::endl;
}

bool Object::Update(float a_fDeltaTime)
{
    #ifdef _FULL_DEBUG_MESSAGES_
    std::cout<<"Object Tick: "<<this<<std::endl;
    #endif

    //SetLocation(256,256);

	//SetRotation(GetRotation() + a_fDeltaTime);

    if(m_pkParent != NULL)
    {
        m_pWorldLocation->x = m_pkParent->GetLocation()->x + GetLocation()->x;
        m_pWorldLocation->y = m_pkParent->GetLocation()->y + GetLocation()->y;
		m_pWorldLocation->z = m_pkParent->GetLocation()->z + GetLocation()->z;
    }
    else
    {
        m_pWorldLocation->x = GetLocation()->x;
        m_pWorldLocation->y = GetLocation()->y;
		m_pWorldLocation->z = GetLocation()->z;
    }

	if(!m_bHidden)
	{
		Draw(a_fDeltaTime);
	}

    return UpdateChild(a_fDeltaTime);
}

bool Object::Draw(float a_fDeltaTime)
{
	//GenerateVertices();

	SceneManager::GetDisplayManager()->SetShaderProgram(m_uiShaderNumber);

	for(unsigned int iDx = 0 ; iDx < m_apkRenderables.size(); iDx++)
    {
        if(m_apkRenderables[iDx].m_bIsHidden)
        {
            continue;
        }
        m_apkRenderables[iDx].m_pkMesh->Update();
        m_apkRenderables[iDx].m_pkTexture->Update(a_fDeltaTime);

        TransformMesh(m_apkRenderables[iDx].m_pkMesh);

        SceneManager::GetDisplayManager()->Draw(m_apkRenderables[iDx].m_pkMesh,4,m_apkRenderables[iDx].m_pkTexture);
    }

	return true;
}

bool Object::HUDDraw(float a_fDeltaTime)
{
	SceneManager::GetDisplayManager()->SetShaderProgram(m_uiShaderNumber);

	for(unsigned int iDx = 0 ; iDx < m_apkRenderables.size(); iDx++)
    {
        if(m_apkRenderables[iDx].m_bIsHidden)
        {
            continue;
        }
        m_apkRenderables[iDx].m_pkMesh->Update();
        m_apkRenderables[iDx].m_pkTexture->Update(a_fDeltaTime);

		for(unsigned int uiDx = 0; uiDx < 4; uiDx++)
		{
			m_apkRenderables[iDx].m_pkMesh->GetVertexArray()[uiDx].SetLocation(Vector(m_apkRenderables[iDx].m_pkMesh->GetVertexArray()[uiDx].GetLocation()->x + GetLocation()->x,m_apkRenderables[iDx].m_pkMesh->GetVertexArray()[uiDx].GetLocation()->y + GetLocation()->y, 0));
		}

		SceneManager::GetDisplayManager()->HUDDraw(m_apkRenderables[iDx].m_pkMesh->GetVertexArray(),4,m_apkRenderables[iDx].m_pkTexture);
    }

	return true;
}

bool Object::UpdateChild(float a_fDeltaTime)
{
    if(m_pkChild != NULL)
    {
        return m_pkChild->Update(a_fDeltaTime);
    }
    else
    {
        return false;
    }
}

bool Object::AttachChild(Object* a_pkObject)
{
    if(m_pkChild == NULL)
    {
        m_pkChild = a_pkObject;

        return true;
    }
    else
    {
        std::cout<<"Child Attach Failed! "<<this<<" "<<a_pkObject<<std::endl;
        return false;
    }
}

bool Object::DetachChild()
{
    if(m_pkChild != NULL)
    {
        m_pkChild->DetachFrom();
        m_pkChild = NULL;

        return true;
    }
    else
    {
        std::cout<<"Child Detach Failed! "<<this<<std::endl;
        return false;
    }
}

bool Object::AttachTo(Object* a_pkObject)
{
    if(a_pkObject != NULL)
    {
        a_pkObject->AttachChild(this);
        m_pkParent = a_pkObject;

        return true;
    }
    else
    {
        std::cout<<"Attaching to parent failed!"<<this<<" "<<a_pkObject<<std::endl;
        return false;
    }
}

bool Object::DetachFrom()
{
    if(m_pkParent != NULL)
    {
        m_pkParent = NULL;
        return true;
    }
    else
    {
        std::cout<<"Detaching from parent failed!"<<this<<" "<<m_pkParent<<std::endl;
        return false;
    }
}

void Object::SetHidden(bool a_bHide)
{
	m_bHidden = a_bHide;
}
bool Object::GetHidden()
{
	return m_bHidden;
}

Scene* Object::GetScene()
{
    return m_pkScene;
}

void Object::TransformMesh(Mesh* a_pkMesh)
{
	Matrix kTempMatrix;

	//kTempMatrix.XAxisRotation(m_fRotation);
	//kTempMatrix.YAxisRotation(m_fRotation);
	kTempMatrix.ZAxisRotation(m_fRotation);

	kTempMatrix.UniformScale(m_fScale);

    kTempMatrix.SetTranslation(*m_pWorldLocation);

	a_pkMesh->SetMatrix(kTempMatrix);

	//TODO: do if using opengl1
	kTempMatrix.TransformVertices(a_pkMesh->GetVertexArray(), a_pkMesh->GetNumberOfVertices());
}

Vector* Object::GetLocation()
{
    return m_pLocation;
}

Vector* Object::GetWorldLocation()
{
    return m_pWorldLocation;
}

void Object::SetLocation(Vector a_pNewLocation, bool a_bInstantWorldUpdate)
{
    m_pLocation->x = a_pNewLocation.x;
    m_pLocation->y = a_pNewLocation.y;
	m_pLocation->z = a_pNewLocation.z;
	if(a_bInstantWorldUpdate)
	{
		if(m_pkParent != NULL)
		{
			m_pWorldLocation->x = m_pkParent->GetLocation()->x + GetLocation()->x;
			m_pWorldLocation->y = m_pkParent->GetLocation()->y + GetLocation()->y;
			m_pWorldLocation->z = m_pkParent->GetLocation()->z + GetLocation()->z;
		}
		else
		{
			m_pWorldLocation->x = GetLocation()->x;
			m_pWorldLocation->y = GetLocation()->y;
			m_pWorldLocation->z = GetLocation()->z;
		}
	}
}

void Object::SetLocation(double a_x, double a_y, double a_z)
{
    m_pLocation->x = a_x;
    m_pLocation->y = a_y;
	m_pLocation->z = a_z;
}

std::vector<stRenderable>& Object::GetRenderables()
{
	return m_apkRenderables;
}

float Object::GetRotation()
{
	return m_fRotation;
}

void Object::SetRotation(float a_fRotation)
{
	m_fRotation = a_fRotation;
}

Vector Object::GetSize()
{
	return m_vSize;
}

void Object::SetSize(Vector a_vSize)
{
	m_vSize = a_vSize;
}

float Object::GetScale()
{
	return m_fScale;
}

void Object::SetScale(float a_fScale)
{
	m_fScale = a_fScale;
}

float Object::FloatLerp(float a_fMin, float a_fMax, float a_fUnitPercentage)
{
	float a_fTemp;

	a_fTemp = (a_fMax - a_fMin) * a_fUnitPercentage;

	a_fTemp += a_fMin;

	return a_fTemp;
}