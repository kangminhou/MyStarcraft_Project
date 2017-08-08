#pragma once
#include "EntityPattern.h"
#include "Include.h"

/* 주변의 적을 검사하여 적이 시야 안에 왔다면 적 추적 패턴으로 바뀜.. */
class CPattern_IdleAlert :
	public CEntityPattern
{
private:
	eObjectType m_eEnemyType;

public:
	CPattern_IdleAlert();
	virtual ~CPattern_IdleAlert();

	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

