// MyForm.cpp : ���� �����Դϴ�.
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


// CMyForm �����Դϴ�.

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


// CMyForm �޽��� ó�����Դϴ�.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	//##[17.07.14_02]
	//m_Font.CreatePointFont(200, L"�ü�");

	m_pMySheet = new CMySheet;

	//��ƮŬ������ �����Ҵ��� �޾����� ��Ʈ��ü�� �������� �ʾұ� ������
	//�� ��ɵ��� ���� �ֵ��� ��Ʈ�� ����� �ش�.
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);			

	//������ ��Ʈ�� ũ�⸦ ��������.
	m_pMySheet->MoveWindow(0, 0, 300, 400);

	//GetDlgItem(IDC_BUTTON_TEST)->SetFont(&m_Font);
	//GetDlgItem() : Ư�� ��Ʈ�ѿ� ��ɼ����� �����ִ� �Լ�.
}


void CMyForm::OnTcnSelchangeTabResource( NMHDR *pNMHDR, LRESULT *pResult )
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

}
