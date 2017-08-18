#include "stdafx.h"
#include "Pattern_IdleAlert.h"

#include "GameEntity.h"


CPattern_IdleAlert::CPattern_IdleAlert( const bool & _bAlertEnemy )
	: m_bAlertEnemy( _bAlertEnemy )
{
}

CPattern_IdleAlert::~CPattern_IdleAlert()
{
}

void CPattern_IdleAlert::Initialize()
{
}

void CPattern_IdleAlert::OnEnable()
{
}

int CPattern_IdleAlert::Update()
{
	if ( this->m_bAlertEnemy )
	{
		vector<CGameEntity*> vecEnemy;

		if ( this->m_pGameEntity->CheckRangeAlertEnemy( this->m_pGameEntity->GetGroundWeaponAttRange() * Object_Scope_Mul, &vecEnemy ) )
		{
			this->m_pGameEntity->SetTarget( NULL );

			D3DXVECTOR3 vPos = this->m_pGameEntity->GetPos();

			auto SortEnemyPos = [&]( CGameEntity* _pEntity1, CGameEntity* _pEntity2 ) {
				return D3DXVec3Length( &(_pEntity1->GetPos() - vPos) ) < D3DXVec3Length( &(_pEntity2->GetPos() - vPos) );
			};

			std::sort( vecEnemy.begin(), vecEnemy.end(), SortEnemyPos );
			this->m_pGameEntity->SetTarget( vecEnemy.front() );

			vecEnemy.clear();

			this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Attack );
			return Event_Pattern_Change;
		}

		if ( this->m_pGameEntity->CheckAlertEnemy( NULL ) )
		{
			this->m_pGameEntity->SetPattern( CGameEntity::Pattern_ChaseTarget );
			return Event_Pattern_Change;
		}
	}
	else
	{
		if ( this->m_pGameEntity->CheckAlertOurForces( NULL ) )
		{
			this->m_pGameEntity->SetPattern( CGameEntity::Pattern_ChaseTarget );
			return Event_Pattern_Change;
		}
	}

	return Event_None;
}

void CPattern_IdleAlert::Release()
{
}
