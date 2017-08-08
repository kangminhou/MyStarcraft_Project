#pragma once

#include "Include.h"		//포함


class CTexture;				//전방선언(사용자 정의 자료형)
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr);

private:
	map<const TCHAR*, CTexture*>		m_mapTexture;
	
public:
	wstring m_wstrCurrentPath;

public:
	HRESULT	InsertTexture( const TCHAR* pFilePath,		//이미지 경로
						   const TCHAR* pObjKey,		//객체를 나타내는 고유 키값(ex.Item, Player)
						   eTextureType eType,		//싱글인지? 멀티인지?
						   const TCHAR* pStateKey = L"",		//Player-> 걷는다, 맞는다, 공격한다등등..
						   const int& iCount = 0,
						   bool bTransparentColor = false,
						   D3DCOLOR transparentColor = D3DCOLOR_ARGB( 255, 0, 0, 0 ) );			//MultiTexture의 이미지 장수.

	const TEX_INFO* GetTexture(const TCHAR* pObjKey
								, const TCHAR* pStateKey = L""
								, const int& iCount = 0);

	void Release(void);

	HRESULT LoadImagePath( wstring wstrImagePath );


private:
	CTextureMgr(void);
public:
	~CTextureMgr(void);
};
