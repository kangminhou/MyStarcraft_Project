#pragma once


// CPathFind ��ȭ �����Դϴ�.

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
	CPathFind( CWnd* pParent = NULL );   // ǥ�� �������Դϴ�.
	virtual ~CPathFind();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PATHFIND };

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListPath();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnDropFiles( HDROP hDropInfo );
	CListBox m_listboxPathFind;
};
