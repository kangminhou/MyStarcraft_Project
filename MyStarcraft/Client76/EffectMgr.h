#pragma once
#include "Include.h"

class CWeapon;
class CEffect;
class CGameObject;
class CGameEntity;

class CEffectMgr
{
	DECLARE_SINGLETON( CEffectMgr )

public:
	enum eEffectKind
	{
		Effect_Kind_Basic,
		Effect_Kind_HitTarget,
		Effect_Kind_ChaseTarget,
		Effect_Kind_LockDown,
		Effect_Kind_Nuclear,
		Effect_Kind_End
	};

private:
	queue<CEffect*>	m_queueEffectArr[CEffectMgr::Effect_Kind_End];
	list<CGameObject*>*	m_pEffectList;

public:
	CEffectMgr();
	~CEffectMgr();

public:
	void Initialize( void );
	void Release( void );

public:
	void ShowEffect( CWeapon* _pWeapon, const CGameEntity* _pEntity );
	void ShowEffect( const wstring& _wstrObjKey, const wstring& _wstrStateKey, const int& _iStart, const int& _iEnd,
					 const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vSize );
	void PushEffect( CEffect* _pPushEffect, const eEffectKind& _eKind );

	void ShowLockDown( const CGameEntity* _pFireEntity, CGameEntity* _pHitEntity );

	void ShowNuclear( CGameEntity* _pFireEntity, const D3DXVECTOR3& _vPos );

private:
	CEffect* PopEffect( const eEffectKind& _eKind );
	void AddEffect( const CEffectMgr::eEffectKind& _eEffectKind );

};

