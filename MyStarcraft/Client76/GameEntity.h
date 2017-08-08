#pragma once
#include "GameObject.h"

class CAnimation;
class CGameEntity :
	public CGameObject
{
protected:
	COMMON_DATA				m_tInfoData;				// ���� �ɷ�ġ ������..

	wstring					m_wstrStateKey;				// ���� ���� Ű ( �̹��� ����� ���� )..
	wstring					m_wstrPatternKey;			// ���� ���� ���� ���� Ű..

	vector<const TEX_INFO*> m_vecTexture;
	CAnimation*				m_pAnimCom;

public:
	CGameEntity();
	virtual ~CGameEntity();

public:
	virtual void SetPattern( const wstring& _wstrPatternKey ) PURE;

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

protected:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

};

