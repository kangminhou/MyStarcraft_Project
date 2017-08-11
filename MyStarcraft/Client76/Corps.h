#pragma once
#include "Component.h"
#include "Unit.h"

/* ���� (or �ϳ�)�� ���ֵ�� �̷���� �δ� Ŭ����.. */
class CCorps :
	public CComponent
{
public:
	enum { MAX_UNIT = 200 };

private:
	CGameEntity*	m_pEntityArr[MAX_UNIT];	// �δ��..
	BYTE			m_byCurUnitNum;			// �δ�� ����..

public:
	CCorps();
	virtual ~CCorps();

public:
	BYTE GetCurUnitNum() const;

public:
	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Release() override;

public:
	void Render();

public:
	/*
	 * SetUnitPattern : �δ��� �ൿ�� �������� ( ex. �δ븦 �����ϰ� Move ����� �����ٸ� �� �δ��� ��ü���� Move ����� �������� )..

	 * �Ű�����
	  - ePatternKind : �δ���鿡�� ���� ���..
	 */
	void SetUnitPattern( CUnit::eGameEntityPattern _ePatternKind );
	void AddUnit( CGameEntity* _pEntity );
	void ResetCorps(void);

};

