#pragma once
#include "Scene.h"

class CKeyMgr;

class CStarcraftLogo :
	public CScene
{
private:
	HWND		m_hVideo; // ������ ����� �ϱ����� ������â �ڵ�.
	CKeyMgr*	m_pKeyMgr;

public:
	CStarcraftLogo();
	virtual ~CStarcraftLogo();

public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

private:
	void InitResource();

};

