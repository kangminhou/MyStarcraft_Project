#pragma once
#include "WeaponTool.h"


// CWeaponPage 대화 상자입니다.

class CWeaponPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWeaponPage)

private:
	CWeaponTool	m_weaponTool;

public:
	CWeaponPage();
	virtual ~CWeaponPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEAPONPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonShowWeaponTool();
};
