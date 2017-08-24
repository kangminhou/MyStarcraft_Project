#pragma once
#include "GameObject.h"

class CGameEntity;

class CResourceObj :
	public CGameObject
{
private:
	CGameEntity*			m_pParentEntity;
	vector<const TEX_INFO*>	m_vecDrawTexture;

	BYTE					m_byDrawIndex;

	bool					m_bHideThis;
	bool					m_bInit;

	D3DXMATRIX				m_matWorld;

public:
	CResourceObj();
	virtual ~CResourceObj();

public:
	void SetParentEntity( CGameEntity* _pParentEntity );
	void SetDrawTexture( const wstring& _wstrObjKey, const wstring& _wstrStateKey, const int& _iStartindex, const int& _iEndIndex );
	void HideResourceObj();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

};

