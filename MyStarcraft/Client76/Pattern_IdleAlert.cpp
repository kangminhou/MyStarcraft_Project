#include "stdafx.h"
#include "Pattern_IdleAlert.h"

#include "GameEntity.h"


CPattern_IdleAlert::CPattern_IdleAlert()
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
	m_eEnemyType = ((this->m_pGameEntity->GetObjectType() == OBJ_TYPE_USER) ? OBJ_TYPE_USER2 : OBJ_TYPE_USER);
}

int CPattern_IdleAlert::Update()
{
	if ( this->m_pGameEntity->CheckAlertEntity( m_eEnemyType, NULL ) )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_ChaseTarget );
		return Event_Pattern_Change;
	}

	return Event_None;
}

void CPattern_IdleAlert::Release()
{
}
