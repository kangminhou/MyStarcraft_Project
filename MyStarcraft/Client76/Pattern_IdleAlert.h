#pragma once
#include "EntityPattern.h"
#include "Include.h"

/* �ֺ��� ���� �˻��Ͽ� ���� �þ� �ȿ� �Դٸ� �� ���� �������� �ٲ�.. */
class CPattern_IdleAlert :
	public CEntityPattern
{
private:
	eObjectType m_eEnemyType;

public:
	CPattern_IdleAlert();
	virtual ~CPattern_IdleAlert();

	// CEntityPattern��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

