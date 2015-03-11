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

#include "Actor.h"

class Player : public Actor
{
public:
	Player(Scene* a_pkScene);
	~Player();

	bool Update(float a_fDeltaTime);

	void Jump(); // Causes an increase in upwards velocity.

	void Attack(float a_fDeltaTime); //Attacks in front of the player //will move to actor eventually;

	void BindToController();

	virtual void Death();

	void Respawn();

	int GetCurrentSoulPowerLevel();
	void SetCurrentSoulPowerLevel(int a_iNewSoulPowerLevel);

private:
	bool m_bJumpLatch; // Used to prevent jumping more than once before hitting the ground.
	int m_iControllerNumberBoundTo;
	int m_iCurrentSoulPowerLevel; //This is the crux of all ability calcuations for things such as run speed and basic damage. // for now we'll work with a max of 100;
	Vector m_vCurrentMaxSpeed;
	int m_iCurrentAttackPower;

	float m_fAttackCooldown;
	float m_fCurrentAttackCooldown;
};

#endif