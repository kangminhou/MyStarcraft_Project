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
		float fGroundWeaponRange = this->m_pGameEntity->GetGroundWeaponAttRange() * Object_Scope_Mul;
		float fAirWeaponRange = this->m_pGameEntity->GetAirWeaponAttRange() * Object_Scope_Mul;

		vector<CGameEntity*> vecEnemy;

		if ( this->m_pGameEntity->GetGroundAttackData().pWeapon && 
			 this->m_pGameEntity->CheckRangeAlertEnemy( fGroundWeaponRange, &vecEnemy ) )
		{
			this->m_pGameEntity->SetTarget( NULL );

			D3DXVECTOR3 vPos = this->m_pGameEntity->GetPos();

			auto SortEnemyPos = [&]( CGameEntity* _pEntity1, CGameEntity* _pEntity2 ) {
				return D3DXVec3Length( &(_pEntity1->GetPos() - vPos) ) < D3DXVec3Length( &(_pEntity2->GetPos() - vPos) );
			};

			std::sort( vecEnemy.begin(), vecEnemy.end(), SortEnemyPos );
			CGameEntity* pTarget = nullptr;

			for ( size_t i = 0; i < vecEnemy.size(); ++i )
			{
				if ( !vecEnemy[i]->GetCheckUnitInformation( CGameEntity::Entity_AirUnit ) )
				{
					pTarget = vecEnemy[i];
					break;
				}
					
			}

			if ( pTarget )
			{
				this->m_pGameEntity->SetTarget( vecEnemy.front() );

				vecEnemy.clear();

				this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Attack );
				return Event_Pattern_Change;
			}
		}
		if ( this->m_pGameEntity->GetAirAttackData().pWeapon && 
			 this->m_pGameEntity->CheckRangeAlertEnemy( fAirWeaponRange, &vecEnemy ) )
		{
			this->m_pGameEntity->SetTarget( NULL );

			D3DXVECTOR3 vPos = this->m_pGameEntity->GetPos();

			auto SortEnemyPos = [&]( CGameEntity* _pEntity1, CGameEntity* _pEntity2 ) {
				return D3DXVec3Length( &(_pEntity1->GetPos() - vPos) ) < D3DXVec3Length( &(_pEntity2->GetPos() - vPos) );
			};

			std::sort( vecEnemy.begin(), vecEnemy.end(), SortEnemyPos );
			CGameEntity* pTarget = nullptr;

			for ( size_t i = 0; i < vecEnemy.size(); ++i )
			{
				if ( vecEnemy[i]->GetCheckUnitInformation( CGameEntity::Entity_AirUnit ) )
				{
					pTarget = vecEnemy[i];
					break;
				}

			}

			if ( pTarget )
			{
				this->m_pGameEntity->SetTarget( pTarget );

				vecEnemy.clear();

				this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Attack );
				return Event_Pattern_Change;
			}
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
