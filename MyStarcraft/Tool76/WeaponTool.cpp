// WeaponTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool76.h"
#include "WeaponTool.h"
#include "afxdialogex.h"


// CWeaponTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWeaponTool, CDialog)

void CWeaponTool::Release()
{
	for ( auto iter = this->m_mapWeaponData.begin(); iter != this->m_mapWeaponData.end(); ++iter )
	{
		safe_delete( iter->second.first );
		safe_delete( iter->second.second );
	}
	this->m_mapWeaponData.clear();
	this->m_WeaponData_ListBox.ResetContent();

}

void CWeaponTool::WeaponListClick( const int & iIndex )
{
	CString cstrFindWeaponName;
	this->m_WeaponData_ListBox.GetText( iIndex, cstrFindWeaponName );
	auto& iter = this->m_mapWeaponData.find( cstrFindWeaponName.GetString() );

	if ( iter == this->m_mapWeaponData.end() )
		return;

	m_WeaponName = iter->second.first->wstrWeaponName.c_str();

	m_WeaponDamage.Format( L"%0.f", iter->second.first->fDamage );
	m_WeaponCoolTime.Format( L"%0.f", iter->second.first->fAttInterval );

	m_Weapon_Upgrade_Type_ComboBox.SetCurSel( iter->second.first->eUpgradeType );
	m_WeaponUpgradePlus.Format( L"%d", iter->second.first->iUpgradePlus );

	m_WeaponMinRange.Format( L"%d", iter->second.first->iMinAttRange );
	m_WeaponMaxRange.Format( L"%d", iter->second.first->iMaxAttRange );

	m_Weapon_Splash_Type_ComboBox.SetCurSel( iter->second.first->eWeaponAttType );
	m_WeaponSplashInside.Format( L"%d", iter->second.first->iInside );
	m_WeaponSplashMiddle.Format( L"%d", iter->second.first->iMiddle );
	m_WeaponSplashOutSide.Format( L"%d", iter->second.first->iOutSide );

	m_EffectShowType_ComboBox.SetCurSel( iter->second.first->eEffectShowKind );

	if ( !iter->second.second )
		this->m_WeaponCurPathData = L"";
	else
	{
		TCHAR str[MAX_PATH];
		swprintf_s( str, L"%s|%s|%d|%s", iter->second.second->wstrObjKey.c_str(), iter->second.second->wstrStateKey.c_str(),
					iter->second.second->iCount, iter->second.second->wstrPath.c_str() );

		this->m_WeaponCurPathData = str;
	}

	if ( iter->second.first->eEffectShowKind == Effect_Show_Hit_Target ||
		 iter->second.first->eEffectShowKind == Effect_Show_Chase_Target )
	{
		if ( iter->second.first->pBulletData )
		{
			m_BulletSpeed.Format( L"%f", iter->second.first->pBulletData->fSpeed );
			m_BulletCastingTime.Format( L"%f", iter->second.first->pBulletData->fCastingTime );

			TCHAR str[MAX_PATH];
			if ( iter->second.first->pBulletData->pImagePath )
			{
				swprintf_s( str, L"%s|%s|%d|%s", iter->second.first->pBulletData->pImagePath->wstrObjKey.c_str(),
							iter->second.first->pBulletData->pImagePath->wstrStateKey.c_str(),
							iter->second.first->pBulletData->pImagePath->iCount,
							iter->second.first->pBulletData->pImagePath->wstrPath.c_str() );

				m_BulletTexturePathDirection = str;
			}

			if ( iter->second.first->pBulletData->pEffectImaePath )
			{
				swprintf_s( str, L"%s|%s|%d|%s", iter->second.first->pBulletData->pEffectImaePath->wstrObjKey.c_str(),
							iter->second.first->pBulletData->pEffectImaePath->wstrStateKey.c_str(),
							iter->second.first->pBulletData->pEffectImaePath->iCount,
							iter->second.first->pBulletData->pEffectImaePath->wstrPath.c_str() );

				m_BulletEffect = str;
			}

			if ( iter->second.first->pBulletData->pCastingImagePath )
			{
				swprintf_s( str, L"%s|%s|%d|%s", iter->second.first->pBulletData->pCastingImagePath->wstrObjKey.c_str(),
							iter->second.first->pBulletData->pCastingImagePath->wstrStateKey.c_str(),
							iter->second.first->pBulletData->pCastingImagePath->iCount,
							iter->second.first->pBulletData->pCastingImagePath->wstrPath.c_str() );

				m_BulletCastingEffect = str;
			}

		}
		else
		{
			m_BulletTexturePathDirection = L"";
			m_BulletEffect = L"";
			m_BulletCastingEffect = L"";
		}
	}
	else
	{
		m_BulletTexturePathDirection = L"";
		m_BulletEffect = L"";
		m_BulletCastingEffect = L"";
	}

	if ( iter->second.first->pImagePath2 )
	{
		TCHAR str[MAX_PATH] = L"";

		swprintf_s( str, L"%s|%s|%d|%s", iter->second.first->pImagePath2->wstrObjKey.c_str(),
					iter->second.first->pImagePath2->wstrStateKey.c_str(),
					iter->second.first->pImagePath2->iCount,
					iter->second.first->pImagePath2->wstrPath.c_str() );

		m_WeaponCurPathData2 = str;
	}
	else
		m_WeaponCurPathData2 = L"";

	UpdateData( FALSE );
}

IMAGE_PATH* CWeaponTool::FindImagePath( const CString& cstrFind )
{
	IMAGE_PATH* pFindImagePath = nullptr;

	for ( auto iter = m_listImagePath.begin(); iter != m_listImagePath.end(); ++iter )
	{
		TCHAR strCount[128];
		swprintf_s( strCount, L"%d", (*iter)->iCount );
		if ( cstrFind.Find( (*iter)->wstrObjKey.c_str() ) == -1 )
			continue;
		else if ( cstrFind.Find( (*iter)->wstrStateKey.c_str() ) == -1 )
			continue;
		else if ( cstrFind.Find( (*iter)->wstrPath.c_str() ) == -1 )
			continue;
		else if ( cstrFind.Find( strCount ) == -1 )
			continue;

		pFindImagePath = (*iter);
		break;
	}

	return pFindImagePath;
}

CWeaponTool::CWeaponTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_WEAPONTOOL, pParent)
	, m_WeaponDamage( _T( "" ) )
	, m_WeaponUpgradePlus( _T( "" ) )
	, m_WeaponCoolTime( _T( "" ) )
	, m_WeaponMinRange( _T( "" ) )
	, m_WeaponMaxRange( _T( "" ) )
	, m_WeaponSplashInside( _T( "" ) )
	, m_WeaponSplashMiddle( _T( "" ) )
	, m_WeaponSplashOutSide( _T( "" ) )
	, m_WeaponName( _T( "" ) )
	, m_WeaponCurPathData( _T( "" ) )
	, m_BulletTexturePathDirection( _T( "" ) )
	, m_BulletSpeed( _T( "" ) )
	, m_BulletEffect( _T( "" ) )
	, m_BulletCastingTime( _T( "" ) )
	, m_BulletCastingEffect( _T( "" ) )
	, m_WeaponCurPathData2( _T( "" ) )
{
}

CWeaponTool::~CWeaponTool()
{
	Release();

	for ( auto iter = this->m_listImagePath.begin(); iter != this->m_listImagePath.end(); ++iter )
	{
		safe_delete( (*iter) );
	}
	this->m_listImagePath.clear();

	this->m_listboxPathFind.ResetContent();
}

void CWeaponTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_COMBO_WEAPON_UPGRADE_TYPE, m_Weapon_Upgrade_Type_ComboBox );
	DDX_Control( pDX, IDC_COMBO_WEAPON_SPLASH_TYPE, m_Weapon_Splash_Type_ComboBox );
	DDX_Control( pDX, IDC_LIST_WEAPON, m_WeaponData_ListBox );
	DDX_Text( pDX, IDC_EDIT_WEAPON_DAMAGE, m_WeaponDamage );
	DDX_Text( pDX, IDC_EDIT_WEAPON_UPGRADE_PLUS, m_WeaponUpgradePlus );
	DDX_Text( pDX, IDC_EDIT_WEAPON_COOLTIME, m_WeaponCoolTime );
	DDX_Text( pDX, IDC_EDIT_WEAPON_MIN_RANGE, m_WeaponMinRange );
	DDX_Text( pDX, IDC_EDIT_WEAPON_MAX_RANGE, m_WeaponMaxRange );
	DDX_Text( pDX, IDC_EDIT_WEAPON_SPLASH_INSIDE, m_WeaponSplashInside );
	DDX_Text( pDX, IDC_EDIT_WEAPON_SPLASH_MIDDLE, m_WeaponSplashMiddle );
	DDX_Text( pDX, IDC_EDIT_WEAPON_SPLASH_OUTSIDE, m_WeaponSplashOutSide );
	DDX_Text( pDX, IDC_EDIT_WEAPON_NAME, m_WeaponName );
	DDX_Control( pDX, IDC_LIST_TEXTURE_DIRECTION, m_listboxPathFind );
	DDX_Text( pDX, IDC_EDIT_CUR_WEAPON_PATH_DATA, m_WeaponCurPathData );
	DDX_Text( pDX, IDC_EDIT_BULLET_TEXTURE_PATH, m_BulletTexturePathDirection );
	DDX_Text( pDX, IDC_EDIT_BULLET_SPEED, m_BulletSpeed );
	DDX_Text( pDX, IDC_EDIT_BULLET_EFFECT, m_BulletEffect );
	DDX_Text( pDX, IDC_EDIT_BULLET_CASTING_TIME, m_BulletCastingTime );
	DDX_Text( pDX, IDC_EDIT_BULLET_CASTING_EFFECT, m_BulletCastingEffect );
	DDX_Control( pDX, IDC_COMBO_EFFECT_SHOW_TYPE, m_EffectShowType_ComboBox );
	DDX_Text( pDX, IDC_EDIT_CUR_WEAPON_PATH_DATA2, m_WeaponCurPathData2 );
}


BEGIN_MESSAGE_MAP(CWeaponTool, CDialog)
	ON_EN_CHANGE( IDC_EDIT_WEAPON_DAMAGE, &CWeaponTool::OnEnChangeEditWeaponDamage )
	ON_BN_CLICKED( IDC_BUTTON_ADD_WEAPON_DATA, &CWeaponTool::OnBnClickedButtonAddWeaponData )
	ON_BN_CLICKED( IDC_BUTTON_SAVE_WEAPON_DATA, &CWeaponTool::OnBnClickedButtonSaveWeaponData )
	ON_BN_CLICKED( IDC_BUTTON_DELETE_WEAPON_DATA, &CWeaponTool::OnBnClickedButtonDeleteWeaponData )
	ON_LBN_SELCHANGE( IDC_LIST_WEAPON, &CWeaponTool::OnLbnSelchangeListWeapon )
	ON_BN_CLICKED( IDC_BUTTON_LOAD_WEAPON_DATA, &CWeaponTool::OnBnClickedButtonLoadWeaponData )
	ON_WM_DROPFILES()
	ON_BN_CLICKED( IDC_BUTTON_DECIDE_BULLET_TEXTURE, &CWeaponTool::OnBnClickedButtonDecideBulletTexture )
	ON_BN_CLICKED( IDC_BUTTON_DECIDE_BULLET_EFFECT_TEXTURE, &CWeaponTool::OnBnClickedButtonDecideBulletEffectTexture )
	ON_BN_CLICKED( IDC_BUTTON_DECIDE_BULLET_CASTING_TEXTURE, &CWeaponTool::OnBnClickedButtonDecideBulletCastingTexture )
	ON_BN_CLICKED( IDC_BUTTON_DECIDE_WEAPON_FIRE_TEXTURE, &CWeaponTool::OnBnClickedButtonDecideWeaponFireTexture )
	ON_BN_CLICKED( IDC_BUTTON_MODIFY_WEAPON_DATA, &CWeaponTool::OnBnClickedButtonModifyWeaponData )
	ON_BN_CLICKED( IDC_BUTTON_DECIDE_WEAPON_FIRE_TEXTURE2, &CWeaponTool::OnBnClickedButtonDecideWeaponFireTexture2 )
END_MESSAGE_MAP()


// CWeaponTool �޽��� ó�����Դϴ�.


void CWeaponTool::OnEnChangeEditWeaponDamage()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CWeaponTool::OnBnClickedButtonAddWeaponData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WEAPON_DATA* pWeaponData = new WEAPON_DATA;

	UpdateData( TRUE );

	pWeaponData->wstrWeaponName = m_WeaponName;

	pWeaponData->fDamage = _ttof( m_WeaponDamage );
	pWeaponData->fAttInterval = _ttof( m_WeaponCoolTime );

	pWeaponData->eUpgradeType = (eWeaponUpgradeType)m_Weapon_Upgrade_Type_ComboBox.GetCurSel();
	pWeaponData->iUpgradePlus = _ttof( m_WeaponUpgradePlus );

	pWeaponData->iMinAttRange = _ttof( m_WeaponMinRange );
	pWeaponData->iMaxAttRange = _ttof( m_WeaponMaxRange );

	pWeaponData->eWeaponAttType = (eWeaponAttType)m_Weapon_Splash_Type_ComboBox.GetCurSel();
	pWeaponData->iInside = _ttof( m_WeaponSplashInside );
	pWeaponData->iMiddle = _ttof( m_WeaponSplashMiddle );
	pWeaponData->iOutSide = _ttof( m_WeaponSplashOutSide );

	pWeaponData->eEffectShowKind = (eEffectShowKind)m_EffectShowType_ComboBox.GetCurSel();

	if ( pWeaponData->eEffectShowKind == Effect_Show_Hit_Target ||
		 pWeaponData->eEffectShowKind == Effect_Show_Chase_Target )
	{
		pWeaponData->pBulletData = new BULLET_DATA;
		pWeaponData->pBulletData->fSpeed = _ttof( m_BulletSpeed );
		pWeaponData->pBulletData->fCastingTime = _ttof( m_BulletCastingTime );

		if ( m_BulletTexturePathDirection != L"NULL" )
		{
			pWeaponData->pBulletData->pImagePath = this->FindImagePath( m_BulletTexturePathDirection );
		}
		else
			pWeaponData->pBulletData->pImagePath = NULL;

		if ( m_BulletEffect != L"NULL" )
		{
			pWeaponData->pBulletData->pEffectImaePath = this->FindImagePath( m_BulletEffect );
		}
		else
			pWeaponData->pBulletData->pEffectImaePath = NULL;

		if ( m_BulletCastingEffect != L"NULL" )
		{
			pWeaponData->pBulletData->pCastingImagePath = this->FindImagePath( m_BulletCastingEffect );
		}
		else
			pWeaponData->pBulletData->pCastingImagePath = NULL;
	}
	else
	{
		pWeaponData->pBulletData = NULL;
	}

	if ( m_WeaponCurPathData2 != L"" )
	{
		pWeaponData->pImagePath2 = this->FindImagePath( m_WeaponCurPathData2 );
		m_WeaponCurPathData2 = L"";
	}
	else
		pWeaponData->pImagePath2 = NULL;

	IMAGE_PATH* pImagePath = NULL;
	pImagePath = this->FindImagePath( this->m_WeaponCurPathData );

	this->m_WeaponCurPathData = L"";

	this->m_mapWeaponData.insert( make_pair( m_WeaponName, make_pair( pWeaponData, pImagePath ) ) );
	this->m_WeaponData_ListBox.AddString( m_WeaponName );

}


void CWeaponTool::OnBnClickedButtonSaveWeaponData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog		Dlg(
		FALSE,		
		L"xml",		
		L"*.xml",	
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	
		L"*.xml",	
		this		
	);

	TCHAR szDirectory[MAX_PATH] = L"";		//����̺��θ� ���� ���ڿ� ����.

	GetCurrentDirectory(MAX_PATH, szDirectory);		//���� ������Ʈ�� ����̺��θ� �޾ƿ´�.
	PathRemoveFileSpec(szDirectory);

	lstrcat(szDirectory, L"\\Xml");

	Dlg.GetOFN().lpstrInitialDir =  szDirectory;

	if(Dlg.DoModal() == IDCANCEL)
		return;
	else
	{
		if ( m_mapWeaponData.empty() )
		{
			MessageBox( L"������ �����Ͱ� �����ϴ�.!", L"������ ����!" );
			return;
		}


		//##[17.08.01_01] : xml������ �غ���.
		IXMLDOMDocument2Ptr	pDoc;

		MSXML2::IXMLDOMProcessingInstructionPtr pPI;

		list<int>::iterator iter;

		//�ν��Ͻ��� ��������.
		pDoc.CreateInstance( __uuidof(MSXML2::DOMDocument) );

		//Root Element����.
		MSXML2::IXMLDOMElementPtr	pRootElementItem;

		/*
		tWeaponData.fDamage = _ttof( m_WeaponDamage );
		tWeaponData.fAttInterval = _ttof( m_WeaponCoolTime );

		tWeaponData.eUpgradeType = (eWeaponUpgradeType)m_Weapon_Upgrade_Type_ComboBox.GetCurSel();
		tWeaponData.iUpgradePlus = _ttof( m_WeaponUpgradePlus );

		tWeaponData.iMinAttRange = _ttof( m_WeaponMinRange );
		tWeaponData.iMaxAttRange = _ttof( m_WeaponMaxRange );

		tWeaponData.eSplashType = (eUnitSplashAttType)m_Weapon_Splash_Type_ComboBox.GetCurSel();
		tWeaponData.iInside = _ttof( m_WeaponSplashInside );
		tWeaponData.iMiddle = _ttof( m_WeaponSplashMiddle );
		tWeaponData.iOutSide = _ttof( m_WeaponSplashOutSide );
		*/


		//���� ������ Element����.
		MSXML2::IXMLDOMElementPtr	pElement_Name;

		MSXML2::IXMLDOMElementPtr	pElement_Damage;
		MSXML2::IXMLDOMElementPtr	pElement_AttInterval;

		MSXML2::IXMLDOMElementPtr	pElement_UpgradeType;
		MSXML2::IXMLDOMElementPtr	pElement_UpgradePlus;

		MSXML2::IXMLDOMElementPtr	pElement_MinAttRange;
		MSXML2::IXMLDOMElementPtr	pElement_MaxAttRange;

		MSXML2::IXMLDOMElementPtr	pElement_SplashType;
		MSXML2::IXMLDOMElementPtr	pElement_SplashInside;
		MSXML2::IXMLDOMElementPtr	pElement_SplashMiddle;
		MSXML2::IXMLDOMElementPtr	pElement_SplashOutside;

		MSXML2::IXMLDOMElementPtr	pElement_ObjKey;
		MSXML2::IXMLDOMElementPtr	pElement_StateKey;
		MSXML2::IXMLDOMElementPtr	pElement_Path;
		MSXML2::IXMLDOMElementPtr	pElement_Count;

		MSXML2::IXMLDOMElementPtr	pElement_ShowEffectKind;

		MSXML2::IXMLDOMElementPtr	pElement_Image2ObjKey;
		MSXML2::IXMLDOMElementPtr	pElement_Image2StateKey;
		MSXML2::IXMLDOMElementPtr	pElement_Image2Path;
		MSXML2::IXMLDOMElementPtr	pElement_Image2Count;

		MSXML2::IXMLDOMElementPtr	pElement_fCastingTime;
		MSXML2::IXMLDOMElementPtr	pElement_fSpeed;
		
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_ObjKey;
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_StateKey;
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_Path;
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_Count;

		MSXML2::IXMLDOMElementPtr	pElement_Bullet_Casting_ObjKey;
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_Casting_StateKey;
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_Casting_Path;
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_Casting_Count;

		MSXML2::IXMLDOMElementPtr	pElement_Bullet_EffectObjKey;
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_EffectStateKey;
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_EffectPath;
		MSXML2::IXMLDOMElementPtr	pElement_Bullet_EffectCount;




		MSXML2::IXMLDOMElementPtr	pElement_Line;		//�����͸� �����ϱ� ���� �����ϴ�.

		MSXML2::IXMLDOMTextPtr		pText_Name;			//xml Text����.




		MSXML2::IXMLDOMTextPtr		pText_Damage;
		MSXML2::IXMLDOMTextPtr		pText_AttInterval;

		MSXML2::IXMLDOMTextPtr		pText_UpgradeType;
		MSXML2::IXMLDOMTextPtr		pText_UpgradePlus;

		MSXML2::IXMLDOMTextPtr		pText_MinAttRange;
		MSXML2::IXMLDOMTextPtr		pText_MaxAttRange;

		MSXML2::IXMLDOMTextPtr		pText_SplashType;
		MSXML2::IXMLDOMTextPtr		pText_SplashInside;
		MSXML2::IXMLDOMTextPtr		pText_SplashMiddle;
		MSXML2::IXMLDOMTextPtr		pText_SplashOutside;

		MSXML2::IXMLDOMTextPtr		pText_ObjKey;
		MSXML2::IXMLDOMTextPtr		pText_StateKey;
		MSXML2::IXMLDOMTextPtr		pText_Path;
		MSXML2::IXMLDOMTextPtr		pText_Count;

		MSXML2::IXMLDOMTextPtr		pText_ShowEffectKind;

		MSXML2::IXMLDOMTextPtr		pText_Image2ObjKey;
		MSXML2::IXMLDOMTextPtr		pText_Image2StateKey;
		MSXML2::IXMLDOMTextPtr		pText_Image2Path;
		MSXML2::IXMLDOMTextPtr		pText_Image2Count;

		MSXML2::IXMLDOMTextPtr		pText_fCastingTime;
		MSXML2::IXMLDOMTextPtr		pText_fSpeed;

		MSXML2::IXMLDOMTextPtr		pText_Bullet_ObjKey;
		MSXML2::IXMLDOMTextPtr		pText_Bullet_StateKey;
		MSXML2::IXMLDOMTextPtr		pText_Bullet_Path;
		MSXML2::IXMLDOMTextPtr		pText_Bullet_Count;

		MSXML2::IXMLDOMTextPtr		pText_Bullet_Casting_ObjKey;
		MSXML2::IXMLDOMTextPtr		pText_Bullet_Casting_StateKey;
		MSXML2::IXMLDOMTextPtr		pText_Bullet_Casting_Path;
		MSXML2::IXMLDOMTextPtr		pText_Bullet_Casting_Count;

		MSXML2::IXMLDOMTextPtr		pText_Bullet_EffectObjKey;
		MSXML2::IXMLDOMTextPtr		pText_Bullet_EffectStateKey;
		MSXML2::IXMLDOMTextPtr		pText_Bullet_EffectPath;
		MSXML2::IXMLDOMTextPtr		pText_Bullet_EffectCount;


		//xml����� ����� ����.
		pPI = pDoc->createProcessingInstruction(
			L"xml", L"version=\"1.0\" encoding=\"euc-kr\"");


		//Root Element����.
		pRootElementItem = pDoc->createElement(L"WEAPON_DATA");

		for ( auto iter = m_mapWeaponData.begin(); iter != m_mapWeaponData.end(); ++iter )
		{
			//�����Ϳ� ���� Element�� ����.
			/* ������ ������.. */
			pElement_Name = pDoc->createElement( L"NAME" );

			pElement_Damage = pDoc->createElement( L"DAMAGE" );
			pElement_AttInterval = pDoc->createElement( L"ATTACK_INTERVAL" );

			pElement_UpgradeType = pDoc->createElement( L"UPGRADE_TYPE" );
			pElement_UpgradePlus = pDoc->createElement( L"UPGRADE_PLUS" );

			pElement_MinAttRange = pDoc->createElement( L"MIN_ATTACK_RANGE" );
			pElement_MaxAttRange = pDoc->createElement( L"MAX_ATTACK_RANGE" );

			pElement_SplashType = pDoc->createElement( L"SPLASH_TYPE" );
			pElement_SplashInside = pDoc->createElement( L"SPLASH_INSIDE" );
			pElement_SplashMiddle = pDoc->createElement( L"SPLASH_MIDDLE" );
			pElement_SplashOutside = pDoc->createElement( L"SPLASH_OUTSIDE" );

			/* ���� �߻� ����Ʈ.. */
			pElement_ObjKey = pDoc->createElement( L"IMAGE_OBJ_KEY" );
			pElement_StateKey = pDoc->createElement( L"IMAGE_STATE_KEY" );
			pElement_Path = pDoc->createElement( L"IMAGE_PATH" );
			pElement_Count = pDoc->createElement( L"IMAGE_COUNT" );

			/* ����Ʈ �߻� �Ӽ�.. */
			pElement_ShowEffectKind = pDoc->createElement( L"SHOW_EFFECT_KIND" );

			/* �� ��° �̹��� ����.. */
			pElement_Image2ObjKey = pDoc->createElement( L"IMAGE2_OBJ_KEY" );
			pElement_Image2StateKey = pDoc->createElement( L"IMAGE2_STATE_KEY" );
			pElement_Image2Path = pDoc->createElement( L"IMAGE2_PATH" );
			pElement_Image2Count = pDoc->createElement( L"IMAGE2_COUNT" );


			/* ����ü ������.. */
			pElement_fCastingTime = pDoc->createElement( L"BULLET_CASTING_TIME" );
			pElement_fSpeed = pDoc->createElement( L"BULLET_SPEED" );

			/* ����ü �̹��� ���..*/
			pElement_Bullet_ObjKey = pDoc->createElement( L"BULLET_OBJ_KEY" );
			pElement_Bullet_StateKey = pDoc->createElement( L"BULLET_STATE_KEY" );
			pElement_Bullet_Path = pDoc->createElement( L"BULLET_IMAGE_PATH" );
			pElement_Bullet_Count = pDoc->createElement( L"BULLET_IMAGE_COUNT" );

			/* ����ü���� ���� ����Ʈ ���..*/
			pElement_Bullet_EffectObjKey = pDoc->createElement( L"BULLET_EFFECT_OBJ_KEY" );
			pElement_Bullet_EffectStateKey = pDoc->createElement( L"BULLET_EFFECT_STATE_KEY" );
			pElement_Bullet_EffectPath = pDoc->createElement( L"BULLET_EFFECT_IMAGE_PATH" );
			pElement_Bullet_EffectCount = pDoc->createElement( L"BULLET_EFFECT_IMAGE_COUNT" );

			/* ����ü ĳ���� �̹��� ���..*/
			pElement_Bullet_Casting_ObjKey = pDoc->createElement( L"BULLET_CASTING_OBJ_KEY" );
			pElement_Bullet_Casting_StateKey = pDoc->createElement( L"BULLET_CASTING_STATE_KEY" );
			pElement_Bullet_Casting_Path = pDoc->createElement( L"BULLET_CASTING_IMAGE_PATH" );
			pElement_Bullet_Casting_Count = pDoc->createElement( L"BULLET_CASTING_IMAGE_COUNT" );


			pElement_Line = pDoc->createElement( L"WEAPON" );


			//TextNode
			pText_Name = pDoc->createTextNode( iter->second.first->wstrWeaponName.c_str() );

			pText_Damage = pDoc->createTextNode( _bstr_t( iter->second.first->fDamage ) );
			pText_AttInterval = pDoc->createTextNode( _bstr_t( iter->second.first->fAttInterval ) );

			pText_UpgradeType = pDoc->createTextNode( _bstr_t( iter->second.first->eUpgradeType ) );
			pText_UpgradePlus = pDoc->createTextNode( _bstr_t( iter->second.first->iUpgradePlus ) );

			pText_MinAttRange = pDoc->createTextNode( _bstr_t( iter->second.first->iMinAttRange ) );
			pText_MaxAttRange = pDoc->createTextNode( _bstr_t( iter->second.first->iMaxAttRange ) );

			pText_SplashType = pDoc->createTextNode( _bstr_t( iter->second.first->eWeaponAttType ) );
			pText_SplashInside = pDoc->createTextNode( _bstr_t( iter->second.first->iInside ) );
			pText_SplashMiddle = pDoc->createTextNode( _bstr_t( iter->second.first->iMiddle ) );
			pText_SplashOutside = pDoc->createTextNode( _bstr_t( iter->second.first->iOutSide ) );

			if ( !iter->second.second )
			{
				pText_ObjKey = pDoc->createTextNode( L"NULL" );
				pText_StateKey = pDoc->createTextNode( L"NULL" );
				pText_Path = pDoc->createTextNode( L"NULL" );
				pText_Count = pDoc->createTextNode( _bstr_t( -1 ) );
			}
			else
			{
				pText_ObjKey = pDoc->createTextNode( iter->second.second->wstrObjKey.c_str() );
				pText_StateKey = pDoc->createTextNode( iter->second.second->wstrStateKey.c_str() );
				pText_Path = pDoc->createTextNode( iter->second.second->wstrPath.c_str() );
				pText_Count = pDoc->createTextNode( _bstr_t( iter->second.second->iCount ) );
			}

			if ( iter->second.first->pImagePath2 )
			{
				/* �� ��° �̹��� ����.. */
				pText_Image2ObjKey = pDoc->createTextNode( iter->second.first->pImagePath2->wstrObjKey.c_str() );
				pText_Image2StateKey = pDoc->createTextNode( iter->second.first->pImagePath2->wstrStateKey.c_str() );
				pText_Image2Path = pDoc->createTextNode( iter->second.first->pImagePath2->wstrPath.c_str() );
				pText_Image2Count = pDoc->createTextNode( _bstr_t( iter->second.first->pImagePath2->iCount ) );
			}
			else
			{
				pText_Image2ObjKey = pDoc->createTextNode( L"NULL" );
				pText_Image2StateKey = pDoc->createTextNode( L"NULL" );
				pText_Image2Path = pDoc->createTextNode( L"NULL" );
				pText_Image2Count = pDoc->createTextNode( _bstr_t( -1 ) );
			}

			pText_ShowEffectKind = pDoc->createTextNode( _bstr_t( iter->second.first->eEffectShowKind ) );

			/* ���𰡸� �߻��ϴ� ������.. */
			if ( iter->second.first->eEffectShowKind == Effect_Show_Hit_Target ||
				 iter->second.first->eEffectShowKind == Effect_Show_Chase_Target )
			{
				pText_fCastingTime = pDoc->createTextNode( _bstr_t( iter->second.first->pBulletData->fCastingTime ) );
				pText_fSpeed = pDoc->createTextNode( _bstr_t( iter->second.first->pBulletData->fSpeed ) );

				if ( iter->second.first->pBulletData->pImagePath )
				{
					pText_Bullet_ObjKey = pDoc->createTextNode( iter->second.first->pBulletData->pImagePath->wstrObjKey.c_str() );
					pText_Bullet_StateKey = pDoc->createTextNode( iter->second.first->pBulletData->pImagePath->wstrStateKey.c_str() );
					pText_Bullet_Path = pDoc->createTextNode( iter->second.first->pBulletData->pImagePath->wstrPath.c_str() );
					pText_Bullet_Count = pDoc->createTextNode( _bstr_t( iter->second.first->pBulletData->pImagePath->iCount ) );
				}
				else
				{
					pText_Bullet_ObjKey = pDoc->createTextNode( L"NULL" );
					pText_Bullet_StateKey = pDoc->createTextNode( L"NULL" );
					pText_Bullet_Path = pDoc->createTextNode( L"NULL" );
					pText_Bullet_Count = pDoc->createTextNode( L"NULL" );
				}

				if ( iter->second.first->pBulletData->pEffectImaePath )
				{
					pText_Bullet_EffectObjKey = pDoc->createTextNode( iter->second.first->pBulletData->pEffectImaePath->wstrObjKey.c_str() );
					pText_Bullet_EffectStateKey = pDoc->createTextNode( iter->second.first->pBulletData->pEffectImaePath->wstrStateKey.c_str() );
					pText_Bullet_EffectPath = pDoc->createTextNode( iter->second.first->pBulletData->pEffectImaePath->wstrPath.c_str() );
					pText_Bullet_EffectCount = pDoc->createTextNode( _bstr_t( iter->second.first->pBulletData->pEffectImaePath->iCount ) );
				}
				else
				{
					pText_Bullet_EffectObjKey = pDoc->createTextNode( L"NULL" );
					pText_Bullet_EffectStateKey = pDoc->createTextNode( L"NULL" );
					pText_Bullet_EffectPath = pDoc->createTextNode( L"NULL" );
					pText_Bullet_EffectCount = pDoc->createTextNode( L"NULL" );
				}

				if ( iter->second.first->pBulletData->pCastingImagePath )
				{
					pText_Bullet_Casting_ObjKey = pDoc->createTextNode( iter->second.first->pBulletData->pCastingImagePath->wstrObjKey.c_str() );
					pText_Bullet_Casting_StateKey = pDoc->createTextNode( iter->second.first->pBulletData->pCastingImagePath->wstrObjKey.c_str() );
					pText_Bullet_Casting_Path = pDoc->createTextNode( iter->second.first->pBulletData->pCastingImagePath->wstrObjKey.c_str() );
					pText_Bullet_Casting_Count = pDoc->createTextNode( _bstr_t( iter->second.first->pBulletData->pCastingImagePath->iCount ) );
				}
				else
				{
					pText_Bullet_Casting_ObjKey = pDoc->createTextNode( L"NULL" );
					pText_Bullet_Casting_StateKey = pDoc->createTextNode( L"NULL" );
					pText_Bullet_Casting_Path = pDoc->createTextNode( L"NULL" );
					pText_Bullet_Casting_Count = pDoc->createTextNode( L"NULL" );
				}
			}
			else
			{
				pText_fCastingTime = pDoc->createTextNode( L"NULL" );
				pText_fSpeed = pDoc->createTextNode( L"NULL" );

				pText_Bullet_ObjKey = pDoc->createTextNode( L"NULL" );
				pText_Bullet_StateKey = pDoc->createTextNode( L"NULL" );
				pText_Bullet_Path = pDoc->createTextNode( L"NULL" );
				pText_Bullet_Count = pDoc->createTextNode( L"NULL" );

				pText_Bullet_EffectObjKey = pDoc->createTextNode( L"NULL" );
				pText_Bullet_EffectStateKey = pDoc->createTextNode( L"NULL" );
				pText_Bullet_EffectPath = pDoc->createTextNode( L"NULL" );
				pText_Bullet_EffectCount = pDoc->createTextNode( L"NULL" );

				pText_Bullet_Casting_ObjKey = pDoc->createTextNode( L"NULL" );
				pText_Bullet_Casting_StateKey = pDoc->createTextNode( L"NULL" );
				pText_Bullet_Casting_Path = pDoc->createTextNode( L"NULL" );
				pText_Bullet_Casting_Count = pDoc->createTextNode( L"NULL" );
			}

			pDoc->appendChild( pPI );

			//Element�� Text�� �߰��ϴ� ����.
			pElement_Name->appendChild( pText_Name );

			pElement_Damage->appendChild( pText_Damage );
			pElement_AttInterval->appendChild( pText_AttInterval );

			pElement_UpgradeType->appendChild( pText_UpgradeType );
			pElement_UpgradePlus->appendChild( pText_UpgradePlus );

			pElement_MinAttRange->appendChild( pText_MinAttRange );
			pElement_MaxAttRange->appendChild( pText_MaxAttRange );

			pElement_SplashType->appendChild( pText_SplashType );
			pElement_SplashInside->appendChild( pText_SplashInside );
			pElement_SplashMiddle->appendChild( pText_SplashMiddle );
			pElement_SplashOutside->appendChild( pText_SplashOutside );

			pElement_ObjKey->appendChild( pText_ObjKey );
			pElement_StateKey->appendChild( pText_StateKey );
			pElement_Path->appendChild( pText_Path );
			pElement_Count->appendChild( pText_Count );

			pElement_ShowEffectKind->appendChild( pText_ShowEffectKind );

			pElement_Image2ObjKey->appendChild( pText_Image2ObjKey );
			pElement_Image2StateKey->appendChild( pText_Image2StateKey );
			pElement_Image2Path->appendChild( pText_Image2Path );
			pElement_Image2Count->appendChild( pText_Image2Count );

			/* �߻�ü�� ������ ���� ����.. */
			pElement_fCastingTime->appendChild( pText_fCastingTime );
			pElement_fSpeed->appendChild( pText_fSpeed );

			pElement_Bullet_ObjKey->appendChild( pText_Bullet_ObjKey );
			pElement_Bullet_StateKey->appendChild( pText_Bullet_StateKey );
			pElement_Bullet_Path->appendChild( pText_Bullet_Path );
			pElement_Bullet_Count->appendChild( pText_Bullet_Count );

			pElement_Bullet_EffectObjKey->appendChild( pText_Bullet_EffectObjKey );
			pElement_Bullet_EffectStateKey->appendChild( pText_Bullet_EffectStateKey );
			pElement_Bullet_EffectPath->appendChild( pText_Bullet_EffectPath );
			pElement_Bullet_EffectCount->appendChild( pText_Bullet_EffectCount );

			pElement_Bullet_Casting_ObjKey->appendChild( pText_Bullet_Casting_ObjKey );
			pElement_Bullet_Casting_StateKey->appendChild( pText_Bullet_Casting_StateKey );
			pElement_Bullet_Casting_Path->appendChild( pText_Bullet_Casting_Path );
			pElement_Bullet_Casting_Count->appendChild( pText_Bullet_Casting_Count );

			pRootElementItem->appendChild( pElement_Line );

			pElement_Line->appendChild( pElement_Name );

			pElement_Line->appendChild( pElement_Damage );
			pElement_Line->appendChild( pElement_AttInterval );

			pElement_Line->appendChild( pElement_UpgradeType );
			pElement_Line->appendChild( pElement_UpgradePlus );

			pElement_Line->appendChild( pElement_MinAttRange );
			pElement_Line->appendChild( pElement_MaxAttRange );

			pElement_Line->appendChild( pElement_SplashType );
			pElement_Line->appendChild( pElement_SplashInside );
			pElement_Line->appendChild( pElement_SplashMiddle );
			pElement_Line->appendChild( pElement_SplashOutside );

			pElement_Line->appendChild( pElement_ObjKey );
			pElement_Line->appendChild( pElement_StateKey );
			pElement_Line->appendChild( pElement_Path );
			pElement_Line->appendChild( pElement_Count );

			pElement_Line->appendChild( pElement_ShowEffectKind );

			pElement_Line->appendChild( pElement_Image2ObjKey );
			pElement_Line->appendChild( pElement_Image2StateKey );
			pElement_Line->appendChild( pElement_Image2Path );
			pElement_Line->appendChild( pElement_Image2Count );

			pElement_Line->appendChild( pElement_fCastingTime );
			pElement_Line->appendChild( pElement_fSpeed );

			pElement_Line->appendChild( pElement_Bullet_ObjKey );
			pElement_Line->appendChild( pElement_Bullet_StateKey );
			pElement_Line->appendChild( pElement_Bullet_Path );
			pElement_Line->appendChild( pElement_Bullet_Count );

			pElement_Line->appendChild( pElement_Bullet_EffectObjKey );
			pElement_Line->appendChild( pElement_Bullet_EffectStateKey );
			pElement_Line->appendChild( pElement_Bullet_EffectPath );
			pElement_Line->appendChild( pElement_Bullet_EffectCount );

			pElement_Line->appendChild( pElement_Bullet_Casting_ObjKey );
			pElement_Line->appendChild( pElement_Bullet_Casting_StateKey );
			pElement_Line->appendChild( pElement_Bullet_Casting_Path );
			pElement_Line->appendChild( pElement_Bullet_Casting_Count );

		}

		pDoc->appendChild(pRootElementItem);

		if(pDoc->save( (_variant_t)Dlg.GetPathName() ) == S_OK)
		{
			MessageBox(L"���强��!", L"���强��!");
		}
		else
			MessageBox(L"�������!", L"�������!");

	}

}


void CWeaponTool::OnBnClickedButtonDeleteWeaponData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = this->m_WeaponData_ListBox.GetCurSel();

	if ( iIndex == -1 )
		return;

	CString cstrDeleteData;
	this->m_WeaponData_ListBox.GetText( iIndex, cstrDeleteData );

	auto& iter = this->m_mapWeaponData.find( cstrDeleteData.GetString() );

	if ( iter == this->m_mapWeaponData.end() )
		return;

	if ( iter->second.first )
	{
		if ( iter->second.first->pBulletData )
		{
			if ( iter->second.first->pBulletData->pImagePath )
				safe_delete( iter->second.first->pBulletData->pImagePath );
			if ( iter->second.first->pBulletData->pEffectImaePath )
				safe_delete( iter->second.first->pBulletData->pEffectImaePath );
			if ( iter->second.first->pBulletData->pCastingImagePath )
				safe_delete( iter->second.first->pBulletData->pCastingImagePath );

			safe_delete( iter->second.first->pBulletData );
		}

		safe_delete( iter->second.first );
	}
	if ( iter->second.second )
		safe_delete( iter->second.second );

	this->m_mapWeaponData.erase( iter );
	this->m_WeaponData_ListBox.DeleteString( iIndex );

}


BOOL CWeaponTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Weapon_Upgrade_Type_ComboBox.AddString( L"Terran_Infantry_Weapons" );
	m_Weapon_Upgrade_Type_ComboBox.AddString( L"Terran_Vehicle_Weapons" );

	m_Weapon_Splash_Type_ComboBox.AddString( L"Weapon_Normal" );
	m_Weapon_Splash_Type_ComboBox.AddString( L"Weapon_Splash_Noraml" );
	m_Weapon_Splash_Type_ComboBox.AddString( L"Weapon_Splash_Circle" );
	m_Weapon_Splash_Type_ComboBox.AddString( L"Weapon_LockDown" );

	m_EffectShowType_ComboBox.AddString( L"Effect_Show_None" );
	m_EffectShowType_ComboBox.AddString( L"Effect_Show_Owner_Position" );
	m_EffectShowType_ComboBox.AddString( L"Effect_Show_Target_Position" );
	m_EffectShowType_ComboBox.AddString( L"Effect_Show_Hit_Target" );
	m_EffectShowType_ComboBox.AddString( L"Effect_Show_Chase_Target" );
	m_EffectShowType_ComboBox.AddString( L"Effect_Show_Target_AND_Owner_Position" );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CWeaponTool::OnLbnSelchangeListWeapon()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = this->m_WeaponData_ListBox.GetCurSel();

	if ( iIndex == -1 )
		return;

	this->WeaponListClick(iIndex);

}


void CWeaponTool::OnBnClickedButtonLoadWeaponData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog		Dlg(
		TRUE,		//��弳�� : TRUE(�ε�), FALSE(����)
		L"xml",		//Ȯ���ڸ�
		L"*.xml",	//���ʿ� ����� �����̸�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	//�̸��� ������ �ߺ����Ͽ� ���ؼ� ó���ϴ� �ɼǰ�.
		L"*.xml",	//���� ���Ŀ� ����� Ȯ���ڸ�
		this		
	);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	//Dlg.m_ofn.lpstrInitialDir = L"..\\Data";		//�ε��� ���.
	//
	//
	//HANDLE hFile = CreateFile(
	//	Dlg.GetPathName(),		//���� ���.
	//	GENERIC_READ,	//� ����? : �б�
	//	NULL,		//�������
	//	NULL,
	//	OPEN_EXISTING,	
	//	FILE_ATTRIBUTE_NORMAL,		//�⺻������ ������ �����϶�¿ɼ�.
	//	NULL
	//);

	this->Release();

	MSXML2::IXMLDOMDocument2Ptr	pDoc = NULL;

	pDoc.CreateInstance(  __uuidof(DOMDocument40) );

	pDoc->async				= VARIANT_FALSE;
	pDoc->validateOnParse	= VARIANT_TRUE;
	pDoc->resolveExternals  = VARIANT_TRUE;

	if(pDoc->load( (_variant_t)Dlg.GetPathName() ) == E_FAIL )
	{
		MessageBox(L"������ ������ �ε� ����!");
		return;
	}

	//���(Element)�� Ž������.
	MSXML2::IXMLDOMNodeListPtr	pElementList = NULL;


	//xml������ ������ �ӽ÷� ������ ����.
	wstring strTemp = L"";


	//Ž���� ���� ����
	int iCount = 0;
	int iTotal = 0;

	pElementList = pDoc->selectNodes(L"//WEAPON");

	iTotal = pElementList->Getlength();		//ITEM Element�� �� ������ �޾ƿ´�.

	for(int i = 0; i < iTotal; ++i)
	{
		IMAGE_PATH*	pImagePath = new IMAGE_PATH;
		WEAPON_DATA* pWeaponData = new WEAPON_DATA;

		ZeroMemory(& pImagePath, sizeof(IMAGE_PATH));
		ZeroMemory(&pWeaponData, sizeof(WEAPON_DATA));

		pWeaponData->pBulletData = new BULLET_DATA;

		/* ���� ���� �ޱ�.. */
		pWeaponData->wstrWeaponName 
			= pElementList->Getitem(i)->selectNodes(L"//NAME")->Getitem(i)->Gettext();

		pWeaponData->fDamage
			= atof( pElementList->Getitem( i )->selectNodes( L"//DAMAGE" )->Getitem(i)->Gettext() );

		pWeaponData->fAttInterval
			= atof( pElementList->Getitem( i )->selectNodes( L"//ATTACK_INTERVAL" )->Getitem(i)->Gettext() );

		pWeaponData->eUpgradeType
			= (eWeaponUpgradeType)atoi( pElementList->Getitem( i )->selectNodes( L"//UPGRADE_TYPE" )->Getitem(i)->Gettext() );

		pWeaponData->iUpgradePlus
			= atoi( pElementList->Getitem( i )->selectNodes( L"//UPGRADE_PLUS" )->Getitem(i)->Gettext() );

		pWeaponData->iMinAttRange
			= atoi( pElementList->Getitem( i )->selectNodes( L"//MIN_ATTACK_RANGE" )->Getitem(i)->Gettext() );

		pWeaponData->iMaxAttRange
			= atoi( pElementList->Getitem( i )->selectNodes( L"//MAX_ATTACK_RANGE" )->Getitem(i)->Gettext() );

		pWeaponData->eWeaponAttType
			= (eWeaponAttType)atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_TYPE" )->Getitem(i)->Gettext() );

		pWeaponData->iInside
			= atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_INSIDE" )->Getitem(i)->Gettext() );

		pWeaponData->iMiddle
			= atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_MIDDLE" )->Getitem(i)->Gettext() );

		pWeaponData->iOutSide
			= atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_OUTSIDE" )->Getitem(i)->Gettext() );
		
		/* �̹��� ��� �ޱ�.. */
		pImagePath->wstrObjKey
			= pElementList->Getitem( i )->selectNodes( L"//IMAGE_OBJ_KEY" )->Getitem( i )->Gettext();
		
		pImagePath->wstrStateKey
			= pElementList->Getitem( i )->selectNodes( L"//IMAGE_STATE_KEY" )->Getitem( i )->Gettext();
		
		pImagePath->wstrPath
			= pElementList->Getitem( i )->selectNodes( L"//IMAGE_PATH" )->Getitem( i )->Gettext();
		
		pImagePath->iCount
			= atoi( pElementList->Getitem( i )->selectNodes( L"//IMAGE_COUNT" )->Getitem( i )->Gettext() );
		
		if ( pImagePath->wstrObjKey == L"NULL" )
			safe_delete( pImagePath );

		pWeaponData->eEffectShowKind
			= (eEffectShowKind)atoi( pElementList->Getitem( i )->selectNodes( L"//SHOW_EFFECT_KIND" )->Getitem( i )->Gettext() );

		/* ����ü ���� �ޱ�.. */
		if ( pWeaponData->eEffectShowKind == Effect_Show_Hit_Target ||
			 pWeaponData->eEffectShowKind == Effect_Show_Chase_Target )
		{
			/* ����ü ������ �ޱ�.. */
			pWeaponData->pBulletData->fCastingTime
				= atof( pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_TIME" )->Getitem( i )->Gettext() );

			pWeaponData->pBulletData->fSpeed
				= atof( pElementList->Getitem( i )->selectNodes( L"//BULLET_SPEED" )->Getitem( i )->Gettext() );

			/* ����ü �̹��� ��� �ޱ�.. */
			pWeaponData->pBulletData->pImagePath = new IMAGE_PATH;

			pWeaponData->pBulletData->pImagePath->wstrObjKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_OBJ_KEY" )->Getitem( i )->Gettext();

			pWeaponData->pBulletData->pImagePath->wstrStateKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_STATE_KEY" )->Getitem( i )->Gettext();

			pWeaponData->pBulletData->pImagePath->wstrPath
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_IMAGE_PATH" )->Getitem( i )->Gettext();

			pWeaponData->pBulletData->pImagePath->iCount
				= atoi( pElementList->Getitem( i )->selectNodes( L"//BULLET_IMAGE_COUNT" )->Getitem( i )->Gettext() );

			if ( pWeaponData->pBulletData->pImagePath->wstrObjKey == L"NULL" )
				safe_delete( pWeaponData->pBulletData->pImagePath );


			/* ����ü �̹��� ��� �ޱ�.. */
			pWeaponData->pBulletData->pCastingImagePath = new IMAGE_PATH;

			pWeaponData->pBulletData->pCastingImagePath->wstrObjKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_OBJ_KEY" )->Getitem( i )->Gettext();

			pWeaponData->pBulletData->pCastingImagePath->wstrStateKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_STATE_KEY" )->Getitem( i )->Gettext();

			pWeaponData->pBulletData->pCastingImagePath->wstrPath
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_IMAGE_PATH" )->Getitem( i )->Gettext();

			pWeaponData->pBulletData->pCastingImagePath->iCount
				= atoi( pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_IMAGE_COUNT" )->Getitem( i )->Gettext() );

			if ( pWeaponData->pBulletData->pCastingImagePath->wstrObjKey == L"NULL" )
				safe_delete( pWeaponData->pBulletData->pCastingImagePath );


			/* ����ü �̹��� ��� �ޱ�.. */
			pWeaponData->pBulletData->pEffectImaePath = new IMAGE_PATH;

			pWeaponData->pBulletData->pEffectImaePath->wstrObjKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_EFFECT_OBJ_KEY" )->Getitem( i )->Gettext();

			pWeaponData->pBulletData->pEffectImaePath->wstrStateKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_EFFECT_STATE_KEY" )->Getitem( i )->Gettext();

			pWeaponData->pBulletData->pEffectImaePath->wstrPath
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_EFFECT_IMAGE_PATH" )->Getitem( i )->Gettext();

			pWeaponData->pBulletData->pEffectImaePath->iCount
				= atoi( pElementList->Getitem( i )->selectNodes( L"//BULLET_EFFECT_IMAGE_COUNT" )->Getitem( i )->Gettext() );

			if ( pWeaponData->pBulletData->pEffectImaePath->wstrObjKey == L"NULL" )
				safe_delete( pWeaponData->pBulletData->pEffectImaePath );

		}
		else if ( pWeaponData->eEffectShowKind == Effect_Show_Target_AND_Owner_Position )
		{
			pWeaponData->pImagePath2 = new IMAGE_PATH;

			pWeaponData->pImagePath2->wstrObjKey
				= pElementList->Getitem( i )->selectNodes( L"//IMAGE2_OBJ_KEY" )->Getitem( i )->Gettext();

			pWeaponData->pImagePath2->wstrStateKey
				= pElementList->Getitem( i )->selectNodes( L"//IMAGE2_STATE_KEY" )->Getitem( i )->Gettext();

			pWeaponData->pImagePath2->wstrPath
				= pElementList->Getitem( i )->selectNodes( L"//IMAGE2_PATH" )->Getitem( i )->Gettext();

			pWeaponData->pImagePath2->iCount
				= atoi( pElementList->Getitem( i )->selectNodes( L"//IMAGE2_COUNT" )->Getitem( i )->Gettext() );
		}
		else
			safe_delete( pWeaponData->pBulletData );

		m_WeaponData_ListBox.AddString( pWeaponData->wstrWeaponName.c_str() );
		this->m_mapWeaponData.insert( make_pair( pWeaponData->wstrWeaponName, make_pair( pWeaponData, pImagePath ) ) );
	}

	if ( !this->m_mapWeaponData.empty() )
		this->WeaponListClick( 0 );

}


void CWeaponTool::OnDropFiles( HDROP hDropInfo )
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles( hDropInfo );

	UpdateData( TRUE );

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, NULL);

	TCHAR szFullPath[MAX_PATH] = L"";

	m_listboxPathFind.ResetContent();


	for(int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);

		DirectoryInfoExtraction(szFullPath, m_listImagePath);

		wstring wstrCombine = L"";		//�ϳ��� ���ڿ��� �����ϱ� ���� �ӽú���.

		TCHAR szIndex[MIN_STR] = L"";	//������ ���ڿ����·� �����ϱ� ���� ����.

		for(list<IMAGE_PATH*>::iterator iter = m_listImagePath.begin();
			 iter != m_listImagePath.end(); ++iter)
		{
			//������ ���ڷ� ��������.
			_itow_s( (*iter)->iCount, szIndex, 10);

			wstrCombine = (*iter)->wstrObjKey + L"|" + 
				(*iter)->wstrStateKey + L"|";

			wstrCombine += szIndex;

			wstrCombine += L"|" + (*iter)->wstrPath;

			m_listboxPathFind.AddString(wstrCombine.c_str());
		}


	}

	UpdateData(FALSE);
}


void CWeaponTool::OnBnClickedButtonDecideBulletTexture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = this->m_listboxPathFind.GetCurSel();

	if ( iIndex == -1 )
		return;

	this->m_listboxPathFind.GetText( iIndex, m_BulletTexturePathDirection );

	UpdateData( FALSE );
}


void CWeaponTool::OnBnClickedButtonDecideBulletEffectTexture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = this->m_listboxPathFind.GetCurSel();

	if ( iIndex == -1 )
		return;

	this->m_listboxPathFind.GetText( iIndex, m_BulletEffect );

	UpdateData( FALSE );
}


void CWeaponTool::OnBnClickedButtonDecideBulletCastingTexture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = this->m_listboxPathFind.GetCurSel();

	if ( iIndex == -1 )
		return;

	this->m_listboxPathFind.GetText( iIndex, m_BulletCastingEffect );

	UpdateData( FALSE );
}


void CWeaponTool::OnBnClickedButtonDecideWeaponFireTexture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = this->m_listboxPathFind.GetCurSel();

	if ( iIndex == -1 )
		return;

	this->m_listboxPathFind.GetText( iIndex, m_WeaponCurPathData );

	UpdateData( FALSE );
}


void CWeaponTool::OnBnClickedButtonModifyWeaponData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = this->m_WeaponData_ListBox.GetCurSel();

	if ( iIndex == -1 )
		return;

	CString cstrFindWeaponData;
	this->m_WeaponData_ListBox.GetText( iIndex, cstrFindWeaponData );
	auto& iter = this->m_mapWeaponData.find( cstrFindWeaponData.GetString() );

	this->m_mapWeaponData.erase( iter );
	m_WeaponData_ListBox.DeleteString( iIndex );

	WEAPON_DATA* pWeaponData = new WEAPON_DATA;

	UpdateData( TRUE );

	pWeaponData->wstrWeaponName = m_WeaponName;

	pWeaponData->fDamage = _ttof( m_WeaponDamage );
	pWeaponData->fAttInterval = _ttof( m_WeaponCoolTime );

	pWeaponData->eUpgradeType = (eWeaponUpgradeType)m_Weapon_Upgrade_Type_ComboBox.GetCurSel();
	pWeaponData->iUpgradePlus = _ttof( m_WeaponUpgradePlus );

	pWeaponData->iMinAttRange = _ttof( m_WeaponMinRange );
	pWeaponData->iMaxAttRange = _ttof( m_WeaponMaxRange );

	pWeaponData->eWeaponAttType = (eWeaponAttType)m_Weapon_Splash_Type_ComboBox.GetCurSel();
	pWeaponData->iInside = _ttof( m_WeaponSplashInside );
	pWeaponData->iMiddle = _ttof( m_WeaponSplashMiddle );
	pWeaponData->iOutSide = _ttof( m_WeaponSplashOutSide );

	pWeaponData->eEffectShowKind = (eEffectShowKind)m_EffectShowType_ComboBox.GetCurSel();

	if ( pWeaponData->eEffectShowKind == Effect_Show_Hit_Target ||
		 pWeaponData->eEffectShowKind == Effect_Show_Chase_Target )
	{
		pWeaponData->pBulletData = new BULLET_DATA;
		pWeaponData->pBulletData->fSpeed = _ttof( m_BulletSpeed );
		pWeaponData->pBulletData->fCastingTime = _ttof( m_BulletCastingTime );

		if ( m_BulletTexturePathDirection != L"NULL" )
		{
			pWeaponData->pBulletData->pImagePath = this->FindImagePath( m_BulletTexturePathDirection );
		}
		else
			pWeaponData->pBulletData->pImagePath = NULL;

		if ( m_BulletEffect != L"NULL" )
		{
			pWeaponData->pBulletData->pEffectImaePath = this->FindImagePath( m_BulletEffect );
		}
		else
			pWeaponData->pBulletData->pEffectImaePath = NULL;

		if ( m_BulletCastingEffect != L"NULL" )
		{
			pWeaponData->pBulletData->pCastingImagePath = this->FindImagePath( m_BulletCastingEffect );
		}
		else
			pWeaponData->pBulletData->pCastingImagePath = NULL;
	}
	else
		pWeaponData->pBulletData = NULL;

	int iImagePathIndex = m_listboxPathFind.GetCurSel();
	IMAGE_PATH* pImagePath = NULL;
	pImagePath = this->FindImagePath( this->m_WeaponCurPathData );
	this->m_WeaponCurPathData = L"";

	if ( m_WeaponCurPathData2 != L"" )
	{
		pWeaponData->pImagePath2 = this->FindImagePath( m_WeaponCurPathData2 );
		m_WeaponCurPathData2 = L"";
	}
	else
		pWeaponData->pImagePath2 = NULL;

	m_WeaponData_ListBox.InsertString( iIndex, pWeaponData->wstrWeaponName.c_str() );
	this->m_mapWeaponData.insert( make_pair( m_WeaponName, make_pair( pWeaponData, pImagePath ) ) );

}


void CWeaponTool::OnBnClickedButtonDecideWeaponFireTexture2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = this->m_listboxPathFind.GetCurSel();

	if ( iIndex == -1 )
		return;

	this->m_listboxPathFind.GetText( iIndex, m_WeaponCurPathData2 );

	UpdateData( FALSE );
}
