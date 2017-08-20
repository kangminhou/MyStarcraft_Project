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
	CGameEntity::eGameEntityPattern	m_eCurPattern;
	const BUTTON_DATA*				m_pPushData;

	RECT			m_rcEntityInclude;
	D3DXVECTOR3		m_vCenterPos;

	bool			m_bGatherEntitys;		// 모여 있나??..
	bool			m_bSameEntity;

public:
	CCorps();
	virtual ~CCorps();

public:
	RECT GetEntityIncludeRect() const;
	BYTE GetCurUnitNum() const;
	D3DXVECTOR3 GetCenterPos() const;
	bool GetGatherEntitys() const;
	bool GetSameUnit() const;
	CGameEntity* GetEntity( const BYTE& _byUnitIndex );

public:
	// CComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Release() override;

public:
	void Update();
	void Render();

public:
	/*
	 * SetUnitPattern : 부대의 행동을 결정해줌 ( ex. 부대를 선택하고 Move 명령을 내린다면 그 부대의 전체에게 Move 명령을 전달해줌 )..

	 * 매개변수
	  - ePatternKind : 부대원들에게 내린 명령..
	 */
	void SetUnitPattern( const CGameEntity::eGameEntityPattern& _ePatternKind );
	void PushMessage( const BUTTON_DATA* pButtonData );
	void SetUnitSkill( const CGameEntity::eGameEntitySkillKind& _eSkillKind );
	void AddUnit( CGameEntity* _pEntity );
	void EraseUnit( CGameEntity* _pEntity );
	void ResetCorps(void);

	int CheckCorpsOnePlaceMove( const D3DXVECTOR3& _vDestination, const CGameEntity* _pEntity ) const;

private:
	void CalcCorpsMoveKind();

};

