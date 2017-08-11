#include "stdafx.h"
#include "TestAStar.h"

#include "TextureMgr.h"
#include "Device.h"
#include "KeyMgr.h"
#include "Random.h"
#include "Mouse.h"
#include "TimeMgr.h"


CTestAStar::CTestAStar()
	: m_bMouseMoveCheck(false)
{
}


CTestAStar::~CTestAStar()
{
	Release();
}

HRESULT CTestAStar::Initialize( void )
{
	//C:\Development\VisualStudioProject\Starcraft_Project\MyStarcraft\Texture\Tile\AStarTest
	CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Tile/AStarTest/TestTile%d.png", L"AStarTest", TEX_MULTI, L"Tile", 7, true, D3DCOLOR_ARGB( 255, 0, 255, 255 ) );

	m_pBackgroundTexture = CTextureMgr::GetInstance()->GetTexture( L"Map" );

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

	//m_iStartIndex = RANDOM_INTERGER( TILEX * TILEY );
	//m_iEndIndex = RANDOM_INTERGER( TILEX * TILEY );

	//m_iStartIndex = 0;
	//m_iEndIndex = TILECX * TILECY - 1;
	m_iStartIndex = TILEX * 6 + 11;
	m_iEndIndex = (WINCX / TILECX) + ((WINCY / TILECY) * TILEX) - 1;

	m_vecTile[m_iStartIndex]->byDrawID = TILE_STARTINDEX;
	m_vecTile[m_iEndIndex]->byDrawID = TILE_ENDINDEX;

	for ( int i = 0; i < 7; ++i )
	{
		m_vecTileTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"AStarTest", L"Tile", i ) );
	}

	return S_OK;
}

int CTestAStar::Update( void )
{
	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( 'S' ) )
	{
		for(int i = 0; i < 300; ++i )
			this->AStarStart();
	}
	else if ( CKeyMgr::GetInstance()->GetKeyOnceDown( 'D' ) )
	{
		this->ReleaseAStarData();
	}

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

		if ( iIndex >= 0 && iIndex < this->m_vecTile.size() )
		{
			/* 시작 or 끝 위치 바꾸기.. */
			if ( this->m_vecTile[iIndex]->byDrawID == TILE_STARTINDEX ||
				 this->m_vecTile[iIndex]->byDrawID == TILE_ENDINDEX )
			{
				this->m_bClickStartIndex = ((this->m_vecTile[iIndex]->byDrawID == TILE_STARTINDEX) ? true : false);
				this->m_bMouseMoveCheck = true;
				this->m_iClickIndex = iIndex;
			}
			/* 갈 수 있는 타일 갈 수 없게끔.. */
			else if ( this->m_vecTile[iIndex]->byDrawID == TILE_IDLE )
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

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_RBUTTON ) )
	{
		D3DXVECTOR3 vMouse = CMouse::GetInstance()->GetPos();
		int iIndex = int(vMouse.x / TILECX) + int(vMouse.y / TILECY) * TILEX;

		if ( iIndex >= 0 && iIndex < this->m_vecTile.size() )
		{
			/* 갈 수 있는 타일 웨이 포인트로.. */
			if ( this->m_vecTile[iIndex]->byDrawID == TILE_IDLE )
			{
				this->m_vecTile[iIndex]->byDrawID = TILE_WAYPOINT;
			}
		}
	}

	if ( CKeyMgr::GetInstance()->GetKeyUp( VK_LBUTTON ) )
	{
		D3DXVECTOR3 vMouse = CMouse::GetInstance()->GetPos() + m_vScroll;
		int iIndex = int(vMouse.x / TILECX) + int(vMouse.y / TILECY) * TILEX;

		if ( iIndex >= 0 && iIndex < this->m_vecTile.size() )
		{
			if ( this->m_bMouseMoveCheck )
			{
				if ( this->m_bClickStartIndex )
				{
					this->m_vecTile[this->m_iStartIndex]->byDrawID = TILE_IDLE;
					this->m_iStartIndex = iIndex;
					this->m_vecTile[this->m_iStartIndex]->byDrawID = TILE_STARTINDEX;
				}
				else
				{
					this->m_vecTile[this->m_iEndIndex]->byDrawID = TILE_IDLE;
					this->m_iEndIndex = iIndex;
					this->m_vecTile[this->m_iEndIndex]->byDrawID = TILE_ENDINDEX;
				}

				this->m_bMouseMoveCheck = false;
			}
		}
	}

	return 0;
}

void CTestAStar::Render( void )
{
	D3DXMATRIX matTrans;

	TCHAR szIndexText[MIDDLE_STR] = L"";

	//D3DXMatrixTranslation( &matTrans, 0.f, 0.f, 0.f );
	//CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
	//
	//RECT rc = { (LONG)m_vScroll.x, (LONG)m_vScroll.y, WINCX, WINCY };
	//rc.right += rc.left;
	//rc.bottom += rc.top;
	//
	//CDevice::GetInstance()->GetSprite()->Draw( m_pBackgroundTexture->pTexture, &rc, NULL, NULL, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	int iStartY = (LONG)((m_vScroll.y) / TILECY);
	int iEndY = (LONG)((m_vScroll.y + WINCY) / TILECY);

	int iStartX = (LONG)((m_vScroll.x) / (TILECX));
	int iEndX = (LONG)((m_vScroll.x + WINCX) / (TILECX));

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

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTex->pTexture,
				NULL, 
				NULL,
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255)
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

void CTestAStar::Release( void )
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		safe_delete(m_vecTile[i]);
	}
	m_vecTile.clear();

	this->ReleaseAStarData();
}

void CTestAStar::AStarStart()
{
	this->ReleaseAStarData();

	this->PathFind();
}

void CTestAStar::ReleaseAStarData()
{
	for each (auto iter in m_openList)
	{
		if ( m_vecTile[iter->iIndex]->byDrawID == TILE_CHECK )
			m_vecTile[iter->iIndex]->byDrawID = TILE_IDLE;

		safe_delete( iter );
	}

	m_openList.clear();

	for each (auto iter in m_closeList)
	{
		if ( m_vecTile[iter]->byDrawID == TILE_CHECK )
			m_vecTile[iter]->byDrawID = TILE_IDLE;
	}

	m_closeList.clear();
	for each (auto iter in m_bestList)
	{
		if ( m_vecTile[iter]->byDrawID == TILE_PATH )
			m_vecTile[iter]->byDrawID = TILE_IDLE;
	}
	m_bestList.clear();
}

void CTestAStar::PathFind()
{
	NODE* pStartNode = MakeNode( m_iStartIndex );

	m_openList.push_back( pStartNode );

	while ( !m_openList.empty() )
	{
		BYTE byCheckSuccess = 0;

		//가장 적은 비용의 노드(타일을)
		auto iter = m_openList.begin();
		NODE* pCheckNode = m_openList.front();

		m_closeList.push_back((*iter)->iIndex);
		m_openList.erase( iter );

		//##위쪽 타일..
		int iIndex = pCheckNode->iIndex - TILEX;

		if( pCheckNode->iIndex >= TILEX			&&
			m_vecTile[iIndex]->byOption == 0  &&
			ListCheck(iIndex)						)
		{
			NODE* pMakeNode = MakeNode(iIndex, pCheckNode);
			m_openList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 0);
		}

		//##아래쪽 타일..
		iIndex = pCheckNode->iIndex + TILEX;

		if( pCheckNode->iIndex < (TILEX * TILEY) - (TILEX) &&
			m_vecTile[iIndex]->byOption == 0  && 
			ListCheck(iIndex)						)
		{
			NODE* pMakeNode = MakeNode(iIndex, pCheckNode);
			m_openList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 1);
		}


		//##왼쪽 타일..
		iIndex = pCheckNode->iIndex - 1;

		if( pCheckNode->iIndex % TILEX != 0  &&
			m_vecTile[iIndex]->byOption == 0  && 
			ListCheck(iIndex)						)
		{
			NODE* pMakeNode = MakeNode(iIndex, pCheckNode);
			m_openList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 2);
		}


		//##오른쪽 타일..
		iIndex = pCheckNode->iIndex + 1;

		if( pCheckNode->iIndex % TILEX != TILEX - 1  &&
			m_vecTile[iIndex]->byOption == 0  && 
			ListCheck(iIndex)						)
		{
			NODE* pMakeNode = MakeNode(iIndex, pCheckNode);
			m_openList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 3);
		}


		//##오른쪽 위 타일..
		iIndex = pCheckNode->iIndex - (TILEX - 1);

		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex >= TILEX - 1				&&
			pCheckNode->iIndex % (TILEX) != (TILEX - 1) &&
			byCheckSuccess & (1 << 0)					&&
			byCheckSuccess & (1 << 3)					&&
			m_vecTile[iIndex]->byOption == 0			&& 
			ListCheck(iIndex)						)
		{
			NODE* pMakeNode = MakeNode(iIndex, pCheckNode);
			m_openList.push_back(pMakeNode);
		}


		//##오른쪽 아래 타일..
		iIndex = pCheckNode->iIndex + (TILEX + 1);

		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex < (TILEX * TILEY) - TILEX	&&
			pCheckNode->iIndex % (TILEX) != (TILEX - 1)		&&
			byCheckSuccess & (1 << 1)						&&
			byCheckSuccess & (1 << 3)						&&
			m_vecTile[iIndex]->byOption == 0				&& 
			ListCheck(iIndex)						)
		{
			NODE* pMakeNode = MakeNode(iIndex, pCheckNode);
			m_openList.push_back(pMakeNode);
		}


		//##왼쪽 아래 타일..
		iIndex = pCheckNode->iIndex + (TILEX - 1);

		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex < (TILEX * TILEY) - TILEX	&&
			pCheckNode->iIndex % (TILEX) != 0				&&
			byCheckSuccess & (1 << 1)						&&
			byCheckSuccess & (1 << 2)						&&
			m_vecTile[iIndex]->byOption == 0				&& 
			ListCheck(iIndex)						)
		{
			NODE* pMakeNode = MakeNode(iIndex, pCheckNode);
			m_openList.push_back(pMakeNode);
		}


		//##왼쪽 위 타일..
		iIndex = pCheckNode->iIndex - (TILEX + 1);

		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex >= TILEX - 1		&&
			pCheckNode->iIndex % (TILEX) != 0	&&
			byCheckSuccess & (1 << 0)			&&
			byCheckSuccess & (1 << 2)			&&
			m_vecTile[iIndex]->byOption == 0  && 
			ListCheck(iIndex)						)
		{
			NODE* pMakeNode = MakeNode(iIndex, pCheckNode);
			m_openList.push_back(pMakeNode);
		}

		//OpenList에 담긴 비용을 정렬한다.
		m_openList.sort( CompareNode() );

		if(pCheckNode->iIndex == m_iEndIndex)
		{
			//BestList를 얻어내자.
			while(true)
			{
				m_bestList.push_back( pCheckNode->iIndex );
		
				//도착지점에서부터 시작지점까지 Node...
				pCheckNode = pCheckNode->pParent;
		
				if(pCheckNode->iIndex == m_iStartIndex)
					break;
			}
		
			//원소를 반전시킨다.
			m_bestList.pop_front();
			//m_bestList.pop_back();
			for ( auto iter : m_openList )
			{
				if ( m_vecTile[iter->iIndex]->byDrawID == TILE_IDLE )
					m_vecTile[iter->iIndex]->byDrawID = TILE_CHECK;
			}

			for ( auto iter : m_closeList )
			{
				if(m_vecTile[iter]->byDrawID == TILE_IDLE )
					m_vecTile[iter]->byDrawID = TILE_CHECK;
			}

			for ( auto iter : m_bestList )
			{
				m_vecTile[iter]->byDrawID = TILE_PATH;
			}
			//bestList.reverse();
			break;
		}
	}

	int a = 10;
}

NODE * CTestAStar::MakeNode( const int & _iIndex, NODE * _pParent )
{
	NODE* pNode = new NODE;

	float fHCost = D3DXVec3Length( &(m_vecTile[_iIndex]->vPos - m_vecTile[m_iEndIndex]->vPos) );
	float fGCost = ((_pParent) ? D3DXVec3Length( &(m_vecTile[_iIndex]->vPos - m_vecTile[_pParent->iIndex]->vPos) ) : 0.f);

	//if ( fHCost <= 100.f )
		//pNode->fCost = fHCost + fGCost + ((_pParent) ? _pParent->fCost : 0.f);
	//else
		pNode->fCost = fHCost + fGCost;
	//pNode->fCost = fHCost + fGCost * 1.f;

	pNode->iIndex = _iIndex;
	pNode->pParent = _pParent;
	return pNode;
}

bool CTestAStar::ListCheck( const int & _iIndex )
{
	for each (auto iter in m_openList)
	{
		if ( iter->iIndex == _iIndex )
			return false;
	}

	for each (auto iter in m_closeList)
	{
		if ( iter == _iIndex )
			return false;
	}

	return true;
}
