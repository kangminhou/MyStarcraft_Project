#pragma once


// CUnitPage ��ȭ �����Դϴ�.

class CUnitPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CUnitPage)

public:
	CUnitPage();
	virtual ~CUnitPage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
