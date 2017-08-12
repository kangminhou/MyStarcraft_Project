#include "stdafx.h"
#include "Astar.h"
#include "ObjMgr.h"
#include "Background.h"

void CAStar::SetBackground( CBackground * _pBackground )
{
	this->m_pBackground = _pBackground;
}

void CAStar::SetEntity( CGameEntity * _pEntity )
{
	this->m_pCheckEntity = _pEntity;
}

bool CAStar::AStarStartPos(const D3DXVECTOR3& vStartPos
							, const D3DXVECTOR3& vEndPos
							, vector<D3DXVECTOR3>& vecGetData)
{
	m_iStartIndex = GetTileIndex(vStartPos);
	m_iEndIndex   = GetTileIndex(vEndPos);

	return AStarStart(m_iStartIndex, m_iEndIndex, vecGetData);
}

bool CAStar::AStarStart(const int& iStartIndex, 
						  const int& iEndIndex,
						  vector<D3DXVECTOR3>& vecGetData)
{
	if ( iStartIndex == iEndIndex )
		return false;

	const vector<TILE*>* pVecTile = m_pBackground->GetTile();

	if ( pVecTile == NULL )
		return false;

	if ( (*pVecTile)[iEndIndex]->byOption == 1 )	//이동 불가 타일이라면.
		return false;

	Release();

	vecGetData.clear();

	return MakeRoute(vecGetData);

}

bool CAStar::MakeRoute(vector<D3DXVECTOR3>& vecGetData)
{
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
	forward_list<int> bestList;
	BYTE byCheckSuccess = 0;
	NODE* pCheckNode = pFirstNode;

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
			this->m_pBackground->CheckCanGoTile(iIndex, 1, this->m_pCheckEntity) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 0);
		}


		//##아래쪽 타일..
		iIndex = pCheckNode->iIndex + TILEX;

		if( pCheckNode->iIndex < (TILEX * TILEY) - (TILEX) &&
			this->m_pBackground->CheckCanGoTile(iIndex, 2, this->m_pCheckEntity) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 1);
		}


		//##왼쪽 타일..
		iIndex = pCheckNode->iIndex - 1;

		if( pCheckNode->iIndex % TILEX != 0  &&
			this->m_pBackground->CheckCanGoTile(iIndex, 3, this->m_pCheckEntity) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 2);
		}


		//##오른쪽 타일..
		iIndex = pCheckNode->iIndex + 1;

		if( pCheckNode->iIndex % TILEX != TILEX - 1  &&
			this->m_pBackground->CheckCanGoTile(iIndex, 4, this->m_pCheckEntity) &&
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
			this->m_pBackground->CheckCanGoTile(iIndex, 5, this->m_pCheckEntity) &&
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
			this->m_pBackground->CheckCanGoTile(iIndex, 6, this->m_pCheckEntity) &&
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
			this->m_pBackground->CheckCanGoTile(iIndex, 7, this->m_pCheckEntity) &&
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
			this->m_pBackground->CheckCanGoTile(iIndex, 8, this->m_pCheckEntity) &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pCheckNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
		}

		auto iter = m_OpenList.begin();
		pCheckNode = (*iter);

		m_CloseList.push_back( pCheckNode );
		m_OpenList.erase( iter );

		if(pCheckNode->iIndex == m_iEndIndex)
		{
			pCheckNode = pCheckNode->pParent;
			//BestList를 얻어내자.
			while( pCheckNode )
			{
				bestList.push_front( pCheckNode->iIndex );

				if ( pCheckNode->iIndex == m_iStartIndex )
				{
					D3DXVECTOR3 vPlusPos( TILECX * 0.5f, TILECY * 0.5f, 0.f );

					for each (auto iter in bestList)
					{
						vecGetData.push_back( (*pVecTile)[iter]->vPos + vPlusPos );
					}

					break;
				}

				//도착지점에서부터 시작지점까지 Node...
				pCheckNode = pCheckNode->pParent;
			}

			//원소를 반전시킨다.
			//bestList.reverse();
			return true;
		}

		//OpenList에 담긴 비용을 정렬한다.
		m_OpenList.sort( Compare );

	}//While문 끝.

	return false;
}

NODE* CAStar::MakeNode(int iIndex, 
						NODE* pParent, 
						const vector<TILE*>* pTile)
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
	pNode->fCost = fPlayerCost + fEndCost;
	//pNode->fCost = fPlayerCost + fEndCost + ((pParent) ? pParent->fCost : 0.f);

	return pNode;
}

bool CAStar::ListCheck(const int& iIndex)
{
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
	const vector<TILE*>* pVecTile 
		= CObjMgr::GetInstance()->FindGameObject<CBackground>()->GetTile();

	if(pVecTile == NULL)
		return -1;

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
