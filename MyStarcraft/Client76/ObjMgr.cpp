#include "StdAfx.h"
#include "ObjMgr.h"
#include "GameObject.h"

#include "GameEntity.h"
#include "Background.h"

IMPLEMENT_SINGLETON(CObjMgr);

void CObjMgr::AddGameObject(CGameObject* pObject, eObjectType eType)
{
	pObject->SetObjectType( eType );
	InsertEntitySpaceData( dynamic_cast<CGameEntity*>(pObject) );
	m_ObjList[eType].push_back(pObject);
}

void CObjMgr::InsertEntitySpaceData( CGameEntity* _pGameEntity )
{
	int iReturnSpaceData = -1;

	/* 오브젝트가 빈 오브젝트라면 ( 예외처리.. ) */
	if ( !_pGameEntity )
		return;
	else if ( _pGameEntity->GetObjectType() < 0 || _pGameEntity->GetObjectType() >= OBJ_TYPE_MAX )
	{
		_pGameEntity->SetEntitySpaceDataKey( iReturnSpaceData );
		return;
	}

	int iEntityObjectType = _pGameEntity->GetObjectType();
	int iCurSpaceIndex = this->GetSpaceIndex( _pGameEntity->GetPos() );

	/* 검사할 인덱스나 현재 인덱스가 리스트의 크기를 벗어났다면 ( 예외처리.. ) */
	if ( iCurSpaceIndex < 0 || iCurSpaceIndex > TOTAL_SPACE_NUM )
	{
		_pGameEntity->SetEntitySpaceDataKey( iReturnSpaceData );
		//ERROR_MSG( L"CObjMgr Class - InsertEntitySpaceData Function" );
		return;
	}

	for ( int i = 0; i < EntitySpaceArrSize; ++i )
	{
		CGameEntity*& pCheckEntity = this->m_EntitySpaceArr[iEntityObjectType][iCurSpaceIndex][i];
		if ( !pCheckEntity )
		{
			pCheckEntity = _pGameEntity;
			iReturnSpaceData = iCurSpaceIndex << 16;
			iReturnSpaceData += i;
			break;
		}
	}

	_pGameEntity->SetEntitySpaceDataKey( iReturnSpaceData );
}

void CObjMgr::EraseEntitySpaceData( CGameEntity * _pGameEntity, const int & iSpaceKey )
{
	if ( iSpaceKey == -1 )
		return;

	int iIndex = iSpaceKey >> 16;
	int iEntityArrIndex = ((iSpaceKey << 16) >> 16);
	int iObjectType = _pGameEntity->GetObjectType();

	/* 오브젝트가 빈 오브젝트라면 or Index 가 배열 범위 밖에 있다면 ( 예외처리.. ) */
	if ( !_pGameEntity || iObjectType < 0 || iObjectType >= OBJ_TYPE_MAX ||
		 iIndex < 0 || iIndex >= TOTAL_SPACE_NUM || iEntityArrIndex < 0 || iEntityArrIndex >= EntitySpaceArrSize )
		return;

	if ( _pGameEntity == m_EntitySpaceArr[iObjectType][iIndex][iEntityArrIndex] )
	{
		m_EntitySpaceArr[iObjectType][iIndex][iEntityArrIndex] = NULL;

		CGameEntity* pCheckGameEntity = NULL;
		CGameEntity* pNextCheckGameEntity = NULL;
		for ( int i = iEntityArrIndex; i < EntitySpaceArrSize - 1; ++i )
		{
			pCheckGameEntity = m_EntitySpaceArr[iObjectType][iIndex][i];
			pNextCheckGameEntity = m_EntitySpaceArr[iObjectType][iIndex][i + 1];
			if ( pCheckGameEntity )
			{
				if ( pNextCheckGameEntity )
					pCheckGameEntity = pNextCheckGameEntity;
				else
					pCheckGameEntity = NULL;
			}
			else
				break;
		}

		_pGameEntity->SetEntitySpaceDataKey( -1 );
	}
	else
	{
		//ERROR_MSG( L"Error - CObjMgr Class - EraseEntitySpaceData Function" );
		return;
	}
}

//void CObjMgr::ReAdjustmentSpace( const D3DXVECTOR3 & _vPrevPos, CGameObject * _pGameObject, const bool& _bFirst /*= false*/ )
//{
//	CGameEntity* pGameEntity = dynamic_cast<CGameEntity*>(_pGameObject);
//
//	/* 오브젝트가 빈 오브젝트라면 ( 예외처리.. ) */
//	if ( !pGameEntity || pGameEntity->GetObjectType() < 0 || pGameEntity->GetObjectType() >= OBJ_TYPE_MAX )
//		return;
//
//	int iPrevSpaceIndex = this->GetSpaceIndex( _vPrevPos );
//	int iCurSpaceIndex = this->GetSpaceIndex( pGameEntity->GetPos() );
//
//	if ( !_bFirst )
//	{
//		/* 현재 있는 공간과 전에 있던 공간이 같을 경우 ( 예외처리.. ) */
//		if ( iPrevSpaceIndex == iCurSpaceIndex )
//			return;
//
//		/* 검사할 인덱스나 현재 인덱스가 리스트의 크기를 벗어났다면 ( 예외처리.. ) */
//		if ( iPrevSpaceIndex < 0 || iPrevSpaceIndex > TOTAL_SPACE_NUM ||
//			 iCurSpaceIndex < 0 || iCurSpaceIndex > TOTAL_SPACE_NUM )
//			return;
//
//		list<CGameEntity*>& curCheckList = this->m_EntitySpaceList[pGameEntity->GetObjectType()][iPrevSpaceIndex];
//
//		/* 전에 있던 공간리스트에서 삭제.. */
//		auto iter_end = curCheckList.end();
//		for ( auto iter = curCheckList.begin(); iter != iter_end; ++iter )
//		{
//			if ( (*iter) == pGameEntity )
//			{
//				curCheckList.erase( iter );
//				break;
//			}
//		}
//	}
//	else
//	{
//		//if ( !m_pBackground )
//		//	m_pBackground = (CBackground*)m_ObjList[OBJ_TYPE_BACKGROUND].front();
//		//
//		//m_pBackground->UpdateUnitData( pGameEntity );
//	}
//
//	/* 현재 자신이 있는 공간리스트에 새로 삽입.. */
//	list<CGameEntity*>& curInsertList = this->m_EntitySpaceList[pGameEntity->GetObjectType()][iCurSpaceIndex];
//
//	curInsertList.push_back( pGameEntity );
//}
//
//void CObjMgr::EraseEntitySpaceData( CGameEntity * _pEntity )
//{
//	/* 오브젝트가 빈 오브젝트라면 ( 예외처리.. ) */
//	if ( !_pEntity || _pEntity->GetObjectType() < 0 || _pEntity->GetObjectType() >= OBJ_TYPE_MAX )
//		return;
//
//	int iCurSpaceIndex = this->GetSpaceIndex( _pEntity->GetPos() );
//
//	/* 검사할 인덱스나 현재 인덱스가 리스트의 크기를 벗어났다면 ( 예외처리.. ) */
//	if ( iCurSpaceIndex < 0 || iCurSpaceIndex > TOTAL_SPACE_NUM )
//		return;
//
//	list<CGameEntity*>& curCheckList = this->m_EntitySpaceList[_pEntity->GetObjectType()][iCurSpaceIndex];
//
//	/* 전에 있던 공간리스트에서 삭제.. */
//	auto iter_end = curCheckList.end();
//	for ( auto iter = curCheckList.begin(); iter != iter_end; ++iter )
//	{
//		if ( (*iter) == _pEntity )
//		{
//			curCheckList.erase( iter );
//			break;
//		}
//	}
//}

bool CObjMgr::CheckEntitysCol( vector<CGameEntity*>* _pOut, const CGameEntity* _pGameEntity, int iVecLimitSize )
{
	bool bFind = false;

	for ( int k = OBJ_TYPE_USER; k <= OBJ_TYPE_USER2; ++k )
	{
		RECT _rcCol = _pGameEntity->GetColRect();

		int iStartX = int( _rcCol.left ) / SPACECX;
		int iEndX = int( _rcCol.right + (SPACECX - 1) ) / SPACECX;

		int iStartY = int( _rcCol.top ) / SPACECY;
		int iEndY = int( _rcCol.bottom + (SPACECY - 1) ) / SPACECY;

		for ( int i = iStartY; i < iEndY; ++i )
		{
			for ( int j = iStartX; j < iEndX; ++j )
			{
				int iIndex = j + i * SPACEX;

				if ( i < 0 || i >= SPACEY ||
					 j < 0 || j >= SPACEX )
					continue;

				CGameEntity* pCheckEntity = NULL;

				for ( int l = 0; l < EntitySpaceArrSize; ++l )
				{
					pCheckEntity = this->m_EntitySpaceArr[k][iIndex][l];

					if ( !pCheckEntity || pCheckEntity->GetIsDie() )
						break;
						
					if ( pCheckEntity== _pGameEntity )
						continue;

					RECT rcCheckEntity = pCheckEntity->GetColRect();
					RECT rc = { 0, 0, 0, 0 };

					if ( IntersectRect( &rc, &rcCheckEntity, &_rcCol ) )
					{
						if ( iVecLimitSize == 0 )
							return true;
						if ( pCheckEntity && _pOut )
						{
							_pOut->push_back( pCheckEntity );
							iVecLimitSize--;
						}
					}

				}
			}
		}
	}
	
	return bFind;
}

bool CObjMgr::CheckNearEntitys( vector<CGameEntity*>* _pOut, const CGameEntity * _pGameEntity, eObjectType _eCheckID, int iVecLimitSize )
{
	return this->CheckNearEntitys( _pOut, _pGameEntity, _pGameEntity->GetScope() * Object_Scope_Mul, _eCheckID, iVecLimitSize );
}

bool CObjMgr::CheckNearEntitys( vector<CGameEntity*>* _pOut, const CGameEntity * _pGameEntity, const float & _fCheckRadius, const eObjectType & _eCheckID, int iVecLimitSize )
{
	bool bFind = false;

	D3DXVECTOR3 vEntityPos = _pGameEntity->GetPos();

	float fScope = _fCheckRadius;

	RECT _rcCol = { vEntityPos.x - fScope * 1.f, vEntityPos.y - fScope * 1.f, vEntityPos.x + fScope * 1.f, vEntityPos.y + fScope * 1.f };

	int iStartX = int( _rcCol.left ) / SPACECX;
	int iEndX = int( _rcCol.right + (SPACECX - 1) ) / SPACECX;

	int iStartY = int( _rcCol.top ) / SPACECY;
	int iEndY = int( _rcCol.bottom + (SPACECY - 1) ) / SPACECY;

	for ( int i = iStartY; i <= iEndY; ++i )
	{
		for ( int j = iStartX; j <= iEndX; ++j )
		{
			int iIndex = j + i * SPACEX;

			if ( i < 0 || i >= SPACEY ||
				 j < 0 || j >= SPACEX )
				continue;

			for ( int k = 0; k < EntitySpaceArrSize; ++k )
			{
				CGameEntity* pCheckEntity = this->m_EntitySpaceArr[_eCheckID][iIndex][k];

				if ( !pCheckEntity )
					break;
				else if ( pCheckEntity->GetIsDie() )
				{
					EraseEntitySpaceData( pCheckEntity, pCheckEntity->GetEntitySpaceDataKey() );
					continue;
				}

				if ( pCheckEntity == _pGameEntity )
					continue;

				D3DXVECTOR3 vCheckEntityPos = pCheckEntity->GetPos();

				if ( vCheckEntityPos.x >= _rcCol.left && vCheckEntityPos.x <= _rcCol.right &&
					 vCheckEntityPos.y >= _rcCol.top && vCheckEntityPos.y <= _rcCol.bottom )
				{
					if ( !bFind )
						bFind = true;

					if ( iVecLimitSize == 0 || !_pOut )
						return true;
					if ( pCheckEntity && _pOut )
					{
						_pOut->push_back( pCheckEntity );
						iVecLimitSize--;
					}
				}
			}

			//for each (auto iter in checkList)
			//{
			//	if ( iter == _pGameEntity )
			//		continue;
			//
			//	D3DXVECTOR3 vCheckEntityPos = iter->GetPos();
			//
			//	if ( vCheckEntityPos.x >= _rcCol.left && vCheckEntityPos.x <= _rcCol.right &&
			//		 vCheckEntityPos.y >= _rcCol.top && vCheckEntityPos.y <= _rcCol.bottom )
			//	{
			//		if ( !bFind )
			//			bFind = true;
			//
			//		if ( iVecLimitSize == 0 || !_pOut )
			//			return true;
			//		if ( iter && _pOut )
			//		{
			//			_pOut->push_back( iter );
			//			iVecLimitSize--;
			//		}
			//	}
			//
			//}

		}
	}

	return bFind;
}

bool CObjMgr::CheckDragEntitys( vector<CGameEntity*>& _vecOut, const MOUSE_DRAG_DATA & _tDragData, const eObjectType & eID )
{
	bool bFind = false;

	/* 마우스 드래그 범위 내의 공간 범위 정하기.. */
	int iStartX = int( _tDragData.vStartPos.x ) / SPACECX;
	int iEndX = int( _tDragData.vEndPos.x + (SPACECX - 1) ) / SPACECX;
	
	int iStartY = int( _tDragData.vStartPos.y ) / SPACECY;
	int iEndY = int( _tDragData.vEndPos.y + (SPACECY - 1) ) / SPACECY;
	
	RECT rcMouse = { (LONG)_tDragData.vStartPos.x, (LONG)_tDragData.vStartPos.y, (LONG)_tDragData.vEndPos.x, (LONG)_tDragData.vEndPos.y };
	
	/* 마우스 클릭일 경우의 예외 처리.. */
	if ( rcMouse.left == rcMouse.right )
		++rcMouse.right;
	if ( rcMouse.top == rcMouse.bottom )
		++rcMouse.bottom;
	
	/* 드래그 범위 내의 공간 리스트들 가져와서 검사.. */
	for ( int i = iStartY; i < iEndY; ++i )
	{
		for ( int j = iStartX; j < iEndX; ++j )
		{
			int iIndex = j + i * SPACEX;

			CGameEntity* pCheckEntity = NULL;

			for ( int k = 0; k < EntitySpaceArrSize; ++k )
			{
				pCheckEntity = this->m_EntitySpaceArr[eID][iIndex][k];

				if ( !pCheckEntity )
					break;

				RECT rcCheckEntity = pCheckEntity->GetColRect();
				RECT rc = { 0, 0, 0, 0 };
				
				if ( IntersectRect( &rc, &rcCheckEntity, &rcMouse ) )
				{
					_vecOut.push_back( pCheckEntity );
					bFind = true;
					if ( _vecOut.size() == 12 )
						return true;
				}
			}
	
			//for each (auto iter in this->m_EntitySpaceArr[eID][iIndex])
			//{
			//	RECT rcCheckEntity = iter->GetColRect();
			//	RECT rc = { 0, 0, 0, 0 };
			//
			//	if ( IntersectRect( &rc, &rcCheckEntity, &rcMouse ) )
			//	{
			//		CGameEntity* pPushEntity = dynamic_cast<CGameEntity*>(iter);
			//		if ( pPushEntity )
			//		{
			//			_vecOut.push_back( pPushEntity );
			//			bFind = true;
			//			//if ( _vecOut.size() == 12 )
			//			//	return true;
			//		}
			//	}
			//
			//}
		}
	}

	/* 시간 계산 ( 마린 50000개 기준 : 공간분한 0ms, 단순 for문 15ms.. */
	/* 공간 분할.. */
	//WORD dwResult1, dwResult3, dwResult2, dwResult4;
	//int cnt2 = 0;
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
	//			for each (auto iter in this->m_EntitySpaceList[eID][iIndex])
	//			{
	//				++cnt2;
	//				RECT rcCheckEntity = iter->GetColRect();
	//				RECT rc = { 0, 0, 0, 0 };
	//
	//				if ( IntersectRect( &rc, &rcCheckEntity, &rcMouse ) )
	//				{
	//					if ( iter )
	//					{
	//						_vecOut.push_back( iter );
	//						bFind = true;
	//						if ( _vecOut.size() == 12 )
	//							break;
	//					}
	//				}
	//
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
	//list<CGameObject*>* pCheckList = &this->m_ObjList[eID];
	//
	//GetLocalTime(&cur_time);

	/* 그냥 단순 for문.. */
	//int cnt = 0;
	//for ( int k = 0; k < 1; ++k )
	//{
	//	for each (auto iter in (*pCheckList))
	//	{
	//		++cnt;
	//		CGameEntity* pPushEntity = (CGameEntity*)(iter);
	//		if (pPushEntity)
	//		{
	//			RECT rcCheckEntity = pPushEntity->GetColRect();
	//			RECT rc = { 0, 0, 0, 0 };
	//
	//			if (IntersectRect(&rc, &rcCheckEntity, &rcMouse))
	//			{
	//				_vecOut.push_back(pPushEntity);
	//				bFind = true;
	//				if (_vecOut.size() == 12)
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
	//
	//int bbbb = cnt2;

	return bFind;
}

list<CGameObject*>* CObjMgr::GetList( eObjectType eType )
{
	return &m_ObjList[eType];
}

HRESULT CObjMgr::Initialize(void)
{
	m_pBackground = NULL;

	for ( int i = 0; i < OBJ_TYPE_MAX; ++i )
	{
		for ( int j = 0; j < TOTAL_SPACE_NUM; ++j )
		{
			for ( int k = 0; k < EntitySpaceArrSize; ++k )
			{
				this->m_EntitySpaceArr[i][j][k] = NULL;
			}
		}
	}

	return S_OK;
}

int CObjMgr::Update(void)
{
	vector<CGameObject*> vecDeleteObject;

	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; )
		{
			if ( !(*iter) )
				continue;

			int iEvent = (*iter)->Update();
			if ( iEvent == Event_DestoryObject )
			{
				vecDeleteObject.push_back( (*iter) );
				iter = m_ObjList[i].erase( iter );
			}
			else if ( iEvent == Event_EraseObjList )
			{
				iter = m_ObjList[i].erase( iter );
			}
			else if ( iEvent == Event_None )
				++iter;
		}
	}
	
	if ( !vecDeleteObject.empty() )
	{
		size_t iLength = vecDeleteObject.size();
		for ( size_t i = 0; i < iLength; ++i )
		{
			safe_delete( vecDeleteObject[i] );
		}

		vecDeleteObject.clear();
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
