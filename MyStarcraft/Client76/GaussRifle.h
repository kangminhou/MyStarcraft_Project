#pragma once
#include "Weapon.h"

class CGaussRifle :
	public CWeapon
{
public:
	CGaussRifle();
	virtual ~CGaussRifle();

	// CWeapon��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Release() override;
};

