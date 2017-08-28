#include "stdafx.h"
#include "AStarManager.h"

#include "Astar.h"
#include "Move.h"

IMPLEMENT_SINGLETON(CAStarManager)


CAStarManager::CAStarManager()
{
}


CAStarManager::~CAStarManager()
{
}

int CAStarManager::AddAStar( CMove * _pMoveComponent, CAStar * _pAStarComponent, const D3DXVECTOR3 & _vStartPos, 
							  const D3DXVECTOR3 & _vEndPos, vector<D3DXVECTOR3>& vecGetData, const bool & _bCheckEntityTile )
{
	if ( _pMoveComponent == nullptr ||
		 _pAStarComponent == nullptr )
		return -1;

	int iEvent = _pAStarComponent->AStarSetting( _vStartPos, _vEndPos, vecGetData, _bCheckEntityTile );

	if ( iEvent != CAStar::Event_Can_PathFind )
		return iEvent;

	this->m_PathfindList.push_back( make_pair( _pMoveComponent, _pAStarComponent ) );

	if ( this->m_PathfindList.size() == 1 )
		this->m_CurList_Iter = this->m_PathfindList.begin();

	return CAStar::Event_Can_PathFind;
}

void CAStarManager::EraseAStar( CMove * _pMoveComponent )
{
	for ( auto iter = m_PathfindList.begin(); iter != m_PathfindList.end(); ++iter )
	{
		if ( iter->first == _pMoveComponent )
		{
			m_PathfindList.erase( iter );
			break;
		}
	}
}

void CAStarManager::Update()
{
	if ( this->m_PathfindList.empty() )
		return;
	
	size_t iTempLoopLength = ((MaxUpdateNum < this->m_PathfindList.size()) ? MaxUpdateNum : this->m_PathfindList.size());

	for ( size_t i = 0; i < iTempLoopLength; ++i )
	{
		int iEvent = (*this->m_CurList_Iter).second->PathFind();
		
		if ( iEvent != CAStar::Event_Not_Ended_PathFind )
		{
			D3DXVECTOR3 vDestination = (*this->m_CurList_Iter).second->GetDestination();
			(*this->m_CurList_Iter).first->GetAStarResult( vDestination, iEvent );
			this->m_CurList_Iter = this->m_PathfindList.erase( this->m_CurList_Iter );
		}
		else
			++this->m_CurList_Iter;

		if ( this->m_CurList_Iter == this->m_PathfindList.end() )
		{
			if ( this->m_PathfindList.empty() )
				return;

			this->m_CurList_Iter = this->m_PathfindList.begin();
		}

	}
}

void CAStarManager::Release()
{
	m_PathfindList.clear();
}
