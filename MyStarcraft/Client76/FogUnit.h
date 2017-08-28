#pragma once
#include "GameObject.h"

class CFogBackground;

class CFogUnit :
	public CGameObject
{
private:
	const TEX_INFO*	m_pTexture;
	CFogBackground*	m_pBackground;

	bool m_bMove;
	bool	m_bClick;
	D3DXVECTOR3 m_vDestination;

	RECT		m_tRect;
	RECT		m_tColRect;

public:

	int	m_iSight;

	float m_fSpeed;

public:
	CFogUnit();
	virtual ~CFogUnit();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	virtual void UpdatePosition( const D3DXVECTOR3& vPrevPos ) override;

private:
	void Scroll();
	void KeyCheck();

};

