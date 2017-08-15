#pragma once
#include "EntityPattern.h"

class CPattern_Die :
	public CEntityPattern
{
public:
	CPattern_Die();
	virtual ~CPattern_Die();

public:
	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

