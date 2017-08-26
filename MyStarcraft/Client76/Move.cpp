#include "stdafx.h"
#include "Move.h"

#include "ObjMgr.h"
#include "TimeMgr.h"

#include "Background.h"
#include "Transform.h"
#include "GameEntity.h"
#include "Astar.h"
#include "Corps.h"
#include "Random.h"
#include "AStarManager.h"


CMove::CMove()
	: m_iCurIndexNum(0)
	, m_bWaitAStarResult(false)
	, m_bMoveStop(false)
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
	this->m_pAStar->SetEntity( this->m_pGameEntity );

	this->m_pAStarManager = CAStarManager::GetInstance();
}

void CMove::Release()
{
	safe_delete( this->m_pAStar );
}

int CMove::Update()
{
	float fEntitySpeed = this->m_pGameEntity->GetSpeed() * GET_TIME * 1.4f;
	float fDist = D3DXVec3Length( &(this->m_vDestination - this->m_pGameEntity->GetPos()) );

	/* 더 이상 이동할 경로가 없다 == 도착했다.. */
	if ( ((unsigned int)(this->m_iCurIndexNum) >= this->m_vecMovePath.size() && fDist <= fEntitySpeed) || 
		 this->m_bMoveStop )
	{
		//m_pBackground->UpdateUnitPosData( this->m_pGameEntity);
		return 1;
	}

	if ( m_fRestReFindPath <= 0.f )
	{
		//if ( fDist <= 500.f )
		{
			this->ReFindPath();
			this->m_bCanRestReFindPath = true;
		}
	}
	else
		m_fRestReFindPath -= GET_TIME;

	if ( this->m_pGameEntity->GetIsCollision() && m_bCanRestReFindPath )
	{
		this->ReFindPath();
		m_bCanRestReFindPath = false;
		return 0;
	}

	if ( !this->m_bWaitAStarResult )
	{
		if ( this->m_bCanMove )
		{
			/* 이동할 경로로 이동.. */
			this->m_pGameEntity->LookPos( m_vTilePos );
			this->m_pGameEntity->MoveEntity();

			D3DXVECTOR3 vEntityPos = this->m_pGameEntity->GetPos();
			D3DXVECTOR3 vTilePos = m_vecMovePath[this->m_iCurIndexNum];

			if ( D3DXVec3Length( &(vTilePos - vEntityPos) ) <= fEntitySpeed )
			{
				++m_iCurIndexNum;
				if ( (unsigned int)(this->m_iCurIndexNum) < this->m_vecMovePath.size() )
				{
					this->SettingMoveData();
				}
			}
		}
	}

	return 0;
}

void CMove::SetDestination( const D3DXVECTOR3& _vDestination, const bool& _bChaseTarget /*= false*/ )
{
	//m_pBackground->UpdateUnitPosData( this->m_pGameEntity, true );
	this->m_vDestination = _vDestination;

	if ( !_bChaseTarget )
	{
		this->DecidePathFindMethod();
	}
	else
		this->ReFindPath();
}

void CMove::PathFind()
{
	if ( this->m_bWaitAStarResult )
		return;
	m_vecMovePath.clear();

	/* AStar 로 최단거리 타일 인덱스 찾기.. */
	//int iEndIndex = m_pBackground->GetTileIndex( m_vDestination );

	//this->m_bCanMove = this->m_pAStar->AStarStartPos( this->m_pGameEntity->GetPos(), m_vDestination, this->m_vecMovePath );
	this->m_bCanMove = true;

	bool bCheckEntityTile = this->m_pGameEntity->GetIsCheckEntityTile();
	int iEvent = this->m_pAStarManager->AddAStar( this, this->m_pAStar, this->m_pGameEntity->GetPos(), m_vDestination, this->m_vecMovePath, bCheckEntityTile );
	//int iEvent = this->m_pAStar->AStarStartPos( this->m_pGameEntity->GetPos(), m_vDestination, this->m_vecMovePath, bCheckEntityTile );
	//int iEvent = this->m_pAStar->AStarStartPos( this->m_pGameEntity->GetPos(), m_vDestination, this->m_vecMovePath, bCheckEntityTile );

	switch ( iEvent )
	{
		case CAStar::Event_Fail_PathFind:
			this->m_bCanMove = false;
			//cout << "이동 경로를 못찾음" << endl;
			if ( D3DXVec3Length( &(m_vDestination - this->m_pGameEntity->GetPos()) ) <= TILECX * 2 )
			{
				this->m_bMoveStop = true;
				system( "cls" );
				cout << "Find Path" << endl;
			}
			//else
			//	cout << "Can't Find Path" << endl;

			return;

		case CAStar::Event_Fail_Find_NearIndex:
			//this->m_fRestReFindPath = RANDOM_FLOAT( 0.0f, 0.3f );
			//cout << "도착 타일을 못찾음" << endl;
			this->m_bCanMove = false;
			cout << "Can't Find NearIndex" << endl;
			//cout << "fDist : " << D3DXVec3Length( &(m_vDestination - this->m_pGameEntity->GetPos()) ) << endl;
			if ( D3DXVec3Length( &(m_vDestination - this->m_pGameEntity->GetPos()) ) <= TILECX * 4 )
			{
				this->m_bMoveStop = true;
			}
			return;

		case CAStar::Event_Index_OutRange_TileSize:
			this->m_bCanMove = false;
			system( "cls" );
			cout << "Event_Index_OutRange_TileSize" << endl;
			//ERROR_MSG( L"Event_Index_OutRange_TileSize" );
			return;

		case CAStar::Event_Success_PathFind:
			this->GetAStarResult( this->m_pAStar->GetDestination(), iEvent );
			break;

		case CAStar::Event_Can_PathFind:
			this->m_bWaitAStarResult = true;
			break;

	}

	//if ( this->m_pAStar->AStarStartPos( this->m_pGameEntity->GetPos(), m_vDestination, this->m_vecMovePath ) )
	//{
	//	//ERROR_MSG( L"이동 경로를 못찾음.." );
	//	//system( "cls" );
	//	cout << "이동 경로를 못찾음" << endl;
	//	this->m_fRestReFindPath = RANDOM_FLOAT( 0.0f, 0.3f );
	//}

	//cout << "x : " << m_vDestination.x << ", y : " << m_vDestination.y << endl;
}

void CMove::ReFindPath()
{
	//cout << "경로 재탐색" << endl;
	this->PathFind();
}

void CMove::GetAStarResult( const D3DXVECTOR3 & _vDestination, const int & _iReturnEvent )
{
	this->m_bWaitAStarResult = false;

	if ( _iReturnEvent != CAStar::Event_Success_PathFind )
		return;

	this->m_vecMovePath.push_back( _vDestination );
	this->m_vDestination = _vDestination;

	this->m_iCurIndexNum = 0;
	this->SettingMoveData();

	this->m_fRestReFindPath = RANDOM_RANGE_FLOAT( 5.f, 10.f );

}

void CMove::SettingMoveData()
{
	m_vTilePos = m_vecMovePath[this->m_iCurIndexNum];
}

void CMove::DecidePathFindMethod()
{
	const CCorps* pBelongToCorps = this->m_pGameEntity->GetEntityBelongToCorps();

	if ( pBelongToCorps && pBelongToCorps->GetCurUnitNum() > 1 )
	{
		int iOnePlaceMoveData = pBelongToCorps->CheckCorpsOnePlaceMove( m_vDestination, this->m_pGameEntity );
	
		if ( iOnePlaceMoveData > 0 )
		{
			switch ( iOnePlaceMoveData )
			{
				case 1:
					m_vDestination.x -= TILECX;
					m_vDestination.y -= TILECY;
					break;
	
				case 2:
					m_vDestination.x += TILECX;
					m_vDestination.y -= TILECY;
					break;
	
				case 3:
					m_vDestination.x -= TILECX;
					m_vDestination.y += TILECY;
					break;
	
				case 4:
					m_vDestination.x += TILECX;
					m_vDestination.y += TILECY;
					break;
	
			}
		}
		else
		{
			if ( pBelongToCorps->GetGatherEntitys() )
				this->m_vDestination = this->m_vDestination + (this->m_pGameEntity->GetPos() - pBelongToCorps->GetCenterPos());
			else
				this->m_vDestination = this->m_vDestination + (this->m_pGameEntity->GetPos() - pBelongToCorps->GetCenterPos());
		}
	}

	this->m_vDestination.z = 0.f;
	
	this->PathFind();
}
