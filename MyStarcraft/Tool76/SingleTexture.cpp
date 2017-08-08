#include "StdAfx.h"
#include "SingleTexture.h"
#include "device.h"

HRESULT CSingleTexture::InsertTexture( const TCHAR* pPath
									   , const TCHAR* pStateKey /*= L""*/
									   , const int& iCount /*= 0*/
									   , bool bTransparentColor /*= false*/	// ����� ����..
									   , D3DCOLOR transparentColor /*= D3DCOLOR_ARGB( 255, 0, 0, 0 )*/ ) // ������ �����..
{
	m_pTextInfo = new TEX_INFO;
	ZeroMemory( m_pTextInfo, sizeof( TEX_INFO ) );


	//�̹����� ������ ����.
	//D3DXGetImageInfoFromFile() : �̹����� ������ ������ �Լ�.
	if ( FAILED( D3DXGetImageInfoFromFile( pPath, &m_pTextInfo->ImageInfo ) ) )
	{
		MessageBox( g_hWnd, pPath, L"�̱��̹��� ���� ��� ����!", MB_OK );
		return E_FAIL;
	}


	//�̹����� �����غ���.
	//m_pTextInfo->pTexture �̳༮.

	if ( FAILED( D3DXCreateTextureFromFileEx( CDevice::GetInstance()->GetDevice(),
		 pPath,		//�̹��� ���.
		 m_pTextInfo->ImageInfo.Width,
		 m_pTextInfo->ImageInfo.Height,
		 m_pTextInfo->ImageInfo.MipLevels,	//3D���� ����ϴ� �ɼ��̴�.
		 0,	//�̹����� �д� ����� �����ϴ� �ɼ� -> 0
		 m_pTextInfo->ImageInfo.Format,
		 D3DPOOL_MANAGED
		 /*���� �д� �̹������� � �޸𸮿� ������ �Ұ����� �����ϴ� ����
		 D3DPOOL_MANAGED : �׷���ī�忡 �޸𸮰� ��� ������̶�� -> �ý��۸޸𸮸� ����ϰ� �Ǵ� �ɼ�
		 D3DPOOL_DEFAULT : �׷���ī�尡 ������ ����� �Ұ���..*/,
		 D3DX_DEFAULT,		//�׵θ� ǥ���� ��� �Ұ�����?
		 D3DX_DEFAULT,	//Ȯ��/��ҽÿ� ��� �̹����� ó���Ұ�����?
		 ((bTransparentColor) ? transparentColor : NULL),		//������ �̹����� ����
		 &m_pTextInfo->ImageInfo,		//�̹��� ������ ��� �����Ұ�����?
		 NULL,		//8��Ʈ �̸��� �̹���ó�� ��Ŀ� ���� �ɼ�
		 &m_pTextInfo->pTexture
		 ) ) )
	{
		MessageBox( g_hWnd, pPath, L"�̱��̹��� �ε� ����!", MB_OK );
		return E_FAIL;
	}


	return S_OK;
}

const TEX_INFO* CSingleTexture::GetTexture( const TCHAR* pKey /*= L"" */
											, const int& iCount /*= 0*/ )
{
	return m_pTextInfo;
}

void CSingleTexture::Release( void )
{
	if ( m_pTextInfo != NULL )
	{
		m_pTextInfo->pTexture->Release();
		safe_delete( m_pTextInfo );
	}
}

CSingleTexture::CSingleTexture( void )
	:m_pTextInfo( NULL )
{
}

CSingleTexture::~CSingleTexture( void )
{
	Release();
}
