#pragma once
#include "Scene.h"

class CTextureMgr;
class CTimeMgr;
class CKeyMgr;

class CStarcraft_LoadingScene :
	public CScene
{
private:
	CTextureMgr*		m_pTextureMgr;
	CTimeMgr*			m_pTimeMgr;
	CKeyMgr*			m_pKeyMgr;

	float				m_fWaitTime;
	const TEX_INFO*		m_pBackTexture;

	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;

	bool				m_bLoadingEnd;

#ifdef _IMAGE_THREAD_LOADING
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
#endif

public:
	CStarcraft_LoadingScene();
	virtual ~CStarcraft_LoadingScene();

#ifdef _IMAGE_THREAD_LOADING
public:
	CRITICAL_SECTION GetCrt( void );
	void LoadingEnd();

private:
	static unsigned int __stdcall ImageLoadThreadFunc(void* pArg);
	bool InitResource();

#endif

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

};

