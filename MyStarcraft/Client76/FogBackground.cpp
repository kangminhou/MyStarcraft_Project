#include "stdafx.h"
#include "FogBackground.h"

#include "FogUnit.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "Random.h"

#include "Device.h"
#include "KeyMgr.h"
#include "TimeMgr.h"


CFogBackground::CFogBackground()
{
}


CFogBackground::~CFogBackground()
{
	Release();
}

HRESULT CFogBackground::Initialize( void )
{
	int iCnt = 0;

	while ( true )
	{
		CFogUnit* pUnit = CObjMgr::GetInstance()->FindGameObject<CFogUnit>( iCnt++ );

		if ( !pUnit )
			break;

		m_vecUnits.push_back( pUnit );
	}

	CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Tile/AStarTest/TestTile1.png", L"Test_Tile", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Tile/AStarTest/TestTile5.png", L"Test_Tile2", TEX_SINGLE );
	this->m_pTexture = CTextureMgr::GetInstance()->GetTexture( L"Test_Tile" );
	this->m_pTexture2 = CTextureMgr::GetInstance()->GetTexture( L"Test_Tile2" );

	for ( int i = 0; i < TEST_TILE_Y; ++i )
	{
		for ( int j = 0; j < TEST_TILE_X; ++j )
		{
			TEST_TILE* pTile = new TEST_TILE;
			pTile->vPos = D3DXVECTOR3( j * TEST_TILE_CX, i * TEST_TILE_CY, 0.f );

			D3DXMatrixTranslation( &pTile->matWorld, j * TEST_TILE_CX, i * TEST_TILE_CY, 0.f );

			pTile->bAlreadyWatch = false;
			pTile->bCheck = false;

			pTile->iOption = CRandom::GetInstance()->GetValue( 0, 50 );

			if ( pTile->iOption != 3 )
				pTile->color = D3DCOLOR_ARGB( 255, 0, 0, 0 );
			else
				pTile->color = D3DCOLOR_ARGB( 255, 255, 0, 0 );

			this->m_vecTIle.push_back( pTile );
		}
	}

	for ( int i = 0; i < TEST_TILE_Y; ++i )
	{
		for ( int j = 0; j < TEST_TILE_X; ++j )
		{
			TEST_TILE* pTile = new TEST_TILE;
			pTile->vPos = D3DXVECTOR3( j * TEST_TILE_CX, i * TEST_TILE_CY, 0.f );

			D3DXMatrixTranslation( &pTile->matWorld, j * TEST_TILE_CX, i * TEST_TILE_CY, 0.f );

			pTile->bAlreadyWatch = false;
			pTile->bCheck = false;

			pTile->iOption = CRandom::GetInstance()->GetValue( 0, 5 );

			pTile->color = D3DCOLOR_ARGB( 255, 255, 255, 255 );

			this->m_vecTIle2.push_back( pTile );
		}
	}

	return S_OK;
}

int CFogBackground::Update( void )
{
	this->m_vecSeeIndex.clear();

	for ( size_t i = 0; i < m_vecUnits.size(); ++i )
	{
		CFogUnit* pUnit = this->m_vecUnits[i];

		D3DXVECTOR3 vPlayerPos = pUnit->GetPos();
		D3DXVECTOR3 vTilePos;
		D3DXVECTOR3 vTileCenterPos( TEST_TILE_CX * 0.5f, TEST_TILE_CY * 0.5f, 0.f );

		int iPlayerSight = pUnit->m_iSight * ((TEST_TILE_CX + TEST_TILE_CY) * 0.5f);

		int iPlayerIndex = this->GetTileIndex( vPlayerPos );

		int iDistX = iPlayerSight / TEST_TILE_CX;
		int iDistY = iPlayerSight / TEST_TILE_CY;

		int iStartX = ((vPlayerPos.x)) / TEST_TILE_CX - iDistX;
		int iStartY = ((vPlayerPos.y)) / TEST_TILE_CY - iDistY;

		int iEndX = iStartX + iDistX * 2;
		int iEndY = iStartY + iDistY * 2;

		for ( int i = iStartY; i <= iEndY + 1; ++i )
		{
			for ( int j = iStartX; j <= iEndX + 1; ++j )
			{
				int iIndex = j + i * TEST_TILE_X;

				if ( j < 0 || i < 0 || j >= TEST_TILE_X || i >= TEST_TILE_Y )
					continue;

				auto& tile = this->m_vecTIle[iIndex];
				vTilePos = tile->vPos;

				if ( tile->iOption == 3 )
					continue;

				float fDist = D3DXVec3Length( &(vPlayerPos - vTilePos) );

				if ( fDist <= iPlayerSight )
				{
					if ( tile->bCheck )
					{
						if ( tile->bAlreadyWatch )
						{
							tile->color = D3DCOLOR_ARGB( 200, 0, 0, 0 );
						}
						else
						{
							if ( tile->iOption != 3 )
								tile->color = D3DCOLOR_ARGB( 255, 0, 0, 0 );
							else
								tile->color = D3DCOLOR_ARGB( 0, 0, 0, 0 );

						}
					}
					else if ( this->CheckRay( this->GetTileIndex( tile->vPos ), this->GetTileIndex( vPlayerPos ) ) )
					{
						m_vecSeeIndex.push_back( iIndex );
						//int iAlpha = (int)(fDist / iPlayerSight * 200.f);
						//tile->color = D3DCOLOR_ARGB( iAlpha, 0, 0, 0 );
						tile->bCheck = true;
					}
					else
					{
						if ( tile->bAlreadyWatch )
						{
							tile->color = D3DCOLOR_ARGB( 200, 0, 0, 0 );
						}
						else
						{
							if ( tile->iOption != 3 )
								tile->color = D3DCOLOR_ARGB( 255, 0, 0, 0 );
							else
								tile->color = D3DCOLOR_ARGB( 0, 0, 0, 0 );

						}
					}
				}
				else
				{
					if ( !tile->bCheck )
					{
						if ( tile->bAlreadyWatch )
						{
							tile->color = D3DCOLOR_ARGB( 200, 0, 0, 0 );
						}
						else
						{
							if ( tile->iOption != 3 )
								tile->color = D3DCOLOR_ARGB( 255, 0, 0, 0 );
							else
								tile->color = D3DCOLOR_ARGB( 0, 0, 0, 0 );

						}
						
					}
				}

			}

		}
	}

	for ( size_t i = 0; i < m_vecSeeIndex.size(); ++i )
	{
		auto& tile = this->m_vecTIle[this->m_vecSeeIndex[i]];

		tile->color = D3DCOLOR_ARGB( 0, 0, 0, 0 );
		tile->bCheck = false;

		if ( !tile->bAlreadyWatch )
			tile->bAlreadyWatch = true;
	}

	this->Scroll();

	return 0;
}

void CFogBackground::Render( void )
{
	int iStartX = m_vScroll.x / TEST_TILE_CX;
	int iStartY = m_vScroll.y / TEST_TILE_CY;

	int iEndX = WINCX / TEST_TILE_CX + iStartX;
	int iEndY = WINCY / TEST_TILE_CY + iStartY;

	D3DXMATRIX matTrans;
	D3DXVECTOR3 vTilePos;

	for ( int i = iStartY; i < iEndY + 2; ++i )
	{
		for ( int j = iStartX; j < iEndX + 2; ++j )
		{
			int iIndex = j + i * TEST_TILE_X;

			if ( j < 0 || i < 0 || j >= TEST_TILE_X || i >= TEST_TILE_Y )
				continue;

			vTilePos = this->m_vecTIle2[iIndex]->vPos;

			D3DXMatrixTranslation( &matTrans, vTilePos.x - m_vScroll.x, vTilePos.y - m_vScroll.y, 0.f );

			CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );

			CDevice::GetInstance()->GetSprite()->Draw(
				this->m_pTexture->pTexture, nullptr, nullptr, nullptr, this->m_vecTIle2[iIndex]->color
			);

			vTilePos = this->m_vecTIle[iIndex]->vPos;
			
			D3DXMatrixTranslation( &matTrans, vTilePos.x - m_vScroll.x, vTilePos.y - m_vScroll.y, 0.f );

			CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );

			CDevice::GetInstance()->GetSprite()->Draw(
				this->m_pTexture2->pTexture, nullptr, nullptr, nullptr, this->m_vecTIle[iIndex]->color
			);

			//if ( this->m_vecTIle[iIndex]->color == D3DCOLOR_ARGB( 255, 255, 255, 255 ) )
			//{
			//	TCHAR str[128];
			//	swprintf_s( str, L"%d", iIndex );
			//
			//	D3DXMATRIX matScale, matWorld;
			//	D3DXMatrixScaling( &matScale, 0.5f, 0.5f, 1.f );
			//	matWorld = matScale * matTrans;
			//
			//	CDevice::GetInstance()->GetSprite()->SetTransform( &matWorld );
			//
			//	CDevice::GetInstance()->GetFont()->DrawTextW(
			//		CDevice::GetInstance()->GetSprite(),
			//		str,
			//		lstrlen( str ),
			//		nullptr,
			//		0, 
			//		D3DCOLOR_ARGB( 255, 255, 255, 255 )
			//	);
			//}

		}

	}


}

void CFogBackground::Release( void )
{
	for ( size_t i = 0; i < this->m_vecTIle.size(); ++i )
	{
		safe_delete( this->m_vecTIle[i] );
	}

	this->m_vecTIle.clear();
}

void CFogBackground::UpdatePosition( const D3DXVECTOR3& vPrevPos )
{
	
}

int CFogBackground::GetTileIndex( const D3DXVECTOR3& _vPos )
{
	int x = _vPos.x / TEST_TILE_CX;
	int y = _vPos.y / TEST_TILE_CY;

	int iIndex = x + y * TEST_TILE_X;

	return iIndex;
}

bool CFogBackground::CheckRay( const D3DXVECTOR3 & _vStart, const D3DXVECTOR3 & _vEnd )
{
	D3DXVECTOR3 vDir = (_vEnd - _vStart);
	vDir.z = 0.f;
	float fDist = D3DXVec3Length( &vDir );
	D3DXVec3Normalize( &vDir, &vDir );

	float fXDir = 0.f;
	float fYDir = 0.f;

	if ( fabsf( vDir.x ) >= 0.00001f )
		fXDir = vDir.x / fabsf( vDir.x );
	if ( fabsf( vDir.y ) >= 0.000001f )
		fYDir = vDir.y / fabsf( vDir.y );

	float fCnt = 0.f;
	int iCnt = 0;

	D3DXVECTOR3 vTempStartPos = _vStart;

	vDir *= 16.f;

	while ( true )
	{
		vTempStartPos += vDir;

		//fCnt += D3DXVec3Length( &vDir );

		//++iCnt;

		int iIndex = this->GetTileIndex( vTempStartPos );

		if ( m_vecTIle[iIndex]->iOption == 3 )
			return false;

		if ( ((fXDir < 0.f && _vEnd.x - vTempStartPos.x >= fXDir) ||
			(fXDir >= 0.f && _vEnd.x - vTempStartPos.x <= fXDir)) &&
			 ((fYDir < 0.f && _vEnd.y - vTempStartPos.y >= fYDir) ||
			 (fYDir >= 0.f && _vEnd.y - vTempStartPos.y <= fYDir)) )
			break;

		//if ( _vEnd.x - vTempStartPos.x <= fXDir && _vEnd.y - vTempStartPos.y <= fYDir )
		//	break;

	}

	//cout << "iCnt : " << iCnt << endl;

	return true;
}

bool CFogBackground::CheckRay( const int & _iStartIndex, const int & _iEndIndex )
{
	D3DXVECTOR3 vCenter( TEST_TILE_CX * 0.5f, TEST_TILE_CY * 0.5, 0.f );
	return this->CheckRay( this->m_vecTIle[_iStartIndex]->vPos + vCenter, this->m_vecTIle[_iEndIndex]->vPos + vCenter );
}

void CFogBackground::Scroll()
{
	D3DXVECTOR3 vMove( 0.f, 0.f, 0.f );
	bool bMove = false;
	float m_fSpeed = 400.f;

	if(CKeyMgr::GetInstance()->GetKeyStayDown(VK_LEFT) )
	{
		vMove.x -= m_fSpeed * GET_TIME;
		bMove = true;
	}
	if(CKeyMgr::GetInstance()->GetKeyStayDown(VK_RIGHT) )
	{
		vMove.x += m_fSpeed * GET_TIME;
		bMove = true;
	}
	if(CKeyMgr::GetInstance()->GetKeyStayDown(VK_UP) )
	{
		vMove.y -= m_fSpeed * GET_TIME;
		bMove = true;
	}
	if(CKeyMgr::GetInstance()->GetKeyStayDown(VK_DOWN) )
	{
		vMove.y += m_fSpeed * GET_TIME;
		bMove = true;
	}

	if ( bMove )
	{
		D3DXVec3Normalize( &vMove, &vMove );

		vMove *= m_fSpeed * GET_TIME;

		this->m_vScroll += vMove;

	}
}
