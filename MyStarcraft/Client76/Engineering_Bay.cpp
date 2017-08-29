#include "stdafx.h"
#include "Engineering_Bay.h"

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


CEngineering_Bay::CEngineering_Bay()
{
}


CEngineering_Bay::~CEngineering_Bay()
{
}

HRESULT CEngineering_Bay::Initialize( void )
{
	this->AddSound( L"trewht00.wav", CGameEntity::Sound_Click );


	this->m_wstrStateKey = L"Weaponpl";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = 850.f;
	this->m_tInfoData.fCurHp = 0.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 0;

	this->m_wstrFaceKey = L"FaceBuiding";
	this->m_wstrWireFrameKey = L"Weaponpl";

	this->m_byFaceFrameNum = 45;

	//RECT tRect = { -8, -9, 8, 10 };
	RECT tRect = { -64, -48, 64, 48 };
	this->m_tOriginColRect = tRect;

	CBuilding::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 8 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 8 );	

	return S_OK;
}

int CEngineering_Bay::Update( void )
{
	return CBuilding::Update();
}

void CEngineering_Bay::Render( void )
{
	CBuilding::Render();
}

void CEngineering_Bay::Release( void )
{
}

void CEngineering_Bay::UpdatePosition( const D3DXVECTOR3 & vPrevPos )
{
	CGameObject::UpdatePosition( vPrevPos );
}

void CEngineering_Bay::SuccessBuild()
{
	CBuilding::SuccessBuild();
}

void CEngineering_Bay::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern )
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

void CEngineering_Bay::DecideTexture()
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

void CEngineering_Bay::InitAnimation()
{
}

void CEngineering_Bay::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Build", new CPattern_Building_Build ) );
}

void CEngineering_Bay::InitTexture()
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

	m_mapAllTexture.insert( make_pair( L"Build", vecTexture ) );
}