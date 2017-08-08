#pragma once
#include "Scene.h"

class CTestScene :
	public CScene
{
public:
	CTestScene();
	virtual ~CTestScene();

	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

};

