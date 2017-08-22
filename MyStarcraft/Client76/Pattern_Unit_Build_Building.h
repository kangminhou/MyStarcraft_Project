#pragma once
#include "EntityPattern.h"
#include "Include.h"

class CBuilding;
class CGameObject;
class CSCV;
class CMove;
class CAnimation;

class CPattern_Unit_Build_Building :
	public CEntityPattern
{
	enum { MovePosEnd = 5 };

private:
	CAnimation*		m_pAnimCom;

	CBuilding*		m_pBuilding;
	CSCV*			m_pSCV;
	list<CGameObject*>*	m_pUserList;
	CMove*				m_pMove;

	pair<D3DXVECTOR3, D3DXVECTOR3>	m_vMovePos[MovePosEnd];
	
	float m_fLerpT;
	float m_fStopMoveTime;

	BYTE m_byCurMoveArrNum;

	bool m_bEndMove;
	bool m_bStopMove;

public:
	CPattern_Unit_Build_Building();
	virtual ~CPattern_Unit_Build_Building();

public:
	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

private:
	void DecideMovePos();
	void InitStopMove();

};

