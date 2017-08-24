#pragma once
#include "EntityPattern.h"
#include "Include.h"

class CMineral;
class CSCV;
class CMove;
class CAnimation;
class CResourceObj;

class CGatherMineral :
	public CEntityPattern
{
private:
	list<CGameEntity*>*	m_pControlList;

	CGameEntity*	m_pCommandCenter;
	CMineral*		m_pCurGatherMineral;
	CSCV*			m_pSCV;
	CMove*			m_pMove;
	CAnimation*		m_pAnimCom;
	CResourceObj*	m_pResourceObj;

	RECT			m_tSCVRect;
	RECT			m_tMineralRect;
	RECT			m_tCommandCetnerRect;

	int				m_iEntityActLevel;
	float			m_fGatherTime;
	float			m_fStopTime;

public:
	CGatherMineral();
	virtual ~CGatherMineral();

public:
	// CEntityPattern��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};
