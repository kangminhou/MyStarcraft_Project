#include "stdafx.h"
#include "Astar.h"
#include "ObjMgr.h"
#include "Background.h"

void CAStar::SetBackground( CBackground * _pBackground )
{
	this->m_pBackground = _pBackground;
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

	if ( (*pVecTile)[iEndIndex]->byOption == 1 )	//�̵� �Ұ� Ÿ���̶��.
		return false;

	Release();

	vecGetData.clear();

	MakeRoute(vecGetData);

	return true;

}

void CAStar::MakeRoute(vector<D3DXVECTOR3>& vecGetData)
{
	NODE* pFirstNode = new NODE;

	pFirstNode->iIndex = m_iStartIndex;

	//���� ������ ���� �θ� ����.
	pFirstNode->pParent = NULL;

	pFirstNode->fCost = 0.f;

	//���������� CloseList�� ��� ������ ����.
	m_CloseList.push_back(pFirstNode);


	const vector<TILE*>* pVecTile = m_pBackground->GetTile();

	if(pVecTile == NULL)
		return;

	NODE* pMakeNode = NULL;
	int   iIndex = 0;
	forward_list<int> bestList;

	while(true)
	{
		/*
		 * Ÿ�� �˻� ���� �� ��Ʈ ������ üũ �� ����..
		 * ���� �ϴ� ������ �밢���� ��ġ�� �˻��� �� ���Ǹ� ����
		 * ex) ������ �� Ÿ���� �˻��Ϸ��� �����ʰ� ���� Ÿ���� �� �� �ִ� Ÿ���� ���¿��� �˻縦 �ؾ� �Ѵ�..
		 *											����		��		�Ʒ�		��
		 * 0		0		0		0		0		0		0		0		0
		 */
		BYTE byCheckSuccess = 0;

		//���⼭ ���� Ž���� �����Ѵ�.
		//1.Index�� �ʻ� ����� Index������?
		//2.Ÿ���� �ɼ��� ���� �ִ� Ÿ���ΰ�?
		//3.Open Or Close List�� �ִ� ����ΰ�?

		//##���� Ÿ��..
		iIndex = pFirstNode->iIndex - TILEX;

		if( pFirstNode->iIndex >= TILEX			&&
			(*pVecTile)[iIndex]->byOption == 0  &&
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pFirstNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 0);
		}


		//##�Ʒ��� Ÿ��..
		iIndex = pFirstNode->iIndex + TILEX;

		if( pFirstNode->iIndex < (TILEX * TILEY) - (TILEX) &&
			(*pVecTile)[iIndex]->byOption == 0  && 
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pFirstNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 1);
		}


		//##���� Ÿ��..
		iIndex = pFirstNode->iIndex - 1;

		if( pFirstNode->iIndex % TILEX != 0  &&
			(*pVecTile)[iIndex]->byOption == 0  && 
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pFirstNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 2);
		}


		//##������ Ÿ��..
		iIndex = pFirstNode->iIndex + 1;

		if( pFirstNode->iIndex % TILEX != TILEX - 1  &&
			(*pVecTile)[iIndex]->byOption == 0  && 
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pFirstNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
			byCheckSuccess |= (1 << 3);
		}


		//##������ �� Ÿ��..
		iIndex = pFirstNode->iIndex - (TILEX - 1);
		
		//����ó���� �߰������� �� �ʿ��ϴ�.
		if( pFirstNode->iIndex >= TILEX - 1				&&
			pFirstNode->iIndex % (TILEX) != (TILEX - 1) &&
			byCheckSuccess & (1 << 0)					&&
			byCheckSuccess & (1 << 3)					&&
			(*pVecTile)[iIndex]->byOption == 0			&& 
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pFirstNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
		}
		
		
		//##������ �Ʒ� Ÿ��..
		iIndex = pFirstNode->iIndex + (TILEX + 1);
		
		//����ó���� �߰������� �� �ʿ��ϴ�.
		if( pFirstNode->iIndex < (TILEX * TILEY) - TILEX	&&
			pFirstNode->iIndex % (TILEX) != (TILEX - 1)		&&
			byCheckSuccess & (1 << 1)						&&
			byCheckSuccess & (1 << 3)						&&
			(*pVecTile)[iIndex]->byOption == 0				&& 
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pFirstNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
		}
		
		
		//##���� �Ʒ� Ÿ��..
		iIndex = pFirstNode->iIndex + (TILEX - 1);
		
		//����ó���� �߰������� �� �ʿ��ϴ�.
		if( pFirstNode->iIndex < (TILEX * TILEY) - TILEX	&&
			pFirstNode->iIndex % (TILEX) != 0				&&
			byCheckSuccess & (1 << 1)						&&
			byCheckSuccess & (1 << 2)						&&
			(*pVecTile)[iIndex]->byOption == 0				&& 
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pFirstNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
		}
		
		
		//##���� �� Ÿ��..
		iIndex = pFirstNode->iIndex - (TILEX + 1);
		
		//����ó���� �߰������� �� �ʿ��ϴ�.
		if( pFirstNode->iIndex >= TILEX - 1		&&
			pFirstNode->iIndex % (TILEX) != 0	&&
			byCheckSuccess & (1 << 0)			&&
			byCheckSuccess & (1 << 2)			&&
			(*pVecTile)[iIndex]->byOption == 0  && 
			ListCheck(iIndex)						)
		{
			pMakeNode = MakeNode(iIndex, pFirstNode, pVecTile);
			m_OpenList.push_back(pMakeNode);
		}

		//OpenList�� ��� ����� �����Ѵ�.
		m_OpenList.sort( Compare );

		//���� ���� ����� ���(Ÿ����)
		list<NODE*>::iterator iter = m_OpenList.begin();
		pFirstNode = *iter;

		m_CloseList.push_back(*iter);
		m_OpenList.erase(iter);

		if(pFirstNode->iIndex == m_iEndIndex)
		{
			//BestList�� ����.
			while(true)
			{
				bestList.push_front( pFirstNode->iIndex );

				//���������������� ������������ Node...
				pFirstNode = pFirstNode->pParent;

				if(pFirstNode->iIndex == m_iStartIndex)
					break;
			}

			//���Ҹ� ������Ų��.
			//bestList.reverse();
			break;
		}

	}//While�� ��.

	D3DXVECTOR3 vPlusPos( TILECX * 0.5f, TILECY * 0.5f, 0.f );

	for each (auto iter in bestList)
	{
		vecGetData.push_back( (*pVecTile)[iter]->vPos + vPlusPos );
	}

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

	//�÷��̾� ��ġ���� ������ ����� �Ÿ�.
	float fPlayerCost = D3DXVec3Length(&fDist);

	//����Ÿ�Ͽ������� �������������� �Ÿ�.
	fDist = (*pTile)[iIndex]->vPos - (*pTile)[m_iEndIndex]->vPos;

	float fEndCost = D3DXVec3Length(&fDist);

	//�÷��̾���ġ���� �������������� �Ÿ�.
	pNode->fCost = fPlayerCost + fEndCost;

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
