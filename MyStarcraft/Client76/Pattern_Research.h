#pragma once
#include "EntityPattern.h"
#include "Include.h"

class CBuilding;
class CResearchMgr;
class CPlayer;

class CPattern_Research :
	public CEntityPattern
{
private:
	CBuilding*		m_pBuilding;
	CResearchMgr*	m_pResearchMgr;
	CPlayer*		m_pPlayer;

	float			m_fRestTime;

public:
	CPattern_Research();
	virtual ~CPattern_Research();

public:
	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

