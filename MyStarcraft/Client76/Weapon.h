#pragma once
#include "Component.h"
#include "Include.h"

class CGameEntity;
class CWeapon :
	public CComponent
{
protected:
	WEAPON_DATA*			m_pWeaonData;			// ���� �Ŵ������� �޾ƿ� �� ( �޸� ������ ����?? )..

	float					m_fRestInterval;

public:
	CWeapon();
	virtual ~CWeapon();

public:
	void SetAttInterval( const float& _fAttInterval );
	void SetWeaponData( WEAPON_DATA* _pWeaponData );

public:
	const vector<TEX_INFO*>* GetHitTexture() const;
	const WEAPON_DATA* GetWeaponData() const;

public:
	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize();
	virtual void Release();

	void Attack( CGameEntity* _pAttTarget );

	bool IsCanAttack();

};

