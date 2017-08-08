// ObjectPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool76.h"
#include "ObjectPage.h"
#include "afxdialogex.h"


// CObjectPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectPage, CPropertyPage)

CObjectPage::CObjectPage()
	: CPropertyPage(IDD_OBJECTPAGE)
{

}

CObjectPage::~CObjectPage()
{
}

void CObjectPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjectPage, CPropertyPage)
END_MESSAGE_MAP()


// CObjectPage 메시지 처리기입니다.
