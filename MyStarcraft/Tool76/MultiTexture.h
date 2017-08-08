#pragma once
#include "texture.h"

class CMultiTexture :
	public CTexture
{
private:
	map<const TCHAR*, vector<TEX_INFO*>>		m_mapMultiTexture;

public:
	virtual HRESULT InsertTexture( const TCHAR* pPath,				// �̹��� ���..
								   const TCHAR* pStateKey = L"",	// ��������Ʈ�� ����� �� ���°��� ǥ��..
								   const int& iCount = 0,			// �̹��� ���..
								   bool bTransparentColor = false,	// ����� ����..
								   D3DCOLOR transparentColor = D3DCOLOR_ARGB( 255, 0, 0, 0 ) ); // ������ �����..

	virtual const TEX_INFO* GetTexture( const TCHAR* pKey = L""
										, const int& iCount = 0 );

	virtual void Release( void );

public:
	CMultiTexture( void );
	virtual ~CMultiTexture( void );
};
