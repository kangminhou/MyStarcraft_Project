#pragma once
#include "Scene.h"

class CRayTestScene :
	public CScene
{
public:
	CRayTestScene();
	virtual ~CRayTestScene();

public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

};

