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
