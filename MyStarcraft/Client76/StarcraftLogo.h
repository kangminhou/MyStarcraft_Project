#pragma once
#include "Scene.h"

class CKeyMgr;

class CStarcraftLogo :
	public CScene
{
private:
	HWND		m_hVideo; // 동영상 재생을 하기위한 윈도우창 핸들.
	CKeyMgr*	m_pKeyMgr;

public:
	CStarcraftLogo();
	virtual ~CStarcraftLogo();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

private:
	void InitResource();

};

