#include "StdAfx.h"
#include "ObjMgr.h"
#include "GameObject.h"

#include "GameEntity.h"
#include "Background.h"
#include "Building.h"

IMPLEMENT_SINGLETON(CObjMgr);

void CObjMgr::AddGameObject(CGameObject* pObject, eObjectType eType)
{
	pObject->SetObjectType( eType );
	//InsertEntitySpaceData( dynamic_cast<CGameEntity*>(pObject) );
	m_ObjList[eType].push_back(pObject);
}

void CObjMgr::InsertEntitySpaceData( CGameEntity* _pGameEntity )
{
	int iReturnSpaceData = -1;

	/* ������Ʈ�� �� ������Ʈ��� ( ����ó��.. ) */
	if ( !_pGameEntity )
		return;
	else if ( _pGameEntity->GetObjectType() < 0 || _pGameEntity->GetObjectType() >= OBJ_TYPE_MAX )
	{
		_pGameEntity->SetEntitySpaceDataKey( iReturnSpaceData );
		return;
	}

	int iEntityObjectType = _pGameEntity->GetObjectType();
	int iCurSpaceIndex = this->GetSpaceIndex( _pGameEntity->GetPos() );

	/* �˻��� �ε����� ���� �ε����� ����Ʈ�� ũ�⸦ ����ٸ� ( ����ó��.. ) */
	if ( iCurSpaceIndex < 0 || iCurSpaceIndex > TOTAL_SPACE_NUM )
	{
		_pGameEntity->SetEntitySpaceDataKey( iReturnSpaceData );
		//ERROR_MSG( L"CObjMgr Class - InsertEntitySpaceData Function" );
		return;
	}

#ifdef _DIV_SPACE_LIST
	auto& checkEntityList = this->m_EntitySpaceList[iEntityObjectType][iCurSpaceIndex];
	checkEntityList.push_back( _pGameEntity );
	iReturnSpaceData = iCurSpaceIndex << 16;
	iReturnSpaceData += checkEntityList.size() - 1;
#else
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
#endif

	_pGameEntity->SetEntitySpaceDataKey( iReturnSpaceData );

	//if ( dynamic_cast<CBuilding*>(_pGameEntity) )
	//{
	//	wcout << "Name : " << _pGameEntity->GetObjKey().c_str() << endl;
	//	cout << "SpaceIndex : " << (iReturnSpaceData >> 16) << endl;
	//	cout << "ArrIndex : " << (iReturnSpaceData << 16 >> 16) << endl;
	//}
}

void CObjMgr::EraseEntitySpaceData( CGameEntity * _pGameEntity, const int & iSpaceKey )
{
	if ( iSpaceKey == -1 )
		return;

	int iIndex = iSpaceKey >> 16;
	int iEntityArrIndex = ((iSpaceKey << 16) >> 16);
	int iObjectType = _pGameEntity->GetObjectType();

	/* ������Ʈ�� �� ������Ʈ��� or Index �� �迭 ���� �ۿ� �ִٸ� ( ����ó��.. ) */
	if ( !_pGameEntity || iObjectType < 0 || iObjectType >= OBJ_TYPE_MAX ||
		 iIndex < 0 || iIndex >= TOTAL_SPACE_NUM || iEntityArrIndex < 0 || iEntityArrIndex >= EntitySpaceArrSize )
		return;

#ifdef _DIV_SPACE_LIST
	auto& checkList = this->m_EntitySpaceList[iObjectType][iIndex];

	for(auto iter = checkList.begin(); iter != checkList.end(); ++iter )
	{
		if ( (*iter) == _pGameEntity )
		{
			iter = checkList.erase( iter );
			_pGameEntity->SetEntitySpaceDataKey( -1 );
			return;
		}
	}
#else
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
#endif
	{
		//ERROR_MSG( L"Error - CObjMgr Class - EraseEntitySpaceData Function" );
		return;
	}
}

//void CObjMgr::ReAdjustmentSpace( const D3DXVECTOR3 & _vPrevPos, CGameObject * _pGameObject, const bool& _bFirst /*= false*/ )
//{
//	CGameEntity* pGameEntity = dynamic_cast<CGameEntity*>(_pGameObject);
//
//	/* ������Ʈ�� �� ������Ʈ��� ( ����ó��.. ) */
//	if ( !pGameEntity || pGameEntity->GetObjectType() < 0 || pGameEntity->GetObjectType() >= OBJ_TYPE_MAX )
//		return;
//
//	int iPrevSpaceIndex = this->GetSpaceIndex( _vPrevPos );
//	int iCurSpaceIndex = this->GetSpaceIndex( pGameEntity->GetPos() );
//
//	if ( !_bFirst )
//	{
//		/* ���� �ִ� ������ ���� �ִ� ������ ���� ��� ( ����ó��.. ) */
//		if ( iPrevSpaceIndex == iCurSpaceIndex )
//			return;
//
//		/* �˻��� �ε����� ���� �ε����� ����Ʈ�� ũ�⸦ ����ٸ� ( ����ó��.. ) */
//		if ( iPrevSpaceIndex < 0 || iPrevSpaceIndex > TOTAL_SPACE_NUM ||
//			 iCurSpaceIndex < 0 || iCurSpaceIndex > TOTAL_SPACE_NUM )
//			return;
//
//		list<CGameEntity*>& curCheckList = this->m_EntitySpaceList[pGameEntity->GetObjectType()][iPrevSpaceIndex];
//
//		/* ���� �ִ� ��������Ʈ���� ����.. */
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
//	/* ���� �ڽ��� �ִ� ��������Ʈ�� ���� ����.. */
//	list<CGameEntity*>& curInsertList = this->m_EntitySpaceList[pGameEntity->GetObjectType()][iCurSpaceIndex];
//
//	curInsertList.push_back( pGameEntity );
//}
//
//void CObjMgr::EraseEntitySpaceData( CGameEntity * _pEntity )
//{
//	/* ������Ʈ�� �� ������Ʈ��� ( ����ó��.. ) */
//	if ( !_pEntity || _pEntity->GetObjectType() < 0 || _pEntity->GetObjectType() >= OBJ_TYPE_MAX )
//		return;
//
//	int iCurSpaceIndex = this->GetSpaceIndex( _pEntity->GetPos() );
//
//	/* �˻��� �ε����� ���� �ε����� ����Ʈ�� ũ�⸦ ����ٸ� ( ����ó��.. ) */
//	if ( iCurSpaceIndex < 0 || iCurSpaceIndex > TOTAL_SPACE_NUM )
//		return;
//
//	list<CGameEntity*>& curCheckList = this->m_EntitySpaceList[_pEntity->GetObjectType()][iCurSpaceIndex];
//
//	/* ���� �ִ� ��������Ʈ���� ����.. */
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
		if ( this->CheckEntitysCol( _pOut, _pGameEntity, (eObjectType)k, iVecLimitSize ) && !bFind )
			bFind = true;
	}
	
	return bFind;
}

bool CObjMgr::CheckEntitysCol( vector<CGameEntity*>* _pOut, const CGameEntity * _pGameEntity, eObjectType _eCheckID, int iVecLimitSize )
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

#ifdef _DIV_SPACE_LIST
			for ( auto& iter : this->m_EntitySpaceList[_eCheckID][iIndex] )
			{
				pCheckEntity = iter;
#else
			for ( int l = 0; l < EntitySpaceArrSize; ++l )
			{
				pCheckEntity = this->m_EntitySpaceArr[_eCheckID][iIndex][l];
#endif

				if ( !pCheckEntity || pCheckEntity->GetIsDie() )
					break;

				if ( pCheckEntity== _pGameEntity )
					continue;

				if ( !pCheckEntity->GetIsCheckEntityTile() )
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

	return false;
}

bool CObjMgr::CheckNearEntitys( vector<CGameEntity*>* _pOut, const CGameEntity * _pGameEntity, eObjectType _eCheckID, int iVecLimitSize )
{
	return this->CheckNearEntitys( _pOut, _pGameEntity, _pGameEntity->GetScope() * Object_Scope_Mul, _eCheckID, iVecLimitSize );
}

bool CObjMgr::CheckNearEntitys( vector<CGameEntity*>* _pOut, const D3DXVECTOR3 _vStartPos, const float & _fCheckRadius, const eObjectType & _eCheckID, int iVecLimitSize )
{
	bool bFind = false;

	D3DXVECTOR3 vEntityPos = _vStartPos;

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

#ifdef _DIV_SPACE_LIST
			for ( auto& iter : m_EntitySpaceList[_eCheckID][iIndex] )
			{
				CGameEntity* pCheckEntity = iter;
#else
			for ( int k = 0; k < EntitySpaceArrSize; ++k )
			{
				CGameEntity* pCheckEntity = this->m_EntitySpaceArr[_eCheckID][iIndex][k];
#endif

				if ( !pCheckEntity )
					break;
				else if ( pCheckEntity->GetIsDie() )
				{
					EraseEntitySpaceData( pCheckEntity, pCheckEntity->GetEntitySpaceDataKey() );
					continue;
				}
				else if ( pCheckEntity->GetIsClocking() )
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

		}
	}

	return bFind;
}

bool CObjMgr::CheckNearEntitys( vector<CGameEntity*>* _pOut, const CGameEntity * _pGameEntity, const float & _fCheckRadius, const eObjectType & _eCheckID, int iVecLimitSize )
{
	bool bFind = false;

	D3DXVECTOR3 vEntityPos = _pGameEntity->GetPos();

	float fScope = _fCheckRadius;

	RECT _rcCol = { vEntityPos.x - fScope * 0.5f, vEntityPos.y - fScope * 0.5f, vEntityPos.x + fScope * 0.5f, vEntityPos.y + fScope * 0.5f };

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

#ifdef _DIV_SPACE_LIST
			for ( auto iter : m_EntitySpaceList[_eCheckID][iIndex] )
			{
				CGameEntity* pCheckEntity = iter;
#else
			for ( int k = 0; k < EntitySpaceArrSize; ++k )
			{
				CGameEntity* pCheckEntity = this->m_EntitySpaceArr[_eCheckID][iIndex][k];
#endif

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

	/* ���콺 �巡�� ���� ���� ���� ���� ���ϱ�.. */
	int iStartX = int( _tDragData.vStartPos.x ) / SPACECX;
	int iEndX = int( _tDragData.vEndPos.x + (SPACECX - 1) ) / SPACECX;
	
	int iStartY = int( _tDragData.vStartPos.y ) / SPACECY;
	int iEndY = int( _tDragData.vEndPos.y + (SPACECY - 1) ) / SPACECY;
	
	RECT rcMouse = { (LONG)_tDragData.vStartPos.x, (LONG)_tDragData.vStartPos.y, (LONG)_tDragData.vEndPos.x, (LONG)_tDragData.vEndPos.y };
	
	/* ���콺 Ŭ���� ����� ���� ó��.. */
	if ( rcMouse.left == rcMouse.right )
		++rcMouse.right;
	if ( rcMouse.top == rcMouse.bottom )
		++rcMouse.bottom;
	
	/* �巡�� ���� ���� ���� ����Ʈ�� �����ͼ� �˻�.. */
	for ( int i = iStartY; i <= iEndY; ++i )
	{
		for ( int j = iStartX; j <= iEndX; ++j )
		{
			if ( j < 0 || i < 0 ||
				 j >= TILEX || i >= TILEY )
				continue;

			int iIndex = j + i * SPACEX;

			CGameEntity* pCheckEntity = NULL;

#ifdef _DIV_SPACE_LIST
			for(auto& iter : this->m_EntitySpaceList[eID][iIndex] )
			{
				pCheckEntity = iter;
#else
			for ( int k = 0; k < EntitySpaceArrSize; ++k )
			{
				pCheckEntity = this->m_EntitySpaceArr[eID][iIndex][k];
#endif

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
		}
	}

	return bFind;
}

bool CObjMgr::GetSameEntitys( vector<CGameEntity*>& _vecOut, const CGameEntity* _pGameEntity, const float & _fRadius, const eObjectType & _eID, int iVecLimitSize /*= -1*/ )
{
	D3DXVECTOR3 vEntityPos = _pGameEntity->GetPos();
	RECT _rcCol = { vEntityPos.x - _fRadius * 0.5f, vEntityPos.y - _fRadius * 0.5f,
		vEntityPos.x + _fRadius * 0.5f, vEntityPos.y + _fRadius * 0.5f };

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

#ifdef _DIV_SPACE_LIST
			for ( auto& iter : this->m_EntitySpaceList[_eID][iIndex] )
			{
				pCheckEntity = iter;
#else
			for ( int l = 0; l < EntitySpaceArrSize; ++l )
			{
				pCheckEntity = this->m_EntitySpaceArr[_eCheckID][iIndex][l];
#endif

				if ( !pCheckEntity || pCheckEntity->GetIsDie() )
					break;

				if ( pCheckEntity== _pGameEntity )
					continue;

				if ( typeid(*pCheckEntity) != typeid(*_pGameEntity) )
					continue;

				RECT rcCheckEntity = pCheckEntity->GetColRect();
				RECT rc = { 0, 0, 0, 0 };

				if ( IntersectRect( &rc, &rcCheckEntity, &_rcCol ) )
				{
					if ( iVecLimitSize == 0 )
						return true;
					if ( pCheckEntity )
					{
						_vecOut.push_back( pCheckEntity );
						iVecLimitSize--;
					}
				}

			}

			}
		}

	return false;
}

list<CGameObject*>* CObjMgr::GetList( eObjectType eType )
{
	return &m_ObjList[eType];
}

HRESULT CObjMgr::Initialize(void)
{
	m_pBackground = NULL;

#ifdef _DIV_SPACE_LIST
#else
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
#endif

	return S_OK;
}

int CObjMgr::Update(void)
{
	vector<CGameObject*> vecDeleteObject;

	for ( size_t i = 0; i < OBJ_TYPE_MAX; ++i )
	{
		for ( auto iter = this->m_ObjList[i].begin(); iter != this->m_ObjList[i].end(); )
		{
			if ( !(*iter) )
			{
				iter = m_ObjList[i].erase( iter );
			}

			int iEvent = (*iter)->Update();
			if ( iEvent == Event_DestoryObject )
			{
				//vecDeleteObject.push_back( (*iter) );
				m_vecDeleteObj.push_back( (*iter) );
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
	
	//if ( !vecDeleteObject.empty() )
	//{
	//	size_t iLength = vecDeleteObject.size();
	//	for ( size_t i = 0; i < iLength; ++i )
	//	{
	//		safe_delete( vecDeleteObject[i] );
	//	}
	//
	//	vecDeleteObject.clear();
	//}

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

	for ( size_t i = 0; i < m_vecDeleteObj.size(); ++i )
	{
		safe_delete( m_vecDeleteObj[i] );
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
