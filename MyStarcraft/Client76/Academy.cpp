#include "stdafx.h"
#include "Academy.h"

#include "TextureMgr.h"
#include "Animation.h"
#include "EntityMgr.h"
#include "ResearchMgr.h"

#include "Move.h"
#include "Pattern_Building_Build.h"
#include "Pattern_Research.h"
#include "UIMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "Background.h"
#include "EntityMgr.h"


CAcademy::CAcademy()
{
}


CAcademy::~CAcademy()
{
}

HRESULT CAcademy::Initialize( void )
{
	this->m_wstrStateKey = L"Academy";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = 600.f;
	this->m_tInfoData.fCurHp = 0.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 0;

	this->m_wstrFaceKey = L"FaceBuiding";
	this->m_wstrWireFrameKey = L"Academy";

	this->m_byFaceFrameNum = 45;

	//RECT tRect = { -8, -9, 8, 10 };
	RECT tRect = { -40, -32, 40, 32 };
	this->m_tOriginColRect = tRect;

	CBuilding::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 6 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 6 );	

	return S_OK;
}

int CAcademy::Update( void )
{
	CBuilding::Update();

	return 0;
}

void CAcademy::Render( void )
{
	CBuilding::Render();

	if ( m_curActPatternKind == Pattern_Research )
	{
		
	}
}

void CAcademy::Release( void )
{
}

void CAcademy::UpdatePosition( const D3DXVECTOR3 & vPrevPos )
{
	CGameObject::UpdatePosition( vPrevPos );
}

void CAcademy::SuccessBuild()
{
	CBuilding::SuccessBuild();
}

void CAcademy::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern )
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

		case CGameEntity::Pattern_Research:
		{
			if ( this->m_curActPatternKind != CGameEntity::Pattern_Research )
			{
				this->m_pCurActPattern = this->m_mapPatterns.find( L"Research" )->second;
				this->m_pAnimCom->ChangeAnimation( L"Research" );
			}
			
			m_eResearchKind = (eResearchKind)this->m_pPushData->iMessage;

		}
			break;

		case CGameEntity::Pattern_Die:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Die" )->second;;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Die" );
			this->m_wstrStateKey = L"Die";

			if ( this->m_pEntityBelongToCorps )
				this->m_pEntityBelongToCorps->EraseUnit( this );
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

void CAcademy::DecideTexture()
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

void CAcademy::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Research", FRAME( 0.f, 2.f, 2.f, 0.f ), CAnimation::Anim_Loop );
}

void CAcademy::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Build", new CPattern_Building_Build ) );
	this->m_mapPatterns.insert( make_pair( L"Research", new CPattern_Research ) );
}

void CAcademy::InitTexture()
{
	vector<const TEX_INFO*> vecTexture;

	/* Build Texture.. */
	for ( int i = 0; i < 3; ++i )
	{
		vecTexture.push_back( m_pBasicBuildTextureArr[0][i] ); 
	}

	for ( int i = 0; i < 2; ++i )
	{
		vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture( this->GetObjKey().c_str(), this->m_wstrStateKey.c_str(), i ) );
	}

	m_vecResearchShowTexture.push_back( NULL );
	m_vecResearchShowTexture.push_back(CTextureMgr::GetInstance()->GetTexture( this->GetObjKey().c_str(), this->m_wstrStateKey.c_str(), 2 ) );

	m_mapAllTexture.insert( make_pair( L"Build", vecTexture ) );
}
