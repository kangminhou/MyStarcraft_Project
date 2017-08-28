#include "stdafx.h"
#include "Player.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Mouse.h"
#include "ObjMgr.h"
#include "Device.h"
#include "Player.h"

#include "UIMgr.h"
#include "UpgradeMgr.h"

#include "Mouse.h"
#include "Mineral.h"
#include "Gas.h"
#include "SCV.h"
#include "Building.h"


CPlayer::CPlayer()
	: m_pCurCorps( NULL )
	, m_bOrderAct( false )
	, m_bWaitAct( false )
	, m_bDoubleClick( false )
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

void CPlayer::GatherMoney( const int & _iMoney )
{
	if ( _iMoney > 0 )
	{
		this->m_tResourceData.iMineral += _iMoney;
		this->m_pUIMgr->UpdatePlayerResourceData( L"Mineral" );
	}
}

void CPlayer::GahterGas( const int & _iGas )
{
	if ( _iGas > 0 )
	{
		this->m_tResourceData.iGas += _iGas;
		this->m_pUIMgr->UpdatePlayerResourceData( L"Gas" );
	}
}

CPlayer::RESOURCE_DATA CPlayer::GetResourceData() const
{
	return this->m_tResourceData;
}

BYTE CPlayer::GetTotalPopulation() const
{
	return this->m_tResourceData.byTotalPopulation;
}

bool CPlayer::GetFullPopulation() const
{
	if ( this->m_tResourceData.byCurPopulation == this->m_tResourceData.byTotalPopulation )
		return true;

	return false;
}

HRESULT CPlayer::Initialize( void )
{
	this->m_pObjMgr = CObjMgr::GetInstance();
	this->m_pKeyMgr = CKeyMgr::GetInstance();

	this->m_vecCancelButton.push_back( new BUTTON_DATA( CGameEntity::Pattern_Cancel, 236, VK_ESCAPE, 3, false, true ) );

	//this->m_tResourceData.byTotalPopulation = 10;
	//this->m_tResourceData.iMineral = 50;

	this->m_tResourceData.byTotalPopulation = 10;
	this->m_tResourceData.iMineral = 300;
	this->m_tResourceData.iGas = 100;

	this->m_clickCorps.Initialize();

	for ( int i = 0; i < 10; ++i )
		this->m_hotKeyCorps[i].Initialize();

	this->m_pMouse = CMouse::GetInstance();

	list<CGameObject*>* pResourceList = this->m_pObjMgr->GetList( OBJ_TYPE_RESOURCE );
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
		if ( m_byCnt <= 0 )
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

		//D3DXMATRIX matTrans;
		//D3DXMatrixTranslation( &matTrans, 50.f, 50.f, 0.f );
		//
		//TCHAR str[128];
		//swprintf_s( str, L"%d", this->m_pCurCorps->GetCurUnitNum() );
		//
		//CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
		//CDevice::GetInstance()->GetFont()->DrawTextW(
		//	CDevice::GetInstance()->GetSprite(),
		//	str,
		//	lstrlen( str ),
		//	NULL,
		//	NULL,
		//	D3DCOLOR_ARGB( 255, 255, 255, 0 )
		//);
	}
}

void CPlayer::Release( void )
{
	safe_delete( m_vecCancelButton.front() );
}

bool CPlayer::CheckNearMineral( CMineral *& _pOutMineral, CGameEntity * _pEntity )
{
	if ( this->m_vecMineral.empty() )
		return false;

	D3DXVECTOR3 vEntityPos = _pEntity->GetPos();

	auto SortMineralToPos = [&]( CMineral* _pMineral1, CMineral* _pMineral2 )
	{
		return D3DXVec3Length( &(_pMineral1->GetPos() - vEntityPos) ) > D3DXVec3Length( &(_pMineral2->GetPos() - vEntityPos) );
	};

	sort( this->m_vecMineral.begin(), this->m_vecMineral.end(), SortMineralToPos );

	for ( size_t i = 0; i < this->m_vecMineral.size(); ++i )
	{
		if ( this->m_vecMineral[i]->GetIsCanGather() )
		{
			_pOutMineral = m_vecMineral[i];
			return true;
		}
	}

	_pOutMineral = this->m_vecMineral.front();

	for ( size_t i = 0; i < this->m_vecMineral.size(); ++i )
	{
		this->m_vecMineral[i]->SetMaxGatherNum( this->m_vecMineral[i]->GetMaxGatherNum() + 1 );
		return true;
	}

	return false;
}

void CPlayer::OrderActPattern( const BUTTON_DATA * _pButtonData )
{
	this->m_bOrderAct = true;
	this->m_pPushButtonData = _pButtonData;
}

void CPlayer::DecideShowButton()
{
	if ( !this->m_pCurCorps )
		return;

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
	if ( this->CheckCanBuyUnit( _tUnitGenData ) )
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

bool CPlayer::BuyUnit( const RESEARCH_DATA & _tResearchData )
{
	if ( this->CheckCanBuyUnit( _tResearchData ) )
		return false;

	this->m_tResourceData.iMineral -= _tResearchData.iRequireMineral;
	this->m_tResourceData.iGas -= _tResearchData.iRequireGas;
	//this->m_tResourceData.byCurPopulation += _tUnitGenData.iRequirePopulation;

	if ( _tResearchData.iRequireMineral > 0 )
		this->m_pUIMgr->UpdatePlayerResourceData( L"Mineral" );
	if ( _tResearchData.iRequireGas > 0 )
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

int CPlayer::CheckCanBuyUnit( const UNIT_GENERATE_DATA & _tUnitGenData )
{
	if ( m_tResourceData.iMineral < _tUnitGenData.iRequireMineral )
	{
		return 1;
	}
	else if ( m_tResourceData.iGas < _tUnitGenData.iRequireGas )
	{
		return 2;
	}

	return 0;
}

int CPlayer::CheckCanBuyUnit( const RESEARCH_DATA & _tResearchData )
{
	if ( m_tResourceData.iMineral < _tResearchData.iRequireMineral )
	{
		return 1;
	}
	else if ( m_tResourceData.iGas < _tResearchData.iRequireGas )
	{
		return 2;
	}

	return 0;
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
		if ( m_pKeyMgr->GetKeyOnceDown( VK_ESCAPE ) )
		{
			this->m_bWaitAct = false;
			this->m_bOrderAct = false;
			this->DecideShowButton();
			this->m_pMouse->ActIdleState();
			return;
		}

		if ( m_pKeyMgr->GetKeyOnceDown( VK_LBUTTON ) )
		{
			this->m_bWaitAct = false;
			this->ActCommand();
			this->DecideShowButton();
			this->m_pMouse->ActIdleState();
			return;
		}
	}

	if ( m_pKeyMgr->GetDoubleClick() )
	{
		this->m_bDoubleClick = true;
	}

	/* 부대 관련 키 체크.. */
	if ( m_pKeyMgr->GetKeyUp( VK_LBUTTON ) )
	{
		for ( size_t i = 0; i < this->m_vecClickEventEntity.size(); ++i )
		{
			this->m_vecClickEventEntity[i]->MouseEvent();
		}

		this->MakeDragUnitCorps();
	}

	//if ( m_pKeyMgr->GetKeyOnceDown( 'A' ) )
	//{
	//	if ( this->m_pCurCorps )
	//		this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_MoveAlert );
	//}
	//
	//if ( m_pKeyMgr->GetKeyOnceDown( 'P' ) )
	//{
	//	if ( this->m_pCurCorps )
	//		this->m_pCurCorps->SetUnitPattern( CGameEntity::Pattern_Patrol );
	//}

	/* 부대 매크로 키 관련.. */
	if ( m_pKeyMgr->GetKeyStayDown( VK_CONTROL ) )
	{
		for ( BYTE i = '0'; i <= '9'; ++i )
		{
			if ( m_pKeyMgr->GetKeyOnceDown( i ) )
			{
				if ( m_pCurCorps )
					m_hotKeyCorps[i - '0'] = (*m_pCurCorps);
				else
					m_hotKeyCorps[i - '0'].ResetCorps();
			}
		}
	}
	else
	{
		for ( BYTE i = '0'; i <= '9'; ++i )
		{
			if ( m_pKeyMgr->GetKeyOnceDown( i ) )
			{
				if ( m_hotKeyCorps[i - '0'].GetCurUnitNum() > 0 )
				{
					m_pCurCorps = &m_hotKeyCorps[i - '0'];
					D3DXVECTOR3 vScroll = this->m_pCurCorps->GetEntity( 0 )->GetPos() - D3DXVECTOR3( WINCX * 0.5f, WINCY * 0.5f, 0.f );
					this->SetScroll( vScroll );
					this->DecideShowButton();
					this->m_pUIMgr->ShowEntityUI( this->m_pCurCorps );
				}
				else
					m_pCurCorps = NULL;
			}
		}
	}

	/* 명령 키 체크.. */
	if ( m_pKeyMgr->GetKeyOnceDown( VK_RBUTTON ) )
	{
		if ( this->m_pCurCorps )
			this->RButtonClick();
	}

	if ( m_pKeyMgr->GetKeyOnceDown( VK_F1 ) )
	{
		this->m_tResourceData.iMineral = 30000;
		this->m_tResourceData.iGas = 30000;
		this->m_tResourceData.byTotalPopulation = 200;

		this->m_pUIMgr->UpdatePlayerResourceData( L"All" );
	}

	if ( m_pKeyMgr->GetKeyStayDown( VK_LEFT ) )
	{
		this->SetScroll( this->m_vScroll + D3DXVECTOR3( -GET_TIME * 500.f, 0.f, 0.f ) );
	}
	if ( m_pKeyMgr->GetKeyStayDown( VK_RIGHT ) )
	{
		this->SetScroll( this->m_vScroll + D3DXVECTOR3( GET_TIME * 500.f, 0.f, 0.f ) );
	}
	if ( m_pKeyMgr->GetKeyStayDown( VK_UP ) )
	{
		this->SetScroll( this->m_vScroll + D3DXVECTOR3( 0.f, -GET_TIME * 500.f, 0.f ) );
	}
	if ( m_pKeyMgr->GetKeyStayDown( VK_DOWN ) )
	{
		this->SetScroll( this->m_vScroll + D3DXVECTOR3( 0.f, GET_TIME * 500.f, 0.f ) );
	}

}

void CPlayer::MakeDragUnitCorps()
{
	vector<CGameEntity*> vecEntity;

	MOUSE_DRAG_DATA mouseDragData = this->m_pMouse->GetDragData();

	mouseDragData.vStartPos += m_vScroll;
	mouseDragData.vEndPos += m_vScroll;

	bool bEnemyDrag = false;

	int iActLevel = 0;

	if ( !this->m_pObjMgr->CheckDragEntitys( vecEntity, mouseDragData, this->GetObjectType() ) )
	{
		for ( int i = OBJ_TYPE_USER2; i <= OBJ_TYPE_USER2; ++i )
		{
			bEnemyDrag = this->m_pObjMgr->CheckDragEntitys( vecEntity, mouseDragData, (eObjectType)i );
		}
	}

	if ( !vecEntity.empty() )
	{
		if ( this->m_bDoubleClick && this->m_pCurCorps && this->m_pCurCorps->GetCurUnitNum() == 1 &&
			 !this->m_pCurCorps->GetEntity( 0 )->GetCheckUnitInformation( CGameEntity::Entity_Building ) )
		{
			CGameEntity* pEntity = this->m_pCurCorps->GetEntity( 0 );

			if ( vecEntity.front() == pEntity )
			{
				iActLevel = 1;
			}
		}

		if ( iActLevel == 0 )
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
				CBuilding* pBuilding = dynamic_cast<CBuilding*>(vecEntity[0]);
				if ( pBuilding )
					m_pUIMgr->ShowBuildingData( pBuilding );

				vecEntity[0]->SetUpdateOrderData( true );
				this->m_clickCorps.AddUnit( vecEntity[0] );
			}
			else
			{
				for ( size_t i = 0; i < vecEntity.size(); ++i )
					this->m_clickCorps.AddUnit( vecEntity[i] );
			}

			this->m_pCurCorps = &this->m_clickCorps;
		}
		else if ( iActLevel == 1 )
		{
			CGameEntity* pEntity = this->m_pCurCorps->GetEntity( 0 );
			vecEntity.clear();

			this->m_pObjMgr->GetSameEntitys( vecEntity, pEntity, 500.f, OBJ_TYPE_USER );


			auto SortEntityToPos = [&]( CGameEntity* _pDstEntity, CGameEntity* _pSrcEntity ) {
				return D3DXVec3Length( &(_pDstEntity->GetPos() - pEntity->GetPos()) ) > D3DXVec3Length( &(_pSrcEntity->GetPos() - pEntity->GetPos()) );
			};

			sort( vecEntity.begin(), vecEntity.end(), SortEntityToPos );

			size_t iLength = ((vecEntity.size() > 11) ? 11 : vecEntity.size());

			this->m_pCurCorps->ResetCorps();
			this->m_pCurCorps->AddUnit( pEntity );

			for ( size_t i = 0; i < iLength; ++i )
			{
				this->m_pCurCorps->AddUnit( vecEntity[i] );
			}

			this->m_pUIMgr->HideBuildingOrderUI();
		}

		this->DecideShowButton();
		this->ShowEntityUI();
		this->m_pCurCorps->GetEntity( 0 )->SoundPlay( CGameEntity::Sound_Click );
	}

	if ( this->m_bDoubleClick )
		this->m_bDoubleClick = false;
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
		if ( this->m_pObjMgr->CheckDragEntitys( vecEntity, tDragData, eObjectType( i ) ) )
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
	if ( !this->m_pCurCorps )
		return;

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
