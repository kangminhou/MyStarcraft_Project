#pragma once
#include "EffectBridge.h"

class CGameEntity;
class CWeapon;

class CEffect_HitTarget :
	public CEffectBridge
{
private:
	CWeapon*		m_pWeapon;
	CGameEntity*	m_pTarget;
	D3DXVECTOR3		m_vDestination;

	bool			m_bLookDestination;

public:
	CEffect_HitTarget();
	virtual ~CEffect_HitTarget();

public:
	void SetWeapon( CWeapon* _pWeapon );

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render() override;
	virtual void Release() override;

};

