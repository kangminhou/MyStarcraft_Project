#pragma once

#include "Include.h"

class CDevice;
class CMainGame
{
private:
	CDevice*		m_pDevice;

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
	bool	InitResource( void );

public:
	CMainGame(void);
	~CMainGame(void);
};
