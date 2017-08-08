// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool76.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

void CMyForm::Release()
{
}

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY( TCN_SELCHANGE, IDC_TAB_RESOURCE, &CMyForm::OnTcnSelchangeTabResource )
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	//##[17.07.14_02]
	//m_Font.CreatePointFont(200, L"궁서");

	m_pMySheet = new CMySheet;

	//시트클래스를 동적할당은 받았지만 시트자체를 생성하지 않았기 때문에
	//탭 기능들이 들어갈수 있도록 시트를 만들어 준다.
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);			

	//생성된 시트의 크기를 지정하자.
	m_pMySheet->MoveWindow(0, 0, 300, 400);

	//GetDlgItem(IDC_BUTTON_TEST)->SetFont(&m_Font);
	//GetDlgItem() : 특정 컨트롤에 기능수행을 도와주는 함수.
}


void CMyForm::OnTcnSelchangeTabResource( NMHDR *pNMHDR, LRESULT *pResult )
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

}
