#include "stdafx.h"
#include "CommandCenter.h"

#include "TextureMgr.h"
#include "Animation.h"
#include "EntityMgr.h"

#include "Move.h"
#include "Pattern_Building_Build.h"
#include "Pattern_Die.h"
#include "Pattern_MakeUnit.h"
#include "UIMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "Background.h"


CCommandCenter::CCommandCenter()
{
}


CCommandCenter::~CCommandCenter()
{
	Release();
}

HRESULT CCommandCenter::Initialize( void )
{
	this->m_wstrStateKey = L"Control";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = 1500.f;
	this->m_tInfoData.fCurHp = 0.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 0;

	this->m_wstrFaceKey = L"FaceBuiding";
	this->m_wstrWireFrameKey = L"Control";

	this->m_byFaceFrameNum = 45;

	//RECT tRect = { -8, -9, 8, 10 };
	RECT tRect = { -48, -48, 48, 48 };
	this->m_tOriginColRect = tRect;

	CBuilding::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 8 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 8 );

	return S_OK;
}

int CCommandCenter::Update( void )
{
	return CBuilding::Update();
}

void CCommandCenter::Render( void )
{
	CBuilding::Render();
}

void CCommandCenter::Release( void )
{
	
}

void CCommandCenter::UpdatePosition( const D3DXVECTOR3& vPrevPos )
{
	CGameObject::UpdatePosition( vPrevPos );
}

void CCommandCenter::SuccessBuild()
{
	CBuilding::SuccessBuild();
}

void CCommandCenter::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern /*= FALSE*/ )
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
			if ( !this->GetIsFullOrderVector() )
			{
				//this->m_pPushData->iMessage
				UNIT_GENERATE_DATA* pUnitGenData = new UNIT_GENERATE_DATA( this->m_pEntityMgr->GetEntityGenData( CEntityMgr::eEntityType( this->m_pPushData->iMessage ) ) );

				if ( this->m_pPlayer->GetFullPopulation() )
				{
					this->SoundPlay( CGameEntity::Sound_ETC, 2 );
					this->m_pUIMgr->ShowFont( CUIMgr::Font_No_Population );

					return;
				}
				else
				{
					int iCanBuyCheck = this->m_pPlayer->CheckCanBuyUnit( *pUnitGenData );
					if ( iCanBuyCheck )
					{
						if ( iCanBuyCheck == 1 )
						{
							this->SoundPlay( CGameEntity::Sound_ETC, 0 );
							this->m_pUIMgr->ShowFont( CUIMgr::Font_No_Mineral );
						}
						else if ( iCanBuyCheck == 2 )
						{
							this->SoundPlay( CGameEntity::Sound_ETC, 1 );
							this->m_pUIMgr->ShowFont( CUIMgr::Font_No_Gas );
						}

						return;
					}
					else
					{
						this->m_pPlayer->BuyUnit( *pUnitGenData );

						this->AddOrderIcon( pUnitGenData->nIconFrame, this->m_pPushData->iMessage, pUnitGenData, CGameEntity::Pattern_Make_Unit );

						this->m_bUseActiveTexture = true;

						this->ShowUpdateOrderData();
					}
				}


			}

			if ( this->m_curActPatternKind != CGameEntity::Pattern_Make_Unit )
				this->m_pCurActPattern = this->m_mapPatterns.find( L"Make_Unit" )->second;
			else
				return;

			//D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos() + m_vScroll;
			//CGameEntity* pEntity = CEntityMgr::GetInstance()->MakeUnit( 
			//						(CEntityMgr::eEntityType)this->m_pPushData->iMessage, vMousePos, this->GetObjectType() );
			//
			//this->RenderBuildingArea( pEntity );
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

void CCommandCenter::DecideTexture()
{
	if ( !this->m_bSuccessBuild )
	{
		int iIndex = int( this->m_tInfoData.fCurHp / this->m_tInfoData.fMaxHp * (this->m_vecTexture.size()) );
		if ( iIndex > 0 )
			--iIndex;
		this->m_pCurDrawTexture = this->m_vecTexture[iIndex];
	}
	else
		this->m_pCurDrawTexture = this->m_vecTexture[m_vecTexture.size() - 1];
}

void CCommandCenter::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Fly", FRAME( 0.f, 3.f, 3.f, 2.f ), CAnimation::Anim_ClampForever );
	this->m_pAnimCom->AddAnimation( L"Fly_Reverse", FRAME( 0.f, 3.f, 3.f, 2.f ), CAnimation::Anim_Reverse_ClampForever );
}

void CCommandCenter::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Build", new CPattern_Building_Build ) );
	this->m_mapPatterns.insert( make_pair( L"Die", new CPattern_Die ) );
	this->m_mapPatterns.insert( make_pair( L"Make_Unit", new CPattern_MakeUnit ) );
}

void CCommandCenter::InitTexture()
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
