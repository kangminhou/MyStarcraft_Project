#include "stdafx.h"
#include "Pattern_ChaseTarget.h"

#include "GameEntity.h"
#include "Move.h"


CPattern_ChaseTarget::CPattern_ChaseTarget()
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
	vector<CGameEntity*> vecEnemy;

	m_eEnemyType = ((this->m_pGameEntity->GetObjectType() == OBJ_TYPE_USER) ? OBJ_TYPE_USER2 : OBJ_TYPE_USER);

	if ( !this->m_pGameEntity->CheckAlertEntity( m_eEnemyType, &vecEnemy ) || vecEnemy.empty() )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return;
	}

	D3DXVECTOR3 vPos = this->m_pGameEntity->GetPos();

	auto SortEnemyPos = [&]( CGameEntity* _pEntity1, CGameEntity* _pEntity2 ) {
		return D3DXVec3Length(&(_pEntity1->GetPos() - vPos)) < D3DXVec3Length(&(_pEntity2->GetPos() - vPos));
	};

	std::sort( vecEnemy.begin(), vecEnemy.end(), SortEnemyPos );
	m_pTarget = vecEnemy.front();

	vecEnemy.clear();

	this->m_pMoveComponent->SetDestination( m_pTarget->GetPos() );
}

int CPattern_ChaseTarget::Update()
{
	this->m_pMoveComponent->Update();

	if ( D3DXVec3Length( &(this->m_pTarget->GetPos() - this->m_pGameEntity->GetPos()) ) <= this->m_pGameEntity->GetScope() * Object_Scope_Mul )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Attack );
		return Event_Pattern_Change;
	}

	return Event_None;
}

void CPattern_ChaseTarget::Release()
{
}
