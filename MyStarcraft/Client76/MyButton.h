#pragma once
#include "GameObject.h"

class CMouse;

class CMyButton :
	public CGameObject
{
private:
	CMouse*					m_pMouse;

	BUTTON_DATA*			m_pButtonData;
	const TEX_INFO*			m_pDrawTexture[3];

	int						m_iDrawIndex;
	bool					m_bClickButton;
	bool					m_bCanUseButton;

	RECT					m_tColRect;

public:
	CMyButton();
	virtual ~CMyButton();

public:
	void SetButtonData( BUTTON_DATA* _pButtonData );
	
public:
	const BUTTON_DATA* GetButtonData() const;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Render() override;
	virtual void Release( void ) override;

public:
	virtual void UpdatePosition( const D3DXVECTOR3& vPrevPos ) override;

};

