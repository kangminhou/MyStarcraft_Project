#pragma once
#include "EntityPattern.h"
#include "GameEntity.h"

class CAnimation;

class CPattern_Attack :
	public CEntityPattern
{
private:
	CGameEntity::ATTACK_DATA		m_tEntityGroundWeapon;
	CGameEntity::ATTACK_DATA		m_tEntityAirWeapon;

	const CGameEntity::ATTACK_DATA*	m_pCurUseWeapon;

	CGameEntity*	m_pTarget;
	CAnimation*		m_pEntityAnim;

	bool			m_bCanAttack;

public:
	CPattern_Attack();
	virtual ~CPattern_Attack();

	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

