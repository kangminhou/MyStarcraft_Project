#include "StdAfx.h"
#include "ObjMgr.h"
#include "GameObject.h"

#include "GameEntity.h"

IMPLEMENT_SINGLETON(CObjMgr);

void CObjMgr::AddGameObject(CGameObject* pObject, eObjectType eType)
{
	pObject->SetObjectType( eType );
	this->AdjustmentSpace( pObject );
	m_ObjList[eType].push_back(pObject);
}

void CObjMgr::ReAdjustmentSpace( const D3DXVECTOR3 & _vPrevPos, CGameObject * _pGameObject )
{
	CGameEntity* pGameEntity = dynamic_cast<CGameEntity*>(_pGameObject);

	/* 오브젝트가 빈 오브젝트라면 ( 예외처리.. ) */
	if ( !pGameEntity || pGameEntity->GetObjectType() < 0 || pGameEntity->GetObjectType() >= OBJ_TYPE_MAX )
		return;

	int iPrevSpaceIndex = this->GetSpaceIndex( _vPrevPos );
	int iCurSpaceIndex = this->GetSpaceIndex( pGameEntity->GetPos() );

	/* 현재 있는 공간과 전에 있던 공간이 같을 경우 ( 예외처리.. ) */
	if ( iPrevSpaceIndex == iCurSpaceIndex )
		return;

	/* 검사할 인덱스나 현재 인덱스가 리스트의 크기를 벗어났다면 ( 예외처리.. ) */
	if ( iPrevSpaceIndex < 0 || iPrevSpaceIndex > TOTAL_SPACE_NUM ||
		 iCurSpaceIndex < 0 || iCurSpaceIndex > TOTAL_SPACE_NUM )
		return;

	list<CGameEntity*>& curCheckList = this->m_EntitySpaceList[pGameEntity->GetObjectType()][iPrevSpaceIndex];
	
	/* 전에 있던 공간리스트에서 삭제.. */
	auto iter_end = curCheckList.end();
	for ( auto iter = curCheckList.begin(); iter != iter_end; ++iter )
	{
		if ( (*iter) == pGameEntity )
		{
			curCheckList.erase( iter );
			break;
		}
	}

	/* 현재 자신이 있는 공간리스트에 새로 삽입.. */
	list<CGameEntity*>& curInsertList = this->m_EntitySpaceList[pGameEntity->GetObjectType()][iCurSpaceIndex];

	curInsertList.push_back( pGameEntity );

	static int a = 0;
	++a;
	if ( a == 10 )
	{
		int f = 0;
	}
}

bool CObjMgr::CheckNearEntitys( vector<CGameEntity*>* _pOut, const D3DXVECTOR3 & _vCenter, const D3DXVECTOR3 & _vRange, const eObjectType & eID, int iVecLimitSize )
{
	list<CGameObject*>* pCheckList = &this->m_ObjList[eID];
	bool bFind = false;

	/*## 공간을 분할하여 좀 더 최적화 시킬 것.. */
	for each (auto iter in (*pCheckList))
	{
		D3DXVECTOR3 vCheckObjectPos = iter->GetPos();

		if ( vCheckObjectPos.x >= _vCenter.x - _vRange.x && vCheckObjectPos.y >= _vCenter.y - _vRange.y &&
			 vCheckObjectPos.x <= _vCenter.x + _vRange.x && vCheckObjectPos.y <= _vCenter.y + _vRange.y )
		{
			if ( !_pOut )
				return true;
			else if ( iVecLimitSize == 0 )
				return true;
			else
			{
				CGameEntity* pPushEntity = dynamic_cast<CGameEntity*>(iter);
				if ( pPushEntity )
				{
					_pOut->push_back( pPushEntity );
					--iVecLimitSize;
					bFind = true;
				}
			}
		}
	}

	return bFind;
}

bool CObjMgr::CheckDragEntitys( vector<CGameEntity*>& _vecOut, const MOUSE_DRAG_DATA & _tDragData, const eObjectType & eID )
{
	list<CGameObject*>* pCheckList = &this->m_ObjList[eID];
	bool bFind = false;

	int iStartX = int( _tDragData.vStartPos.x ) / SPACECX;
	int iEndX = int( _tDragData.vEndPos.x + (SPACECX - 1) ) / SPACECX;

	int iStartY = int( _tDragData.vStartPos.y ) / SPACECY;
	int iEndY = int( _tDragData.vEndPos.y + (SPACECY - 1) ) / SPACECY;

	RECT rcMouse = { _tDragData.vStartPos.x, _tDragData.vStartPos.y, _tDragData.vEndPos.x, _tDragData.vEndPos.y };

	if ( rcMouse.left == rcMouse.right )
		++rcMouse.right;
	if ( rcMouse.top == rcMouse.bottom )
		++rcMouse.bottom;

	for ( int i = iStartY; i < iEndY; ++i )
	{
		for ( int j = iStartX; j < iEndX; ++j )
		{
			int iIndex = j + i * SPACEX;

			for each (auto iter in this->m_EntitySpaceList[eID][iIndex])
			{
				RECT rcCheckEntity = iter->GetColRect();
				RECT rc = { 0, 0, 0, 0 };

				//bool bCondition1 = (rcCheckEntity.left >= _tDragData.vStartPos.x && rcCheckEntity.top >= _tDragData.vStartPos.y &&
				//					 rcCheckEntity.right <= _tDragData.vEndPos.x && rcCheckEntity.bottom <= _tDragData.vEndPos.y);
				//
				//bool bCondition2 = (rcCheckEntity.left >= _tDragData.vStartPos.x && rcCheckEntity.top >= _tDragData.vStartPos.y &&
				//					 rcCheckEntity.right <= _tDragData.vEndPos.x && rcCheckEntity.bottom <= _tDragData.vEndPos.y);
				//
				//if ( bCondition1 || bCondition2 )
				if ( IntersectRect( &rc, &rcCheckEntity, &rcMouse ) )
				{
					CGameEntity* pPushEntity = dynamic_cast<CGameEntity*>(iter);
					if ( pPushEntity )
					{
						_vecOut.push_back( pPushEntity );
						bFind = true;
						if ( _vecOut.size() == 12 )
							return true;
					}
				}

			}
		}
	}

	/* 시간 계산 ( 마린 50000개 기준 : 공간분한 0ms, 단순 for문 15ms.. */
	/* 공간 분할.. */
	//WORD dwResult1, dwResult3, dwResult2, dwResult4;
	//
	//SYSTEMTIME cur_time, end_Time;
	//GetLocalTime(&cur_time);
	//
	////dwStart = GetTickCount();
	//for ( int k = 0; k < 1; ++k )
	//{
	//	for ( int i = iStartY; i < iEndY; ++i )
	//	{
	//		for ( int j = iStartX; j < iEndX; ++j )
	//		{
	//			int iIndex = j + i * SPACEX;
	//
	//			for each (auto iter in this->m_ObjSpaceList[eID][iIndex])
	//			{
	//				D3DXVECTOR3 vCheckObjectPos = iter->GetPos();
	//
	//				if ( vCheckObjectPos.x >= _tDragData.vStartPos.x && vCheckObjectPos.y >= _tDragData.vStartPos.y &&
	//					 vCheckObjectPos.x <= _tDragData.vEndPos.x && vCheckObjectPos.y <= _tDragData.vEndPos.y )
	//				{
	//					T* pPushEntity = dynamic_cast<T*>(iter);
	//					if ( pPushEntity )
	//					{
	//						_vecOut.push_back( pPushEntity );
	//						bFind = true;
	//						if ( _vecOut.size() == 12 )
	//							break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//
	//	_vecOut.clear();
	//}
	////dwEnd = GetTickCount();
	//GetLocalTime(&end_Time);
	//dwResult1 = end_Time.wSecond - cur_time.wSecond;
	//dwResult3 = end_Time.wMilliseconds - cur_time.wMilliseconds;
	//
	//GetLocalTime(&cur_time);

	/* 그냥 단순 for문.. */
	//for ( int k = 0; k < 1; ++k )
	//{
	//	for each (auto iter in (*pCheckList))
	//	{
	//		D3DXVECTOR3 vCheckObjectPos = iter->GetPos();
	//
	//		if ( vCheckObjectPos.x >= _tDragData.vStartPos.x && vCheckObjectPos.y >= _tDragData.vStartPos.y &&
	//			 vCheckObjectPos.x <= _tDragData.vEndPos.x && vCheckObjectPos.y <= _tDragData.vEndPos.y )
	//		{
	//			T* pPushEntity = dynamic_cast<T*>(iter);
	//			if ( pPushEntity )
	//			{
	//				_vecOut.push_back( pPushEntity );
	//				bFind = true;
	//				if ( _vecOut.size() == 12 )
	//					break;
	//			}
	//		}
	//	}
	//	_vecOut.clear();
	//}
	////dwEnd = GetTickCount();
	////dwResult2 = dwEnd - dwStart;
	//
	//GetLocalTime(&end_Time);
	//dwResult2 = end_Time.wSecond - cur_time.wSecond;
	//dwResult4 = end_Time.wMilliseconds - cur_time.wMilliseconds;

	return bFind;
}

list<CGameObject*>* CObjMgr::GetList( eObjectType eType )
{
	return &m_ObjList[eType];
}

HRESULT CObjMgr::Initialize(void)
{
	return S_OK;
}

int CObjMgr::Update(void)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; )
		{
			if( (*iter)->Update() == 1 )
			{
				safe_delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return 0;
}

void CObjMgr::Render(void)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}
}

void CObjMgr::Release(void)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			safe_delete(*iter);
		}
		m_ObjList[i].clear();
	}
}

void CObjMgr::Release(eObjectType eType)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		if(eType == i)
			continue;

		for(iter; iter != iter_end; ++iter)
		{
			safe_delete(*iter);
		}
		m_ObjList[i].clear();
	}
}

void CObjMgr::AdjustmentSpace( CGameObject * _pGameObject )
{
	CGameEntity* pGameEntity = dynamic_cast<CGameEntity*>(_pGameObject);

	if ( !pGameEntity )
		return;

	int iCurSpaceIndex = this->GetSpaceIndex( pGameEntity->GetPos() );
	if ( iCurSpaceIndex < 0 || iCurSpaceIndex > TOTAL_SPACE_NUM )
		return;
	list<CGameEntity*>& curInsertList = this->m_EntitySpaceList[pGameEntity->GetObjectType()][iCurSpaceIndex];

	curInsertList.push_back( pGameEntity );
}

int CObjMgr::GetSpaceIndex( const D3DXVECTOR3 & _vPos )
{
	//int iOut = (_vPos.x / SPACECX) + ((_vPos.y / SPACECY) * SPACEX);

	return int(_vPos.x / SPACECX) + (int(_vPos.y / SPACECY) * SPACEX);
}

CObjMgr::CObjMgr(void)
{
}

CObjMgr::~CObjMgr(void)
{
	Release();
}
