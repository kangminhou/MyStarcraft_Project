#pragma once
#include "Weapon.h"

class CGaussRifle :
	public CWeapon
{
public:
	CGaussRifle();
	virtual ~CGaussRifle();

	// CWeapon을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Release() override;
};

