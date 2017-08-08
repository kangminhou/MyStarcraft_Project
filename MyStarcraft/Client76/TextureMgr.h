#pragma once

#include "Include.h"		//����


class CTexture;				//���漱��(����� ���� �ڷ���)
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr);

private:
	map<const TCHAR*, CTexture*>		m_mapTexture;
	
public:
	wstring m_wstrCurrentPath;

public:
	HRESULT	InsertTexture( const TCHAR* pFilePath,		//�̹��� ���
						   const TCHAR* pObjKey,		//��ü�� ��Ÿ���� ���� Ű��(ex.Item, Player)
						   eTextureType eType,		//�̱�����? ��Ƽ����?
						   const TCHAR* pStateKey = L"",		//Player-> �ȴ´�, �´´�, �����Ѵٵ��..
						   const int& iCount = 0,
						   bool bTransparentColor = false,
						   D3DCOLOR transparentColor = D3DCOLOR_ARGB( 255, 0, 0, 0 ) );			//MultiTexture�� �̹��� ���.

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
