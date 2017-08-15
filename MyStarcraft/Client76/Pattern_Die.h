#pragma once
#include "EntityPattern.h"

class CPattern_Die :
	public CEntityPattern
{
public:
	CPattern_Die();
	virtual ~CPattern_Die();

public:
	// CEntityPattern��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

