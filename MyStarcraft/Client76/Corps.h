#pragma once
#include "Component.h"
#include "Unit.h"

/* 여러 (or 하나)의 유닛들로 이루어진 부대 클래스.. */
class CCorps :
	public CComponent
{
public:
	enum { MAX_UNIT = 200 };

private:
	CGameEntity*	m_pEntityArr[MAX_UNIT];	// 부대원..
	BYTE			m_byCurUnitNum;			// 부대원 개수..

public:
	CCorps();
	virtual ~CCorps();

public:
	BYTE GetCurUnitNum() const;

public:
	// CComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Release() override;

public:
	void Render();

public:
	/*
	 * SetUnitPattern : 부대의 행동을 결정해줌 ( ex. 부대를 선택하고 Move 명령을 내린다면 그 부대의 전체에게 Move 명령을 전달해줌 )..

	 * 매개변수
	  - ePatternKind : 부대원들에게 내린 명령..
	 */
	void SetUnitPattern( CUnit::eGameEntityPattern _ePatternKind );
	void AddUnit( CGameEntity* _pEntity );
	void ResetCorps(void);

};

