#pragma once
#include "EntityPattern.h"
#include "Include.h"

class CMove;

/* 타겟으로 찍은 적을 쫓아감.. */
class CPattern_ChaseTarget :
	public CEntityPattern
{
private:
	CGameEntity*	m_pTarget;
	CMove*			m_pMoveComponent;
	eObjectType		m_eEnemyType;

public:
	CPattern_ChaseTarget();
	virtual ~CPattern_ChaseTarget();

public:
	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

