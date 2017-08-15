#pragma once
#include "Component.h"
#include "Include.h"

class CGameEntity;
class CWeapon :
	public CComponent
{
protected:
	WEAPON_DATA*			m_pWeaonData;			// 웨폰 매니저에서 받아올 것 ( 메모리 절약을 위해?? )..
	CGameEntity*			m_pOwnerEntity;

	float					m_fRestInterval;

public:
	CWeapon();
	virtual ~CWeapon();

public:
	void SetAttInterval( const float& _fAttInterval );
	void SetWeaponData( WEAPON_DATA* _pWeaponData );
	void SetWeaponOwner( CGameEntity* _pOwnerEntity );

public:
	const vector<TEX_INFO*>* GetHitTexture() const;
	const WEAPON_DATA* GetWeaponData() const;

public:
	// CComponent을(를) 통해 상속됨
	virtual void Initialize();
	virtual void Release();

	void Attack( CGameEntity* _pAttTarget );
	bool AttackCoolTimeUpdate();

	bool IsCanAttack();

private:
	void NormalAttack( CGameEntity* _pAttTarget );
	void NormalSplashAttack( CGameEntity* _pAttTarget );
	void CircleSplashAttack( CGameEntity* _pAttTarget );

};

