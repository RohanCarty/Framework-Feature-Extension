#ifndef _PLAYER_H_
#define _PLAYER_H_
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

#include <iostream>

//Forward declare
class Scene;
class Vector;

enum eAbilities
{
	eNone = 0,
	eHeal,
	eHealParty,
	eBurst,
	eRevive
};

#include "Actor.h"

class Player : public Actor
{
public:
	Player(Scene* a_pkScene);
	~Player();

	bool Update(float a_fDeltaTime);

	void Jump(); // Causes an increase in upwards velocity.

	virtual void Hurt();

	void Attack(float a_fDeltaTime); //Attacks in front of the player //will move to actor eventually;

	void BindToController();

	virtual void Death();

	void Respawn();

	int GetCurrentSoulPowerLevel();
	void SetCurrentSoulPowerLevel(int a_iNewSoulPowerLevel);

	bool GetIsAlive();

	void BeginCastingAbility(int a_iAbilityBeingCast); // Kicks off the casting of an ability, locks controls and sets time for casting.

	void StopCastingAbility(); // Is called if button is released, cancelling the cast.

	void UpdateAbilityStatus(float a_fDeltaTime); // Draws progress bars and ability name, also updates the current cast time and "procs" the cast when it hits 100%

	void ApplyAbility();

	std::string GetStringOfNameOfAbility();
	std::string GetResourceStringOfAbility(int a_iAbilityEnum);

	int m_iSpecial1Ability; // Keeps which power is selected by the player in the first slot (Y)
	int m_iSpecial2Ability; // Keeps which power is selected by the player in the second slot (B)
private:
	bool m_bIsUsingAbility;
	float m_fAbilityCurrentCastTime; // Tracks time since begining of current ability to be casted.
	float m_fAbilityCastTime; // Is set when an ability is started to be cast in order to track when it'll be completed.
	int m_iAbilityBeingCast; // Integer that corrosponds to an enum of abilities to display name while it is being cast and to trigger its effects.
	bool m_bIsAlive; // Used to enable HUD messages or end the game, etc.
	bool m_bJumpLatch; // Used to prevent jumping more than once before hitting the ground.
	int m_iControllerNumberBoundTo;
	int m_iCurrentSoulPowerLevel; //This is the crux of all ability calcuations for things such as run speed and basic damage. // for now we'll work with a max of 100;
	Vector m_vCurrentMaxSpeed;
	int m_iCurrentAttackPower;

	float m_fAttackCooldown;
	float m_fCurrentAttackCooldown;
};

#endif