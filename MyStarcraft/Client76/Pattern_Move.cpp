#include "stdafx.h"
#include "Pattern_Move.h"

#include "Mouse.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "Background.h"

#include "GameEntity.h"
#include "Astar.h"
#include "Move.h"


CPattern_Move::CPattern_Move()
{
}


CPattern_Move::~CPattern_Move()
{
}

void CPattern_Move::Initialize()
{
	this->m_pMoveComponent = const_cast<CMove*>(this->m_pGameEntity->GetComponent<CMove>());
}

void CPattern_Move::OnEnable()
{
	D3DXVECTOR3 vMouse = CMouse::GetMousePos();
	vMouse += this->m_pGameEntity->GetScroll();

	this->m_pMoveComponent->SetDestination( vMouse );
}

int CPattern_Move::Update()
{
	int iResult = this->m_pMoveComponent->Update();

	if ( iResult == 1 )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return Event_Pattern_Change;
	}

	return Event_None;
}

void CPattern_Move::Release()
{
}
