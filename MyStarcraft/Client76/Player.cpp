#include "stdafx.h"
#include "Player.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Mouse.h"
#include "ObjMgr.h"

#include "Mouse.h"


CPlayer::CPlayer()
	: m_pCurCorps(NULL)
{
	ZeroMemory( &this->m_tResourceData, sizeof( RESOURCE_DATA ) );
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Initialize( void )
{
	this->m_tResourceData.byTotalPopulation = 200;
	this->m_tResourceData.iMineral = 50;

	this->m_clickCorps.Initialize();

	for ( int i = 0; i < 10; ++i )
		this->m_hotKeyCorps[i].Initialize();

	this->m_pMouse = CMouse::GetInstance();

	return S_OK;
}

int CPlayer::Update( void )
{
	this->KeyCheck();

	return 0;
}

void CPlayer::Render( void )
{
	if ( this->m_pCurCorps )
		this->m_pCurCorps->Render();
}

void CPlayer::Release( void )
{
}

void CPlayer::KeyCheck( void )
{
	/* 부대 관련 키 체크.. */
	if ( CKeyMgr::GetInstance()->GetKeyUp( VK_LBUTTON ) )
	{
		this->MakeDragUnitCorps();
	}

	if ( GetAsyncKeyState( VK_CONTROL ) )
	{
		for ( BYTE i = '0'; i <= '9'; ++i )
		{
			if (CKeyMgr::GetInstance()->GetKeyOnceDown(i))
			{
				if (m_pCurCorps)
					m_hotKeyCorps[i - '0'] = (*m_pCurCorps);
				else
					m_hotKeyCorps[i - '0'].ResetCorps();
			}
		}
	}
	else
	{
		for (BYTE i = '0'; i <= '9'; ++i)
		{
			if (CKeyMgr::GetInstance()->GetKeyOnceDown(i))
			{
				if (m_hotKeyCorps[i - '0'].GetCurUnitNum() > 0)
					m_pCurCorps = &m_hotKeyCorps[i - '0'];
				else
					m_pCurCorps = NULL;
			}
		}
	}

	/* 명령 키 체크.. */
	if ( GetAsyncKeyState( VK_RBUTTON ) )
	{
		if ( this->m_pCurCorps )
			this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_Move );
	}

	if ( this->m_bScrollMove )
		this->m_bScrollMove = false;

	if ( GetAsyncKeyState( VK_RIGHT ) )
	{
		this->m_vScroll.x += 1000 * GET_TIME;
		this->m_bScrollMove = true;
	}
	else if ( GetAsyncKeyState( VK_LEFT ) )
	{
		this->m_vScroll.x -= 1000 * GET_TIME;
		this->m_bScrollMove = true;
	}
	if ( GetAsyncKeyState( VK_DOWN ) )
	{
		this->m_vScroll.y += 1000 * GET_TIME;
		this->m_bScrollMove = true;
	}
	else if ( GetAsyncKeyState( VK_UP ) )
	{
		this->m_vScroll.y -= 1000 * GET_TIME;
		this->m_bScrollMove = true;
	}

}

void CPlayer::MakeDragUnitCorps()
{
	vector<CGameEntity*> vecEntity;

	MOUSE_DRAG_DATA mouseDragData = this->m_pMouse->GetDragData();

	mouseDragData.vStartPos += m_vScroll;
	mouseDragData.vEndPos += m_vScroll;

	CObjMgr::GetInstance()->CheckDragEntitys( vecEntity, mouseDragData, this->GetObjectType() );

	this->m_clickCorps.ResetCorps();

	for ( size_t i = 0; i < vecEntity.size(); ++i )
		this->m_clickCorps.AddUnit( vecEntity[i] );

	this->m_pCurCorps = &this->m_clickCorps;
}
