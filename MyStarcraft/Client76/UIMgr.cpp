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


IMPLEMENT_SINGLETON(CUIMgr)


CUIMgr::CUIMgr()
{
}


CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::SetPlayer( CPlayer * _pPlayer )
{
	this->m_pPlayer = _pPlayer;
}

void CUIMgr::Initialize()
{
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

	/* 그 외의 변수들 초기화.. */
	this->m_bShowEntityUI = false;
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

		if ( !this->m_bShowCorpsWire )
		{
			int iEntityCurHp = m_pCurShowHpEntity->GetCurHp();
			int iEntityMaxHp = m_pCurShowHpEntity->GetMaxHp();
			int iCnt = 0;
			vector<BYTE> vecEntityHpFrame;

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
					this->m_byDrawHpUINum = iLength;
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
	}
}

void CUIMgr::Render()
{
	this->m_pMainUI->Render();

	if ( this->m_bShowEntityUI )
	{
		this->m_pFaceUI->Render();

		for ( size_t i = 0; i < this->m_vecDrawButton.size(); ++i )
		{
			m_vecDrawButton[i]->Render();
		}

		if ( this->m_bShowCorpsWire )
		{
			for ( int i = 0; i < this->m_byDrawUINum; ++i )
			{
				this->m_pCorpsWireFrameUIArr[i]->Render();
			}
		}
		else
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
}

void CUIMgr::Release()
{
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

	/* 와이어 프레임 설정.. */
	BYTE byCorpsEntityLength = _pCorps->GetCurUnitNum();
	if ( byCorpsEntityLength == 1 )
	{
		this->m_pEntityWireFrameUI->ChangeDrawTexture( _pCorps->GetEntity( 0 )->GetWireFrameKey().c_str(), L"Big", 6 );
		this->m_bShowCorpsWire = false;
		this->m_pCurShowHpEntity = pEntity;

		this->m_pShowUnitData = pEntity->GetSelectShowData();

		//365, 490
		float fNameX = 365.f - (this->m_pShowUnitData->wstrName.size() / 2) * 9.f;
		float fRankX = 365.f - (this->m_pShowUnitData->wstrRank.size() / 2) * 9.f;

		D3DXMatrixTranslation( &this->m_matDrawUnitName, fNameX, 490.f, 0.f );
		D3DXMatrixTranslation( &this->m_matDrawUnitRank, fRankX, 520.f, 0.f );


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

void CUIMgr::AddDeleteUIList( CUI *& _pDeleteUI )
{
	this->m_vecDeleteUI.push_back( _pDeleteUI );
}
