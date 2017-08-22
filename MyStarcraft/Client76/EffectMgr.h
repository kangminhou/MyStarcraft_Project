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
	void PushEffect( CEffect* _pPushEffect, const eEffectKind& _eKind );

private:
	CEffect* PopEffect( const eEffectKind& _eKind );
	void AddEffect( const CEffectMgr::eEffectKind& _eEffectKind );

};
