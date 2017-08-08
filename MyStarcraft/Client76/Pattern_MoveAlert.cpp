#include "stdafx.h"
#include "Pattern_MoveAlert.h"

#include "Mouse.h"

#include "GameEntity.h"


CPattern_MoveAlert::CPattern_MoveAlert()
{
}


CPattern_MoveAlert::~CPattern_MoveAlert()
{
}

void CPattern_MoveAlert::Initialize()
{
}

void CPattern_MoveAlert::OnEnable()
{
	m_eEnemyType = ((this->m_pGameEntity->GetObjectType()) ? OBJ_TYPE_USER : OBJ_TYPE_USER2);

	if ( this->m_pGameEntity->CheckAlertEntity( m_eEnemyType, NULL ) )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_AttackTarget );
		return;
	}

	m_vDestination = CMouse::GetMousePos();
	D3DXVECTOR3 vGameEntityPos = this->m_pGameEntity->GetPos();

	D3DXVECTOR3 vTempDir;
	D3DXVec3Normalize( &vTempDir, &(m_vDestination - vGameEntityPos) );

	this->m_pGameEntity->SetDir( vTempDir );
}

int CPattern_MoveAlert::Update()
{
	this->m_pGameEntity->MoveEntity();

	if ( D3DXVec3Length( &(this->m_pGameEntity->GetPos() - m_vDestination) ) <= this->m_pGameEntity->GetSpeed() )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return Event_Pattern_Change;
	}

	if ( this->m_pGameEntity->CheckAlertEntity( m_eEnemyType, NULL ) )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_AttackTarget );
		return Event_Pattern_Change;
	}

	return Event_None;
}

void CPattern_MoveAlert::Release()
{
}
