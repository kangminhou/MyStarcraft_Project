#pragma once
#include "EntityPattern.h"
#include "Include.h"

class CMove;

class CPattern_MoveAlert :
	public CEntityPattern 
{
private:
	CMove*	m_pMoveComponent;
	bool	m_bCheckEnemy;

public:
	CPattern_MoveAlert(const bool& _bCheckEnemy = true);
	~CPattern_MoveAlert();

	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

