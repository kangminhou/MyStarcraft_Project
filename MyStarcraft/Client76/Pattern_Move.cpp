#include "stdafx.h"
#include "Pattern_Move.h"

#include "Mouse.h"

#include "GameEntity.h"


CPattern_Move::CPattern_Move()
{
}


CPattern_Move::~CPattern_Move()
{
}

void CPattern_Move::Initialize()
{
}

void CPattern_Move::OnEnable()
{
	m_vDestination = CMouse::GetMousePos();
	D3DXVECTOR3 vGameEntityPos = this->m_pGameEntity->GetPos();

	D3DXVECTOR3 vTempDir;
	D3DXVec3Normalize( &vTempDir, &(m_vDestination - vGameEntityPos) );

	this->m_pGameEntity->SetDir( vTempDir );
}

int CPattern_Move::Update()
{
	this->m_pGameEntity->MoveEntity();

	if ( D3DXVec3Length( &(this->m_pGameEntity->GetPos() - m_vDestination) ) <= this->m_pGameEntity->GetSpeed() )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return Event_Pattern_Change;
	}

	return Event_None;
}

void CPattern_Move::Release()
{
}
