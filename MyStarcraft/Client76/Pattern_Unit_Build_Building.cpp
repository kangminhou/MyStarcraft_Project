#include "stdafx.h"
#include "Pattern_Unit_Build_Building.h"

#include "TimeMgr.h"
#include "EffectMgr.h"

#include "SCV.h"
#include "ObjMgr.h"
#include "Move.h"
#include "Building.h"
#include "Animation.h"


CPattern_Unit_Build_Building::CPattern_Unit_Build_Building()
{
}


CPattern_Unit_Build_Building::~CPattern_Unit_Build_Building()
{
}

void CPattern_Unit_Build_Building::Initialize()
{
	this->m_pAnimCom = const_cast<CAnimation*>(this->m_pGameEntity->GetComponent<CAnimation>());
	this->m_pMove = const_cast<CMove*>(this->m_pGameEntity->GetComponent<CMove>());

	this->m_vMovePos[0].first = D3DXVECTOR3( 0.4f, 0.3f, 0.f );
	this->m_vMovePos[1].first = D3DXVECTOR3( -1.1f, 0.8f, 0.f );
	this->m_vMovePos[2].first = D3DXVECTOR3( 1.1f, 0.7f, 0.f );
	this->m_vMovePos[3].first = D3DXVECTOR3( 0.5f, -1.1f, 0.f );
	this->m_vMovePos[4].first = D3DXVECTOR3( 1.1f, 0.4f, 0.f );
}

void CPattern_Unit_Build_Building::OnEnable()
{
	this->m_pSCV = dynamic_cast<CSCV*>(this->m_pGameEntity);
	this->m_pBuilding = dynamic_cast<CBuilding*>(this->m_pSCV->GetBuildEntity());

	this->m_pUserList = CObjMgr::GetInstance()->GetList( this->m_pSCV->GetObjectType() );

	this->m_pMove->SetDestination( this->m_pBuilding->GetPos() );

	RECT rcBuildingCol = this->m_pBuilding->GetColRect();
	D3DXVECTOR3 vBuildingStartPos = this->m_pBuilding->GetPos();
	D3DXVECTOR3 vSize( (float)(rcBuildingCol.right - rcBuildingCol.left) / 2.f, (float)(rcBuildingCol.bottom - rcBuildingCol.top) / 2.f, 0.f );
	for ( int i = 0; i < MovePosEnd; ++i )
	{
		this->m_vMovePos[i].second = 
			vBuildingStartPos + D3DXVECTOR3( this->m_vMovePos[i].first.x * vSize.x, this->m_vMovePos[i].first.y * vSize.y, 0.f );
	}

	this->m_byCurMoveArrNum = 0;
	this->m_bEndMove = false;
	this->DecideMovePos();
}

int CPattern_Unit_Build_Building::Update()
{
	if ( !this->m_pSCV->GetIsUnderConstruction() )
	{
		if ( this->m_pMove->Update() )
		{
			this->m_pSCV->BuildStart();
		}
	}
	else
	{
		if ( m_bStopMove )
		{
			m_fStopMoveTime += GET_TIME;

			if ( this->m_pAnimCom->GetIsAnimEnd() )
			{
				this->m_pAnimCom->ResetFrame();
				this->m_pSCV->SoundPlay( CGameEntity::Sound_ETC, 5 );
				CEffectMgr::GetInstance()->ShowEffect( this->m_pSCV->GetGroundAttackData().pWeapon, this->m_pBuilding );
			}

			if ( m_fStopMoveTime >= 2.f )
			{
				++this->m_byCurMoveArrNum;
				this->DecideMovePos();
			}
		}
		else if ( !this->m_bEndMove )
		{
			D3DXVECTOR3 vSCVPos;
			m_fLerpT += GET_TIME;
			D3DXVec3Lerp( &vSCVPos, &this->m_vMovePos[m_byCurMoveArrNum].second, &this->m_vMovePos[m_byCurMoveArrNum + 1].second, m_fLerpT );

			this->m_pSCV->SetPos( vSCVPos );

			if ( m_fLerpT >= 1.f )
			{
				this->InitStopMove();
			}
		}

		if ( this->m_pBuilding->GetIsSuccessBuild() )
		{
			this->m_pSCV->SuccessBuild();
			this->m_pSCV->SetPattern( CGameEntity::Pattern_Idle );
			return CEntityPattern::Event_Pattern_Change;
		}
	}

	return CEntityPattern::Event_None;
}

void CPattern_Unit_Build_Building::Release()
{
}

void CPattern_Unit_Build_Building::DecideMovePos()
{	
	if ( this->m_byCurMoveArrNum + 1 >= MovePosEnd )
	{
		this->m_bEndMove = true;
		this->InitStopMove();
	}
	else
	{
		this->m_pSCV->LookPos( this->m_vMovePos[this->m_byCurMoveArrNum].second, false );
		this->m_bStopMove = false;
		this->m_pSCV->ChangeAnimation( L"Idle" );
		this->m_fLerpT = 0.f;
	}
}

void CPattern_Unit_Build_Building::InitStopMove()
{
	CEffectMgr::GetInstance()->ShowEffect( this->m_pSCV->GetGroundAttackData().pWeapon, this->m_pBuilding );
	this->m_pSCV->ChangeAnimation( L"Attack" );
	this->m_fStopMoveTime = 0.f;
	this->m_bStopMove = true;
	this->m_pSCV->LookPos( m_pBuilding->GetPos(), false );    
}
