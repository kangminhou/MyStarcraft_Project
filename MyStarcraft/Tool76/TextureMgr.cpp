#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON( CTextureMgr );

HRESULT CTextureMgr::InsertTexture( const TCHAR* pFilePath, /*이미지 경로 */
									const TCHAR* pObjKey, /*객체를 나타내는 고유 키값(ex.Item, Player) */
									eTextureType eType, /*싱글인지? 멀티인지? */
									const TCHAR* pStateKey /*= L""*/, /*Player-> 걷는다, 맞는다, 공격한다등등.. */
									const int& iCount /*= 0*/,
									bool bTransparentColor /*= false*/,
									D3DCOLOR transparentColor /*= D3DCOLOR_ARGB( 255, 0, 0, 0 )*/ )
{
	map<const TCHAR*, CTexture*>::iterator iter;

	iter = find_if( m_mapTexture.begin(), m_mapTexture.end(), STRING_COMPARE( pObjKey ) );

	if ( iter == m_mapTexture.end() )
	{
		CTexture* pTexture = NULL;

		switch ( eType )
		{
			case TEX_SINGLE:
				pTexture = new CSingleTexture;
				break;

			case TEX_MULTI:
				pTexture = new CMultiTexture;
				break;
		}

		if ( FAILED( pTexture->InsertTexture( pFilePath, pStateKey, iCount, bTransparentColor, transparentColor ) ) )
		{
			MessageBox( g_hWnd, pObjKey, L"Error!!", MB_OK );
			return E_FAIL;
		}

		TCHAR* pKey = new TCHAR[MAX_PATH];
		lstrcpy( pKey, pObjKey );

		m_mapTexture.insert( make_pair( pKey, pTexture ) );
	}
	else
	{
		if ( FAILED( iter->second->InsertTexture( pFilePath, pStateKey, iCount, bTransparentColor, transparentColor ) ) )
		{
			MessageBox( g_hWnd, pObjKey, L"Error!!", MB_OK );
			return E_FAIL;
		}
	}


	return S_OK;
}

const TEX_INFO* CTextureMgr::GetTexture( const TCHAR* pObjKey
										 , const TCHAR* pStateKey /*= L"" */
										 , const int& iCount /*= 0*/ )
{
	map<const TCHAR*, CTexture*>::iterator iter;

	iter = find_if( m_mapTexture.begin(), m_mapTexture.end(), STRING_COMPARE( pObjKey ) );

	if ( iter == m_mapTexture.end() )
		return NULL;

	return iter->second->GetTexture( pStateKey, iCount );
}

void CTextureMgr::Release( void )
{
	/*
	map<const TCHAR*, CTexture*>::iterator iter;
	map<const TCHAR*, CTexture*>::iterator iter_end;

	iter	 = m_mapTexture.begin();
	iter_end = m_mapTexture.end();

	for(iter; iter != iter_end; ++iter)
	{
	delete [] iter->first;
	//SAFE_DELETE_ARRAY(iter->first);
	//SAFE_DELETE(iter->second);
	//const_cast

	safe_delete(iter->second);
	}
	*/

	for_each( m_mapTexture.begin(), m_mapTexture.end(), DeleteMap() );
	m_mapTexture.clear();
}

CTextureMgr::CTextureMgr( void )
{
}

CTextureMgr::~CTextureMgr( void )
{
}
