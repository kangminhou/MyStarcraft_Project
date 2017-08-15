#pragma once
#include "EntityPattern.h"
#include "Include.h"

/* �ֺ��� ���� �˻��Ͽ� ���� �þ� �ȿ� �Դٸ� �� ���� �������� �ٲ�.. */
class CPattern_IdleAlert :
	public CEntityPattern
{
private:
	bool	m_bAlertEnemy;

public:
	CPattern_IdleAlert(const bool& _bAlertEnemy = true);
	virtual ~CPattern_IdleAlert();

	// CEntityPattern��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

