// UnitPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool76.h"
#include "UnitPage.h"
#include "afxdialogex.h"


// CUnitPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitPage, CPropertyPage)

CUnitPage::CUnitPage()
	: CPropertyPage(IDD_UNITPAGE)
{

}

CUnitPage::~CUnitPage()
{
}

void CUnitPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUnitPage, CPropertyPage)
END_MESSAGE_MAP()


// CUnitPage 메시지 처리기입니다.
