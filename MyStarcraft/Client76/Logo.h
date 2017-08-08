#pragma once
#include "scene.h"

class CLogo :
	public CScene
{
private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	float fTime1, fTime2;

private:
	static unsigned int __stdcall ImageLoadThreadFunc(void* pArg);
	unsigned int __stdcall ImageLoadThreadFunc2(void* pArg);
	unsigned int __stdcall TestFunc( void* pArg );

public:
	CRITICAL_SECTION GetCrt( void ) { return m_Crt; }

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	CLogo(void);
	virtual ~CLogo(void);
};
