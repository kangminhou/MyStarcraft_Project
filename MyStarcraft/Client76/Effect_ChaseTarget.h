#pragma once
#include "EffectBridge.h"

class CGameEntity;
class CWeapon;

class CEffect_ChaseTarget :
	public CEffectBridge
{
private:
	CGameEntity*	m_pTarget;
	CWeapon*		m_pWeapon;

	bool			m_bUseFollowDustEffect;

public:
	CEffect_ChaseTarget();
	virtual ~CEffect_ChaseTarget();

public:
	void SetWeapon( CWeapon* _pWeapon );
	void SetIsUseFollowDustEffect( const bool& _bUseFollowDustEffect );

public:
	// CEffectBridge을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render() override;
	virtual void Release() override;
};

