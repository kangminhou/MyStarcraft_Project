#include "stdafx.h"
#include "Player.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Mouse.h"
#include "ObjMgr.h"
#include "Device.h"

#include "UIMgr.h"

#include "Mouse.h"


CPlayer::CPlayer()
	: m_pCurCorps(NULL)
	, m_bOrderAct(false)
{
	ZeroMemory( &this->m_tResourceData, sizeof( RESOURCE_DATA ) );
}


CPlayer::~CPlayer()
{
	this->Release();
}

HRESULT CPlayer::Initialize( void )
{
	this->m_pCancelButton = new BUTTON_DATA( CGameEntity::Pattern_Cancel, 236, VK_ESCAPE, 3, false, true );

	this->m_tResourceData.byTotalPopulation = 200;
	this->m_tResourceData.iMineral = 50;

	this->m_clickCorps.Initialize();

	for ( int i = 0; i < 10; ++i )
		this->m_hotKeyCorps[i].Initialize();

	this->m_pMouse = CMouse::GetInstance();

	CUIMgr::GetInstance()->SetPlayer( this );

	return S_OK;
}

int CPlayer::Update( void )
{
	if ( m_bOrderAct )
	{
		this->m_pCurCorps->PushMessage( this->m_pPushButtonData );

		if ( this->m_pPushButtonData->bSkill )
			this->m_pCurCorps->SetUnitSkill( CGameEntity::eGameEntitySkillKind( this->m_pPushButtonData->iFunc ) );
		else
			this->m_pCurCorps->SetUnitPattern( CGameEntity::eGameEntityPattern( this->m_pPushButtonData->iFunc ) );

		this->m_bOrderAct = false;
		
		return 0;
	}

	this->KeyCheck();

	return 0;
}

void CPlayer::Render( void )
{
	if ( this->m_pCurCorps )
	{
		this->m_pCurCorps->Render();

		D3DXMATRIX matTrans;
		D3DXMatrixTranslation( &matTrans, 50.f, 50.f, 0.f );

		TCHAR str[128];
		swprintf_s( str, L"%d", this->m_pCurCorps->GetCurUnitNum() );

		CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
		CDevice::GetInstance()->GetFont()->DrawTextW(
			CDevice::GetInstance()->GetSprite(),
			str,
			lstrlen( str ),
			NULL,
			NULL,
			D3DCOLOR_ARGB( 255, 255, 255, 0 )
		);
	}
}

void CPlayer::Release( void )
{
	safe_delete( m_pCancelButton );
}

void CPlayer::OrderActPattern( const BUTTON_DATA * _pButtonData )
{
	this->m_bOrderAct = true;
	this->m_pPushButtonData = _pButtonData;
}

void CPlayer::DecideShowButton()
{
	m_vecCurCanActButton.clear();
	if ( this->m_pCurCorps->GetCurUnitNum() > 0 )
	{
		int iLength = ((this->m_pCurCorps->GetSameUnit()) ? 1 : this->m_pCurCorps->GetCurUnitNum());
		for ( int i = 0; i < iLength; ++i )
		{
			vector<BUTTON_DATA*>* pVecButtonData = this->m_pCurCorps->GetEntity( i )->GetButtonData();

			for ( size_t j = 0; j < pVecButtonData->size(); ++j )
			{
				bool bFind = false;
				for ( size_t k = 0; k < m_vecCurCanActButton.size(); ++k )
				{
					if ( this->m_vecCurCanActButton[k] == (*pVecButtonData)[j] )
					{
						bFind = true;
						break;
					}
				}

				if ( !bFind )
				{
					if ( (*pVecButtonData)[j]->bSkill && !this->m_pCurCorps->GetSameUnit() )
						continue;

					this->m_vecCurCanActButton.push_back( (*pVecButtonData)[j] );
				}
			}

		}

		CUIMgr::GetInstance()->ShowButton( &this->m_vecCurCanActButton );
	}
}

void CPlayer::ShowOnlyCancelButtonInterface()
{
	CUIMgr::GetInstance()->ShowButton( &this->m_vecCurCanActButton );
}

void CPlayer::ResetMouseClickEventEntity()
{
	m_vecClickEventEntity.clear();
}

void CPlayer::AddMouseClickEventEntity( CGameEntity * _pEntity )
{
	m_vecClickEventEntity.push_back( _pEntity );
}

void CPlayer::EraseMouseClickEventEntity( CGameEntity * _pEntity )
{
	for ( size_t i = 0; i < m_vecClickEventEntity.size(); ++i )
	{
		if ( m_vecClickEventEntity[i] == _pEntity )
		{
			m_vecClickEventEntity.erase( m_vecClickEventEntity.begin() + i );
		}
	}
}

void CPlayer::KeyCheck( void )
{
	/* 부대 관련 키 체크.. */
	if ( CKeyMgr::GetInstance()->GetKeyUp( VK_LBUTTON ) )
	{
		for ( size_t i = 0; i < this->m_vecClickEventEntity.size(); ++i )
		{
			this->m_vecClickEventEntity[i]->MouseEvent();
		}

		this->MakeDragUnitCorps();
	}

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( 'A' ) )
	{
		if ( this->m_pCurCorps )
			this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_MoveAlert );
	}

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( 'P' ) )
	{
		if ( this->m_pCurCorps )
			this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_Patrol );
	}

	/* 부대 매크로 키 관련.. */
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
			this->UnitMove();
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

	if ( !vecEntity.empty() )
	{
		this->m_clickCorps.ResetCorps();

		for ( size_t i = 0; i < vecEntity.size(); ++i )
			this->m_clickCorps.AddUnit( vecEntity[i] );

		this->m_pCurCorps = &this->m_clickCorps;

		this->DecideShowButton();

		CUIMgr::GetInstance()->ShowEntityUI( this->m_pCurCorps );
	}
}

void CPlayer::UnitMove()
{
	this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_Move );
}
