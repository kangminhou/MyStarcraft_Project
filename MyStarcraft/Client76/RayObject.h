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

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void )override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;
};

