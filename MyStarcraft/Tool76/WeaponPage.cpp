// WeaponPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool76.h"
#include "WeaponPage.h"
#include "afxdialogex.h"


// CWeaponPage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWeaponPage, CPropertyPage)

CWeaponPage::CWeaponPage()
	: CPropertyPage(IDD_WEAPONPAGE)
{

}

CWeaponPage::~CWeaponPage()
{
}

void CWeaponPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWeaponPage, CPropertyPage)
	ON_BN_CLICKED( IDC_BUTTON_SHOW_WEAPON_TOOL, &CWeaponPage::OnBnClickedButtonShowWeaponTool )
END_MESSAGE_MAP()


// CWeaponPage �޽��� ó�����Դϴ�.


void CWeaponPage::OnBnClickedButtonShowWeaponTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_weaponTool.GetSafeHwnd() == NULL)
		m_weaponTool.Create(IDD_WEAPONTOOL);

	m_weaponTool.ShowWindow(SW_SHOW);

}
