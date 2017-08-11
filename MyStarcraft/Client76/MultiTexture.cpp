#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"

HRESULT CMultiTexture::InsertTexture( const TCHAR* pPath, /* 이미지 경로.. */
									  const TCHAR* pStateKey /*= L""*/,/* 스프라이트를 출력할 때 상태값을 표현.. */
									  const int& iCount /*= 0*/, /* 이미지 장수.. */
									  bool bTransparentColor /*= false*/, /* 투명색 적용.. */
									  D3DCOLOR transparentColor /*= D3DCOLOR_ARGB( 255, 0, 0, 0 ) */ )
{
	TCHAR szPath[MAX_STR] = L"";

	vector<TEX_INFO*>	vecTexture;

	for ( int i = 0; i < iCount; ++i )
	{
		wsprintf( szPath, pPath, i );
		//szPath -> ../Texture/Tile/Tile0.png

		TEX_INFO* pTexInfo = new TEX_INFO;
		ZeroMemory( pTexInfo, sizeof( TEX_INFO ) );

		if ( FAILED( D3DXGetImageInfoFromFile( szPath, &pTexInfo->ImageInfo ) ) )
			return E_FAIL;

		if ( FAILED( D3DXCreateTextureFromFileEx(
			CDevice::GetInstance()->GetDevice(),
			szPath,
			pTexInfo->ImageInfo.Width,
			pTexInfo->ImageInfo.Height,
			pTexInfo->ImageInfo.MipLevels,
			0,
			pTexInfo->ImageInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			((bTransparentColor) ? transparentColor : NULL),
			&pTexInfo->ImageInfo,
			NULL,
			&pTexInfo->pTexture
			) ) )
		{
			return E_FAIL;
		}

		vecTexture.push_back( pTexInfo );
	}

	TCHAR* pKey = new TCHAR[MAX_PATH];
	lstrcpy( pKey, pStateKey );

	m_mapMultiTexture.insert( make_pair( pKey, vecTexture ) );

	return S_OK;
}

const TEX_INFO* CMultiTexture::GetTexture(const TCHAR* pKey /*= L"" */, 
										  const int& iCount /*= 0*/)
{
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter;

	//iter = m_mapMultiTexture.find();

	iter = find_if(m_mapMultiTexture.begin(), m_mapMultiTexture.end(), STRING_COMPARE(pKey) );

	if(iter == m_mapMultiTexture.end())
		return NULL;

	return iter->second[iCount];
}

void CMultiTexture::Release(void)
{
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter;
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter_end;

	iter	 = m_mapMultiTexture.begin();
	iter_end = m_mapMultiTexture.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete [] iter->first;
		
		for(size_t i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->pTexture->Release();
			//delete iter->second[i];
			safe_delete(iter->second[i]);
		}
		iter->second.clear();
	}

	m_mapMultiTexture.clear();
}

CMultiTexture::CMultiTexture(void)
{
}

CMultiTexture::~CMultiTexture(void)
{
	Release();
}
