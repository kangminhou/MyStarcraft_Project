#include "StdAfx.h"
#include "Device.h"

//##Ư�� ���ɿ� ���� ��ġ�� ������ �˾ƺ���.
//1.Ư�� ���� : ���ؽ� ���μ����̶�� ���̴�.
//   ->���̷�Ʈ������ �ǻ�� ����ϰ� ǥ���ϱ� ���ؼ�
//     � ��ü�� �ݻ��ϴ� ������ ��� ������ ���´�.

//'���ؽ�'?
//��(.)�̴�.
//���̷�Ʈ���� �̹����� �þ�� ȸ���� ������ ������
//������ ��ȯ�� �ֱ� ������ �����ϴ�.

//���� : ���ؽ� ���μ��� -> ���Ǻ�ȯ + ����

//void Test(int*& pTes)
//{
//
//}
//
//void Test(int** ppTes)
//{
//	*ppTes = 10;
//}


IMPLEMENT_SINGLETON(CDevice);

HRESULT CDevice::Init3D(void)
{
	//��ġ�� ��ü������ �Ҵ����ش�.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	//��ġ�� ������ �������ִ� ������ �����Ѵ�.
	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if( FAILED( m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps) ) )
	{
		//Afx : MFC�����Լ�.
		AfxMessageBox(L"��ġ ���� ����!");
		return E_FAIL;
	}


	DWORD vp;

	//������� ����
	//���� ���� ���� ������ �ϵ����(�׷���ī��)�� ����? ����Ʈ����(CPU)�� ����? �Ǵ����ش�.
	
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;		//�׷��� ī�带 �����Ѵٸ� �׷���ī�忡��.
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		//�׷��� ī�带 ����X -> CPU

	
	//��ġ�� �����Ҷ� �����ڰ� ����������ϴ� ����ü.
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SerParameters(d3dpp);		//��ġ�� �����ϴ� ����� ���� �Լ�.
	//�� ���Լ��� ����̽��� �ʱ�ȭ�ϴ� �������� ����Ұ��̴�.

	//D3DDEVTYPE_HAL : �ϵ���� �߻�ȭ ����..

	if(FAILED( m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, g_hWnd
		, vp
		, &d3dpp
		, &m_pD3dDevice) ) )
	{
		//m_pD3dDevice : (����̽���ü) ȭ�鿡 �׸��� �׸��� ���� ��ü.
		AfxMessageBox(L"����̽� ���� ����!");
		return E_FAIL;
	}


	//��������Ʈ ��ü�� ����� �ش�.
	if(FAILED(D3DXCreateSprite(m_pD3dDevice, &m_pSprite)) )
	{
		AfxMessageBox(L"��������Ʈ��ü ���� ����!");
		return E_FAIL;
	}


	//��Ʈ ��ü�� ����� ����.
	//������ ��Ʈ ��ü�� ���� �ʱ�ȭ(����������).
	D3DXFONT_DESC FontInfo;
	ZeroMemory(&FontInfo, sizeof(FontInfo));


	FontInfo.Height = 20;
	FontInfo.Width = 10;
	FontInfo.Weight = FW_BOLD;
	FontInfo.CharSet = HANGEUL_CHARSET;
	
	lstrcpy(FontInfo.FaceName, L"����ü");

	
	if(FAILED( D3DXCreateFontIndirect(m_pD3dDevice, &FontInfo, &m_pFont) ))
	{
		AfxMessageBox(L"��Ʈ ��ü ���� ����!");
		return E_FAIL;
	}


	return S_OK;
}

void CDevice::SerParameters(D3DPRESENT_PARAMETERS& d3dp)
{
	d3dp.BackBufferWidth  = WINCX;			//������� ���� ũ��
	d3dp.BackBufferHeight = WINCY;			//������� ���� ũ��
	d3dp.BackBufferCount  = 1;				//������� ����.
	d3dp.BackBufferFormat = D3DFMT_A8R8G8B8;		
	//����۴� �ȼ������� �̷���� �ִ�.
	//32��Ʈ�� ����ϰ� �Ǵµ� 32��Ʈ�� ���? ����ϰڴ���?
	

	d3dp.MultiSampleType = D3DMULTISAMPLE_NONE;
	//���߻��ø� ����� ������� �ʰڴ�.
	
	//���伥���� �̹����� ��� �Ǵ� Ȯ��...

	//���̷�Ʈ���� ��� �Ǵ� Ȯ���Ҷ� �׵θ��� �ܻ��� ���� ��찡 �ִµ�
	//�ܻ��� ����ִ� ������ ������� �ʰڴ�.
	//����� ���� ...

	d3dp.MultiSampleQuality = 0;
	//������� �ʱ� ������ 0�� �־��ش�.

	
	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//������� ���� ���۸� ����� ���Ѵ�.

	//���� : ���� �����ΰ� �����ų�� �ִ� ������ ���Ѵ�.

	//���������� ������۸��� ���ִ� �����Ͱ� ���縦 �Ѵ�.
	//���� ���۸� ����Ű�� �ִ�.

	//�� �����͸� ���ؼ� �� ������ �ȼ��� �����ؼ� ������ ��������ټ��� �ִ�.
	//������ ������ �ּҸ� ������ �ؼ� ������۸�ȿ���� ������ �ȴ�.

	//"����ü��"

	d3dp.hDeviceWindow = g_hWnd;		//������ �ڵ�
	d3dp.Windowed	   = TRUE;			//��üȭ��:FALSE / â���:TRUE


	d3dp.EnableAutoDepthStencil = TRUE;
	//���̷�Ʈ3D���� �ڵ����� ����/���ٽǹ��۸� ����� �����ϱ⸦ ���Ѵٸ� TRUE

	d3dp.AutoDepthStencilFormat = D3DFMT_D24S8;
	//���̹��۰� �����Ǵ� �ȼ��� ������ �����ϴ� �ɼ�.

	d3dp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//��üȭ����� ����ϰ� �ɶ� ������� �ֻ����� ���󰡵��� �����ϴ� �ɼ��̴�.
	//�ֻ���

	d3dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//�ֻ����� ������ ������ ������ ��� �Ұ��ΰ�?

	//D3DPRESENT_INTERVAL_IMMEDIATE : ��� ȭ������ ó���� �Ѵ�.
	//D3DPRESENT_INTERVAL_DEFAULT : ȭ��ó���ϴ� ������ ���̷�Ʈ���� �����ϰ� �ȴ�.
}

//1.���۸� �����. 2.�׸���. 3.������� ����Ѵ�.
void CDevice::Render_Begin(void)
{
	//�ĸ� ���۸� �����ش�.
	m_pD3dDevice->Clear(0,		//������ ����
						NULL,	//���۸� �����Ҷ� �����ϰ� ���� ��ġ����.
						D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	
						//���κ��� �����Ұ�����? -> �ĸ����, ���ٽ�, ����
						D3DCOLOR_ARGB(255, 0, 0, 255),	//������ ǥ��Ǵ� ������ ���� ����.
						1.f,		//������ ���� ������ �ʱⰪ.
						0			//���ٽ� ������ �ʱⰪ.
						);

	m_pD3dDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(void)
{
	//������۸� ����Ѵ�.
	m_pSprite->End();
	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, NULL, NULL);
	//1,2���� : ��Ʈ���� �䱸�ϰ� �ִ�.
	//		    �� ��Ʈ���� �������̸�ŭ ȭ���� ���/Ȯ��
	//		    1�� ������Ʈ  2�� ū��Ʈ -> Ȯ��
	//			1�� ū��      2�� ���� ��Ʈ -> ���

	//3���� : � ������(�ڵ�)���� ó���Ұ��ΰ�?

	//4���� : ȭ�鿡�� �������� ���� ������ �����Ҽ� �ִ�.
}

void CDevice::Render_End(HWND& _hWnd)
{
	m_pSprite->End();
	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, _hWnd, NULL);
}

void CDevice::Release(void)
{
	//���� ������ �ݵ�� ����ϰ� ����!!
	m_pSprite->Release();
	m_pD3dDevice->Release();
	m_pD3D->Release();
}

CDevice::CDevice(void)
:m_pD3D(NULL)
,m_pD3dDevice(NULL)
{
}

CDevice::~CDevice(void)
{
	Release();
}
