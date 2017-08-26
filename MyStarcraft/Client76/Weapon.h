#pragma once
#include "Component.h"
#include "Include.h"

class CGameEntity;
class CWeapon :
	public CComponent
{
protected:
	WEAPON_DATA*			m_pWeaonData;			// ���� �Ŵ������� �޾ƿ� �� ( �޸� ������ ����?? )..
	CGameEntity*			m_pOwnerEntity;
	CGameEntity*			m_pTarget;

	float					m_fRestInterval;
	float					m_fAttInterval;

public:
	CWeapon();
	virtual ~CWeapon();

public:
	void SetAttInterval( const float& _fAttInterval );
	void SetWeaponData( WEAPON_DATA* _pWeaponData );
	void SetWeaponOwner( CGameEntity* _pOwnerEntity );

public:
	const WEAPON_DATA* GetWeaponData() const;
	const CGameEntity* GetEntity() const;
	const CGameEntity* GetTarget() const;

public:
	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize();
	virtual void Release();

	void Attack( CGameEntity* _pAttTarget );
	void HitTarget();
	bool AttackCoolTimeUpdate();

	bool IsCanAttack();

private:
	void NormalAttack();
	void NormalSplashAttack();
	void CircleSplashAttack();

};

