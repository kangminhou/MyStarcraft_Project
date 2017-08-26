#pragma once
#include "GameObject.h"
#include "Animation.h"

class CUI :
	public CGameObject
{
public:
	enum eUI_Kind
	{
		UI_Normal,
		UI_Anim_Loop,
	};

private:
	vector<const TEX_INFO*>	m_vecDrawTexture;
	map<wstring, vector<const TEX_INFO*>>	m_mapAllTexture;

	CAnimation*				m_pAnim;

	int						m_iDrawIndex;

	eUI_Kind				m_eKind;

	bool					m_bShowCenterPos;

public:
	CUI();
	virtual ~CUI();

public:
	void SetDrawIndex( const int& _iIndex );
	void SetShowCenterPos( const bool& _bShowCenterPos );

public:
	void AddFrame( const wstring& _wstrName, const FRAME& tFrame, const CAnimation::eAnim_Kind& eKind );
	bool ChangeFrame( const wstring& _wstrName );
	void DecideFrame( const wstring& _wstrName, const FRAME& tFrame, const CAnimation::eAnim_Kind& eKind );
	void SetUIKind( const eUI_Kind& _eKind );
	void AddTexture( const wstring& _wstrTextureKey, const wstring& _wstrObjKey, const wstring& _wstrStateKey = L"", const int& _iCount = 1 );
	bool DecideDrawTexture( const wstring& _wstrTextureKey );
	void ChangeDrawTexture( const wstring& _wstrObjKey, const wstring& _wstrStateKey = L"", const int& _iCount = 1 );
	void ChangeDrawTexture( const vector<const TEX_INFO*>& vecDrawTexture );
	void ChangeDrawTexture( const TEX_INFO* _pTexture );

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	void Render( const int& _iDrawIndex, const D3DXMATRIX& _vDrawWorldMatrix );
	virtual void Release( void ) override;

public:

};

