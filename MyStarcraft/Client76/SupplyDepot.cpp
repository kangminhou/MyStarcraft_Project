#include "stdafx.h"
#include "SupplyDepot.h"

#include "TextureMgr.h"
#include "Animation.h"
#include "EntityMgr.h"

#include "Move.h"
#include "Pattern_Building_Build.h"
#include "Pattern_Die.h"
#include "UIMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "Background.h"


CSupplyDepot::CSupplyDepot()
{
}


CSupplyDepot::~CSupplyDepot()
{
}

HRESULT CSupplyDepot::Initialize( void )
{
	this->AddSound( L"tpgwht00.wav", CGameEntity::Sound_Click );


	this->m_wstrStateKey = L"Depot";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = 500.f;
	this->m_tInfoData.fCurHp = 0.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = 0.f;
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 0;

	this->m_wstrFaceKey = L"FaceBuiding";
	this->m_wstrWireFrameKey = L"Depot";

	this->m_byFaceFrameNum = 45;

	//RECT tRect = { -8, -9, 8, 10 };
	RECT tRect = { -48, -32, 48, 32 };
	this->m_tOriginColRect = tRect;

	CBuilding::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 7 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 7 );	

	return S_OK;
}

int CSupplyDepot::Update( void )
{
	return CBuilding::Update();
}

void CSupplyDepot::Render( void )
{
	CBuilding::Render();

	if ( this->m_bSuccessBuild && this->m_pAnimCom )
	{
		const FRAME* pCurFrame = this->m_pAnimCom->GetCurAnimation();
		if ( pCurFrame->fIndex < this->m_vecWindmillTexture.size() )
		{
			const TEX_INFO* pTexture = m_vecWindmillTexture[(unsigned int)pCurFrame->fIndex];
			D3DXVECTOR3 vCenter( pTexture->ImageInfo.Width * 0.5f, pTexture->ImageInfo.Height * 0.5f, 0.f );
			this->DrawTexture( pTexture, this->GetWorldMatrix(), vCenter );
		}
	}

	this->DrawRect( this->m_tColRect );
}

void CSupplyDepot::Release( void )
{
}

void CSupplyDepot::UpdatePosition( const D3DXVECTOR3 & vPrevPos )
{
	CGameObject::UpdatePosition( vPrevPos );
}

void CSupplyDepot::SuccessBuild()
{
	BYTE byTotalPopulation = this->m_pPlayer->GetTotalPopulation();

	//cout << "Population Up" << endl;

	byTotalPopulation += 8;

	if ( byTotalPopulation > 200 )
		byTotalPopulation = 200;

	this->m_pPlayer->SetTotalPopulation( byTotalPopulation );
	this->m_pAnimCom->ChangeAnimation( L"Windmill" );
	this->m_vecWindmillTexture = this->m_mapAllTexture.find( L"Idle" )->second;

	CBuilding::SuccessBuild();
}

void CSupplyDepot::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern )
{
	if ( m_curActPatternKind == CGameEntity::Pattern_Die )
		return;

	bool bChangeAnimation = false;

	switch ( _ePatternKind )
	{
		case CGameEntity::Pattern_Idle:
		{
			if ( !m_bSuccessBuild )
			{
				this->m_pCurActPattern = this->m_mapPatterns.find( L"Build" )->second;
				this->m_vecTexture = this->m_mapAllTexture.find( L"Build" )->second;
			}
			else
				this->m_pCurActPattern = NULL;
		}
		break;

		case CGameEntity::Pattern_Die:
		{
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Die" )->second;;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Die" );
			this->m_wstrStateKey = L"Die";

			if ( this->m_pEntityBelongToCorps )
				this->m_pEntityBelongToCorps->EraseUnit( this );
		}
		break;

	}

	if ( this->m_pCurActPattern )
	{
		this->m_pCurActPattern->SetPrevPattern( _bPrevPattern );
		this->m_pCurActPattern->OnEnable();
	}

	this->m_curActPatternKind = _ePatternKind;

	if ( bChangeAnimation )
		this->ChangeLookAnimTexture();
}

void CSupplyDepot::DecideTexture()
{
	if ( !this->m_bSuccessBuild )
	{
		int iIndex = int(this->m_tInfoData.fCurHp / this->m_tInfoData.fMaxHp * (this->m_vecTexture.size()));
		if ( iIndex > 0 )
			--iIndex;
		this->m_pCurDrawTexture = this->m_vecTexture[iIndex];
	}
	else
		this->m_pCurDrawTexture = this->m_vecTexture[m_vecTexture.size() - 1];
}

void CSupplyDepot::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Windmill", FRAME( 0.f, 6.f, 6.f, 0.f ), CAnimation::Anim_Loop );
}

void CSupplyDepot::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Build", new CPattern_Building_Build ) );
	this->m_mapPatterns.insert( make_pair( L"Die", new CPattern_Die ) );
}

void CSupplyDepot::InitTexture()
{
	vector<const TEX_INFO*> vecTexture;

	/* Build Texture.. */
	for ( int i = 0; i < 3; ++i )
	{
		vecTexture.push_back( m_pBasicBuildTextureArr[1][i] ); 
	}

	for ( int i = 0; i < 2; ++i )
	{
		vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture( this->GetObjKey().c_str(), this->m_wstrStateKey.c_str(), i ) );
	}

	m_mapAllTexture.insert( make_pair( L"Build", vecTexture ) );

	vecTexture.clear();
	for ( int i = 2; i < 8; ++i )
	{
		vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture( this->GetObjKey().c_str(), this->m_wstrStateKey.c_str(), i ) );
	}

	m_mapAllTexture.insert( make_pair( L"Idle", vecTexture ) );
}