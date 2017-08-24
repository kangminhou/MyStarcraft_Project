#pragma once
#include "EntityPattern.h"

class CGatherGas :
	public CEntityPattern
{
public:
	CGatherGas();
	virtual ~CGatherGas();

public:
	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

