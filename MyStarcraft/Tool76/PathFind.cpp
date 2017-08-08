// PathFind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool76.h"
#include "PathFind.h"


// CPathFind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC( CPathFind, CDialog )

void CPathFind::Release( void )
{
	for ( list<IMAGE_PATH*>::iterator iter = m_listImagePath.begin();
		  iter != m_listImagePath.end(); ++iter )
	{
		safe_delete( *iter );
	}
	m_listImagePath.clear();
}

CPathFind::CPathFind( CWnd* pParent /*=NULL*/ )
	: CDialog( CPathFind::IDD, pParent )
{

}

CPathFind::~CPathFind()
{
	Release();
}

void CPathFind::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_LIST_PATH, m_listboxPathFind );
}


BEGIN_MESSAGE_MAP( CPathFind, CDialog )
	ON_LBN_SELCHANGE( IDC_LIST_PATH, &CPathFind::OnLbnSelchangeListPath )
	ON_BN_CLICKED( IDC_BUTTON_SAVE, &CPathFind::OnBnClickedButtonSave )
	ON_BN_CLICKED( IDC_BUTTON_LOAD, &CPathFind::OnBnClickedButtonLoad )
	ON_WM_DROPFILES()
END_MESSAGE_MAP()




void CPathFind::OnLbnSelchangeListPath()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CPathFind::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	//[17.07.24_00] : C++����� ��ü�� ���ؼ� ��������/�ε�

	wofstream	SaveFile;
	//C++ ����� ��ü.


	SaveFile.open( L"../Data/ImagePath.txt", ios::out );
	//fopen�� ����ؼ� ����ϴ� �Լ�.
	//ios::out -> Txt����.

	int iCount = 0;

	for ( list<IMAGE_PATH*>::iterator iter = m_listImagePath.begin();
		  iter != m_listImagePath.end(); ++iter )
	{
		++iCount;

		SaveFile << (*iter)->wstrObjKey.c_str() << L"|";
		SaveFile << (*iter)->wstrStateKey.c_str() << L"|";
		SaveFile << (*iter)->iCount << L"|";

		if ( iCount == m_listImagePath.size() )	//������ ��.
			SaveFile << (*iter)->wstrPath.c_str() << flush;
		else
			SaveFile << (*iter)->wstrPath.c_str() << endl;
	}

	SaveFile.close();

	WinExec( "notepad.exe ../Data/ImagePath.txt", SW_SHOW );
	//WinExec : ���μ����� �����߿� �ٸ� ������ ��������ټ� �ִ� �Լ�.
}

void CPathFind::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData( TRUE );

	wifstream LoadFile;

	LoadFile.open( "../Data/ImagePath.txt", ios::in );

	TCHAR szObjKey[MIN_STR] = L"";
	TCHAR szStateKey[MIN_STR] = L"";
	TCHAR szCount[MIN_STR] = L"";
	TCHAR szPath[MAX_PATH] = L"";

	wstring wstrCombine = L"";

	m_listboxPathFind.ResetContent();		//����Ʈ�ڽ� ����.

	Release();

	while ( !LoadFile.eof() )
	{
		//������ ������ ��°�� �о���� �Լ�.
		LoadFile.getline( szObjKey, MIN_STR, '|' );
		LoadFile.getline( szStateKey, MIN_STR, '|' );
		LoadFile.getline( szCount, MIN_STR, '|' );
		LoadFile.getline( szPath, MAX_PATH );

		wstrCombine = wstring( szObjKey ) + L"|";
		wstrCombine += wstring( szStateKey ) + L"|"
			+ wstring( szCount ) + L"|"
			+ wstring( szPath );

		m_listboxPathFind.AddString( wstrCombine.c_str() );
	}

	LoadFile.close();

	UpdateData( FALSE );

}

void CPathFind::OnDropFiles( HDROP hDropInfo )
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles( hDropInfo );


	UpdateData( TRUE );

	int iFileCount = DragQueryFile( hDropInfo, -1, NULL, NULL );

	TCHAR szFullPath[MAX_PATH] = L"";


	for ( int i = 0; i < iFileCount; ++i )
	{
		DragQueryFile( hDropInfo, i, szFullPath, MAX_PATH );

		DirectoryInfoExtraction( szFullPath, m_listImagePath );

		wstring wstrCombine = L"";		//�ϳ��� ���ڿ��� �����ϱ� ���� �ӽú���.

		TCHAR szIndex[MIN_STR] = L"";	//������ ���ڿ����·� �����ϱ� ���� ����.

		for ( list<IMAGE_PATH*>::iterator iter = m_listImagePath.begin();
			  iter != m_listImagePath.end(); ++iter )
		{
			//������ ���ڷ� ��������.
			_itow_s( (*iter)->iCount, szIndex, 10 );

			wstrCombine = (*iter)->wstrObjKey + L"|" +
				(*iter)->wstrStateKey + L"|";

			wstrCombine += szIndex;

			wstrCombine += L"|" + (*iter)->wstrPath;

			m_listboxPathFind.AddString( wstrCombine.c_str() );
		}


	}

	UpdateData( FALSE );
}
