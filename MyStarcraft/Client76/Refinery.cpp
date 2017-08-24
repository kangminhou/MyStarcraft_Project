#include "stdafx.h"
#include "Refinery.h"

#include "TextureMgr.h"
#include "Animation.h"
#include "EntityMgr.h"

#include "Move.h"
#include "Pattern_Building_Build.h"
#include "UIMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "Background.h"
#include "EntityMgr.h"


CRefinery::CRefinery()
{
}


CRefinery::~CRefinery()
{
}

HRESULT CRefinery::Initialize( void )
{
	this->m_wstrStateKey = L"Refinery";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = 750.f;
	this->m_tInfoData.fCurHp = 0.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 0;

	this->m_wstrFaceKey = L"FaceBuiding";
	this->m_wstrWireFrameKey = L"Refinery";

	this->m_byFaceFrameNum = 45;

	//RECT tRect = { -8, -9, 8, 10 };
	RECT tRect = { -48, -32, 48, 32 };
	this->m_tOriginColRect = tRect;

	CBuilding::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 8 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 8 );

	this->m_pVecTile = this->m_pBackground->GetTile();

	return S_OK;
}

int CRefinery::Update( void )
{
	CBuilding::Update();

	return 0;
}

void CRefinery::Render( void )
{
	CBuilding::Render();
}

void CRefinery::Release( void )
{
}

void CRefinery::UpdatePosition( const D3DXVECTOR3 & vPrevPos )
{
	CGameObject::UpdatePosition( vPrevPos );
}

void CRefinery::SuccessBuild()
{
	CBuilding::SuccessBuild();
}

void CRefinery::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern )
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

void CRefinery::DecideTexture()
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

bool CRefinery::CheckCanBuild( const int & _iIndex )
{
	if ( (*this->m_pVecTile)[_iIndex]->byOption == 3 )
		return true;

	return false;
}

void CRefinery::InitAnimation()
{
}

void CRefinery::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Build", new CPattern_Building_Build ) );
}

void CRefinery::InitTexture()
{
	vector<const TEX_INFO*> vecTexture;

	/* Build Texture.. */
	for ( int i = 0; i < 5; ++i )
	{
		vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture( this->GetObjKey().c_str(), this->m_wstrStateKey.c_str(), i ) );
	}

	m_mapAllTexture.insert( make_pair( L"Build", vecTexture ) );
}
