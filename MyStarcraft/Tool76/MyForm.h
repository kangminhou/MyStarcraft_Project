#pragma once



// CMyForm 폼 뷰입니다.

//##[17.07.14_03-01 : 실제 메모리 할당하자.
#include "MapTool.h"
#include "PathFind.h"
#include "afxcmn.h"
#include "MySheet.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE( CMyForm )

private:
	//##[17.07.14_01]
	CFont		m_Font;

private:
	//##[17.07.14_03] : 다이얼로그를 생성하기 위한 변수를 추가.

	/* Delete.. */
	CMapTool	m_MapTool;
	CPathFind	m_PathFind;
	/* //.. */

	CMySheet*	m_pMySheet;

public:
	CMapTool* GetMapTool( void )
	{
		return &m_MapTool;
	}

	CMySheet* GetSheet( void ) const
	{
		return m_pMySheet;
	}

public:
	void Release();


protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump( CDumpContext& dc ) const;
#endif
#endif

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTabResource( NMHDR *pNMHDR, LRESULT *pResult );
};


