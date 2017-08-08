#pragma once
#include "texture.h"

class CMultiTexture :
	public CTexture
{
private:
	map<const TCHAR*, vector<TEX_INFO*>>		m_mapMultiTexture;

public:
	virtual HRESULT InsertTexture( const TCHAR* pPath,				// 이미지 경로..
								   const TCHAR* pStateKey = L"",	// 스프라이트를 출력할 때 상태값을 표현..
								   const int& iCount = 0,			// 이미지 장수..
								   bool bTransparentColor = false,	// 투명색 적용..
								   D3DCOLOR transparentColor = D3DCOLOR_ARGB( 255, 0, 0, 0 ) ); // 적용할 투명색..

	virtual const TEX_INFO* GetTexture( const TCHAR* pKey = L""
										, const int& iCount = 0 );

	virtual void Release( void );

public:
	CMultiTexture( void );
	virtual ~CMultiTexture( void );
};
