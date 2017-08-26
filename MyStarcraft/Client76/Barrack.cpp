#include "stdafx.h"
#include "Barrack.h"

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


CBarrack::CBarrack()
{
}


CBarrack::~CBarrack()
{
}

HRESULT CBarrack::Initialize( void )
{
	this->m_wstrStateKey = L"Barrack";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = 1000.f;
	this->m_tInfoData.fCurHp = 0.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 0;

	this->m_wstrFaceKey = L"FaceBuiding";
	this->m_wstrWireFrameKey = L"Barrack";

	this->m_byFaceFrameNum = 45;

	//RECT tRect = { -8, -9, 8, 10 };
	RECT tRect = { -48, -48, 48, 48 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CBuilding::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 8 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 8 );

	return S_OK;
}

int CBarrack::Update( void )
{
	CBuilding::Update();

	return 0;
}

void CBarrack::Render( void )
{
	CBuilding::Render();
}

void CBarrack::Release( void )
{
}

void CBarrack::UpdatePosition( const D3DXVECTOR3 & vPrevPos )
{
	CGameObject::UpdatePosition( vPrevPos );
}

void CBarrack::SuccessBuild()
{
	CBuilding::SuccessBuild();
	//CEntityMgr::GetInstance()->
}

void CBarrack::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern )
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

		case CGameEntity::Pattern_Make_Unit:
		{
			//this->m_pPushData->iMessage
			UNIT_GENERATE_DATA tUnitGenData = this->m_pEntityMgr->GetEntityGenData( CEntityMgr::eEntityType( this->m_pPushData->iMessage ) );
			if ( !this->m_pPlayer->CheckCanBuyUnit( tUnitGenData ) )
			{
				this->SetPattern( CGameEntity::Pattern_Idle );
				return;
			}
			else
				this->m_pPlayer->BuyUnit( tUnitGenData );

			CGameEntity* pEntity = CEntityMgr::GetInstance()->MakeUnit( CEntityMgr::eEntityType( this->m_pPushData->iMessage ), this->CalcNearEmptySpace(), this->GetObjectType() );

			CObjMgr::GetInstance()->AddGameObject( pEntity, this->GetObjectType() );
			pEntity->UpdatePosition( pEntity->GetPos() );
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

void CBarrack::DecideTexture()
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

void CBarrack::InitAnimation()
{
}

void CBarrack::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Build", new CPattern_Building_Build ) );
}

void CBarrack::InitTexture()
{
	vector<const TEX_INFO*> vecTexture;

	/* Build Texture.. */
	for ( int i = 0; i < 3; ++i )
	{
		vecTexture.push_back( m_pBasicBuildTextureArr[2][i] );
	}

	for ( int i = 0; i < 2; ++i )
	{
		vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture( this->GetObjKey().c_str(), this->m_wstrStateKey.c_str(), i ) );
	}

	m_mapAllTexture.insert( make_pair( L"Build", vecTexture ) );

	vecTexture.clear();
	for ( int i = 2; i < 5; ++i )
	{
		vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture( this->GetObjKey().c_str(), this->m_wstrStateKey.c_str(), i ) );
	}
	m_mapAllTexture.insert( make_pair( L"Fly", vecTexture ) );
}
