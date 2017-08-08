#pragma once
#include "EntityPattern.h"
#include "Include.h"

class CPattern_MoveAlert :
	public CEntityPattern 
{
private:
	D3DXVECTOR3 m_vDestination;
	eObjectType m_eEnemyType;

public:
	CPattern_MoveAlert();
	~CPattern_MoveAlert();

	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

