#include "stdafx.h"
#include "Pattern_Attack.h"

#include "Weapon.h"
#include "Animation.h"


CPattern_Attack::CPattern_Attack()
{
}


CPattern_Attack::~CPattern_Attack()
{
}

void CPattern_Attack::Initialize()
{
	this->m_tEntityGroundWeapon = this->m_pGameEntity->GetGroundAttackData();
	this->m_tEntityAirWeapon = this->m_pGameEntity->GetAirAttackData();
	this->m_pEntityAnim = const_cast<CAnimation*>(this->m_pGameEntity->GetComponent<CAnimation>());
}

void CPattern_Attack::OnEnable()
{
	this->m_pTarget = this->m_pGameEntity->GetTarget();

	/* 타겟이 없을 경우 다시 Idle 패턴으로.. */
	if ( !this->m_pTarget )
	{
		this->m_pGameEntity->SetPrevPattern();
		return;
	}

	this->m_pGameEntity->LookPos( this->m_pTarget->GetPos() );

	/* 타겟이 공중유닛이이라면.. */
	if ( false )
	{
		this->m_pCurUseWeapon = &this->m_tEntityAirWeapon;
	}
	else
	{
		this->m_pCurUseWeapon = &this->m_tEntityGroundWeapon;
	}

	/* 무기가 없을 경우 ( 공격을 못하기 때문에 Idle 패턴으로.. ) */
	if ( !m_pCurUseWeapon )
	{
		this->m_pGameEntity->SetPrevPattern();
		return;
	}

	this->m_bCanAttack = true;
	this->m_pCurUseWeapon->pWeapon->Initialize();
}

int CPattern_Attack::Update()
{
	if ( !this->m_pTarget || this->m_pTarget->GetIsDie() )
	{
		this->m_pGameEntity->SetTarget( NULL );
		this->m_pGameEntity->SetPrevPattern();
		return Event_Pattern_Change;
	}

	float fEntityAttRange = ((/* 공중 유닛이라면.. */ false) ? this->m_pGameEntity->GetAirWeaponAttRange() : this->m_pGameEntity->GetGroundWeaponAttRange());

	if ( D3DXVec3Length( &(this->m_pTarget->GetPos() - this->m_pGameEntity->GetPos()) ) > fEntityAttRange * Object_Scope_Mul )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_ChaseTarget );
		return Event_Pattern_Change;
	}

	this->m_pGameEntity->LookPos( this->m_pTarget->GetPos() );

	if ( this->m_pEntityAnim->GetIsAnimEnd() )
	{
		if ( this->m_bCanAttack )
		{
			for ( int i = 0; i < this->m_pCurUseWeapon->byAttackNum; ++i )
				this->m_pCurUseWeapon->pWeapon->Attack( this->m_pTarget );

			this->m_bCanAttack = false;
		}
		else
		{
			if ( this->m_pCurUseWeapon->pWeapon->AttackCoolTimeUpdate() )
			{
				this->m_pEntityAnim->ResetFrame();
				this->m_bCanAttack = true;
			}
		}
	}

	return Event_None;
}

void CPattern_Attack::Release()
{
}
