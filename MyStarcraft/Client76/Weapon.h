#pragma once
#include "Component.h"
#include "Include.h"

class CWeapon :
	public CComponent
{
protected:
	TEX_INFO*		m_pHitTexture;
	WEAPON_DATA		m_tWeaonData;

	float			m_fRestInterval;

public:
	CWeapon();
	virtual ~CWeapon();

public:
	void SetAttInterval( const float& _fAttInterval );

public:
	const TEX_INFO* GetHitTexture() const;
	const WEAPON_DATA* GetWeaponData();

public:
	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize() PURE;
	virtual void Release() PURE;

	bool IsCanAttack();

};

