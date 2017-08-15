#pragma once
#include "EntityPattern.h"
#include "Include.h"

class CMove;

/* Å¸°ÙÀ¸·Î ÂïÀº ÀûÀ» ÂÑ¾Æ°¨.. */
class CPattern_ChaseTarget :
	public CEntityPattern
{
private:
	CGameEntity*	m_pTarget;
	CMove*			m_pMoveComponent;
	bool			m_bChaseEnemy;

public:
	CPattern_ChaseTarget( const bool& _bChaseEnemy = true );
	virtual ~CPattern_ChaseTarget();

public:
	// CEntityPatternÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

