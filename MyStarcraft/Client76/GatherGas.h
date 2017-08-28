#pragma once
#include "EntityPattern.h"
#include "Include.h"

class CGas;
class CSCV;
class CMove;
class CAnimation;
class CResourceObj;
class CPlayer;

class CGatherGas :
	public CEntityPattern
{
private:
	CPlayer*			m_pPlayer;

	list<CGameEntity*>*	m_pControlList;

	CGameEntity*	m_pCommandCenter;
	CGas*			m_pCurGatherGas;
	CSCV*			m_pSCV;
	CMove*			m_pMove;
	CAnimation*		m_pAnimCom;
	CResourceObj*	m_pResourceObj;

	RECT			m_tSCVRect;
	RECT			m_tGasRect;
	RECT			m_tCommandCetnerRect;

	int				m_iEntityActLevel;
	float			m_fGatherTime;
	float			m_fStopTime;

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

