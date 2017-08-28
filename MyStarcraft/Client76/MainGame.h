#pragma once

#include "Include.h"

class CDevice;
class CSoundMgr;
class CMouse;
class CSceneMgr;
class CKeyMgr;
class CTimeMgr;

class CMainGame
{
private:
	CDevice*		m_pDevice;

	CSoundMgr*	m_pSoundMgr;
	CMouse*		m_pMouse;
	CSceneMgr*	m_pSceneMgr;
	CKeyMgr*	m_pKeyMgr;
	CTimeMgr*	m_pTimeMgr;


private:
	float		m_fTime;
	int			m_iCount;
	TCHAR		m_szFps[MIN_STR];
	D3DXMATRIX  m_matTrans;

public:
	void FPS_Render(void);

public:
	HRESULT	Initialize(void);
	int		Update(void);
	void	Render(void);
	void	Release(void);

private:
#ifndef _IMAGE_THREAD_LOADING
	bool	InitResource( void );
#endif
	HRESULT InitSound( void );

public:
	CMainGame(void);
	~CMainGame(void);
};
