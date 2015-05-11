//
//  UIElement.h
//  Pegasus Feather
//
//  Created by Rohan Carty on 11/05/15.
//  Copyright (c) 2015 Dancing Dinosaur Games Pty Ltd. All rights reserved.
//

#ifndef _UIELEMENT_H_
#define _UIELEMENT_H_
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

#include "Object.h"

#include <iostream>
#include <vector>

//Forward declare the Object class so as to use it's pointer.
class Actor;

//Forward declare SceneManager
class SceneManager;

class UIElement : public Object
{
public:
    UIElement(Scene* a_pkScene);
    virtual ~UIElement();

    virtual bool Update(float a_fDeltaTime);

	virtual void SetSize(Vector a_fSize);

    virtual void SetScale(float a_fScale);
protected:
	bool m_bIsInFocus;
};

#endif
