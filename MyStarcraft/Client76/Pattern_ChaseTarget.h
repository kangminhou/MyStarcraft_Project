#pragma once
#include "EntityPattern.h"
#include "Include.h"

class CMove;

/* Ÿ������ ���� ���� �Ѿư�.. */
class CPattern_ChaseTarget :
	public CEntityPattern
{
private:
	CGameEntity*	m_pTarget;
	CMove*			m_pMoveComponent;

	D3DXVECTOR3		m_vPrevTargetPos;

	bool			m_bChaseEnemy;

public:
	CPattern_ChaseTarget( const bool& _bChaseEnemy = true );
	virtual ~CPattern_ChaseTarget();

public:
	// CEntityPattern��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

