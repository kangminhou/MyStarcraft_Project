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
	CGameEntity::eGameEntityPattern	m_eCurPattern;

	RECT			m_rcEntityInclude;
	D3DXVECTOR3		m_vCenterPos;
	bool			m_bGatherEntitys;		// �� �ֳ�??..

public:
	CCorps();
	virtual ~CCorps();

public:
	RECT GetEntityIncludeRect() const;
	BYTE GetCurUnitNum() const;
	D3DXVECTOR3 GetCenterPos() const;
	bool GetGatherEntitys() const;

public:
	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Release() override;

public:
	void Update();
	void Render();

public:
	/*
	 * SetUnitPattern : �δ��� �ൿ�� �������� ( ex. �δ븦 �����ϰ� Move ����� �����ٸ� �� �δ��� ��ü���� Move ����� �������� )..

	 * �Ű�����
	  - ePatternKind : �δ���鿡�� ���� ���..
	 */
	void SetUnitPattern( const CUnit::eGameEntityPattern& _ePatternKind );
	void AddUnit( CGameEntity* _pEntity );
	void EraseUnit( CGameEntity* _pEntity );
	void ResetCorps(void);

	int CheckCorpsOnePlaceMove( const D3DXVECTOR3& _vDestination, const CGameEntity* _pEntity ) const;

private:
	void CalcCorpsMoveKind();

};

