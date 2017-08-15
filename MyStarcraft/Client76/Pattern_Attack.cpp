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

	/* Ÿ���� ���� ��� �ٽ� Idle ��������.. */
	if ( !this->m_pTarget )
	{
		this->m_pGameEntity->SetPrevPattern();
		return;
	}

	this->m_pGameEntity->LookPos( this->m_pTarget->GetPos() );

	/* Ÿ���� �����������̶��.. */
	if ( false )
	{
		this->m_pCurUseWeapon = &this->m_tEntityAirWeapon;
	}
	else
	{
		this->m_pCurUseWeapon = &this->m_tEntityGroundWeapon;
	}

	/* ���Ⱑ ���� ��� ( ������ ���ϱ� ������ Idle ��������.. ) */
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

	float fEntityAttRange = ((/* ���� �����̶��.. */ false) ? this->m_pGameEntity->GetAirWeaponAttRange() : this->m_pGameEntity->GetGroundWeaponAttRange());

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
