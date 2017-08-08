#pragma once


// CUnitPage 대화 상자입니다.

class CUnitPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CUnitPage)

public:
	CUnitPage();
	virtual ~CUnitPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
