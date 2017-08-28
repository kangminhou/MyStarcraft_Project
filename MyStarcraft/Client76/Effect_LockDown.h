#pragma once
#include "EffectBridge.h"

class CGameEntity;

class CEffect_LockDown :
	public CEffectBridge
{
private:
	vector<const TEX_INFO*>	m_vecBulletTexture;

	CGameEntity*	m_pTarget;

	D3DXVECTOR3		m_vBeginPos;

	float			m_fDestoryTime;
	float			m_fLerpT;

	BYTE			m_byBulletDir;
	BYTE			m_byLockDownLevel;

	bool			m_bHitTarget;
	bool			m_bDestory;

public:
	CEffect_LockDown();
	virtual ~CEffect_LockDown();

public:
	void SetTarget( CGameEntity* _pTarget );

public:
	// CEffectBridge을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render() override;
	virtual void Release() override;

};

