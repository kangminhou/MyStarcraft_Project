#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice);

//LPDIRECT3D9 : ��ġ�� ���õ� ���� �ϴ� ��ü.
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


//##��ġ �ʱ�ȭ
//��ġ�� ���õ� ��ü���� Com��ü��� �Ѵ�.
//Component Object Model����.

//Com��ü�� ����?
//����ũ�μ���Ʈ���� å���� ��ü���� ��űԾ฻�Ѵ�.

//��ý�Ʈ�� ���� ���־�Ʃ����� ���� ���������� ����Ǵ� COm��ü�� �ش�ȴ�.
