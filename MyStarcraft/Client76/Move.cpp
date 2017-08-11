#include "stdafx.h"
#include "Move.h"

#include "ObjMgr.h"
#include "TimeMgr.h"

#include "Background.h"
#include "Transform.h"
#include "GameEntity.h"
#include "Astar.h"


CMove::CMove()
{
}


CMove::~CMove()
{
	Release();
}

void CMove::Initialize()
{
	this->m_pGameEntity = const_cast<CGameEntity*>(dynamic_cast<const CGameEntity*>(this->GetGameObject()));
	this->m_pTransform = const_cast<CTransform*>(this->GetGameObject()->GetTransform());
	this->m_pBackground = CObjMgr::GetInstance()->FindGameObject<CBackground>();

	this->m_pAStar = new CAStar;
	this->m_pAStar->SetBackground( this->m_pBackground );
}

void CMove::Release()
{
	safe_delete( this->m_pAStar );
}

int CMove::Update()
{
	/* 더 이상 이동할 경로가 없다 == 도착했다.. */
	if ( (unsigned int)(this->m_iCurIndexNum) >= this->m_vecMovePath.size() )
		return 1;

	/* 이동할 경로로 이동.. */
	this->m_pGameEntity->LookPos( m_vTilePos );
	this->m_pGameEntity->MoveEntity();

	D3DXVECTOR3 vEntityPos = this->m_pGameEntity->GetPos();
	D3DXVECTOR3 vTilePos = m_vecMovePath[this->m_iCurIndexNum];

	float fEntitySpeed = this->m_pGameEntity->GetSpeed() * GET_TIME;

	if ( D3DXVec3Length( &(vTilePos - vEntityPos) ) <= fEntitySpeed )
	{
		++m_iCurIndexNum;
		if ( (unsigned int)(this->m_iCurIndexNum) < this->m_vecMovePath.size() )
		{
			this->SettingMoveData();
		}
	}

	return 0;
}

void CMove::SetDestination( const D3DXVECTOR3& _vDestination )
{
	if ( true )
	{
		/* AStar 로 최단거리 타일 인덱스 찾기.. */
		int iEndIndex = m_pBackground->GetTileIndex( _vDestination );
		//vector<int> vBestIndex;
		if ( !this->m_pAStar->AStarStartPos( this->m_pGameEntity->GetPos(), _vDestination, this->m_vecMovePath ) )
			return;

		TILE* pEndTile = (*m_pBackground->GetTile())[iEndIndex];

		/* 찾은 타일 인덱스를 좌표로 바꿔서 넣기.. */
		//vector<TILE*>* pVecTempTile = m_pBackground->GetTile();
		//D3DXVECTOR3 vPlusPos( TILECX * 0.5f, TILECY * 0.5f, 0.f );
		//this->m_vecMovePath.clear();
		//
		//for ( size_t i = 0; i < vBestIndex.size() - 1; ++i )
		//	this->m_vecMovePath.push_back( (*pVecTempTile)[vBestIndex[i]]->vPos + vPlusPos );

		if ( pEndTile->byOption != 0 )
			return;

	}

	this->m_vecMovePath.push_back( _vDestination );

	this->m_iCurIndexNum = 0;
	this->SettingMoveData();
}

void CMove::SettingMoveData()
{
	m_vTilePos = m_vecMovePath[this->m_iCurIndexNum];
}
