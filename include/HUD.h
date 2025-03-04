#ifndef _HUD_H_
#define _HUD_H_
//Dem debug things
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

#include <string>
#include <vector>

//Forward declare
class Scene;
class TextLibrary;
class GameInfo;
class Object;
class Vector;
class UIElement;

struct stPlayerInfo
{
	std::string szGamertag;
	UIElement* pkBackgroundObject;
	UIElement* pkAbilityIcon1;
	UIElement* pkYButton;
	UIElement* pkAbilityIcon2;
	UIElement* pkBButton;
	UIElement* pkReviveIcon;
	UIElement* pkViewButton;
};

class HUD
{
public:
    HUD(Scene* a_pkScene);
    ~HUD();

    bool Update(float a_fDeltaTime);

	TextLibrary* GetTextLibrary();

	void DrawProgressBar(Vector a_vPosition, Vector a_vSize, float a_fNormalisedPercentage);

	void DrawHUDProgressBar(Vector a_vPosition, Vector a_vSzie, float a_fNormalisedPercentage);

	void PrintString(std::string& sString, double x, double y, unsigned int CharacterSize);

	void PrintHUDString(std::string& sString, double x, double y, unsigned int CharacterSize);

	void PopulatePlayerInfos();

	void SetPositionOfPlayerInfoObjects(unsigned int a_uiInfoToUpdate, Vector a_vNewPosition);

private:
	Object* m_pkProgressBarBacking;
	Object* m_pkProgressBar;
    Scene* m_pkScene;
    GameInfo* m_pkGameInfo;
    TextLibrary* m_pkTextLibrary;
    char* m_cpTempString;

	std::vector<stPlayerInfo> m_astPlayerInfos;
};

#endif
