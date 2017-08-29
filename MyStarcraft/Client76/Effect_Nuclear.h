#pragma once
#include "EffectBridge.h"

class CGameEntity;
class CEntityMgr;
class CObjMgr;

class CEffect_Nuclear :
	public CEffectBridge
{
private:
	CGameEntity*	m_pFireEntity;
	CObjMgr*		m_pObjMgr;

	vector<const TEX_INFO*>	m_vecNuclearTrunkTexture;
	vector<const TEX_INFO*>	m_vecReadyTexture;
	vector<const TEX_INFO*>	m_vecBombTexture;

	const TEX_INFO*	m_pNuclearTexture;

	D3DXMATRIX		m_matNuclearWorld;
	D3DXVECTOR3		m_vNuclearPos;

	int				m_iLevel;
	float			m_fWaitTime;
	float			m_fNuclearLunchWaitTime;

	bool			m_bDie;
	bool			m_bNuclearLunchSoundPlay;

public:
	CEffect_Nuclear();
	virtual ~CEffect_Nuclear();

	void SetFireEntity( CGameEntity* _pFireEntity );

public:
	// CEffectBridge을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render() override;
	virtual void Release() override;

};

