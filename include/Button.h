//
//  Button.h
//  Pegasus Feather
//
//  Created by Rohan Carty on 11/05/15.
//  Copyright (c) 2015 Dancing Dinosaur Games Pty Ltd. All rights reserved.
//

#ifndef _BUTTON_H_
#define _BUTTON_H_
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

#include "UIElement.h"

#include <iostream>
#include <vector>

//Forward declare the Object class so as to use it's pointer.
class Actor;

//Forward declare SceneManager
class SceneManager;

class Button : public UIElement
{
public:
    Button(Scene* a_pkScene);
    virtual ~Button();

    virtual bool Update(float a_fDeltaTime);

	bool GetIsToggled();
	void SetIsToggled(bool a_bIsToggled);
protected:
	bool m_bIsToggled;
};

#endif
