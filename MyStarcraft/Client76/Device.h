#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice);

//LPDIRECT3D9 : 장치와 관련된 일을 하는 객체.
private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3dDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;

public:
	LPDIRECT3DDEVICE9 GetDevice(void)
	{
		return m_pD3dDevice;
	}

	LPD3DXSPRITE	GetSprite(void)
	{
		return m_pSprite;
	}

	LPD3DXFONT		GetFont(void)
	{
		return m_pFont;
	}

public:
	HRESULT Init3D(void);
	void SerParameters(D3DPRESENT_PARAMETERS& d3dp);
	void Render_Begin(void);
	void Render_End(void);
	void Render_End(HWND& _hWnd);
	void Release(void);

public:
	CDevice(void);
	~CDevice(void);
};


//##장치 초기화
//장치에 관련된 객체들은 Com객체라고 한다.
//Component Object Model약자.

//Com객체는 뭘까?
//마이크로소프트에서 책정한 객체간의 통신규약말한다.

//어시스트는 원래 비주얼스튜디오에 대한 독립적으로 적용되는 COm객체에 해당된다.
