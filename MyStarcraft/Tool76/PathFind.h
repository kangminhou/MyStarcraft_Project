#pragma once


// CPathFind 대화 상자입니다.

#include "Include.h"
#include "afxwin.h"

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC( CPathFind )

private:
	list<IMAGE_PATH*>		m_listImagePath;

public:
	void Release( void );

public:
	CPathFind( CWnd* pParent = NULL );   // 표준 생성자입니다.
	virtual ~CPathFind();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATHFIND };

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListPath();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnDropFiles( HDROP hDropInfo );
	CListBox m_listboxPathFind;
};
