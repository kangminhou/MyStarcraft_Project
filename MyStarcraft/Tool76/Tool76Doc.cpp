
// Tool76Doc.cpp : CTool76Doc Ŭ������ ����
//

#include "stdafx.h"
#include "Tool76.h"

#include "Tool76Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTool76Doc

IMPLEMENT_DYNCREATE(CTool76Doc, CDocument)

BEGIN_MESSAGE_MAP(CTool76Doc, CDocument)
END_MESSAGE_MAP()


// CTool76Doc ����/�Ҹ�

CTool76Doc::CTool76Doc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CTool76Doc::~CTool76Doc()
{
}

BOOL CTool76Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CTool76Doc serialization

void CTool76Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CTool76Doc ����

#ifdef _DEBUG
void CTool76Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTool76Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTool76Doc ���
