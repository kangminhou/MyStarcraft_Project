#pragma once
#include "EntityPattern.h"

class CPattern_HealOurForces :
	public CEntityPattern
{
private:
	CGameEntity*	m_pTarget;

public:
	CPattern_HealOurForces();
	virtual ~CPattern_HealOurForces();

public:
	// CEntityPattern��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

