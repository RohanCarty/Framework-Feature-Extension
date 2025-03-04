#include "Cursor.h"

#include "Camera.h"
#include "Scene.h"
#include "SceneManager.h"
#include "AnimatedTexture.h"
#include "InputManager.h"
#include "Object.h"
#include "Actor.h"
#include "Unit.h"
#include "UnitManager.h"
#include "GameScene.h"
#include "HUD.h"
#include "TextLibrary.h"
#include "Mesh.h"
#include "Tile.h"

Cursor::Cursor(Scene* a_pkScene) : Object(a_pkScene)
{
	m_vSize = Vector(32.0, 32.0, 1.0);
	SetScale(0.5f);
	m_apkRenderables[0].m_pkTexture->LoadTexture("Resources/Textures/cccursor.png", SceneManager::GetDisplayManager());
}

Cursor::~Cursor()
{

}

bool Cursor::Update(float a_fDeltaTime)
{
#ifndef __UWP__ //don't want to draw a mouse cursor when we don't need to, add in a thing that unhides it if it moves
	Object::Update(a_fDeltaTime);
	Cursor::Draw(a_fDeltaTime);
#endif

	/*SetLocation(SceneManager::GetInputManager()->GetMousePosition()->x - (m_pkCamera->GetViewportX() / 2) + m_pkCamera->GetWorldLocation()->x,
		SceneManager::GetInputManager()->GetMousePosition()->y - (m_pkCamera->GetViewportY() / 2) + m_pkCamera->GetWorldLocation()->y,
		0.0);*/

    //Places a building foundation for a house //TODO: Allow multiple types to be placed, only allow placement when a building is chosen in the U.I.
	/*if(SceneManager::GetInputManager()->GetIsKeyDown(SDL_BUTTON_RIGHT))
	{
		eBuildingType eTemp = eHouse;
		SceneManager::GetUnitManager()->SpawnNewBuilding(SceneManager::GetInputManager()->GetMouseWorldPosition(), eTemp);
	}*/

    // Highlighting of locations when placing a building //TODO: Get Building type and size for it, only ighlight anything when a building is chosen to be place in the U.I.
	/*for(unsigned int iDx = 0; iDx < SceneManager::GetTileManager()->GetTileList().size(); iDx++)
	{
        SceneManager::GetTileManager()->GetTileList()[iDx]->SetNoZone();

        //TODO: Remove hard coding
        //Hard coded check for tiles to the left and right of the current one.
        Vector* vSize = new Vector(3,1,0);
        
		if(SceneManager::GetInputManager()->GetMouseWorldPosition().WithinBox(*SceneManager::GetTileManager()->GetTileList()[iDx]->GetWorldLocation(), SceneManager::GetTileManager()->GetTileList()[iDx]->GetSize()))
		{
            SceneManager::GetTileManager()->SetHighlightsForBuildingFootprint(SceneManager::GetTileManager()->GetTileList()[iDx]->GetCoordinate(), *vSize);
		}
        
        delete vSize;
	}*/

	/*if(IsOverObject())
	{
        //Give a temp string to avoid non-const errors in LLVM compiliers
        std::string szTempString;
		if(m_bIsObjectHoveredOverABuilding)
		{
			//Print Name
            szTempString = ((Building*)m_pkObjectHoveredOver)->GetBuildingTypeString();
			((GameScene*)SceneManager::GetCurrentScene())->m_pkHUD->GetTextLibrary()->PrintHUDString(szTempString, SceneManager::GetInputManager()->GetMousePosition()->x + 30, SceneManager::GetInputManager()->GetMousePosition()->y, 36);

			//Print Resources
			std::stringstream szTempStringStream;
			szTempStringStream<<"Wood: "<<((Building*)m_pkObjectHoveredOver)->m_stCurrentResources.iWood<<"/"<<((Building*)m_pkObjectHoveredOver)->m_stMaxResources.iWood;
            szTempString = szTempStringStream.str();
			((GameScene*)SceneManager::GetCurrentScene())->m_pkHUD->GetTextLibrary()->PrintHUDString(szTempString, SceneManager::GetInputManager()->GetMousePosition()->x + 30, SceneManager::GetInputManager()->GetMousePosition()->y + 36, 36);
		}
		else //Is unit
		{
			//Print Name
            szTempString = ((Unit*)m_pkObjectHoveredOver)->GetName();
			((GameScene*)SceneManager::GetCurrentScene())->m_pkHUD->GetTextLibrary()->PrintHUDString(szTempString, SceneManager::GetInputManager()->GetMousePosition()->x + 30, SceneManager::GetInputManager()->GetMousePosition()->y, 36);
			//Print Type
            szTempString = ((Unit*)m_pkObjectHoveredOver)->GetUnitTypeString();
			((GameScene*)SceneManager::GetCurrentScene())->m_pkHUD->GetTextLibrary()->PrintHUDString(szTempString, SceneManager::GetInputManager()->GetMousePosition()->x + 30, SceneManager::GetInputManager()->GetMousePosition()->y + 36, 36);
			//Print Task																																																													// make sure to add the text size to the Y
            szTempString = ((Unit*)m_pkObjectHoveredOver)->GetCurrentTaskString();
			((GameScene*)SceneManager::GetCurrentScene())->m_pkHUD->GetTextLibrary()->PrintHUDString(szTempString, SceneManager::GetInputManager()->GetMousePosition()->x + 30, SceneManager::GetInputManager()->GetMousePosition()->y + 72, 36);
			//Print Resources
			if(((Unit*)m_pkObjectHoveredOver)->HasResources())
			{
				std::stringstream szTempStringStream;
				szTempStringStream <<"Wood: "<<((Unit*)m_pkObjectHoveredOver)->GetCurrentResources().iWood;
                szTempString = szTempStringStream.str();
				((GameScene*)SceneManager::GetCurrentScene())->m_pkHUD->GetTextLibrary()->PrintHUDString(szTempString, SceneManager::GetInputManager()->GetMousePosition()->x + 30, SceneManager::GetInputManager()->GetMousePosition()->y + 108, 36);
			}
		}
	}*/

	return true;
}

bool Cursor::Draw(float a_fDeltaTime)
{

	SceneManager::GetDisplayManager()->SetShaderProgram(m_uiShaderNumber);

	for(unsigned int iDx = 0 ; iDx < m_apkRenderables.size(); iDx++)
    {
        m_apkRenderables[iDx].m_pkTexture->Update(a_fDeltaTime);

        m_apkRenderables[iDx].m_pkMesh->Update();

		Vector vTempPos = *GetWorldLocation();

		SetLocation(*SceneManager::GetInputManager()->GetMousePosition(), true);

        TransformMesh(m_apkRenderables[iDx].m_pkMesh);

		SetLocation(vTempPos, true);

		//TODO: Hardware Transforms like in OGL4.5
		m_apkRenderables[iDx].m_pkMesh->GetMatrix().TransformVertices(m_apkRenderables[iDx].m_pkMesh->GetVertexArray(), m_apkRenderables[iDx].m_pkMesh->GetNumberOfVertices());

        SceneManager::GetDisplayManager()->HUDDraw(m_apkRenderables[iDx].m_pkMesh->GetVertexArray(),4,m_apkRenderables[iDx].m_pkTexture);
    }

	return true;
}

bool Cursor::IsOverObject()
{
	/*for(unsigned int iDx = 0; iDx < SceneManager::GetUnitManager()->GetUnitList().size(); iDx++)
	{
		if(SceneManager::GetInputManager()->GetMouseWorldPosition().WithinBox(*SceneManager::GetUnitManager()->GetUnitList()[iDx]->GetWorldLocation(), SceneManager::GetUnitManager()->GetUnitList()[iDx]->GetSize()))
		{
			m_pkObjectHoveredOver = SceneManager::GetUnitManager()->GetUnitList()[iDx];

			m_bIsObjectHoveredOverABuilding = false;

			return true;
		}
	}

	for(unsigned int iDx = 0; iDx < SceneManager::GetUnitManager()->GetBuildingList().size(); iDx++)
	{
		if(SceneManager::GetInputManager()->GetMouseWorldPosition().WithinBox(*SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetWorldLocation(), SceneManager::GetUnitManager()->GetBuildingList()[iDx]->GetSize()))
		{
			//std::cout<<"Over Building"<<std::endl;

			m_pkObjectHoveredOver = SceneManager::GetUnitManager()->GetBuildingList()[iDx];

			m_bIsObjectHoveredOverABuilding = true;

			return true;
		}
	}*/
	return false;
}

void Cursor::SetCamera(Camera* a_pkCamera)
{
	m_pkCamera = a_pkCamera;
}
