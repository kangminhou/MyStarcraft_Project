#pragma once
#include "EntityPattern.h"

/* �ֺ��� ���� �˻��Ͽ� ���� �þ� �ȿ� �Դٸ� �� ���� �������� �ٲ�.. */
class CPattern_Move :
	public CEntityPattern
{
private:
	D3DXVECTOR3 m_vDestination;

public:
	CPattern_Move();
	virtual ~CPattern_Move(); 

public:
	// CEntityPattern��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

};

