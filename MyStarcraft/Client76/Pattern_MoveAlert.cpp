#include "stdafx.h"
#include "Pattern_MoveAlert.h"

#include "Mouse.h"

#include "GameEntity.h"
#include "Move.h"


CPattern_MoveAlert::CPattern_MoveAlert( const bool & _bCheckEnemy )
	: m_bCheckEnemy( _bCheckEnemy )
{
}

CPattern_MoveAlert::~CPattern_MoveAlert()
{
}

void CPattern_MoveAlert::Initialize()
{
	this->m_pMoveComponent = const_cast<CMove*>(this->m_pGameEntity->GetComponent<CMove>());
}

void CPattern_MoveAlert::OnEnable()
{
	if ( this->m_bPrevPattern )
	{
		this->m_pMoveComponent->ReFindPath();
	}
	else
	{
		D3DXVECTOR3 vMouse = CMouse::GetInstance()->GetPos();
		D3DXVECTOR3 vDestination;
		vMouse += this->m_pGameEntity->GetScroll();

		if ( this->m_pGameEntity->GetIsUseDestination() )
			vDestination = this->m_pGameEntity->GetDestination();
		else
			vDestination = vMouse;

		this->m_pMoveComponent->SetDestination( vDestination );
	}
}

int CPattern_MoveAlert::Update()
{
	int iResult = this->m_pMoveComponent->Update();
	vector<CGameEntity*> vecEnemyEntity;

	if ( m_bCheckEnemy )
	{
		if ( this->m_pGameEntity->CheckAlertEnemy( &vecEnemyEntity, 1 ) )
		{
			this->m_pGameEntity->SetTarget( vecEnemyEntity.front() );
			this->m_pGameEntity->SetPattern( CGameEntity::Pattern_ChaseTarget );
			return Event_Pattern_Change;
		}
	}
	else
	{
		if ( this->m_pGameEntity->CheckAlertOurForces( &vecEnemyEntity, 1 ) )
		{
			this->m_pGameEntity->SetTarget( vecEnemyEntity.front() );
			this->m_pGameEntity->SetPattern( CGameEntity::Pattern_ChaseTarget );
			return Event_Pattern_Change;
		}
	}

	if ( iResult == 1 )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return Event_Pattern_Change;
	}

	return Event_None;
}

void CPattern_MoveAlert::Release()
{
}
