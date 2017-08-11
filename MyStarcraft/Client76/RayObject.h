#pragma once
#include "GameObject.h"

class CRayObject :
	public CGameObject
{
private:
	const TEX_INFO*	m_pTex;
	D3DXVECTOR3	m_vDestination;
	bool m_bMove;
	float fSpeed;

public:
	CRayObject();
	virtual ~CRayObject();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize( void )override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;
};

