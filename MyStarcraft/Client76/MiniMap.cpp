#include "stdafx.h"
#include "MiniMap.h"

#include "TextureMgr.h"
#include "ObjMgr.h"

#include "GameEntity.h"


CMiniMap::CMiniMap()
{
}


CMiniMap::~CMiniMap()
{
}

HRESULT CMiniMap::Initialize( void )
{
	this->m_pBackTexture = CTextureMgr::GetInstance()->GetTexture( L"BackGround" );

	for ( size_t i = 0; i < 2; ++i )
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Minimap", L"Entity", i );
		vector<MINI_ENTITY_DATA> vecMiniEntityData;
	
		m_vecMiniEntityData.push_back( make_pair( vecMiniEntityData, pTexture ) );
	}

	CGameObject::Initialize();

	D3DXVECTOR3 vSize( 1.f, 1.f, 1.f );
	vSize.x = (float)Minimap_CX / (float)this->m_pBackTexture->ImageInfo.Width;
	vSize.y = (float)Minimap_CY / (float)this->m_pBackTexture->ImageInfo.Height;

	this->SetSize( vSize );

	this->m_vImageHalfSize.x = Minimap_CX * 0.5f;
	this->m_vImageHalfSize.y = Minimap_CY * 0.5f;
	this->m_vImageHalfSize.z = 0.f;

	return S_OK;
}

int CMiniMap::Update( void )
{
	return 0;
}

void CMiniMap::Render( void )
{
	this->DrawTexture( this->m_pBackTexture, this->GetWorldMatrix(), 
					   D3DXVECTOR3( this->m_pBackTexture->ImageInfo.Width * 0.5f, this->m_pBackTexture->ImageInfo.Height * 0.5f, 0.f ) );

	for ( size_t i = 0; i < m_vecMiniEntityData.size(); ++i )
	{
		const TEX_INFO* pTexture = this->m_vecMiniEntityData[i].second;
		D3DXVECTOR3 vCenter( pTexture->ImageInfo.Width * 0.5f, pTexture->ImageInfo.Height * 0.5f, 0.f );

		for ( size_t j = 0; j < m_vecMiniEntityData[i].first.size(); ++j )
		{
			D3DXMATRIX& matDraw = this->m_vecMiniEntityData[i].first[j].matMiniWorld;
			this->DrawTexture( pTexture, matDraw, vCenter );
		}
	}
}

void CMiniMap::Release( void )
{
}

int CMiniMap::MoveEntity( const CGameEntity * _pEntity )
{
	if ( _pEntity->GetObjectType() < OBJ_TYPE_USER || _pEntity->GetObjectType() > OBJ_TYPE_USER2 )
		return -1;

	int iOut = _pEntity->GetMinimapSpaceDataKey();
	int iVecIndex = _pEntity->GetObjectType() - OBJ_TYPE_USER;

	if ( iOut == -1 )
	{
		MINI_ENTITY_DATA miniEntityData;
		miniEntityData.pEntity = _pEntity;

		iOut = this->m_vecMiniEntityData[iVecIndex].first.size();

		this->m_vecMiniEntityData[iVecIndex].first.push_back( miniEntityData );
	}

	D3DXVECTOR3 vSize = this->GetTransform()->GetSize();
	D3DXVECTOR3 vEntityPos = _pEntity->GetPos();
	D3DXVECTOR3 vMiniPos = D3DXVECTOR3( vEntityPos.x * vSize.x, vEntityPos.y * vSize.y, 0.f ) + this->GetPos() - m_vImageHalfSize;

	D3DXMATRIX matTrans, matScale;

	RECT rcEntityCol = _pEntity->GetColRect();
	D3DXVECTOR3 vScale( float( rcEntityCol.right - rcEntityCol.left ) / 16.f, float( rcEntityCol.bottom - rcEntityCol.top ) / 16.f, 1.f );

	D3DXMatrixScaling( &matScale, vScale.x, vScale.y, 1.f );
	D3DXMatrixTranslation( &matTrans, vMiniPos.x, vMiniPos.y, 0.f );
	this->m_vecMiniEntityData[iVecIndex].first[iOut].matMiniWorld = matScale * matTrans;
	//this->m_vecMiniEntityData.first[iOut].matMiniWorld

	return iOut;
}

bool CMiniMap::EraseEntity( const CGameEntity * _pEntity )
{
	if ( _pEntity->GetObjectType() < OBJ_TYPE_USER || _pEntity->GetObjectType() > OBJ_TYPE_USER2 )
		return false;

	int iOut = _pEntity->GetMinimapSpaceDataKey();
	int iVecIndex = _pEntity->GetObjectType() - OBJ_TYPE_USER;

	if ( iOut == -1 )
		return false;

	this->m_vecMiniEntityData[iVecIndex].first.erase( this->m_vecMiniEntityData[iVecIndex].first.begin() + iOut );
	for ( size_t i = iOut; i < this->m_vecMiniEntityData[iVecIndex].first.size(); ++i )
	{
		const_cast<CGameEntity*>(this->m_vecMiniEntityData[iVecIndex].first[i].pEntity)->SetMinimapSpaceDataKey( i );
	}

	return true;
}
