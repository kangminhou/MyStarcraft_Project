#pragma once
#include "EntityPattern.h"

class CMove;
/* 주변의 적을 검사하여 적이 시야 안에 왔다면 적 추적 패턴으로 바뀜.. */
class CPattern_Move :
	public CEntityPattern
{
private:
	CMove*	m_pMoveComponent;

public:
	CPattern_Move();
	virtual ~CPattern_Move(); 

public:
	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

