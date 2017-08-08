// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool76.h"
#include "MapTool.h"
#include "include.h"

#include "MainFrm.h"
#include "Tool76View.h"
#include "BackGround.h"

void CMapTool::HorizontalScroll(void)
{
	CString strName;
	CSize	Size;

	int iCX = 0;

	CDC* pDc = m_listBoxImagePath.GetDC();
	//����Ʈ�ڽ��� DC�� ���´�.


	//����Ʈ�ڽ��� �ִ� ����� ������ �޾ƿ���.
	for(int i = 0; i < m_listBoxImagePath.GetCount(); ++i)
	{
		//����Ʈ�ڽ��� �ִ� ���ڿ��� ���´�.
		m_listBoxImagePath.GetText(i, strName);

		//������ ũ�⸦ �ȼ�ũ��� ��ȯ���ִ� �Լ�.
		Size = pDc->GetTextExtent(strName);

		//���� �� ���ڿ��� �˻�����.
		if(Size.cx > iCX)
		{
			iCX = Size.cx;
		}
	}


	//�˻��� �Ϸ�Ǹ� DC�� ��ȯ����.
	m_listBoxImagePath.ReleaseDC(pDc);

	if(m_listBoxImagePath.GetHorizontalExtent() < iCX)
	{
		m_listBoxImagePath.SetHorizontalExtent(iCX);
		//���� �� ���ڿ��� ũ���� ��ũ�� �ٸ� �����Ѵ�.
	}

}

// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(CMapTool::IDD, pParent)
	, m_iDrawID(0)
{

}

CMapTool::~CMapTool()
{
	map<CString, CImage*>::iterator iter = m_mapPngImage.begin();
	map<CString, CImage*>::iterator iter_end = m_mapPngImage.end();

	for(iter; iter != iter_end; ++iter)
	{
		iter->second->Destroy();		//�̹��� ��ü�� �ı�����.
		safe_delete(iter->second);
	}
	if(!m_mapPngImage.empty())
		m_mapPngImage.clear();
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IMAGE_PATH, m_listBoxImagePath);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_IMAGE_PATH, &CMapTool::OnLbnSelchangeListImagePath)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMapTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMapTool::OnBnClickedButtonLoad)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.

void CMapTool::OnLbnSelchangeListImagePath()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_listBoxImagePath.GetCurSel();
	
	if(iSelectIndex < 0)
		return;

	m_listBoxImagePath.GetText(iSelectIndex, strName);

	map<CString, CImage*>::iterator iter;

	iter = m_mapPngImage.find(strName);

	if(iter == m_mapPngImage.end())
		return;

	m_Picture.SetBitmap( *iter->second );


	int i = 0;

	for(i = 0; i < strName.GetLength(); ++i)
	{
		//���ڿ��� 0�� �ε������� �˻�
		//���ڰ� ���������� �˻��غ���. -> 
		if( isdigit( strName[i] ) != 0 )		//Tile0
		{
			break;
			//�Լ�isdigit()�� ��ȯ���� 0�� �ƴ� ���̸� ���ڰ� '����'�̴�.
		}
	}

	//i -> ���ڰ� �ִ°������� Index
	strName.Delete(0, i);
	//0�� �ε������� i���� ���ڿ��� �����Ѵ�.


	//_tstoi() : string to int -> ���ڿ��� ���������� �����ϴ� �Լ�.
	m_iDrawID = _tstoi(strName);	//strName = Tile0

	UpdateData(FALSE);
}

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	////##[17.07.18_00] : ��ӵ� ������ ������ �޾ƿ���.

	UpdateData(TRUE);

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	//1���� : ��ӵ� ������ ��Ÿ���� ������ �䱸�ϰ� �ִ�.

	//2���� : ������ �ε����� �����ϴ� ���ڰ�.
	//        -1�� ���ڰ����� ����ϰ� �ȴٸ� ���� ��ü������ ��ȯ�Ѵ�.

	//3���� : ���ڿ��� �����͸� ��� ����.

	//4���� : ������ ���ۻ���� UniCode�� ��쿡 ������ ���� ������ �Ѵ�.
	

	TCHAR szFullPath[MAX_PATH] = L"";			//��ӵ� ������ ��θ� ������ ����.

	for(int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);

		std::wstring wstrRelativePath = RelativePath(szFullPath);

		CString strImageName;

		//##[17.07.18_02]
		//������ �̸��� ������ �Լ�.
		strImageName = PathFindFileName(wstrRelativePath.c_str());


		//Ȯ���ڸ� �������ִ� �Լ�.
		PathRemoveExtension( (LPWSTR)strImageName.operator LPCWSTR() );
		

		//map�����̳� ����.
		map<CString, CImage*>::iterator iter;

		iter = m_mapPngImage.find(strImageName);

		if(iter == m_mapPngImage.end())
		{
			CImage* pPngImage = new CImage;
			
			//PNG�̹����� ����θ� ���ؼ� �̹����� �ε��Ѵ�.
			pPngImage->Load(wstrRelativePath.c_str());

			//Ű : �����̸�(ex.Tile0)   
			m_mapPngImage.insert( make_pair(strImageName, pPngImage) );
		}


		m_listBoxImagePath.AddString(strImageName);
	}


	HorizontalScroll();
	

	CDialog::OnDropFiles(hDropInfo);


	UpdateData(FALSE);
}

void CMapTool::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	vector<TILE*>* pVecTile = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetTile();

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/Tile.dat",		
		GENERIC_WRITE,	
		NULL,		
		NULL,
		CREATE_ALWAYS,	
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	for(size_t i = 0; i < pVecTile->size(); ++i)
	{
		WriteFile(hFile, (*pVecTile)[i] , sizeof(TILE), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void CMapTool::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	vector<TILE*>* pVecTile = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetTile();

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/Tile.dat",		
		GENERIC_READ,	
		NULL,		
		NULL,
		OPEN_EXISTING,	
		FILE_ATTRIBUTE_NORMAL,		//�⺻������ ������ �����϶�¿ɼ�.
		NULL
		);

	for(size_t i = 0; i < pVecTile->size(); ++i)
	{
		safe_delete((*pVecTile)[i]);
	}
	pVecTile->clear();

	while(true)
	{
		TILE* pTileData = new TILE;

		ReadFile(hFile, pTileData, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			safe_delete(pTileData);
			break;
		}

		pVecTile->push_back(pTileData);
	}

	CloseHandle(hFile);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
