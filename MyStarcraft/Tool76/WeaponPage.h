#pragma once
#include "WeaponTool.h"


// CWeaponPage ��ȭ �����Դϴ�.

class CWeaponPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWeaponPage)

private:
	CWeaponTool	m_weaponTool;

public:
	CWeaponPage();
	virtual ~CWeaponPage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEAPONPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonShowWeaponTool();
};
