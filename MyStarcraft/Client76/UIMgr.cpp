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
	: m_bShowBuildOrderUI(false)
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

		this->m_rcUnitUpgradeCol[i].left =   (LONG)fX + i * (40 + 6);
		this->m_rcUnitUpgradeCol[i].top =    (LONG)fY;
		this->m_rcUnitUpgradeCol[i].right =  (LONG)fX + i * (40 + 6) + 36;
		this->m_rcUnitUpgradeCol[i].bottom = (LONG)fY + 36;

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

	/* Player's Resoruce Data UI.. */
	float fX = 528.f;
	float fY = 8.f;
	for ( int i = 0; i < 3; ++i )
	{
		this->m_pShowResourceDataUI[i] = new CUI;
		this->m_pShowResourceDataUI[i]->SetUIKind( CUI::UI_Normal );
		this->m_pShowResourceDataUI[i]->Initialize();
		this->m_pShowResourceDataUI[i]->SetPos( fX + i * (76 + 14), fY );
		this->AddDeleteUIList( this->m_pShowResourceDataUI[i] );
	}
	
	this->m_pShowResourceDataUI[0]->ChangeDrawTexture( L"Info_Mineral" );
	this->m_pShowResourceDataUI[1]->ChangeDrawTexture( L"Info_Gas" );
	this->m_pShowResourceDataUI[2]->ChangeDrawTexture( L"Info_Human" );

	/* Building Order UI initialize.. */
	fX = 300.f;
	fY = 500.f;
	this->m_pBuildingOrderUI = new CUI;
	this->m_pBuildingOrderUI->SetUIKind( CUI::UI_Normal );
	this->m_pBuildingOrderUI->Initialize();
	this->m_pBuildingOrderUI->ChangeDrawTexture( L"BuildingOrder" );
	this->m_pBuildingOrderUI->SetPos( fX, fY );
	this->AddDeleteUIList( this->m_pBuildingOrderUI );

	/* Building Order Icon UI Initialize.. */
	for ( int i = 0; i < 5; ++i )
	{
		this->m_pOrderIconUI[i] = new CUI;
		this->m_pOrderIconUI[i]->SetUIKind( CUI::UI_Normal );
		this->m_pOrderIconUI[i]->Initialize();
		if(i == 0 )
			this->m_pOrderIconUI[i]->SetPos( fX + 4, fY + 7 );
		else
			this->m_pOrderIconUI[i]->SetPos( fX + 4 + (i - 1) * 49, fY + 7 + 49 );

		this->AddDeleteUIList( this->m_pOrderIconUI[i] );
	}

	/* Order Ratio Texture Initialize.. */
	fX += 40.f;
	fY += 0.f;
	this->m_pRatioBackUI = new CUI;
	this->m_pRatioBackUI->SetUIKind( CUI::UI_Normal );
	this->m_pRatioBackUI->Initialize();
	this->m_pRatioBackUI->ChangeDrawTexture( L"ProgressBar" );
	this->m_pRatioBackUI->SetPos( fX, fY );
	this->AddDeleteUIList( this->m_pRatioBackUI );

	this->m_pRatioTexture = CTextureMgr::GetInstance()->GetTexture( L"ProgressGauge" );

	D3DXMatrixTranslation( &m_ratioTextureWorldMatrix, fX, fY, 0.f );


	/* 그 외의 변수들 초기화.. */
	this->m_pSprite = CDevice::GetInstance()->GetSprite();
	this->m_pFont = CDevice::GetInstance()->GetFont();

	this->m_bShowEntityUI = false;
	this->m_bDrawBuildingFrame = false;
}

void CUIMgr::Update()
{
	if ( this->m_pMinimap )
		this->m_pMinimap->Update();

	if ( this->m_bShowEntityUI )
	{
		this->m_pFaceUI->Update();

		for ( size_t i = 0; i < this->m_vecDrawButton.size(); ++i )
		{
			int iEvent = m_vecDrawButton[i]->Update();

			if ( iEvent == 1 )
			{
				const BUTTON_DATA* pButtonData = this->m_vecDrawButton[i]->GetButtonData();
				this->m_pPlayer->OrderActPattern( pButtonData );
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
			CGameEntity* pEntity = m_pCurShowUICorps->GetEntity( (BYTE)i );

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

				if ( j >= (size_t)iEmptyHpBarNum )
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
			if ( this->m_pCurShowHpEntity->GetIsDie() )
			{
				m_bShowEntityUI = false;
				return;
			}

			{
				/* 
				* Render Unit Data Check
				* When Unit Data Rect Hover
				*/
			}

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

	this->m_pBuildingOrderUI->Render();

	/* Building Frame Image Data Update.. */
	if ( this->m_bDrawBuildingFrame )
	{
		D3DXVECTOR3 vScrollPos = CGameObject::GetScroll();
		D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos();
		RECT rcCol = m_pDrawBuilding->GetColRect();
		vMousePos -= D3DXVECTOR3( float( int( vMousePos.x ) % TILECX ), float( int( vMousePos.y ) % TILECY ), 0.f ) -
			D3DXVECTOR3( float( rcCol.right % TILECX ), float( rcCol.bottom % TILECX ), 0.f );

		//vMousePos -= D3DXVECTOR3( float( int( vMousePos.x ) % TILECX ), float( int( vMousePos.y ) % TILECY ), 0.f ) -
		//	D3DXVECTOR3( float( rcCol.right % TILECX ), float( rcCol.bottom % TILECX ), 0.f );

		//vMousePos -= vScrollPos;

		D3DXMatrixTranslation( &this->m_buildingWorldMatrix, vMousePos.x, vMousePos.y, 0.f );

		rcCol.left +=   (LONG)vMousePos.x;
		rcCol.right +=  (LONG)vMousePos.x;
		rcCol.top +=    (LONG)vMousePos.y;
		rcCol.bottom += (LONG)vMousePos.y;

		this->m_rcBuilding = rcCol;
	}
}

void CUIMgr::Render()
{
	this->m_pMainUI->Render();

	this->m_pMinimap->Render();

	/* 플레이어의 자원 출력.. */
	this->RenderPlayerResourceData();

	if ( this->m_bShowEntityUI )
	{
		this->m_pFaceUI->Render();

		for ( size_t i = 0; i < this->m_vecDrawButton.size(); ++i )
		{
			m_vecDrawButton[i]->Render();
		}

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

				RECT _rc = this->m_rcUnitUpgradeCol[i];
				D3DXMATRIX matTrans, matScale, matWorld;

				D3DXVECTOR3 vPoint[4];

				vPoint[0] = D3DXVECTOR3( FLOAT(_rc.left), FLOAT(_rc.top), 0.f );
				vPoint[1] = D3DXVECTOR3( FLOAT(_rc.left), FLOAT(_rc.top), 0.f );
				vPoint[2] = D3DXVECTOR3( FLOAT(_rc.left), FLOAT(_rc.bottom), 0.f );
				vPoint[3] = D3DXVECTOR3( FLOAT(_rc.right), FLOAT(_rc.top), 0.f );

				for ( int i = 0; i < 4; ++i )
				{
					D3DXMatrixTranslation( &matTrans, vPoint[i].x, vPoint[i].y, vPoint[i].z );
					if ( i % 2 )
						D3DXMatrixScaling( &matScale, 1.f, FLOAT( _rc.bottom - _rc.top ), 1.f );
					else
						D3DXMatrixScaling( &matScale, FLOAT( _rc.right - _rc.left ), 1.f, 1.f );

					matWorld = matScale * matTrans;
					m_pSprite->SetTransform( &matWorld );
					m_pSprite->Draw(
						CTextureMgr::GetInstance()->GetTexture( L"Drag" )->pTexture,
						NULL,
						NULL,
						NULL,
						D3DCOLOR_ARGB( 255, 255, 255, 255 )
					);
				}

			}

			/* Entity 객체의 이름과 계급 출력.. */
			m_pSprite->SetTransform( &m_matDrawUnitName );

			m_pFont->DrawTextW(
				m_pSprite,
				m_pShowUnitData->wstrName.c_str(),
				lstrlen( m_pShowUnitData->wstrName.c_str() ),
				NULL,
				NULL,
				D3DCOLOR_ARGB( 255, 255, 255, 255 )
			);

			m_pSprite->SetTransform( &m_matDrawUnitRank );

			m_pFont->DrawTextW(
				m_pSprite,
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

			if ( m_bShowBuildOrderUI )
			{
				this->m_pBuildingOrderUI->Render();

				for ( size_t i = 0; i < this->m_iDrawOrderIconSize; ++i )
				{
					m_pOrderIconUI[i]->Render();
				}

				this->m_pRatioBackUI->Render();

				RECT rc = { 0, 0, this->m_pRatioTexture->ImageInfo.Width * 0.01f * this->m_byOrderRatio, this->m_pRatioTexture->ImageInfo.Height };

				this->m_pSprite->SetTransform( &this->m_ratioTextureWorldMatrix );
				this->m_pSprite->Draw( this->m_pRatioTexture->pTexture, &rc, NULL, NULL, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

		}
	}

	if ( this->m_bDrawBuildingFrame )
	{
		//this->m_rcUnitUpgradeCol[i]
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
		CGameEntity* pEntity = _pCorps->GetEntity( (BYTE)i );

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
		float fNameX = 390.f - (this->m_pShowUnitData->wstrName.size() / 2) * 5.f;
		float fRankX = 390.f - (this->m_pShowUnitData->wstrRank.size() / 2) * 5.f;

		D3DXMATRIX matScale, matTrans;

		D3DXMatrixScaling( &matScale, 0.5f, 0.5f, 1.f );

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
			this->m_pUnitUpgradeFontUI[0]->SetDrawIndex( this->m_pCurShowHpEntity->GetCommanData().iUpgradeDefense );

			vector<const TEX_INFO*> vecTexture;
			const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWire", pEntity->GetCommanData().nDefenceIconFrame );
			vecTexture.push_back( pTexture );

			this->m_pUnitUpgradeUI[0]->ChangeDrawTexture( vecTexture );
			vecTexture.clear();

			auto& tGroundAttData = pEntity->GetGroundAttackData();
			auto& tAirAttData = pEntity->GetAirAttackData();

			if ( tGroundAttData.pWeapon )
			{
				pTexture = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWire", tGroundAttData.pWeapon->GetWeaponData()->nIconFrame );
				vecTexture.push_back( pTexture );
				this->m_pUnitUpgradeUI[1]->ChangeDrawTexture( vecTexture );
				this->m_pUnitUpgradeFontUI[1]->SetDrawIndex( tGroundAttData.pWeapon->GetWeaponData()->iUpgradeNum );

				if ( tAirAttData.pWeapon &&
					 tAirAttData.pWeapon->GetWeaponData()->wstrWeaponName != tGroundAttData.pWeapon->GetWeaponData()->wstrWeaponName )
				{
					vecTexture.clear();
					pTexture = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWire", tAirAttData.pWeapon->GetWeaponData()->nIconFrame );
					vecTexture.push_back( pTexture );

					this->m_pUnitUpgradeUI[2]->ChangeDrawTexture( vecTexture );
					this->m_pUnitUpgradeFontUI[2]->SetDrawIndex( tAirAttData.pWeapon->GetWeaponData()->iUpgradeNum );
					this->m_byDrawUINum = 3;
				}
				else
					this->m_byDrawUINum = 2;
			}
			else if ( tAirAttData.pWeapon )
			{
				pTexture = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWire", tAirAttData.pWeapon->GetWeaponData()->nIconFrame );
				vecTexture.push_back( pTexture );

				this->m_pUnitUpgradeUI[1]->ChangeDrawTexture( vecTexture );
				this->m_pUnitUpgradeFontUI[1]->SetDrawIndex( tGroundAttData.pWeapon->GetWeaponData()->iUpgradeNum );
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

void CUIMgr::ShowBuildingOrderUI( const vector<SHORT>& _vecOrderIcon )
{
	if ( _vecOrderIcon.empty() )
	{
		this->HideBuildingOrderUI();
		return;
	}

	this->m_bShowBuildOrderUI = true;
	
	for ( size_t i = 0; i < _vecOrderIcon.size(); ++i )
	{
		if ( i >= 5 )
			break;

		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"Icon", _vecOrderIcon[i] );

		m_pOrderIconUI[i]->ChangeDrawTexture( pTexture );
	}

	this->m_iDrawOrderIconSize = _vecOrderIcon.size();

}

void CUIMgr::HideBuildingOrderUI()
{
	this->m_bShowBuildOrderUI = false;
}

void CUIMgr::UpdateOrderRatio( const BYTE & _byRatio )
{
	this->m_byOrderRatio = _byRatio;
}

void CUIMgr::UpdatePlayerResourceData( const wstring& _wstrChangeResourceData )
{
	if ( _wstrChangeResourceData == L"All" )
	{
		this->UpdatePlayerMineralData();
		this->UpdatePlayerGasData();
		this->UpdatePlayerPopulationData();
	}
	else if ( _wstrChangeResourceData == L"Mineral" )
	{
		this->UpdatePlayerMineralData();
	}
	else if ( _wstrChangeResourceData == L"Gas" )
	{
		this->UpdatePlayerGasData();
	}
	else if ( _wstrChangeResourceData == L"Population" )
	{
		this->UpdatePlayerPopulationData();
	}
	else
	{
		ERROR_MSG( L"UIMgr UpdatePlayerResourceData Wrong Parameter" );
		return;
	}
}

void CUIMgr::UpdatePlayerMineralData()
{
	CPlayer::RESOURCE_DATA tResourceData = this->m_pPlayer->GetResourceData();

	int iTempMineral = tResourceData.iMineral;

	auto& vecCheck = m_vecResourceShowFontData[0];

	vecCheck.clear();

	vector<BYTE> vecTempMineralFontIndex;

	while ( true )
	{
		vecTempMineralFontIndex.push_back( iTempMineral % 10 );
		iTempMineral /= 10;

		if ( iTempMineral == 0 )
			break;
	}

	D3DXVECTOR3 vStart = this->m_pShowResourceDataUI[0]->GetPos() + D3DXVECTOR3( 10.f + 14.f, 4.f, 0.f );
	size_t iLength = vecTempMineralFontIndex.size();
	D3DXMATRIX matWorld;
	for ( size_t i = 0; i < iLength; ++i )
	{
		D3DXMatrixTranslation( &matWorld, vStart.x + i * (2 + 4), vStart.y, 0.f );

		vecCheck.push_back( make_pair( vecTempMineralFontIndex[iLength - (i + 1)], matWorld ) );
	}

}

void CUIMgr::UpdatePlayerGasData()
{
	CPlayer::RESOURCE_DATA tResourceData = this->m_pPlayer->GetResourceData();

	int iTempGas = tResourceData.iGas;

	auto& vecCheck = m_vecResourceShowFontData[1];

	vecCheck.clear();

	vector<BYTE> vecTempGasFontIndex;

	while ( true )
	{
		vecTempGasFontIndex.push_back( iTempGas % 10 );
		iTempGas /= 10;

		if ( iTempGas == 0 )
			break;
	}

	D3DXVECTOR3 vStart = this->m_pShowResourceDataUI[1]->GetPos() + D3DXVECTOR3( 10.f + 14.f, 4.f, 0.f );
	size_t iLength = vecTempGasFontIndex.size();
	D3DXMATRIX matWorld;
	for ( size_t i = 0; i < iLength; ++i )
	{
		D3DXMatrixTranslation( &matWorld, vStart.x + i * (2 + 4), vStart.y, 0.f );

		vecCheck.push_back( make_pair( vecTempGasFontIndex[iLength - (i + 1)], matWorld ) );
	}
}

void CUIMgr::UpdatePlayerPopulationData()
{
	CPlayer::RESOURCE_DATA tResourceData = this->m_pPlayer->GetResourceData();

	BYTE byTempMaxPopulation = tResourceData.byTotalPopulation;
	BYTE byTempCurPopulation = tResourceData.byCurPopulation;

	auto& vecCheck = m_vecResourceShowFontData[2];

	vecCheck.clear();

	vector<BYTE> vecTempPopulationFontIndex;

	while ( true )
	{
		vecTempPopulationFontIndex.push_back( byTempMaxPopulation % 10 );
		byTempMaxPopulation /= 10;
		
		if ( byTempMaxPopulation == 0 )
		{
			vecTempPopulationFontIndex.push_back( 10 );
			break;
		}
	}

	while ( true )
	{
		vecTempPopulationFontIndex.push_back( byTempCurPopulation % 10 );
		byTempCurPopulation /= 10;

		if ( byTempCurPopulation == 0 )
			break;
	}

	D3DXVECTOR3 vStart = this->m_pShowResourceDataUI[2]->GetPos() + D3DXVECTOR3( 10.f + 14.f, 4.f, 0.f );
	size_t iLength = vecTempPopulationFontIndex.size();
	D3DXMATRIX matWorld;
	for ( size_t i = 0; i < iLength; ++i )
	{
		D3DXMatrixTranslation( &matWorld, vStart.x + i * (2 + 4), vStart.y, 0.f );

		vecCheck.push_back( make_pair( vecTempPopulationFontIndex[iLength - (i + 1)], matWorld ) );
	}
}

void CUIMgr::AddDeleteUIList( CUI *& _pDeleteUI )
{
	this->m_vecDeleteUI.push_back( _pDeleteUI );
}

void CUIMgr::RenderPlayerResourceData()
{
	for ( int i = 0; i < 3; ++i )
	{
		this->m_pShowResourceDataUI[i]->Render();

		for ( size_t j = 0; j < this->m_vecResourceShowFontData[i].size(); ++j )
		{
			auto& showFontData = m_vecResourceShowFontData[i][j];
			m_pEntityHpUI->Render( showFontData.first, showFontData.second );
			//m_vecResourceShowFontData[i][j]
			//this->m_pfont
		}
	}

}
