#include "stdafx.h"
#include "MiniMap.h"

#include "TextureMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "Mouse.h"

#include "GameEntity.h"
#include "Player.h"


CMiniMap::CMiniMap()
{
}


CMiniMap::~CMiniMap()
{
}

HRESULT CMiniMap::Initialize( void )
{
	this->m_pBackTexture = CTextureMgr::GetInstance()->GetTexture( L"BackGround" );

	this->m_pAreaTexture = CTextureMgr::GetInstance()->GetTexture( L"Mini_Area" );

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

	this->m_vMinimapSize.x = (float)Minimap_CX / ((float)this->m_pBackTexture->ImageInfo.Width - WINCX * 0.5f);
	this->m_vMinimapSize.y = (float)Minimap_CY / ((float)this->m_pBackTexture->ImageInfo.Height - WINCY * 0.5f);
	this->m_vMinimapSize.z = 1.f;

	this->m_bInitRect = false;

	return S_OK;
}

int CMiniMap::Update( void )
{
	if ( !m_bInitRect )
	{
		D3DXVECTOR3 vPos = this->GetPos();

		this->m_rcMouseCol.left =   (LONG)(vPos.x - this->m_vImageHalfSize.x);
		this->m_rcMouseCol.top =    (LONG)(vPos.y - this->m_vImageHalfSize.y);
		this->m_rcMouseCol.right =  (LONG)(vPos.x + this->m_vImageHalfSize.x);
		this->m_rcMouseCol.bottom = (LONG)(vPos.y + this->m_vImageHalfSize.y);

		this->m_pPlayer = CObjMgr::GetInstance()->FindGameObject<CPlayer>();
		this->m_pMouse = CMouse::GetInstance();

		this->m_bInitRect = true;
	}

	D3DXVECTOR3 vAreaPos( (float)this->m_rcMouseCol.left, (float)this->m_rcMouseCol.top, 0.f );
	vAreaPos.x += (this->m_vScroll.x) * this->m_vMinimapSize.x;
	vAreaPos.y += (this->m_vScroll.y) * this->m_vMinimapSize.y;

	if ( LONG( vAreaPos.x + this->m_pAreaTexture->ImageInfo.Width ) > m_rcMouseCol.right )
	{
		vAreaPos.x = (float)m_rcMouseCol.right - this->m_pAreaTexture->ImageInfo.Width;
	}

	D3DXMatrixTranslation( &m_matAreaWorld, vAreaPos.x, vAreaPos.y, 0.f );

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) )
	{
		D3DXVECTOR3 vMouse = this->m_pMouse->GetPos();
		POINT ptMouse = { vMouse.x, vMouse.y };

		if ( PtInRect( &this->m_rcMouseCol, ptMouse ) )
		{
			if ( !this->m_pMouse->GetMinimapClick() )
				this->m_pMouse->SetMinimapClick( true );
		}
	}

	if ( CKeyMgr::GetInstance()->GetKeyStayDown( VK_LBUTTON ) )
	{
		if ( this->m_pMouse->GetMinimapClick() )
		{
			D3DXVECTOR3 vMouse = this->m_pMouse->GetPos();
			POINT ptMouse = { vMouse.x, vMouse.y };

			if ( PtInRect( &this->m_rcMouseCol, ptMouse ) )
			{
				vMouse -= D3DXVECTOR3( (float)m_rcMouseCol.left, (float)m_rcMouseCol.top, 0.f );

				vMouse.x /= this->m_vMinimapSize.x;
				vMouse.y /= this->m_vMinimapSize.y;

				D3DXVECTOR3 vWinHalf( WINCX * 0.5f, WINCY * 0.5f, 0.f );

				vMouse -= vWinHalf * 0.5f;

				this->m_pPlayer->SetScroll( vMouse );
			}
		}
	}

	if ( CKeyMgr::GetInstance()->GetKeyUp( VK_LBUTTON ) )
	{
		if ( this->m_pMouse->GetMinimapClick() )
			this->m_pMouse->SetMinimapClick( false );
	}

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

	this->DrawTexture( this->m_pAreaTexture, this->m_matAreaWorld );

	this->DrawRect( this->m_rcMouseCol );
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
