#include "stdafx.h"
#include "Pattern_ChaseTarget.h"

#include "GameEntity.h"
#include "Move.h"


CPattern_ChaseTarget::CPattern_ChaseTarget( const bool & _bChaseEnemy )
	: m_bChaseEnemy( _bChaseEnemy )
{
}

CPattern_ChaseTarget::~CPattern_ChaseTarget()
{
}

void CPattern_ChaseTarget::Initialize()
{
	this->m_pMoveComponent = const_cast<CMove*>(this->m_pGameEntity->GetComponent<CMove>());
}

void CPattern_ChaseTarget::OnEnable()
{
	if ( !this->m_pGameEntity->GetTarget() || this->m_pGameEntity->GetTarget()->GetIsDie() )
	{
		vector<CGameEntity*> vecEnemy;
		this->m_pGameEntity->SetTarget( NULL );

		if ( this->m_bChaseEnemy )
		{
			if ( !this->m_pGameEntity->CheckAlertEnemy( &vecEnemy ) || vecEnemy.empty() )
			{
				this->m_pGameEntity->SetPrevPattern();
				return;
			}
		}
		else
		{
			if ( !this->m_pGameEntity->CheckAlertOurForces( &vecEnemy ) || vecEnemy.empty() )
			{
				this->m_pGameEntity->SetPrevPattern();
				return;
			}
		}

		D3DXVECTOR3 vPos = this->m_pGameEntity->GetPos();

		auto SortEnemyPos = [&]( CGameEntity* _pEntity1, CGameEntity* _pEntity2 ) {
			return D3DXVec3Length(&(_pEntity1->GetPos() - vPos)) < D3DXVec3Length(&(_pEntity2->GetPos() - vPos));
		};

		std::sort( vecEnemy.begin(), vecEnemy.end(), SortEnemyPos );
		this->m_pGameEntity->SetTarget( vecEnemy.front() );

		vecEnemy.clear();
	}

	m_pTarget = this->m_pGameEntity->GetTarget();

	this->m_pMoveComponent->SetDestination( m_pTarget->GetPos(), true );

	this->m_vPrevTargetPos = this->m_pTarget->GetPos();
}

int CPattern_ChaseTarget::Update()
{
	if ( this->m_pTarget->GetIsDie() )
	{
		this->m_pGameEntity->SetPrevPattern();
		return Event_Pattern_Change;
	}

	float fEntityAttRange = ((m_pTarget->GetCheckUnitInformation( CGameEntity::Entity_AirUnit )) ? 
							  this->m_pGameEntity->GetAirWeaponAttRange() : this->m_pGameEntity->GetGroundWeaponAttRange());

	D3DXVECTOR3 vTargetPos = this->m_pTarget->GetPos();

	if ( D3DXVec3Length( &(vTargetPos - this->m_vPrevTargetPos) ) >= EPSILON )
	{
		this->m_pMoveComponent->SetDestination( vTargetPos, true );
		this->m_vPrevTargetPos = vTargetPos;
	}

	this->m_pMoveComponent->Update();

	if ( D3DXVec3Length( &(vTargetPos - this->m_pGameEntity->GetPos()) ) <= fEntityAttRange * Object_Scope_Mul )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Attack );
		return Event_Pattern_Change;
	}

	return Event_None;
}

void CPattern_ChaseTarget::Release()
{
}
