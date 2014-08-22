#ifndef _OBJECT_H_
#define _OBJECT_H_
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
#include <vector>

//Forward declare
class Scene;
class Vector;
class Vertex;
class Mesh;
class AnimatedTexture;
class Matrix;

class Object
{
public:
    Object(Scene* a_pkScene);
    virtual ~Object();

	virtual bool LoadResources(std::string a_szFileName);

    virtual bool Update(float a_fDeltaTime);

	virtual bool Draw(float a_fDeltaTime);
    
    bool UpdateChild(float a_fDeltaTime);

    bool AttachChild(Object* a_pkObject);
    bool DetachChild();

    bool AttachTo(Object* a_pkObject);
    bool DetachFrom();

	void SetHidden(bool a_bHide);
	bool GetHidden();

    Scene* GetScene();

	void TransformMesh(Mesh* a_pkMesh);

    Vector* GetLocation();
    Vector* GetWorldLocation();
    void SetLocation(Vector a_pNewLocation, bool a_bInstantWorldUpdate = false);
    void SetLocation(double a_x, double a_y, double a_z);

	float GetRotation();
	void SetRotation(float a_fRotation);

	float GetSize();
	void SetSize(float a_fSize);
    
    float GetScale();
    void SetScale(float a_fScale);

protected:
    Object* m_pkParent;
    Object* m_pkChild;

    Scene* m_pkScene;

    Vector* m_pLocation;
    Vector* m_pWorldLocation;

	float m_fScale;
    float m_fRotation; // in Radians
    float m_fSize;

    AnimatedTexture* m_pkTexture;

	unsigned int m_uiShaderNumber;

    std::vector<Mesh*> m_apkRenderables;

	bool m_bHidden;
};

#endif
