#include "stdafx.h"
#include "Factory_Building.h"

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


CFactory_Building::CFactory_Building()
{
}


CFactory_Building::~CFactory_Building()
{
}

HRESULT CFactory_Building::Initialize( void )
{
	this->m_wstrStateKey = L"Factory";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = 1250.f;
	this->m_tInfoData.fCurHp = 0.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 0;

	this->m_wstrFaceKey = L"FaceBuiding";
	this->m_wstrWireFrameKey = L"Factory";

	this->m_byFaceFrameNum = 45;

	//RECT tRect = { -8, -9, 8, 10 };
	RECT tRect = { -48, -48, 48, 48 };
	this->m_tOriginColRect = tRect;

	CBuilding::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	return S_OK;
}

int CFactory_Building::Update( void )
{
	CBuilding::Update();

	return 0;
}

void CFactory_Building::Render( void )
{
	/* 그림이 중앙이 객체의 좌표가 되도록 설정.. */
	if ( m_pCurDrawTexture )
	{
		float fX = m_pCurDrawTexture->ImageInfo.Width * 0.5f;
		float fY = m_pCurDrawTexture->ImageInfo.Height * 0.5f;
		this->DrawTexture( m_pCurDrawTexture, this->GetWorldMatrix(), D3DXVECTOR3( fX, fY, 0.f ) );
	}

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation( &matTrans, 500.f, 400.f, 0.f );
	TCHAR str[128];
	swprintf_s( str, L"%f, %f", this->m_tInfoData.fMaxHp, this->m_tInfoData.fCurHp );
	this->DrawFont( matTrans, str );
}

void CFactory_Building::Release( void )
{
}

void CFactory_Building::UpdatePosition( const D3DXVECTOR3 & vPrevPos )
{
	CGameObject::UpdatePosition( vPrevPos );
}

void CFactory_Building::SuccessBuild()
{
	CBuilding::SuccessBuild();
	//CEntityMgr::GetInstance()->
}

void CFactory_Building::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern )
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

		case CGameEntity::Pattern_Make_Unit:
		{
			//this->m_pPushData->iMessage
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

void CFactory_Building::DecideTexture()
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

void CFactory_Building::InitAnimation()
{
}

void CFactory_Building::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Build", new CPattern_Building_Build ) );
}

void CFactory_Building::InitTexture()
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
