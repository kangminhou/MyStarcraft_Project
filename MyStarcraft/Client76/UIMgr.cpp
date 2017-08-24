#include "stdafx.h"
#include "UIMgr.h"

#include "TextureMgr.h"
#include "Device.h"

#include "UI.h"
#include "GameEntity.h"
#include "Corps.h"
#include "Weapon.h"
#include "MyButton.h"
#include "Player.h"
#include "MiniMap.h"

#include "Building.h"
#include "Mouse.h"


IMPLEMENT_SINGLETON(CUIMgr)


CUIMgr::CUIMgr()
{
}


CUIMgr::~CUIMgr()
{
	Release();
}

CUI * CUIMgr::GetHpBarBackUI() const
{
	return this->m_pHpBarBackUI;
}

CUI * CUIMgr::GetHpBarUI() const
{
	return this->m_pHpBarUI;
}

CMiniMap * CUIMgr::GetMiniMap() const
{
	return this->m_pMinimap;
}

void CUIMgr::SetPlayer( CPlayer * _pPlayer )
{
	this->m_pPlayer = _pPlayer;
}

void CUIMgr::Initialize()
{
	this->m_pMinimap = new CMiniMap;
	this->m_pMinimap->Initialize();
	this->m_pMinimap->SetPos( 8.f + 80.f, 435.f + 80.f );

	/* Main UI.. */
	this->m_pMainUI = new CUI;
	this->m_pMainUI->SetUIKind( CUI::UI_Normal );
	this->m_pMainUI->ChangeDrawTexture( L"MainUi" );
	this->m_pMainUI->Initialize();
	this->m_pMainUI->SetPos( 0.f, 0.f );
	this->AddDeleteUIList( this->m_pMainUI );

	/* 유닛 얼굴 UI.. */
	this->m_pFaceUI = new CUI;
	this->m_pFaceUI->SetUIKind( CUI::UI_Anim_Loop );
	this->m_pFaceUI->Initialize();
	this->m_pFaceUI->AddFrame( L"Idle", FRAME( 0.f, 0.f, 0.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pFaceUI->SetPos( 521.f, 521.f );
	this->AddDeleteUIList( this->m_pFaceUI );

	/* 와이어 프레임 UI.. */
	for ( int i = 0; i < 12; ++i )
	{
		this->m_pCorpsWireFrameUIArr[i] = new CUI;
		this->m_pCorpsWireFrameUIArr[i]->SetUIKind( CUI::UI_Normal );
		this->m_pCorpsWireFrameUIArr[i]->Initialize();
		this->m_pCorpsWireFrameUIArr[i]->SetPos( 224.f + (8 + 32) * (i / 2), 498.f + (12 + 32) * (i % 2) );
		this->AddDeleteUIList( this->m_pCorpsWireFrameUIArr[i] );
	}

	this->m_pEntityWireFrameUI = new CUI;
	this->m_pEntityWireFrameUI->SetUIKind( CUI::UI_Normal );
	this->m_pEntityWireFrameUI->Initialize();
	this->m_pEntityWireFrameUI->SetPos( 224.f, 498.f );
	this->AddDeleteUIList( this->m_pEntityWireFrameUI );

	/* 유닛 공격력, 방어력 관련.. */
	for ( int i = 0; i < 3; ++i )
	{
		float fX = 322.f;
		float fY = 550.f;

		this->m_pUnitUpgradeUI[i] = new CUI;
		this->m_pUnitUpgradeUI[i]->SetUIKind( CUI::UI_Normal );
		this->m_pUnitUpgradeUI[i]->Initialize();
		this->m_pUnitUpgradeUI[i]->SetPos( (fX + 2) + i * (40 + 6), fY  + 2);
		this->AddDeleteUIList( this->m_pUnitUpgradeUI[i] );

		this->m_pUnitUpgradeBackUI[i] = new CUI;
		this->m_pUnitUpgradeBackUI[i]->SetUIKind( CUI::UI_Normal );
		this->m_pUnitUpgradeBackUI[i]->Initialize();
		this->m_pUnitUpgradeBackUI[i]->ChangeDrawTexture( L"UnitUpgradeBack" );
		this->m_pUnitUpgradeBackUI[i]->SetPos( fX + i * (40 + 6), fY );
		this->AddDeleteUIList( this->m_pUnitUpgradeBackUI[i] );

		this->m_pUnitUpgradeFontUI[i] = new CUI;
		this->m_pUnitUpgradeFontUI[i]->SetUIKind( CUI::UI_Normal );
		this->m_pUnitUpgradeFontUI[i]->Initialize();
		this->m_pUnitUpgradeFontUI[i]->ChangeDrawTexture( L"Font", L"Shield", 11 );
		this->m_pUnitUpgradeFontUI[i]->SetPos( (fX + 26) + i * (40 + 6), (fY + 26) );
		this->AddDeleteUIList( this->m_pUnitUpgradeFontUI[i] );
	}

	/* 유닛 HP UI.. */
	this->m_pEntityHpUI = new CUI;
	this->m_pEntityHpUI->SetUIKind( CUI::UI_Normal );
	this->m_pEntityHpUI->Initialize();
	this->m_pEntityHpUI->ChangeDrawTexture( L"Font", L"Normal", 11 );
	this->AddDeleteUIList( this->m_pEntityHpUI );

	for ( int i = 0; i < 9; ++i )
	{
		CMyButton* pButton = new CMyButton;
		pButton->Initialize();
		pButton->SetPos( 640.f + (i % 3) * (40 + 17), 455.f + (i / 3) * (40 + 10) );
		this->m_vecButton.push_back( pButton );
	}

	/* 유닛 Hp Bar UI.. */
	this->m_pHpBarBackUI = new CUI;
	this->m_pHpBarBackUI->SetUIKind( CUI::UI_Normal );
	this->m_pHpBarBackUI->Initialize();
	this->m_pHpBarBackUI->ChangeDrawTexture( L"HpBar", L"Back", 1 );
	this->m_pHpBarBackUI->SetShowCenterPos( true );
	this->AddDeleteUIList( this->m_pHpBarBackUI );

	this->m_pHpBarUI = new CUI;
	this->m_pHpBarUI->SetUIKind( CUI::UI_Normal );
	this->m_pHpBarUI->Initialize();
	this->m_pHpBarUI->ChangeDrawTexture( L"HpBar", L"Front", 4 );
	this->AddDeleteUIList( this->m_pHpBarUI );

	/* 그 외의 변수들 초기화.. */
	this->m_bShowEntityUI = false;
	this->m_bDrawBuildingFrame = false;
}

void CUIMgr::Update()
{
	if ( this->m_bShowEntityUI )
	{
		this->m_pFaceUI->Update();

		for ( size_t i = 0; i < this->m_vecDrawButton.size(); ++i )
		{
			int iEvent = m_vecDrawButton[i]->Update();

			if ( iEvent == 1 )
			{

			}
			else if ( iEvent == 2 )
			{
				const BUTTON_DATA* pButtonData = this->m_vecDrawButton[i]->GetButtonData();
				this->m_pPlayer->OrderActPattern( pButtonData );
			}
		}

		/* Hp Bar UI 데이터 갱신.. */
		for ( size_t i = 0; i < this->m_vecHpBarMatrixData.size(); ++i )
		{
			CGameEntity* pEntity = m_pCurShowUICorps->GetEntity( i );

			if ( !pEntity )
				continue;

			float fEntityCurHp = pEntity->GetCurHp();
			float fEntityMaxHp = pEntity->GetMaxHp();
			float fHpRatio = fEntityCurHp / fEntityMaxHp;

			size_t jLength = this->m_vecHpBarMatrixData[i].vecHpBarData.size();
			int iEmptyHpBarNum = int(fHpRatio * jLength);
			float fHpBarBackSize = (float)(jLength * 6 + 2);
			D3DXVECTOR3 vEntityPos = pEntity->GetPos() - CGameObject::GetScroll();
			D3DXVECTOR3 vHpBarBackStartPos( vEntityPos.x - fHpBarBackSize * 0.5f, vEntityPos.y + pEntity->GetImageSize().y * 0.3f, 0.f );

			if ( iEmptyHpBarNum == 0 )
				iEmptyHpBarNum = 1;

			if ( pEntity->GetCheckUnitInformation( CGameEntity::Entity_Building ) )
				vHpBarBackStartPos.y = vEntityPos.y + pEntity->GetImageSize().y * 0.5f;

			m_vecHpBarMatrixData[i].hpBarBackWordMatrix._41 = vHpBarBackStartPos.x + fHpBarBackSize * 0.5f;
			m_vecHpBarMatrixData[i].hpBarBackWordMatrix._42 = vHpBarBackStartPos.y + 5;
			m_vecHpBarMatrixData[i].hpBarBackWordMatrix._43 = 0.f;
			
			for ( size_t j = 0; j < jLength; ++j )
			{
				int iHpBarTextureIndex = 0;

				if ( j >= iEmptyHpBarNum )
					iHpBarTextureIndex = 3;
				else if ( fHpRatio <= HpUI_Level3 )
					iHpBarTextureIndex = 2;
				else if ( fHpRatio <= HpUI_Level2 )
					iHpBarTextureIndex = 1;

				D3DXMATRIX matTrans;
				D3DXMatrixTranslation( &matTrans, vHpBarBackStartPos.x + 2 + j * 6, vHpBarBackStartPos.y + 2.f, 0.f );
				this->m_vecHpBarMatrixData[i].vecHpBarData[j] = make_pair( iHpBarTextureIndex, matTrans );
			}

			pEntity->SetHpBarData( this->m_vecHpBarMatrixData[i] );

		}

		if ( !this->m_bShowCorpsWire )
		{
			int iEntityCurHp = (int)m_pCurShowHpEntity->GetCurHp();
			int iEntityMaxHp = (int)m_pCurShowHpEntity->GetMaxHp();
			int iCnt = 0;
			vector<BYTE> vecEntityHpFrame;

			int iWireIndex = 5 - int(float(iEntityCurHp) / float(iEntityMaxHp) * 5);
			this->m_pEntityWireFrameUI->SetDrawIndex( iWireIndex );

			while ( true )
			{
				vecEntityHpFrame.push_back( iEntityMaxHp % 10 );
				iEntityMaxHp /= 10;

				if ( iEntityMaxHp == 0 )
				{
					vecEntityHpFrame.push_back( 10 );
					break;
				}
			}

			while ( true )
			{
				vecEntityHpFrame.push_back( iEntityCurHp % 10 );
				iEntityCurHp /= 10;

				if ( iEntityCurHp == 0 )
				{
					this->m_vecHpShowData.clear();
					size_t iLength = vecEntityHpFrame.size();
					this->m_byDrawHpUINum = (BYTE)iLength;
					float fStartX = 250.f - float(iLength / 2) * 4.f;
					for ( size_t i = 0; i < iLength; ++i )
					{
						D3DXMATRIX matWorld;
						D3DXMatrixTranslation( &matWorld, fStartX + 4.f * i, 570.f, 0.f );
						this->m_vecHpShowData.push_back( pair<BYTE, D3DXMATRIX>( vecEntityHpFrame[iLength - 1 - i], matWorld ) );
					}
					break;
				}
			}
		}
		else
		{
			for ( int i = 0; i < this->m_byDrawUINum; ++i )
			{
				CGameEntity* pEntity = this->m_pCurShowUICorps->GetEntity( i );
				if ( !pEntity )
					continue;

				float fEntityCurHp = pEntity->GetCurHp();
				float fEntityMaxHp = pEntity->GetMaxHp();
				int iWireIndex = 5 - int(fEntityCurHp / fEntityMaxHp * 5);
				this->m_pCorpsWireFrameUIArr[i]->SetDrawIndex( iWireIndex );
			}
		}
	}

	if ( this->m_bDrawBuildingFrame )
	{
		D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos();
		RECT rcCol = m_pDrawBuilding->GetColRect();
		vMousePos -= D3DXVECTOR3( float( int( vMousePos.x ) % TILECX ), float( int( vMousePos.y ) % TILECY ), 0.f ) -
			D3DXVECTOR3( float( rcCol.right % TILECX ), float( rcCol.bottom % TILECX ), 0.f );
		D3DXMatrixTranslation( &this->m_buildingWorldMatrix, vMousePos.x, vMousePos.y, 0.f );

		rcCol.left += (LONG)vMousePos.x;
		rcCol.right += (LONG)vMousePos.x;
		rcCol.top += (LONG)vMousePos.y;
		rcCol.bottom += (LONG)vMousePos.y;

		this->m_rcBuilding = rcCol;
	}
}

void CUIMgr::Render()
{
	this->m_pMainUI->Render();

	this->m_pMinimap->Render();

	if ( this->m_bShowEntityUI )
	{
		this->m_pFaceUI->Render();

		for ( size_t i = 0; i < this->m_vecDrawButton.size(); ++i )
		{
			m_vecDrawButton[i]->Render();
		}

		//for ( size_t i = 0; i < this->m_vecHpBarMatrixData.size(); ++i )
		//{
		//	this->m_pHpBarBackUI->Render( 0, this->m_vecHpBarMatrixData[i].hpBarBackWorldMatrix );
		//	for ( size_t j = 0; j < this->m_vecHpBarMatrixData[i].vecHpBarData.size(); ++j )
		//	{
		//		this->m_pHpBarUI->Render( 
		//			this->m_vecHpBarMatrixData[i].vecHpBarData[j].first, this->m_vecHpBarMatrixData[i].vecHpBarData[j].second );
		//	}
		//}

		if ( this->m_bShowCorpsWire ) // 부대 ( 2명 이상의 유닛 ) UI 출력..
		{
			for ( int i = 0; i < this->m_byDrawUINum; ++i )
			{
				this->m_pCorpsWireFrameUIArr[i]->Render();
			}
		}
		else // 1명의 유닛 UI 출력..
		{
			this->m_pEntityWireFrameUI->Render();

			/* Entity 객체의 업그레이드 상황 출력.. */
			for ( int i = 0; i < this->m_byDrawUINum; ++i )
			{
				this->m_pUnitUpgradeUI[i]->Render();
				this->m_pUnitUpgradeBackUI[i]->Render();
				this->m_pUnitUpgradeFontUI[i]->Render();
			}

			/* Entity 객체의 이름과 계급 출력.. */
			CDevice::GetInstance()->GetSprite()->SetTransform( &m_matDrawUnitName );

			CDevice::GetInstance()->GetFont()->DrawTextW(
				CDevice::GetInstance()->GetSprite(),
				m_pShowUnitData->wstrName.c_str(),
				lstrlen( m_pShowUnitData->wstrName.c_str() ),
				NULL,
				NULL,
				D3DCOLOR_ARGB( 255, 255, 255, 255 )
			);

			CDevice::GetInstance()->GetSprite()->SetTransform( &m_matDrawUnitRank );

			CDevice::GetInstance()->GetFont()->DrawTextW(
				CDevice::GetInstance()->GetSprite(),
				m_pShowUnitData->wstrRank.c_str(),
				lstrlen( m_pShowUnitData->wstrRank.c_str() ),
				NULL,
				NULL,
				D3DCOLOR_ARGB( 255, 255, 255, 255 )
			);

			for ( int i = 0; i < this->m_byDrawHpUINum; ++i )
			{
				this->m_pEntityHpUI->Render( this->m_vecHpShowData[i].first, this->m_vecHpShowData[i].second );
			}
		}
	}

	if ( this->m_bDrawBuildingFrame )
	{
		this->m_pDrawBuilding->RectRender( this->m_rcBuilding );
		this->m_pDrawBuilding->FrameRender( this->m_buildingWorldMatrix );
	}
}

void CUIMgr::Release()
{
	safe_delete( this->m_pMinimap );

	for ( size_t i = 0; i < m_vecDeleteUI.size(); ++i )
	{
		safe_delete( m_vecDeleteUI[i] );
	}

	for ( size_t i = 0; i < m_vecButton.size(); ++i )
	{
		safe_delete( m_vecButton[i] );
	}

	this->m_vecButton.clear();
	this->m_vecHpShowData.clear();
}

void CUIMgr::ShowEntityUI( CCorps* _pCorps )
{
	if ( !m_vecHpBarShowEntity.empty() )
	{
		for ( size_t i = 0; i < m_vecHpBarShowEntity.size(); ++i )
		{
			m_vecHpBarShowEntity[i]->OffRenderHpBar();
		}

		m_vecHpBarShowEntity.clear();
	}

	m_pCurShowUICorps = _pCorps;

	if ( _pCorps->GetCurUnitNum() <= 0 )
	{
		this->HideEntityUI();
		return;
	}
	/* Face UI 설정.. */
	CGameEntity* pEntity = _pCorps->GetEntity( 0 );
	int iFaceAniNum = pEntity->GetFaceFrameNum();

	this->m_pFaceUI->DecideFrame( L"Idle", FRAME( 0.f, (float)(iFaceAniNum), (float)(iFaceAniNum), 0.f ), CAnimation::Anim_Loop );
	this->m_pFaceUI->ChangeDrawTexture( pEntity->GetFaceKey().c_str(), L"Idle", iFaceAniNum );

	BYTE byCorpsEntityLength = _pCorps->GetCurUnitNum();

	/* HP Bar 설정.. */
	this->m_vecHpBarMatrixData.clear();
	this->m_vecHpBarMatrixData.resize( byCorpsEntityLength );

	for ( size_t i = 0; i < this->m_vecHpBarMatrixData.size(); ++i )
	{
		CGameEntity* pEntity = _pCorps->GetEntity( i );

		if ( !pEntity )
			continue;

		float fHpBarSize = pEntity->GetImageSize().x / 10.f;
		float fHpBarBackSize = (float)(((int)fHpBarSize) * 6 + 2);

		D3DXMatrixScaling( &m_vecHpBarMatrixData[i].hpBarBackWordMatrix, fHpBarBackSize * 0.5f, 1.f, 1.f );

		this->m_vecHpBarMatrixData[i].vecHpBarData.resize( (size_t)fHpBarSize );

		//float fHpBarSize = pEntity->GetImageSize().x / 10.f;
		//float fHpBarBackSize = (float)(((int)fHpBarSize) * 6 + 2);
		//D3DXMATRIX matScale;
		//D3DXVECTOR3 vEntityPos = pEntity->GetPos() - CGameObject::GetScroll();
		//D3DXVECTOR3 vHpBarBackStartPos( vEntityPos.x - fHpBarBackSize * 0.5f, vEntityPos.y + pEntity->GetImageSize().y * 0.3f, 0.f );
		//
		//D3DXMatrixScaling( &matScale, fHpBarBackSize * 0.5f, 1.f, 1.f );
		//
		//if ( pEntity->GetCheckUnitInformation( CGameEntity::Entity_Building ) )
		//	vHpBarBackStartPos.y = vEntityPos.y + pEntity->GetImageSize().y * 0.5f;
		//
		//m_vecHpBarMatrixData[i].hpBarBackData.second = matScale;
		//m_vecHpBarMatrixData[i].hpBarBackData.second._41 = vHpBarBackStartPos.x + fHpBarBackSize * 0.5f;
		//m_vecHpBarMatrixData[i].hpBarBackData.second._42 = vHpBarBackStartPos.y + 5;
		//m_vecHpBarMatrixData[i].hpBarBackData.second._43 = 0.f;
		//
		//for ( size_t j = 0; j < (size_t)fHpBarSize; ++j )
		//{
		//	D3DXMATRIX matTrans;
		//	D3DXMatrixTranslation( &matTrans, vHpBarBackStartPos.x + 2 + j * 6, vHpBarBackStartPos.y + 2.f, 0.f );
		//	this->m_vecHpBarMatrixData[i].vecHpBarData.push_back( make_pair(0, matTrans) );
		//}

		pEntity->OnRenderHpBar();
		m_vecHpBarShowEntity.push_back( pEntity );

	}

	/* 와이어 프레임 설정.. */
	if ( byCorpsEntityLength == 1 )
	{
		this->m_pEntityWireFrameUI->ChangeDrawTexture( _pCorps->GetEntity( 0 )->GetWireFrameKey().c_str(), L"Big", 6 );
		this->m_bShowCorpsWire = false;
		this->m_pCurShowHpEntity = pEntity;

		this->m_pShowUnitData = pEntity->GetSelectShowData();

		//365, 490
		float fNameX = 365.f - (this->m_pShowUnitData->wstrName.size() / 2) * 5.f;
		float fRankX = 365.f - (this->m_pShowUnitData->wstrRank.size() / 2) * 5.f;

		D3DXMATRIX matScale, matTrans;

		D3DXMatrixScaling( &matScale, 0.7f, 0.7f, 1.f );

		D3DXMatrixTranslation( &matTrans, fNameX, 490.f, 0.f );
		this->m_matDrawUnitName = matScale * matTrans;

		D3DXMatrixTranslation( &matTrans, fRankX, 520.f, 0.f );
		this->m_matDrawUnitRank = matScale * matTrans;


		if ( pEntity->GetCheckUnitInformation( CGameEntity::Entity_Building ) )
		{
			this->m_byDrawUINum = 0;
		}
		else
		{
			this->m_pUnitUpgradeFontUI[0]->SetDrawIndex( 0 );
			this->m_pUnitUpgradeFontUI[1]->SetDrawIndex( 0 );

			vector<const TEX_INFO*> vecTexture;
			const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWire", pEntity->GetCommanData().nDefenceIconFrame );
			vecTexture.push_back( pTexture );

			this->m_pUnitUpgradeUI[0]->ChangeDrawTexture( vecTexture );
			vecTexture.clear();

			if ( pEntity->GetGroundAttackData().pWeapon )
			{
				pTexture = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWire", pEntity->GetGroundAttackData().pWeapon->GetWeaponData()->nIconFrame );
				vecTexture.push_back( pTexture );
				this->m_pUnitUpgradeUI[1]->ChangeDrawTexture( vecTexture );

				if ( pEntity->GetAirAttackData().pWeapon &&
					 pEntity->GetAirAttackData().pWeapon->GetWeaponData()->wstrWeaponName != pEntity->GetGroundAttackData().pWeapon->GetWeaponData()->wstrWeaponName )
				{
					vecTexture.clear();
					pTexture = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWire", pEntity->GetAirAttackData().pWeapon->GetWeaponData()->nIconFrame );
					vecTexture.push_back( pTexture );

					this->m_pUnitUpgradeUI[2]->ChangeDrawTexture( vecTexture );
					this->m_byDrawUINum = 3;
				}
				else
					this->m_byDrawUINum = 2;
			}
			else if ( pEntity->GetAirAttackData().pWeapon )
			{
				pTexture = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWire", pEntity->GetAirAttackData().pWeapon->GetWeaponData()->nIconFrame );
				vecTexture.push_back( pTexture );

				this->m_pUnitUpgradeUI[1]->ChangeDrawTexture( vecTexture );
				this->m_byDrawUINum = 2;
			}
			else
				m_byDrawUINum = 1;
		}
	}
	else if ( byCorpsEntityLength > 1 )
	{
		for ( int i = 0; i < byCorpsEntityLength; ++i )
		{
			this->m_pCorpsWireFrameUIArr[i]->ChangeDrawTexture( _pCorps->GetEntity( i )->GetWireFrameKey().c_str(), L"Small", 6 );
		}

		this->m_byDrawUINum = byCorpsEntityLength;
		this->m_bShowCorpsWire = true;
	}

	this->m_bShowEntityUI = true;
}

void CUIMgr::ShowButton( vector<BUTTON_DATA*>* pVecButton )
{
	size_t iLength = pVecButton->size();
	BYTE byFullVecButtonData = 0;

	this->m_vecDrawButton.clear();

	for ( size_t i = 0; i < iLength; ++i )
	{
		int iDecideIndex = -1;
		BYTE byCheckIndex = (*pVecButton)[i]->byDrawColumn;
		if ( byCheckIndex == 3 )
			iDecideIndex = 8;
		else
		{
			for ( byte j = byCheckIndex * 3; j < byCheckIndex * 3 + 3; ++j )
			{
				if ( byFullVecButtonData & (1 << j) )
					continue;

				iDecideIndex = j;
				byFullVecButtonData |= (1 << j);
				break;
			}
		}

		if ( iDecideIndex != -1 )
		{
			this->m_vecButton[iDecideIndex]->SetButtonData( (*pVecButton)[i] );
			this->m_vecDrawButton.push_back( this->m_vecButton[iDecideIndex] );
		}
	}

	int a = 10;
}

void CUIMgr::HideEntityUI()
{
	this->m_bShowEntityUI = false;
}

void CUIMgr::ShowFrameBuilding( CBuilding* _pBuilding )
{
	this->m_bDrawBuildingFrame = true;
	this->m_pDrawBuilding = _pBuilding;
}

void CUIMgr::HideFrameBuilding()
{
	this->m_bDrawBuildingFrame = false;
}

void CUIMgr::AddDeleteUIList( CUI *& _pDeleteUI )
{
	this->m_vecDeleteUI.push_back( _pDeleteUI );
}
