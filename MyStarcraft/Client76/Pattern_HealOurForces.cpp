#include "stdafx.h"
#include "Pattern_HealOurForces.h"

#include "GameEntity.h"



CPattern_HealOurForces::CPattern_HealOurForces()
{
}


CPattern_HealOurForces::~CPattern_HealOurForces()
{
}

void CPattern_HealOurForces::Initialize()
{
}

void CPattern_HealOurForces::OnEnable()
{
	this->m_pTarget = this->m_pGameEntity->GetTarget();

	/* 타겟이 없을 경우 다시 Idle 패턴으로.. */
	if ( !this->m_pTarget )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return;
	}

	this->m_pGameEntity->LookPos( this->m_pTarget->GetPos(), false );

	if ( this->m_pTarget->GetIsDie() || (this->m_pTarget->GetMaxHp() - this->m_pTarget->GetCurHp()) <= EPSILON )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return;
	}
}

int CPattern_HealOurForces::Update()
{
	this->m_pGameEntity->LookPos( this->m_pTarget->GetPos(), false );

	//if ( D3DXVec3Length( &(this->m_pGameEntity->GetPos() - this->m_pTarget->GetPos()) ) <= this->m_pGameEntity->GetScope() * Object_Scope_Mul )
	//{
	//	this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
	//	return Event_Pattern_Change;
	//}

	if ( this->m_pTarget->GetIsDie() || (this->m_pTarget->GetMaxHp() - this->m_pTarget->GetCurHp()) <= EPSILON )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return Event_Pattern_Change;
	}
	else
	{
		this->m_pGameEntity->UseSkill( CGameEntity::Skill_Heal, this->m_pTarget );
	}

	return Event_None;
}

void CPattern_HealOurForces::Release() 
{
}
