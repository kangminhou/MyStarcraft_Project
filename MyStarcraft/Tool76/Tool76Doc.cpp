
// Tool76Doc.cpp : CTool76Doc 클래스의 구현
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


// CTool76Doc 생성/소멸

CTool76Doc::CTool76Doc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CTool76Doc::~CTool76Doc()
{
}

BOOL CTool76Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CTool76Doc serialization

void CTool76Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CTool76Doc 진단

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


// CTool76Doc 명령
