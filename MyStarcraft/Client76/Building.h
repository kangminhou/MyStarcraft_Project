#pragma once
#include "GameObject.h"
class CBuilding :
	public CGameObject
{
public:
	CBuilding();
	virtual ~CBuilding();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT	Initialize(void) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

};

