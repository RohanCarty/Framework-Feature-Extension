#include "Camera.h"

#include "Scene.h"
#include "SceneManager.h"
#include "Vector.h"

Camera::Camera(Scene* a_pkScene) : Object(a_pkScene)
{
	m_pViewMatrix = new Matrix();
	m_pViewMatrix->CreateIdentity();

	m_fFoV = 90;
	m_fNear = -10;
	m_fFar = 10;
	m_fAspect = 1;
	m_fViewportX = SceneManager::GetDisplayManager()->GetXScreenResolution();
	m_fViewportY = SceneManager::GetDisplayManager()->GetYScreenResolution();

	SetLocation(0,0, 1);

	m_dTargetZoom = 1.0;

	m_bHidden = true;
}

Camera::~Camera()
{
	delete m_pViewMatrix;
}

bool Camera::Update(float a_fDeltaTime)
{
    Object::Update(a_fDeltaTime);

    //Move Camera
	Vector vMousePosition = *SceneManager::GetInputManager()->GetMousePosition();

	if(SceneManager::GetInputManager()->GetWindowIsInFocus())
	{
		if(vMousePosition.x < 10)
		{
			SetLocation(GetLocation()->x - 500*a_fDeltaTime, GetLocation()->y, GetLocation()->z);
		}

		if(vMousePosition.x > SceneManager::GetDisplayManager()->GetXScreenResolution() - 10)
		{
			SetLocation(GetLocation()->x + 500*a_fDeltaTime, GetLocation()->y, GetLocation()->z);
		}

		if(vMousePosition.y < 10)
		{
			SetLocation(GetLocation()->x, GetLocation()->y - 500*a_fDeltaTime, GetLocation()->z);
		}

		if(vMousePosition.y > SceneManager::GetDisplayManager()->GetYScreenResolution() - 10)
		{
			SetLocation(GetLocation()->x, GetLocation()->y + 500*a_fDeltaTime, GetLocation()->z);
		}
	}

	//Clamp zooms
	if(m_dTargetZoom < 0.5)
	{
		m_dTargetZoom = 0.5;
	}
	else if(m_dTargetZoom > 2.0)
	{
		m_dTargetZoom = 2.0;
	}

	if(GetLocation()->z > m_dTargetZoom)
	{
		double dTemp = 0.0;
		if((GetLocation()->z - m_dTargetZoom) * (a_fDeltaTime) < 0.01)
		{
			dTemp = 0.01;
		}
		else
		{
			dTemp = (GetLocation()->z - m_dTargetZoom) * (a_fDeltaTime);
		}
		//Perform the zoom
		SetLocation(GetLocation()->x, GetLocation()->y, GetLocation()->z -= dTemp);
		//Fix the position of the camera after the zoom //TODO: Get it to work perfectly
		SetLocation(GetLocation()->x - GetLocation()->x * dTemp, GetLocation()->y - GetLocation()->y * dTemp, GetLocation()->z);
		//std::cout<<"Camera is moving, currently "<<GetLocation()->z<<" target is "<<m_dTargetZoom<<std::endl;
		if(GetLocation()->z < m_dTargetZoom)
		{
			SetLocation(GetLocation()->x, GetLocation()->y, m_dTargetZoom);
		}
	}

	if(GetLocation()->z < m_dTargetZoom)
	{
		double dTemp = 0.0;
		if((GetLocation()->z - m_dTargetZoom) * (a_fDeltaTime) > -0.01)
		{
			dTemp = -0.01;
		}
		else
		{
			dTemp = (GetLocation()->z - m_dTargetZoom) * (a_fDeltaTime);
		}
		//Perform the zoom
		SetLocation(GetLocation()->x, GetLocation()->y, GetLocation()->z -= dTemp);
		//Fix the position of the camera after the zoom //TODO: Get it to work perfectly
		SetLocation(GetLocation()->x - GetLocation()->x * dTemp, GetLocation()->y - GetLocation()->y * dTemp, GetLocation()->z);
		//std::cout<<"Camera is moving, currently "<<GetLocation()->z<<" target is "<<m_dTargetZoom<<std::endl;
		if(GetLocation()->z > m_dTargetZoom)
		{
			SetLocation(GetLocation()->x, GetLocation()->y, m_dTargetZoom);
		}
	}

	if(SceneManager::GetInputManager()->GetMouseWheelDelta() != 0)
	{
		m_dTargetZoom += (float)SceneManager::GetInputManager()->GetMouseWheelDelta() / 10.0f;
		//std::cout<<"New Target Zoom: "<<m_dTargetZoom<<std::endl;
	}

	//Scale Camera
	m_fViewportX = SceneManager::GetDisplayManager()->GetXScreenResolution() * GetScale();
	m_fViewportY = SceneManager::GetDisplayManager()->GetYScreenResolution() * GetScale();


    if(SceneManager::GetInputManager()->GetIsKeyDown(SDL_SCANCODE_W))
    {
		//m_dTargetZoom = GetLocation()->z + 0.1f;
        SetLocation(GetLocation()->x, GetLocation()->y - 500*a_fDeltaTime, GetLocation()->z);
    }

    if(SceneManager::GetInputManager()->GetIsKeyDown(SDL_SCANCODE_A))
    {
        SetLocation(GetLocation()->x - 500*a_fDeltaTime, GetLocation()->y, GetLocation()->z);
    }

    if(SceneManager::GetInputManager()->GetIsKeyDown(SDL_SCANCODE_S))
    {
		//m_dTargetZoom = GetLocation()->z - 0.1f;
       SetLocation(GetLocation()->x, GetLocation()->y + 500*a_fDeltaTime, GetLocation()->z);
    }

    if(SceneManager::GetInputManager()->GetIsKeyDown(SDL_SCANCODE_D))
    {
        SetLocation(GetLocation()->x + 500*a_fDeltaTime, GetLocation()->y, GetLocation()->z);
    }

	//std::cout<<"Camera: X: "<<GetWorldLocation()->x<<" Y: "<<GetWorldLocation()->y<<" Z: "<<GetWorldLocation()->z<<std::endl;

	float top = m_fNear * tan((0.5f * m_fFoV) * (3.14159 / 180));
	float right = top * m_fAspect;

	//m_pViewMatrix->SetPersepective(90, 1, -1000, 1000);

	m_pViewMatrix->SetFrustrum(-right, right, -top, top, m_fNear, m_fFar);

	m_pViewMatrix->SetTranslation(*GetWorldLocation());

	//Update OpenGL Camera Matrix
	SceneManager::GetDisplayManager()->SetViewMatrix(m_pViewMatrix);

	return true;
}

float Camera::GetViewportX()
{
	return m_fViewportX;
}

float Camera::GetViewportY()
{
	return m_fViewportY;
}
