#pragma once

#include "Include.h"

class CTexture abstract
{
public:
	virtual HRESULT InsertTexture( const TCHAR* pPath,				// �̹��� ���..
								   const TCHAR* pStateKey = L"",	// ��������Ʈ�� ����� �� ���°��� ǥ��..
								   const int& iCount = 0,			// �̹��� ���..
								   bool bTransparentColor = false,	// ����� ����..
								   D3DCOLOR transparentColor = D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) PURE; // ������ �����..

	virtual const TEX_INFO* GetTexture( const TCHAR* pKey = L"", const int& iCount = 0 )PURE;

	virtual void Release( void )PURE;

public:
	CTexture( void );
	virtual ~CTexture( void );
};
