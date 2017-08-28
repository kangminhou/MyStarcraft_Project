#pragma once
#include "EntityPattern.h"

class CPattern_Stop :
	public CEntityPattern
{
public:
	CPattern_Stop();
	virtual ~CPattern_Stop();

public:
	// CEntityPattern��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

