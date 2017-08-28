#pragma once
#include "Scene.h"

//#define STARCRAFT_START_SCENE_DEBUG

class CTimeMgr;
class CMouse;
class CKeyMgr;
class CSceneMgr;

class CStarcraftStartScene :
	public CScene
{
private:
	typedef struct tagButton
	{
		RECT					rcClick;
		vector<const TEX_INFO*>	vecTexture;
		vector<const TEX_INFO*> vecHoverTexture;
		const TEX_INFO*			pFontTexture[2];
		float					fCurFrame;
		D3DXMATRIX				matWorld;
		D3DXMATRIX				matFontWorld;
		D3DXMATRIX				matHoverWorld;

		bool					bSelect;

	}BUTTON;

private:
	CTimeMgr*		m_pTimeMgr;
	CMouse*			m_pMouse;
	CKeyMgr*		m_pKeyMgr;
	LPD3DXSPRITE	m_pSprite;
	CSceneMgr*		m_pSceneMgr;

	BUTTON	m_tButton[4];
	int		m_iHoverButton;

	const TEX_INFO*	m_pBackTexture;
	const TEX_INFO*	m_pDragTexture;

public:
	CStarcraftStartScene();
	virtual ~CStarcraftStartScene();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

private:
	void DrawRect( const RECT& _rc );

};

