#include "stdafx.h"
#include "GatherGas.h"

#include "EntityMgr.h"
#include "TimeMgr.h"
#include "EffectMgr.h"
#include "ResourceMgr.h"
#include "ObjMgr.h"

#include "SCV.h"
#include "Gas.h"
#include "Move.h"
#include "Animation.h"
#include "ResourceObj.h"
#include "Player.h"


CGatherGas::CGatherGas()
	: m_pControlList(NULL)
{
}


CGatherGas::~CGatherGas()
{
}

void CGatherGas::Initialize()
{
	m_pSCV = dynamic_cast<CSCV*>(this->m_pGameEntity);
	m_pMove = const_cast<CMove*>(m_pSCV->GetComponent<CMove>());
	m_pAnimCom = const_cast<CAnimation*>(this->m_pSCV->GetComponent<CAnimation>());
	m_pPlayer = CObjMgr::GetInstance()->FindGameObject<CPlayer>();

	if ( !m_pSCV )
	{
		ERROR_MSG( L"CGatherMineral Initialize Failed" );
		return;
	}
}

void CGatherGas::OnEnable()
{
	if ( !this->m_pControlList )
		m_pControlList = CEntityMgr::GetInstance()->GetEntityList( this->m_pGameEntity->GetObjectType(), CEntityMgr::Entity_Control );

	float fMinDist = INFINITY;

	D3DXVECTOR3 vEntityPos = m_pGameEntity->GetPos();
	this->m_pCommandCenter = NULL;

	for ( auto& iter : (*m_pControlList) )
	{
		float fDist = D3DXVec3Length( &(vEntityPos - iter->GetPos()) );
		if ( fDist < fMinDist )
		{
			fMinDist = fDist;
			this->m_pCommandCenter = iter;
		}
	}

	this->m_pCurGatherGas = dynamic_cast<CGas*>(this->m_pSCV->GetGatherObject());

	if ( !m_pCurGatherGas )
	{
		this->m_pSCV->SetPattern( CGameEntity::Pattern_Idle );
		ERROR_MSG( L"CGatherGas OnEnable Failed" );
		return;
	}

	this->m_iEntityActLevel = 0;
	this->m_pSCV->SetIsCheckEntityTile( false );
	this->m_pMove->SetDestination( this->m_pCurGatherGas->GetPos(), true );

	this->m_tGasRect = this->m_pCurGatherGas->GetColRect();

	this->m_tGasRect.top -= 32;
	this->m_tGasRect.left -= 8;
	this->m_tGasRect.right += 8;

	this->m_tSCVRect = this->m_pSCV->GetColRect();
}

int CGatherGas::Update()
{
	this->m_tSCVRect = this->m_pSCV->GetColRect();

	switch ( m_iEntityActLevel )
	{
		case 0:
		{
			RECT rc = { 0, 0, 0, 0 };
			if ( this->m_pMove->Update() || IntersectRect( &rc, &this->m_tSCVRect, &this->m_tGasRect ) )
			{
				if ( this->m_pCurGatherGas->GatherGas() )
				{
					this->m_iEntityActLevel = 1;
					this->m_fGatherTime = 0.f;
					this->m_pSCV->SetHideUnit( true );
				}
				else
					this->m_iEntityActLevel = 5;
			}
		}
		break;

		case 1:
		{
			m_fGatherTime += GET_TIME;
		
			if ( this->m_fGatherTime >= 1.f )
			{
				this->m_pSCV->SetHideUnit( false );
				this->m_pCurGatherGas->GatherEnd();
		
				if ( this->m_pCommandCenter )
				{
					this->m_tCommandCetnerRect = this->m_pCommandCenter->GetColRect();
					this->m_tCommandCetnerRect.left -= 10;
					this->m_tCommandCetnerRect.right += 10;
					this->m_tCommandCetnerRect.top -= 10;
					this->m_tCommandCetnerRect.bottom += 10;
		
					this->m_iEntityActLevel = 2;
					this->m_pMove->SetDestination( this->m_pCommandCenter->GetPos(), true );
		
					m_pResourceObj = CResourceMgr::GetInstance()->PopObject( CResourceMgr::Resource_Gas, this->m_pSCV );
					CObjMgr::GetInstance()->GetList( this->m_pSCV->GetObjectType() )->push_back( m_pResourceObj );
				}
				else
					this->m_iEntityActLevel = 4;
			}
		
		}
		break;
		
		case 2:
		{
			RECT rc = { 0, 0, 0, 0 };
			if ( this->m_pMove->Update() || IntersectRect( &rc, &this->m_tSCVRect, &this->m_tCommandCetnerRect ) )
			{
				this->m_iEntityActLevel = 3;
				this->m_fStopTime = 0.f;

				this->m_pPlayer->GahterGas( 8 );
			}
		}
		break;
		
		case 3:
		{
			m_fStopTime += GET_TIME;
			if ( m_fStopTime >= 0.5f )
			{
				this->m_iEntityActLevel = 0;
				this->m_pMove->SetDestination( this->m_pCurGatherGas->GetPos(), true );
				this->m_pResourceObj->HideResourceObj();
			}
		}
		break;
		
		case 4:
		{
			float fMinDist = INFINITY;
			D3DXVECTOR3 vEntityPos = this->m_pSCV->GetPos();
		
			for ( auto& iter : (*m_pControlList) )
			{
				float fDist = D3DXVec3Length( &(vEntityPos - iter->GetPos()) );
				if ( fDist < fMinDist )
				{
					fMinDist = fDist;
					this->m_pCommandCenter = iter;
					this->m_iEntityActLevel = 3;
				}
			}
		}
		break;
		
		case 5:
		{
			if ( this->m_pCurGatherGas->GatherGas() )
			{
				this->m_fGatherTime = 0.f;
				this->m_iEntityActLevel = 1;
				this->m_pSCV->SetHideUnit( true );
			}
		}
			break;

	}

	return 0;
}

void CGatherGas::Release()
{
}
