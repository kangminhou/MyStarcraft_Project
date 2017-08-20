#pragma once
#include "EntityPattern.h"

class CPattern_Building_Build :
	public CEntityPattern
{
private:
	float	m_fHpPlusSpeed;

public:
	CPattern_Building_Build();
	virtual ~CPattern_Building_Build();

public:
	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

