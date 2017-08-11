#pragma once
#include "Scene.h"

class CRayTestScene :
	public CScene
{
public:
	CRayTestScene();
	virtual ~CRayTestScene();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

};

