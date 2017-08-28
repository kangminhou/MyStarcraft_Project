#include "stdafx.h"
#include "StarcraftLogo.h"

#include "KeyMgr.h"
#include "SceneMgr.h"


CStarcraftLogo::CStarcraftLogo()
{
}


CStarcraftLogo::~CStarcraftLogo()
{
	Release();
}

HRESULT CStarcraftLogo::Initialize( void )
{
	// MCI (Media Control Interface)
	//MCIWNDF_NOTIFYANSI | MCIWNDF_NOMENU |
	//MCIWNDF_NOTIFYALL | MCIWNDF_NOPLAYBAR

	// ����� �� ���� �ֺ� ��ġ�� �����ϴ� �������� ����� �������α׷��� �����ϴ� �������̽���.
	//m_hVideo = MCIWndCreate(g_hWnd, NULL, MCIWNDF_NOPLAYBAR | WS_VISIBLE | WS_CHILD
	//						 , L"../Video/Logo.wmv");

	m_hVideo = MCIWndCreate( g_hWnd, NULL, MCIWNDF_NOTIFYANSI | MCIWNDF_NOMENU | MCIWNDF_NOTIFYALL | MCIWNDF_NOPLAYBAR,
							 L"../Video/Logo.wmv" );

	RECT rcWindow;
	GetClientRect( g_hWnd, &rcWindow );
	//SetWindowPos(m_hVideo, NULL, 0, 0, rcWindow.right, rcWindow.bottom, SWP_NOZORDER|SWP_NOMOVE);   
	MoveWindow(m_hVideo, rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom, FALSE);
	//MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, TRUE);

	MCIWndSetVolume(m_hVideo, 10);

	MCIWndPlay(m_hVideo); // �̵�� ���

	this->m_pKeyMgr = CKeyMgr::GetInstance();

	return S_OK;
}

int CStarcraftLogo::Update( void )
{
	if ( m_pKeyMgr->GetKeyOnceDown( VK_RETURN ) )
	{
		CSceneMgr::GetInstance()->SetChangeScene( SCENE_Loading );
		return 0;
	}

	return 0;
}

void CStarcraftLogo::Render( void )
{
}

void CStarcraftLogo::Release( void )
{
	MCIWndClose(m_hVideo);
}

void CStarcraftLogo::InitResource()
{
}
