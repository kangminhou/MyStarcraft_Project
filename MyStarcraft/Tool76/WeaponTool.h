#pragma once
#include "afxwin.h"

#include "Include.h"

// CWeaponTool 대화 상자입니다.

class CWeaponTool : public CDialog
{
	DECLARE_DYNAMIC(CWeaponTool)

private:
	map<wstring, pair<WEAPON_DATA*, IMAGE_PATH*>> m_mapWeaponData;
	list<IMAGE_PATH*>		m_listImagePath;

public:
	void Release();

	void WeaponListClick( const int& iIndex );

	IMAGE_PATH* FindImagePath(const CString& cstrFind);

public:
	CWeaponTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWeaponTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEAPONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditWeaponDamage();
	CComboBox m_Weapon_Upgrade_Type_ComboBox;
	CComboBox m_Weapon_Splash_Type_ComboBox;
private:
	CListBox m_WeaponData_ListBox;
public:
	afx_msg void OnBnClickedButtonAddWeaponData();
	afx_msg void OnBnClickedButtonSaveWeaponData();
private:
	CString m_WeaponDamage;
	CString m_WeaponUpgradePlus;
	CString m_WeaponCoolTime;
	CString m_WeaponMinRange;
	CString m_WeaponMaxRange;
	CString m_WeaponSplashInside;
	CString m_WeaponSplashMiddle;
	CString m_WeaponSplashOutSide;
	CString m_WeaponName;
public:
	afx_msg void OnBnClickedButtonDeleteWeaponData();
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListWeapon();
	afx_msg void OnBnClickedButtonLoadWeaponData();
	afx_msg void OnDropFiles( HDROP hDropInfo );
private:
	CListBox m_listboxPathFind;
	CString m_WeaponCurPathData;
	CString m_BulletTexturePathDirection;
	CString m_BulletSpeed;
	CString m_BulletEffect;
	CString m_BulletCastingTime;
	CString m_BulletCastingEffect;
public:
	afx_msg void OnBnClickedButtonDecideBulletTexture();
	afx_msg void OnBnClickedButtonDecideBulletEffectTexture();
	afx_msg void OnBnClickedButtonDecideBulletCastingTexture();
	afx_msg void OnBnClickedButtonDecideWeaponFireTexture();
	afx_msg void OnBnClickedButtonModifyWeaponData();
	CComboBox m_EffectShowType_ComboBox;
private:
	CString m_WeaponCurPathData2;
public:
	afx_msg void OnBnClickedButtonDecideWeaponFireTexture2();
};
