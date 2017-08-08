#include "StdAfx.h"
#include "SingleTexture.h"
#include "device.h"

HRESULT CSingleTexture::InsertTexture( const TCHAR* pPath
									   , const TCHAR* pStateKey /*= L""*/
									   , const int& iCount /*= 0*/
									   , bool bTransparentColor /*= false*/	// 투명색 적용..
									   , D3DCOLOR transparentColor /*= D3DCOLOR_ARGB( 255, 0, 0, 0 )*/ ) // 적용할 투명색..
{
	m_pTextInfo = new TEX_INFO;
	ZeroMemory( m_pTextInfo, sizeof( TEX_INFO ) );


	//이미지의 정보를 얻어보자.
	//D3DXGetImageInfoFromFile() : 이미지의 정보를 얻어오는 함수.
	if ( FAILED( D3DXGetImageInfoFromFile( pPath, &m_pTextInfo->ImageInfo ) ) )
	{
		MessageBox( g_hWnd, pPath, L"싱글이미지 정보 얻기 실패!", MB_OK );
		return E_FAIL;
	}


	//이미지를 생성해보자.
	//m_pTextInfo->pTexture 이녀석.

	if ( FAILED( D3DXCreateTextureFromFileEx( CDevice::GetInstance()->GetDevice(),
		 pPath,		//이미지 경로.
		 m_pTextInfo->ImageInfo.Width,
		 m_pTextInfo->ImageInfo.Height,
		 m_pTextInfo->ImageInfo.MipLevels,	//3D에서 사용하는 옵션이다.
		 0,	//이미지를 읽는 방식을 설정하는 옵션 -> 0
		 m_pTextInfo->ImageInfo.Format,
		 D3DPOOL_MANAGED
		 /*현재 읽는 이미지들을 어떤 메모리에 보관을 할것인지 결정하는 인자
		 D3DPOOL_MANAGED : 그래픽카드에 메모리가 모두 사용중이라면 -> 시스템메모리를 사용하게 되는 옵션
		 D3DPOOL_DEFAULT : 그래픽카드가 꽉차면 출력이 불가능..*/,
		 D3DX_DEFAULT,		//테두리 표현을 어떻게 할것인지?
		 D3DX_DEFAULT,	//확대/축소시에 어떻게 이미지를 처리할것인지?
		 ((bTransparentColor) ? transparentColor : NULL),		//제거할 이미지의 색상
		 &m_pTextInfo->ImageInfo,		//이미지 정보를 어디에 저장할것인지?
		 NULL,		//8비트 미만의 이미지처리 방식에 대한 옵션
		 &m_pTextInfo->pTexture
		 ) ) )
	{
		MessageBox( g_hWnd, pPath, L"싱글이미지 로드 실패!", MB_OK );
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
