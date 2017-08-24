#pragma once
#include "EntityPattern.h"

class CGatherGas :
	public CEntityPattern
{
public:
	CGatherGas();
	virtual ~CGatherGas();

public:
	// CEntityPattern��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

