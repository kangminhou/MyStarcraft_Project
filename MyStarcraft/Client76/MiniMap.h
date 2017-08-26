#pragma once
#include "GameObject.h"

class CGameEntity;
class CPlayer;
class CMouse;

class CMiniMap :
	public CGameObject
{
private:
	typedef struct MiniEntityData
	{
		const CGameEntity*	pEntity;
		D3DXMATRIX			matMiniWorld;
	}MINI_ENTITY_DATA;

private:
	const TEX_INFO*	m_pBackTexture;
	const TEX_INFO*	m_pAreaTexture;

	D3DXVECTOR3 m_vImageHalfSize;
	D3DXVECTOR3	m_vMinimapSize;

	D3DXMATRIX	m_matAreaWorld;

	CPlayer*	m_pPlayer;
	CMouse*		m_pMouse;

	RECT		m_rcMouseCol;

	bool		m_bInitRect;

	vector<pair<vector<MINI_ENTITY_DATA>, const TEX_INFO*>>	m_vecMiniEntityData;
	
public:
	CMiniMap();
	virtual ~CMiniMap();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize( void )override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	int MoveEntity( const CGameEntity* _pEntity );
	bool EraseEntity( const CGameEntity* _pEntity );

};

