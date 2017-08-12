#include "stdafx.h"
#include "RayTestBackground.h"

#include "Random.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Mouse.h"
#include "Device.h"


CRayTestBackground::CRayTestBackground()
{
}


CRayTestBackground::~CRayTestBackground()
{
}

HRESULT CRayTestBackground::Initialize( void )
{
	CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Tile/AStarTest/TestTile%d.png", L"AStarTest", TEX_MULTI, L"Tile", 7, true, D3DCOLOR_ARGB( 255, 0, 255, 255 ) );

	for ( int i = 0; i < 7; ++i )
	{
		m_vecTileTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"AStarTest", L"Tile", i ) );
	}

	for(int i = 0; i < TILEY; ++i)
	{
		for ( int j = 0; j < TILEX; ++j )
		{
			TEST_TILE* pTile = new TEST_TILE;

			//float fX = j * TILECX + ((i % 2) * (TILECX / 2.f));
			//float fY = i * (TILECY * 0.5f);

			float fX = float( j * TILECX );
			float fY = float( i * TILECY );

			pTile->vPos = D3DXVECTOR3( fX, fY, 0.f );
			pTile->vSize = D3DXVECTOR3( (float)TILECX, (float)TILECY, 0.f );

			int iRandom = RANDOM_INTERGER( 1 );
			if ( iRandom > 7 )
			{
				pTile->byOption = 1;
				pTile->byDrawID = TILE_WALL;
			}
			else
			{
				pTile->byOption = 0;
				pTile->byDrawID = TILE_IDLE;
			}

			m_vecTile.push_back(pTile);
		}
	}

	CGameObject::Initialize();

	return S_OK;
}

int CRayTestBackground::Update( void )
{
	if ( CKeyMgr::GetInstance()->GetKeyStayDown( VK_LEFT ) )
	{
		this->m_vScroll.x -= 1000 * GET_TIME;
	}
	else if ( CKeyMgr::GetInstance()->GetKeyStayDown( VK_RIGHT ) )
	{
		this->m_vScroll.x += 1000 * GET_TIME;
	}

	if ( CKeyMgr::GetInstance()->GetKeyStayDown( VK_UP ) )
	{
		this->m_vScroll.y -= 1000 * GET_TIME;
	}
	else if ( CKeyMgr::GetInstance()->GetKeyStayDown( VK_DOWN ) )
	{
		this->m_vScroll.y += 1000 * GET_TIME;
	}

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) )
	{
		D3DXVECTOR3 vMouse = CMouse::GetInstance()->GetPos() + m_vScroll;
		int iIndex = int(vMouse.x / TILECX) + int(vMouse.y / TILECY) * TILEX;

		if ( iIndex >= 0 && (unsigned int)iIndex < this->m_vecTile.size() )
		{
			/* 갈 수 있는 타일 갈 수 없게끔.. */
			if ( this->m_vecTile[iIndex]->byDrawID == TILE_IDLE )
			{
				this->m_vecTile[iIndex]->byOption = 1;
				this->m_vecTile[iIndex]->byDrawID = TILE_WALL;
			}
			else /* 갈 수 없는 타일 갈 수 있게끔.. */
			{
				this->m_vecTile[iIndex]->byOption = 0;
				this->m_vecTile[iIndex]->byDrawID = TILE_IDLE;
			}
		}
	}

	return 0;
}

void CRayTestBackground::Render( void )
{
	int iStartY = (LONG)((m_vScroll.y) / TILECY);
	int iEndY = (LONG)((m_vScroll.y + WINCY) / TILECY);

	int iStartX = (LONG)((m_vScroll.x) / (TILECX));
	int iEndX = (LONG)((m_vScroll.x + WINCX) / (TILECX));

	D3DXMATRIX matTrans;

	for(int i = iStartY; i < iEndY + 1; ++i)
	{
		for(int j = iStartX; j < iEndX + 1; ++j)
		{
			int iIndex = i * TILEX + j;

			if ( i < 0 || i >= TILEY || j < 0 || j >= TILEX )
				continue;

			const TEX_INFO* pTileTex = this->m_vecTileTexture[m_vecTile[iIndex]->byDrawID];

			D3DXMatrixTranslation(&matTrans
								   , m_vecTile[iIndex]->vPos.x - m_vScroll.x	//0 : x
								   , m_vecTile[iIndex]->vPos.y - m_vScroll.y	//1 : y
								   , 0.f);

			CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTex->pTexture,
				NULL,
				NULL,
				NULL,
				D3DCOLOR_ARGB( 255, 255, 255, 255 )
			);

			////##폰트 출력
			//swprintf_s(szIndexText, L"%d", iIndex);
			//
			//CDevice::GetInstance()->GetFont()->DrawTextW(
			//	CDevice::GetInstance()->GetSprite(),
			//	szIndexText,
			//	lstrlen(szIndexText),
			//	NULL,
			//	NULL,
			//	D3DCOLOR_ARGB(255, 0, 0, 255)
			//	);
		}
	}
}

void CRayTestBackground::Release( void )
{
}
