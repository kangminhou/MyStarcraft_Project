// WeaponPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool76.h"
#include "WeaponPage.h"
#include "afxdialogex.h"


// CWeaponPage 대화 상자입니다.

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


// CWeaponPage 메시지 처리기입니다.


void CWeaponPage::OnBnClickedButtonShowWeaponTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_weaponTool.GetSafeHwnd() == NULL)
		m_weaponTool.Create(IDD_WEAPONTOOL);

	m_weaponTool.ShowWindow(SW_SHOW);

}
