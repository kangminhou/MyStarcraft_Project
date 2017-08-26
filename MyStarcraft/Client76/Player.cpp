#include "stdafx.h"
#include "Player.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Mouse.h"
#include "ObjMgr.h"
#include "Device.h"

#include "UIMgr.h"
#include "UpgradeMgr.h"

#include "Mouse.h"
#include "Mineral.h"
#include "Gas.h"
#include "SCV.h"


CPlayer::CPlayer()
	: m_pCurCorps(NULL)
	, m_bOrderAct(false)
	, m_bWaitAct(false)
{
	ZeroMemory( &this->m_tResourceData, sizeof( RESOURCE_DATA ) );
}


CPlayer::~CPlayer()
{
	this->Release();
}

void CPlayer::SetTotalPopulation( const BYTE & _byTotalPopulation )
{
	if ( _byTotalPopulation < 10 || _byTotalPopulation > 200 )
		return;

	this->m_tResourceData.byTotalPopulation = _byTotalPopulation;
	this->m_pUIMgr->UpdatePlayerResourceData( L"Population" );
}

CPlayer::RESOURCE_DATA CPlayer::GetResourceData() const
{
	return this->m_tResourceData;
}

BYTE CPlayer::GetTotalPopulation() const
{
	return this->m_tResourceData.byTotalPopulation;
}

HRESULT CPlayer::Initialize( void )
{
	this->m_vecCancelButton.push_back( new BUTTON_DATA( CGameEntity::Pattern_Cancel, 236, VK_ESCAPE, 3, false, true ) );

	//this->m_tResourceData.byTotalPopulation = 10;
	//this->m_tResourceData.iMineral = 50;

	this->m_tResourceData.byTotalPopulation = 10;
	this->m_tResourceData.iMineral = 5000;
	this->m_tResourceData.iGas = 500;

	this->m_clickCorps.Initialize();

	for ( int i = 0; i < 10; ++i )
		this->m_hotKeyCorps[i].Initialize();

	this->m_pMouse = CMouse::GetInstance();

	list<CGameObject*>* pResourceList = CObjMgr::GetInstance()->GetList( OBJ_TYPE_RESOURCE );
	for ( auto& iter : (*pResourceList) )
	{
		CMineral* pMineral = dynamic_cast<CMineral*>(iter);
		if ( pMineral )
		{
			this->m_vecMineral.push_back( pMineral );
			continue;
		}

		CGas* pGas = dynamic_cast<CGas*>(iter);

		if ( pGas )
			this->m_vecGas.push_back( pGas );
	}

	this->m_pUIMgr = CUIMgr::GetInstance();
	this->m_pUIMgr->SetPlayer( this );
	this->m_pUIMgr->UpdatePlayerResourceData( L"All" );

	this->m_pUpgradeMgr = CUpgradeMgr::GetInstance();

	this->SetScroll( D3DXVECTOR3( 0.f, 3000.f, 0.f ) );

	this->m_pMouse->SetPlayer( this );

	return S_OK;
}

int CPlayer::Update( void )
{
	if ( this->m_bScrollMove )
	{
		if(m_byCnt <= 0 )
			this->m_bScrollMove = false;

		--m_byCnt;
	}

	if ( m_bOrderAct )
	{
		if ( this->m_pPushButtonData->bImmediate )
		{
			this->ActCommand();
			return 0;
		}
		else
		{
			if ( !m_bWaitAct )
			{
				m_bWaitAct = true;
				this->ShowOnlyCancelButtonInterface();
				this->m_pMouse->ActSelectTargetState();
			}
		}
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
	safe_delete( m_vecCancelButton.front() );
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
					//if ( (*pVecButtonData)[j]->bSkill && !this->m_pCurCorps->GetSameUnit() )
					if ( (*pVecButtonData)[j]->iBtnKind == Button_Act_Skill && !this->m_pCurCorps->GetSameUnit() )
						continue;

					this->m_vecCurCanActButton.push_back( (*pVecButtonData)[j] );
				}
			}

		}

		this->m_pUIMgr->ShowButton( &this->m_vecCurCanActButton );
	}
}

void CPlayer::ShowOnlyCancelButtonInterface()
{
	this->m_pUIMgr->ShowButton( &this->m_vecCancelButton );
}

void CPlayer::ShowEntityUI()
{
	if ( this->m_pCurCorps )
		this->m_pUIMgr->ShowEntityUI( this->m_pCurCorps );
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

bool CPlayer::BuyUnit( const UNIT_GENERATE_DATA & _tUnitGenData )
{
	if ( !this->CheckCanBuyUnit( _tUnitGenData ) )
		return false;

	this->m_tResourceData.iMineral -= _tUnitGenData.iRequireMineral;
	this->m_tResourceData.iGas -= _tUnitGenData.iRequireGas;
	//this->m_tResourceData.byCurPopulation += _tUnitGenData.iRequirePopulation;

	if ( _tUnitGenData.iRequireMineral > 0 )
		this->m_pUIMgr->UpdatePlayerResourceData( L"Mineral" );
	if ( _tUnitGenData.iRequireGas > 0 )
		this->m_pUIMgr->UpdatePlayerResourceData( L"Gas" );

	return true;
}

bool CPlayer::CheckCanMakeUnit( const UNIT_GENERATE_DATA & _tUnitGenData )
{
	if ( m_tResourceData.byTotalPopulation - m_tResourceData.byCurPopulation < _tUnitGenData.iRequirePopulation )
		return false;

	this->m_tResourceData.byCurPopulation += _tUnitGenData.iRequirePopulation;

	if ( _tUnitGenData.iRequirePopulation > 0 )
		this->m_pUIMgr->UpdatePlayerResourceData( L"Population" );

	return true;
}

bool CPlayer::CheckCanBuyUnit( const UNIT_GENERATE_DATA & _tUnitGenData )
{
	if ( m_tResourceData.iMineral < _tUnitGenData.iRequireMineral || m_tResourceData.iGas < _tUnitGenData.iRequireGas )
		return false;

	return true;
}

void CPlayer::SetScroll( const D3DXVECTOR3 & _vScroll )
{
	this->m_vScroll = _vScroll;
	this->m_bScrollMove = true;

	this->m_byCnt = 1;

	if ( this->m_vScroll.x < 0.f )
		this->m_vScroll.x = 0.f;
	else if ( this->m_vScroll.x >= TILECX * TILEX - WINCX )
		m_vScroll.x = TILECX * TILEX - WINCX;

	if ( this->m_vScroll.y < 0.f )
		this->m_vScroll.y = 0.f;
	else if ( this->m_vScroll.y >= TILECY * TILEY - WINCY )
		this->m_vScroll.y = TILECY * TILEY - WINCY;
}

void CPlayer::KeyCheck( void )
{
	if ( this->m_bWaitAct )
	{
		if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_ESCAPE ) )
		{
			this->m_bWaitAct = false;
			this->m_bOrderAct = false;
			this->DecideShowButton();
			this->m_pMouse->ActIdleState();
			return;
		}

		if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) )
		{
			this->m_bWaitAct = false;
			this->ActCommand();
			this->DecideShowButton();
			this->m_pMouse->ActIdleState();
			return;
		}
	}

	/* 부대 관련 키 체크.. */
	if ( CKeyMgr::GetInstance()->GetKeyUp( VK_LBUTTON ) )
	{
		for ( size_t i = 0; i < this->m_vecClickEventEntity.size(); ++i )
		{
			this->m_vecClickEventEntity[i]->MouseEvent();
		}

		this->MakeDragUnitCorps();
	}

	//if ( CKeyMgr::GetInstance()->GetKeyOnceDown( 'A' ) )
	//{
	//	if ( this->m_pCurCorps )
	//		this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_MoveAlert );
	//}
	//
	//if ( CKeyMgr::GetInstance()->GetKeyOnceDown( 'P' ) )
	//{
	//	if ( this->m_pCurCorps )
	//		this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_Patrol );
	//}

	/* 부대 매크로 키 관련.. */
	if ( CKeyMgr::GetInstance()->GetKeyStayDown( VK_CONTROL ) )
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
				if ( m_hotKeyCorps[i - '0'].GetCurUnitNum() > 0 )
				{
					m_pCurCorps = &m_hotKeyCorps[i - '0'];
					this->DecideShowButton();
					this->m_pUIMgr->ShowEntityUI( this->m_pCurCorps );
				}
				else
					m_pCurCorps = NULL;
			}
		}
	}

	/* 명령 키 체크.. */
	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_RBUTTON ) )
	{
		if ( this->m_pCurCorps )
			this->RButtonClick();
	}

}

void CPlayer::MakeDragUnitCorps()
{
	vector<CGameEntity*> vecEntity;

	MOUSE_DRAG_DATA mouseDragData = this->m_pMouse->GetDragData();

	mouseDragData.vStartPos += m_vScroll;
	mouseDragData.vEndPos += m_vScroll;

	bool bEnemyDrag = false;
	if ( !CObjMgr::GetInstance()->CheckDragEntitys( vecEntity, mouseDragData, this->GetObjectType() ) )
	{
		for ( int i = OBJ_TYPE_USER2; i <= OBJ_TYPE_USER2; ++i )
		{
			bEnemyDrag = CObjMgr::GetInstance()->CheckDragEntitys( vecEntity, mouseDragData, (eObjectType)i );
		}
	}

	if ( !vecEntity.empty() )
	{
		this->m_clickCorps.ResetCorps();

		bool bFindBuildingEntity = false;
		bool bErawBuildingEntity = false;
		for ( size_t i = 0; i < vecEntity.size(); ++i )
		{
			if ( !bFindBuildingEntity )
			{
				bFindBuildingEntity = vecEntity[i]->GetCheckUnitInformation( CGameEntity::Entity_Building );
			}
			else
			{
				if ( !vecEntity[i]->GetCheckUnitInformation( CGameEntity::Entity_Building ) )
				{
					bErawBuildingEntity = true;
					for ( size_t j = 0; j < vecEntity.size(); )
					{
						if ( vecEntity[j]->GetCheckUnitInformation( CGameEntity::Entity_Building ) )
							vecEntity.erase( vecEntity.begin() + j );
						else
							++j;
					}
					break;

				}
			}
		}
		 
		this->m_pUIMgr->HideBuildingOrderUI();

		if ( bFindBuildingEntity && !bErawBuildingEntity )
		{
			vecEntity[0]->ShowOrderIcon();
			this->m_clickCorps.AddUnit( vecEntity[0] );
		}
		else
		{
			for ( size_t i = 0; i < vecEntity.size(); ++i )
				this->m_clickCorps.AddUnit( vecEntity[i] );
		}

		this->m_pCurCorps = &this->m_clickCorps;

		this->DecideShowButton();
		this->ShowEntityUI();
	}
}

void CPlayer::RButtonClick()
{
	vector<CGameEntity*> vecEntity;

	MOUSE_DRAG_DATA tDragData;
	tDragData.vStartPos = tDragData.vEndPos = m_pMouse->GetPos();

	RECT rcMouse = { (LONG)(tDragData.vStartPos.x + m_vScroll.x), (LONG)(tDragData.vStartPos.y + m_vScroll.y), 
			(LONG)(tDragData.vEndPos.x + m_vScroll.x), (LONG)(tDragData.vEndPos.y + m_vScroll.y) };

	if ( rcMouse.left == rcMouse.right )
		++rcMouse.right;
	if ( rcMouse.top == rcMouse.bottom )
		++rcMouse.bottom;

	/* 리소스 클릭인지 검사.. */
	for ( size_t i = 0; i < this->m_vecMineral.size(); ++i )
	{
		RECT rc = { 0, 0, 0, 0 };
		RECT rcCol = this->m_vecMineral[i]->GetColRect();
		D3DXVECTOR3 vPos = this->m_vecMineral[i]->GetPos();

		if ( IntersectRect( &rc, &rcCol, &rcMouse ) )
		{
			size_t iLength = this->m_pCurCorps->GetCurUnitNum();
			for ( size_t j = 0; j < iLength; ++j )
			{
				CSCV* pSCV = dynamic_cast<CSCV*>(this->m_pCurCorps->GetEntity( (BYTE)j ));

				if ( pSCV )
				{
					pSCV->GatherResource( true, this->m_vecMineral[i] );
				}

			}

			this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_Gather );
			return;
		}
	}

	for ( size_t i = 0; i < this->m_vecGas.size(); ++i )
	{
		if ( !this->m_vecGas[i]->GetIsCanGather() )
			continue;

		RECT rc = { 0, 0, 0, 0 };
		RECT rcCol = this->m_vecGas[i]->GetColRect();
		D3DXVECTOR3 vPos = this->m_vecGas[i]->GetPos();

		if ( IntersectRect( &rc, &rcCol, &rcMouse ) )
		{
			size_t iLength = this->m_pCurCorps->GetCurUnitNum();
			for ( size_t i = 0; i < iLength; ++i )
			{
				CSCV* pSCV = dynamic_cast<CSCV*>(this->m_pCurCorps->GetEntity( (BYTE)i ));

				if ( pSCV )
				{
					pSCV->GatherResource( false, this->m_vecGas[i] );
				}

			}

			this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_Gather );
			return;
		}
	}

	/* 유닛을 클릭했는지 검사.. */
	for ( int i = OBJ_TYPE_USER; i <= OBJ_TYPE_USER2; ++i )
	{
		if ( CObjMgr::GetInstance()->CheckDragEntitys( vecEntity, tDragData, eObjectType( i ) ) )
		{
			auto SortEntityToPos = [&]( CGameEntity* _pDstEntity, CGameEntity* _pSrcEntity ) {
				return D3DXVec3Length( &(_pDstEntity->GetPos() - this->GetPos()) ) > D3DXVec3Length( &(_pSrcEntity->GetPos() - this->GetPos()) );
			};

			sort( vecEntity.begin(), vecEntity.end(), SortEntityToPos );

			CGameEntity* pCheckEntity = vecEntity.front();
			if ( pCheckEntity->GetObjectType() == OBJ_TYPE_USER )
			{
				this->UnitMove();
				return;
			}
			else
			{
				for ( size_t i = 0; i < m_pCurCorps->GetCurUnitNum(); ++i )
				{
					this->m_pCurCorps->GetEntity( (BYTE)i )->SetTarget( pCheckEntity );
					this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_ChaseTarget );
					return;
				}
			}
		}

	}

	this->UnitMove();

}

void CPlayer::UnitMove()
{
	this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_Move );
}

void CPlayer::ActCommand()
{
	this->m_pCurCorps->PushMessage( this->m_pPushButtonData );

	switch ( this->m_pPushButtonData->iBtnKind )
	{
		case Button_Act_Pattern:
			this->m_pCurCorps->SetUnitPattern( CGameEntity::eGameEntityPattern( this->m_pPushButtonData->iFunc ) );
			break;

		case Button_Act_Skill:
			this->m_pCurCorps->SetUnitSkill( CGameEntity::eGameEntitySkillKind( this->m_pPushButtonData->iFunc ) );
			break;

		case Button_Act_Upgrade:
			if ( this->m_pPushButtonData->iFunc == 0 )
			{
				this->m_pUpgradeMgr->UpgradeWeapon( OBJ_TYPE_USER, eWeaponUpgradeType( this->m_pPushButtonData->iMessage ) );
			}
			else if ( this->m_pPushButtonData->iFunc == 1 )
			{
				this->m_pUpgradeMgr->UpgradeArmor( OBJ_TYPE_USER, eArmorUpgradeType( this->m_pPushButtonData->iMessage ) );
			}
			break;

		case Button_Act_Research:
			this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_Research );
			break;

	}

	this->m_bOrderAct = false;
}
