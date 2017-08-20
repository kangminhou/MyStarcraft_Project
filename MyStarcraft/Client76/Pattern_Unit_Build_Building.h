#pragma once
#include "EntityPattern.h"

class CPattern_Unit_Build_Building :
	public CEntityPattern
{
public:
	CPattern_Unit_Build_Building();
	virtual ~CPattern_Unit_Build_Building();

public:
	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

