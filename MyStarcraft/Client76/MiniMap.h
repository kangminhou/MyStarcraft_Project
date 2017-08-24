#pragma once
#include "GameObject.h"

class CGameEntity;

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
	D3DXVECTOR3 m_vImageHalfSize;

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

