#include "stdafx.h"
#include "Astar.h"
#include "ObjMgr.h"
#include "Background.h"

#include "Random.h"

void CAStar::SetBackground( CBackground * _pBackground )
{
	this->m_pBackground = _pBackground;
}

void CAStar::SetEntity( CGameEntity * _pEntity )
{
	this->m_pCheckEntity = _pEntity;
}

D3DXVECTOR3 CAStar::GetDestination() const
{
	return this->m_vDestination;
}

int CAStar::AStarSetting( const D3DXVECTOR3& _vStartPos, const D3DXVECTOR3& _vEndPos, 
						  vector<D3DXVECTOR3>& vecGetData, const bool & _bCheckEntityTile )
{
	this->m_iStartIndex = GetTileIndex( _vStartPos );
	this->m_iEndIndex = GetTileIndex( _vEndPos );

	this->m_pVecBestPath = &vecGetData;

	this->m_bCheckEntityTile = _bCheckEntityTile;

	//if ( !this->m_pBackground->CheckCanGoTile( this->m_iStartIndex, 0, m_pCheckEntity, this->m_bCheckEntityTile ) )
	//	return Event_Fail_Find_NearIndex;

	if ( m_iEndIndex < 0 || m_iEndIndex >= TILEX * TILEY )
		return Event_Index_OutRange_TileSize;

	if ( !this->m_pBackground->CheckCanGoTile( m_iEndIndex, 0, this->m_pCheckEntity, this->m_bCheckEntityTile ) )	//이동 불가 타일이라면.
	{
		m_iEndIndex = this->m_pBackground->CalcNearCanGoTile( m_iStartIndex, m_iEndIndex, this->m_bCheckEntityTile );
	}

	if ( m_iEndIndex == -1 )
		return Event_Fail_Find_NearIndex;

	this->m_vDestination = D3DXVECTOR3( float( (int( m_iEndIndex ) % TILEX) * TILECX ), 
										float( (int( m_iEndIndex ) / TILEX) * TILECY ), 0.f );
	
	this->m_vDestination += D3DXVECTOR3( float( int( _vEndPos.x ) % TILECX ), float( int( _vEndPos.y ) % TILECY ), 0.f );

	//this->m_vDestination = _vEndPos;

	Release();

	this->m_pVecBestPath->clear();

	return this->SettingPathFind();
}

int CAStar::PathFind()
{
	NODE* pMakeNode = NULL;
	int   iIndex = 0;
	forward_list<NODE*> bestList;
	BYTE byCheckSuccess = 0;
	NODE* pCheckNode = this->m_pCurCheckNode;

	int	iCnt = 0;

	while( !m_OpenList.empty() )
	{
		byCheckSuccess = 0;
		/*
		* 타일 검사 성공 시 비트 단위로 체크 후 저장..
		* 저장 하는 이유는 대각선의 위치를 검사할 때 편의를 위해
		* ex) 오른쪽 위 타일을 검사하려면 오른쪽과 위의 타일이 갈 수 있는 타일인 상태에서 검사를 해야 한다..
		*											오른		왼		아래		위
		* 0		0		0		0		0		0		0		0		0
		*/

		//여기서 실제 탐색을 시작한다.
		//1.Index가 맵상에 존재는 Index인지를?
		//2.타일의 옵션이 갈수 있는 타일인가?
		//3.Open Or Close List에 있는 노드인가?

		//##위쪽 타일..
		//cout << "Index : " << pCheckNode->iIndex << endl;
		iIndex = pCheckNode->iIndex - TILEX;

		if( pCheckNode->iIndex >= TILEX			&&
			this->m_pBackground->CheckCanGoTile(iIndex, 1, this->m_pCheckEntity, m_bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, this->m_pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 0);
		}


		//##아래쪽 타일..
		iIndex = pCheckNode->iIndex + TILEX;

		if( pCheckNode->iIndex < (TILEX * TILEY) - (TILEX) &&
			this->m_pBackground->CheckCanGoTile(iIndex, 2, this->m_pCheckEntity, m_bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, this->m_pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 1);
		}


		//##왼쪽 타일..
		iIndex = pCheckNode->iIndex - 1;

		if( pCheckNode->iIndex % TILEX != 0  &&
			this->m_pBackground->CheckCanGoTile(iIndex, 3, this->m_pCheckEntity, m_bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, this->m_pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 2);
		}


		//##오른쪽 타일..
		iIndex = pCheckNode->iIndex + 1;

		if( pCheckNode->iIndex % TILEX != TILEX - 1  &&
			this->m_pBackground->CheckCanGoTile(iIndex, 4, this->m_pCheckEntity, m_bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, this->m_pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 3);
		}

		//##오른쪽 위 타일..
		iIndex = pCheckNode->iIndex - (TILEX - 1);

		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex >= TILEX - 1				&&
			pCheckNode->iIndex % (TILEX) != (TILEX - 1) &&
			byCheckSuccess & (1 << 0)					&&
			byCheckSuccess & (1 << 3)					&&
			this->m_pBackground->CheckCanGoTile(iIndex, 5, this->m_pCheckEntity, m_bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, this->m_pVecTile);
			m_OpenList.push_back(pMakeNode);
		}


		//##오른쪽 아래 타일..
		iIndex = pCheckNode->iIndex + (TILEX + 1);

		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex < (TILEX * TILEY) - TILEX	&&
			pCheckNode->iIndex % (TILEX) != (TILEX - 1)		&&
			byCheckSuccess & (1 << 1)						&&
			byCheckSuccess & (1 << 3)						&&
			this->m_pBackground->CheckCanGoTile(iIndex, 6, this->m_pCheckEntity, m_bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, this->m_pVecTile);
			m_OpenList.push_back(pMakeNode);
		}


		//##왼쪽 아래 타일..
		iIndex = pCheckNode->iIndex + (TILEX - 1);

		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex < (TILEX * TILEY) - TILEX	&&
			pCheckNode->iIndex % (TILEX) != 0				&&
			byCheckSuccess & (1 << 1)						&&
			byCheckSuccess & (1 << 2)						&&
			this->m_pBackground->CheckCanGoTile(iIndex, 7, this->m_pCheckEntity, m_bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, this->m_pVecTile);
			m_OpenList.push_back(pMakeNode);
		}


		//##왼쪽 위 타일..
		iIndex = pCheckNode->iIndex - (TILEX + 1);

		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex >= TILEX - 1		&&
			pCheckNode->iIndex % (TILEX) != 0	&&
			byCheckSuccess & (1 << 0)			&&
			byCheckSuccess & (1 << 2)			&&
			this->m_pBackground->CheckCanGoTile(iIndex, 8, this->m_pCheckEntity, m_bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, this->m_pVecTile);
			m_OpenList.push_back(pMakeNode);
		}

		if(pCheckNode->iIndex == m_iEndIndex)
		{
			//pCheckNode = pCheckNode->pParent;
			//BestList를 얻어내자.
			while( pCheckNode )
			{
				if ( pCheckNode->iIndex != m_iStartIndex && pCheckNode->iIndex != m_iEndIndex )
					bestList.push_front( pCheckNode );

				if ( pCheckNode->iIndex == m_iStartIndex )
				{
					D3DXVECTOR3 vPlusPos( TILECX * 0.5f, TILECY * 0.5f, 0.f );

					/* Entity 객체가 갈 위치 정하기.. */
					for each (auto iter in bestList)
					{
						BYTE byDecideDetailIndex = 0;
						BYTE byTempEntityTileData = iter->byEntityTileData;

						if ( !m_bCheckEntityTile )
							byTempEntityTileData = 0;

						if ( byTempEntityTileData == 0 )
							byDecideDetailIndex = 0;
						else
						{
							while ( true )
							{
								byDecideDetailIndex = RANDOM_RANGE_INTERGER( 0, CBackground::ENTITY_TOTAL_TILE_DIV );
								if ( !((1 << byDecideDetailIndex) & byTempEntityTileData) )
									break;

							}
						}

						switch ( byDecideDetailIndex )
						{
							case 0:
								vPlusPos = D3DXVECTOR3( TILECX * 0.5f, TILECY * 0.5f, 0.f );
								break;

							case 1:
							{
								int iDetailTileCX = TILECX / CBackground::ENTITY_TILE_DIV_X;
								int iDetailTileCY = TILECY / CBackground::ENTITY_TILE_DIV_Y;

								vPlusPos = D3DXVECTOR3( iDetailTileCX * 0.5f, iDetailTileCY * 0.5f, 0.f );
							}
							break;

							case 2:
							{
								int iDetailTileCX = TILECX / CBackground::ENTITY_TILE_DIV_X;
								int iDetailTileCY = TILECY / CBackground::ENTITY_TILE_DIV_Y;

								vPlusPos = D3DXVECTOR3( iDetailTileCX * 1.5f, iDetailTileCY * 0.5f, 0.f );
							}
							break;

							case 3:
							{
								int iDetailTileCX = TILECX / CBackground::ENTITY_TILE_DIV_X;
								int iDetailTileCY = TILECY / CBackground::ENTITY_TILE_DIV_Y;

								vPlusPos = D3DXVECTOR3( iDetailTileCX * 0.5f, iDetailTileCY * 1.5f, 0.f );
							}
							break;

							case 4:
							{
								int iDetailTileCX = TILECX / CBackground::ENTITY_TILE_DIV_X;
								int iDetailTileCY = TILECY / CBackground::ENTITY_TILE_DIV_Y;

								vPlusPos = D3DXVECTOR3( iDetailTileCX * 1.5f, iDetailTileCY * 1.5f, 0.f );
							}
							break;

						}


						m_pVecBestPath->push_back( (*this->m_pVecTile)[iter->iIndex]->vPos + vPlusPos );
					}

					//if ( !vecGetData.empty() )
					//	vecGetData.erase( vecGetData.begin() );

					break;
				}

				//도착지점에서부터 시작지점까지 Node...
				pCheckNode = pCheckNode->pParent;
			}

			//원소를 반전시킨다.
			//bestList.reverse();
			//cout << "Find Path" << endl;
			return Event_Success_PathFind;
		}

		m_CloseList.push_back( pCheckNode );
		m_OpenList.pop_front();

		//OpenList에 담긴 비용을 정렬한다.
		m_OpenList.sort( Compare );

		if ( this->m_OpenList.empty() )
			return Event_Fail_PathFind;

		pCheckNode = m_OpenList.front();

		++iCnt;

		if ( iCnt > 50 )
		{
			this->m_pCurCheckNode = pCheckNode;
			return Event_Not_Ended_PathFind;
		}

		if ( m_CloseList.size() >= Limit_Close_List_Num )
		{
			//cout << "Too Many size close list" << endl;
			if ( this->AStarSetting( this->m_pCheckEntity->GetPos() ) != Event_Can_PathFind )
				return Event_Fail_PathFind;

			return Event_Not_Ended_PathFind;
		}

	}//While문 끝.

	return Event_Fail_PathFind;
}

int CAStar::AStarSetting( const D3DXVECTOR3 & _vStartPos )
{
	this->m_iStartIndex = GetTileIndex( _vStartPos );
	this->m_iEndIndex = GetTileIndex( m_vDestination );

	D3DXVECTOR3 vTempDestination = this->m_vDestination;

	if ( !this->m_pBackground->CheckCanGoTile( this->m_iStartIndex, 0, m_pCheckEntity, this->m_bCheckEntityTile ) )
		return Event_Fail_Find_NearIndex;

	if ( m_iEndIndex < 0 || m_iEndIndex >= TILEX * TILEY )
		return Event_Index_OutRange_TileSize;

	if ( !this->m_pBackground->CheckCanGoTile( m_iEndIndex, 0, this->m_pCheckEntity, this->m_bCheckEntityTile ) )	//이동 불가 타일이라면.
	{
		m_iEndIndex = this->m_pBackground->CalcNearCanGoTile( m_iStartIndex, m_iEndIndex, this->m_bCheckEntityTile );
	}

	if ( m_iEndIndex == -1 )
		return Event_Fail_Find_NearIndex;

	this->m_vDestination = D3DXVECTOR3( float( (int( m_iEndIndex ) % TILEX) * TILECX ), 
										float( (int( m_iEndIndex ) / TILEX) * TILECY ), 0.f );

	this->m_vDestination += D3DXVECTOR3( float( int( vTempDestination.x ) % TILECX ), float( int( vTempDestination.y ) % TILECY ), 0.f );

	Release();

	this->m_pVecBestPath->clear();

	return this->SettingPathFind();
}

int CAStar::SettingPathFind()
{
	if ( this->m_pBackground->CheckConnectPath( this->m_pCheckEntity, this->m_iStartIndex, this->m_iEndIndex, m_bCheckEntityTile ) )
	{
		return Event_Success_PathFind;
	}

	NODE* pFirstNode = new NODE;

	pFirstNode->iIndex = m_iStartIndex;

	//최초 생성한 노드는 부모가 없다.
	pFirstNode->pParent = NULL;

	pFirstNode->fCost = 0.f;
	pFirstNode->fGCost = 0.f;

	//시작지점을 CloseList에 담고 시작을 하자.
	m_OpenList.push_back( pFirstNode );
	//m_CloseList.push_back( pFirstNode );

	m_pCurCheckNode = pFirstNode;

	m_pVecTile = m_pBackground->GetTile();

	if ( !m_pVecTile )
		return Event_Fail_PathFind;

	return Event_Can_PathFind;
}

int CAStar::MakeRoute(vector<D3DXVECTOR3>& vecGetData, const bool& _bCheckEntityTile)
{
	if ( this->m_pBackground->CheckConnectPath( this->m_pCheckEntity, this->m_iStartIndex, this->m_iEndIndex, _bCheckEntityTile ) )
	{
		//cout << "Connect Path" << endl;
		return Event_Success_PathFind;
	}

	NODE* pFirstNode = new NODE;

	pFirstNode->iIndex = m_iStartIndex;

	//최초 생성한 노드는 부모가 없다.
	pFirstNode->pParent = NULL;

	pFirstNode->fCost = 0.f;

	//시작지점을 CloseList에 담고 시작을 하자.
	m_OpenList.push_back( pFirstNode );
	//m_CloseList.push_back( pFirstNode );


	const vector<TILE*>* pVecTile = m_pBackground->GetTile();

	if(pVecTile == NULL)
		return false;

	NODE* pMakeNode = NULL;
	int   iIndex = 0;
	forward_list<NODE*> bestList;
	BYTE byCheckSuccess = 0;
	NODE* pCheckNode = pFirstNode;

	int	iCnt = 0;

	while(!m_OpenList.empty())
	{
		byCheckSuccess = 0;
		/*
		 * 타일 검사 성공 시 비트 단위로 체크 후 저장..
		 * 저장 하는 이유는 대각선의 위치를 검사할 때 편의를 위해
		 * ex) 오른쪽 위 타일을 검사하려면 오른쪽과 위의 타일이 갈 수 있는 타일인 상태에서 검사를 해야 한다..
		 *											오른		왼		아래		위
		 * 0		0		0		0		0		0		0		0		0
		 */

		//여기서 실제 탐색을 시작한다.
		//1.Index가 맵상에 존재는 Index인지를?
		//2.타일의 옵션이 갈수 있는 타일인가?
		//3.Open Or Close List에 있는 노드인가?

		//##위쪽 타일..
		iIndex = pCheckNode->iIndex - TILEX;

		if( pCheckNode->iIndex >= TILEX			&&
			this->m_pBackground->CheckCanGoTile(iIndex, 1, this->m_pCheckEntity, _bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 0);
		}


		//##아래쪽 타일..
		iIndex = pCheckNode->iIndex + TILEX;

		if( pCheckNode->iIndex < (TILEX * TILEY) - (TILEX) &&
			this->m_pBackground->CheckCanGoTile(iIndex, 2, this->m_pCheckEntity, _bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 1);
		}


		//##왼쪽 타일..
		iIndex = pCheckNode->iIndex - 1;

		if( pCheckNode->iIndex % TILEX != 0  &&
			this->m_pBackground->CheckCanGoTile(iIndex, 3, this->m_pCheckEntity, _bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 2);
		}


		//##오른쪽 타일..
		iIndex = pCheckNode->iIndex + 1;

		if( pCheckNode->iIndex % TILEX != TILEX - 1  &&
			this->m_pBackground->CheckCanGoTile(iIndex, 4, this->m_pCheckEntity, _bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 3);
		}

		//##오른쪽 위 타일..
		iIndex = pCheckNode->iIndex - (TILEX - 1);
		
		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex >= TILEX - 1				&&
			pCheckNode->iIndex % (TILEX) != (TILEX - 1) &&
			byCheckSuccess & (1 << 0)					&&
			byCheckSuccess & (1 << 3)					&&
			this->m_pBackground->CheckCanGoTile(iIndex, 5, this->m_pCheckEntity, _bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
		}
		
		
		//##오른쪽 아래 타일..
		iIndex = pCheckNode->iIndex + (TILEX + 1);
		
		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex < (TILEX * TILEY) - TILEX	&&
			pCheckNode->iIndex % (TILEX) != (TILEX - 1)		&&
			byCheckSuccess & (1 << 1)						&&
			byCheckSuccess & (1 << 3)						&&
			this->m_pBackground->CheckCanGoTile(iIndex, 6, this->m_pCheckEntity, _bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
		}
		
		
		//##왼쪽 아래 타일..
		iIndex = pCheckNode->iIndex + (TILEX - 1);
		
		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex < (TILEX * TILEY) - TILEX	&&
			pCheckNode->iIndex % (TILEX) != 0				&&
			byCheckSuccess & (1 << 1)						&&
			byCheckSuccess & (1 << 2)						&&
			this->m_pBackground->CheckCanGoTile(iIndex, 7, this->m_pCheckEntity, _bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
		}
		
		
		//##왼쪽 위 타일..
		iIndex = pCheckNode->iIndex - (TILEX + 1);
		
		//예외처리가 추가적으로 더 필요하다.
		if( pCheckNode->iIndex >= TILEX - 1		&&
			pCheckNode->iIndex % (TILEX) != 0	&&
			byCheckSuccess & (1 << 0)			&&
			byCheckSuccess & (1 << 2)			&&
			this->m_pBackground->CheckCanGoTile(iIndex, 8, this->m_pCheckEntity, _bCheckEntityTile) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
		}

		auto iter = m_OpenList.begin();
		pCheckNode = (*iter);

		m_CloseList.push_back( pCheckNode );
		m_OpenList.erase( iter );

		++iCnt;

		if ( iCnt > 50 )
			return Event_Not_Ended_PathFind;

		if(pCheckNode->iIndex == m_iEndIndex)
		{
			//pCheckNode = pCheckNode->pParent;
			//BestList를 얻어내자.
			while( pCheckNode )
			{
				if ( pCheckNode->iIndex != m_iStartIndex && pCheckNode->iIndex != m_iEndIndex )
					bestList.push_front( pCheckNode );

				if ( pCheckNode->iIndex == m_iStartIndex )
				{
					D3DXVECTOR3 vPlusPos( TILECX * 0.5f, TILECY * 0.5f, 0.f );

					/* Entity 객체가 갈 위치 정하기.. */
					for each (auto iter in bestList)
					{
						BYTE byDecideDetailIndex = 0;
						BYTE byTempEntityTileData = iter->byEntityTileData;

						if ( !_bCheckEntityTile )
							byTempEntityTileData = 0;

						if ( byTempEntityTileData == 0 )
							byDecideDetailIndex = 0;
						else
						{
							while ( true )
							{
								byDecideDetailIndex = RANDOM_RANGE_INTERGER( 0, CBackground::ENTITY_TOTAL_TILE_DIV );
								if ( !((1 << byDecideDetailIndex) & byTempEntityTileData) )
									break;

							}
						}

						switch ( byDecideDetailIndex )
						{
							case 0:
								vPlusPos = D3DXVECTOR3( TILECX * 0.5f, TILECY * 0.5f, 0.f );
								break;

							case 1:
							{
								int iDetailTileCX = TILECX / CBackground::ENTITY_TILE_DIV_X;
								int iDetailTileCY = TILECY / CBackground::ENTITY_TILE_DIV_Y;

								vPlusPos = D3DXVECTOR3( iDetailTileCX * 0.5f, iDetailTileCY * 0.5f, 0.f );
							}
								break;

							case 2:
							{
								int iDetailTileCX = TILECX / CBackground::ENTITY_TILE_DIV_X;
								int iDetailTileCY = TILECY / CBackground::ENTITY_TILE_DIV_Y;

								vPlusPos = D3DXVECTOR3( iDetailTileCX * 1.5f, iDetailTileCY * 0.5f, 0.f );
							}
								break;

							case 3:
							{
								int iDetailTileCX = TILECX / CBackground::ENTITY_TILE_DIV_X;
								int iDetailTileCY = TILECY / CBackground::ENTITY_TILE_DIV_Y;

								vPlusPos = D3DXVECTOR3( iDetailTileCX * 0.5f, iDetailTileCY * 1.5f, 0.f );
							}
								break;

							case 4:
							{
								int iDetailTileCX = TILECX / CBackground::ENTITY_TILE_DIV_X;
								int iDetailTileCY = TILECY / CBackground::ENTITY_TILE_DIV_Y;

								vPlusPos = D3DXVECTOR3( iDetailTileCX * 1.5f, iDetailTileCY * 1.5f, 0.f );
							}
								break;

						}


						vecGetData.push_back( (*pVecTile)[iter->iIndex]->vPos + vPlusPos );
					}

					//if ( !vecGetData.empty() )
					//	vecGetData.erase( vecGetData.begin() );

					break;
				}

				//도착지점에서부터 시작지점까지 Node...
				pCheckNode = pCheckNode->pParent;
			}

			//원소를 반전시킨다.
			//bestList.reverse();
			//cout << "Find Path" << endl;
			return Event_Success_PathFind;
		}

		//OpenList에 담긴 비용을 정렬한다.
		m_OpenList.sort( Compare );

	}//While문 끝.

	return Event_Fail_PathFind;
}

NODE* CAStar::MakeNode( int iIndex, NODE * pParent, const vector<TILE*>* pTile )
{
	NODE* pNode = new NODE;

	ZeroMemory(pNode, sizeof(NODE));

	pNode->iIndex  = iIndex;
	pNode->pParent = pParent;

	D3DXVECTOR3 fDist = (*pTile)[iIndex]->vPos - (*pTile)[pParent->iIndex]->vPos;

	//플레이어 위치에서 인접한 노드의 거리.
	float fPlayerCost = D3DXVec3Length(&fDist);

	//인접타일에서부터 도착지점까지의 거리.
	fDist = (*pTile)[iIndex]->vPos - (*pTile)[m_iEndIndex]->vPos;

	float fEndCost = D3DXVec3Length(&fDist);

	//플레이어위치에서 도착지점까지의 거리.
	pNode->fGCost = fPlayerCost + pParent->fGCost;
	//pNode->fCost = fPlayerCost + fEndCost;
	pNode->fCost = pNode->fGCost + fEndCost;
	//pNode->fCost = fPlayerCost + fEndCost + ((pParent) ? pParent->fCost : 0.f);

	pNode->byEntityTileData = this->m_pBackground->CalcEntityTileData( iIndex, m_pCheckEntity );

	return pNode;
}

bool CAStar::ListCheck(const int& iIndex)
{
	if ( iIndex < 0 || iIndex >= TILEX * TILEY )
		return false;

	for(list<NODE*>::iterator iter = m_OpenList.begin();
		 iter != m_OpenList.end(); ++iter)
	{
		if( ((*iter))->iIndex == iIndex )
			return false;
	}

	for(list<NODE*>::iterator iter = m_CloseList.begin();
		 iter != m_CloseList.end(); ++iter)
	{
		if( ((*iter))->iIndex == iIndex )
			return false;
	}

	return true;
}

void CAStar::Release(void)
{
	for_each(m_OpenList.begin(), m_OpenList.end(), DeleteObj());
	m_OpenList.clear();

	for_each(m_CloseList.begin(), m_CloseList.end(), DeleteObj());
	m_CloseList.clear();
}

int CAStar::GetTileIndex(const D3DXVECTOR3& vPos)
{
	return int( int(vPos.x / TILECX) + int(vPos.y / TILECY) * TILEX );
}

bool CAStar::Compare(const NODE* pNode1, const NODE* pNode2)
{
	return pNode1->fCost < pNode2->fCost;
}

CAStar::CAStar(void)
{
}

CAStar::~CAStar(void)
{
	Release();
}
