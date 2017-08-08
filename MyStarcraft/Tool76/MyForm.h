#pragma once



// CMyForm �� ���Դϴ�.

//##[17.07.14_03-01 : ���� �޸� �Ҵ�����.
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
	//##[17.07.14_03] : ���̾�α׸� �����ϱ� ���� ������ �߰�.

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
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTabResource( NMHDR *pNMHDR, LRESULT *pResult );
};


