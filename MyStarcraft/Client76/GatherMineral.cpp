#include "stdafx.h"
#include "GatherMineral.h"

#include "EntityMgr.h"
#include "TimeMgr.h"
#include "EffectMgr.h"
#include "ResourceMgr.h"
#include "ObjMgr.h"

#include "SCV.h"
#include "Mineral.h"
#include "Move.h"
#include "Animation.h"
#include "ResourceObj.h"
#include "Player.h"


CGatherMineral::CGatherMineral()
	: m_pControlList(NULL)
{
}


CGatherMineral::~CGatherMineral()
{
}

void CGatherMineral::Initialize()
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

void CGatherMineral::OnEnable()
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

	this->m_pCurGatherMineral = dynamic_cast<CMineral*>(this->m_pSCV->GetGatherObject());

	if ( !m_pCurGatherMineral )
	{
		this->m_pSCV->SetPattern( CGameEntity::Pattern_Idle );
		ERROR_MSG( L"CGatherMineral OnEnable Failed" );
		return;
	}

	if ( this->m_pCurGatherMineral->GetIsCanGather() )
	{
		this->m_pCurGatherMineral->SetUseEntity( this->m_pGameEntity );
	}
	else
	{
		if ( this->m_pPlayer->CheckNearMineral(this->m_pCurGatherMineral, this->m_pGameEntity) )
		{
			this->m_pCurGatherMineral->SetUseEntity( this->m_pGameEntity );
		}
		else
		{
			this->m_pSCV->SetPattern( CGameEntity::Pattern_Idle );
			ERROR_MSG( L"CGatherMineral OnEnable Failed" );
			return;
		}
	}

	this->m_iEntityActLevel = 0;
	this->m_pSCV->SetIsCheckEntityTile( false );
	this->m_pMove->SetDestination( this->m_pCurGatherMineral->GetPos(), true );

	this->m_tMineralRect = this->m_pCurGatherMineral->GetColRect();
	this->m_tSCVRect = this->m_pSCV->GetColRect();
}

int CGatherMineral::Update()
{
	this->m_tSCVRect = this->m_pSCV->GetColRect();

	switch ( m_iEntityActLevel )
	{
		case 0:
		{
			RECT rc = { 0, 0, 0, 0 };
			if ( this->m_pMove->Update() || IntersectRect( &rc, &this->m_tSCVRect, &this->m_tMineralRect ) )
			{
				this->m_iEntityActLevel = 1;
				this->m_fGatherTime = 0.f;
				this->m_pSCV->ChangeAnimation( L"Attack" );
				this->m_pSCV->LookPos( this->m_pCurGatherMineral->GetPos(), false );
			}
		}
			break;

		case 1:
		{
			m_fGatherTime += GET_TIME;

			if ( this->m_pAnimCom->GetIsAnimEnd() )
			{
				this->m_pAnimCom->ResetFrame();
				CEffectMgr::GetInstance()->ShowEffect( this->m_pSCV->GetGroundAttackData().pWeapon, this->m_pCurGatherMineral );

				if ( m_fGatherTime >= 2.f )
				{
					if ( this->m_pCommandCenter )
					{
						this->m_tCommandCetnerRect = this->m_pCommandCenter->GetColRect();
						this->m_tCommandCetnerRect.left -= 10;
						this->m_tCommandCetnerRect.right += 10;
						this->m_tCommandCetnerRect.top -= 10;
						this->m_tCommandCetnerRect.bottom += 10;

						this->m_iEntityActLevel = 2;
						this->m_pMove->SetDestination( this->m_pCommandCenter->GetPos(), true );
						this->m_pSCV->ChangeAnimation( L"Idle" );

						m_pResourceObj = CResourceMgr::GetInstance()->PopObject( CResourceMgr::Resource_Mineral, this->m_pSCV );
						CObjMgr::GetInstance()->GetList( this->m_pSCV->GetObjectType() )->push_back( m_pResourceObj );
					}
					else
						this->m_iEntityActLevel = 4;
				}
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

				this->m_pPlayer->GatherMoney( 8 );
			}
		}
			break;

		case 3:
		{
			m_fStopTime += GET_TIME;
			if ( m_fStopTime >= 0.5f )
			{
				this->m_iEntityActLevel = 0;
				this->m_pMove->SetDestination( this->m_pCurGatherMineral->GetPos(), true );
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

	}

	return 0;
}

void CGatherMineral::Release()
{
}
