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
	this->m_pAStar->SetEntity( this->m_pGameEntity );
}

void CMove::Release()
{
	safe_delete( this->m_pAStar );
}

int CMove::Update()
{
	m_fRestReFindPath -= GET_TIME;

	/* 더 이상 이동할 경로가 없다 == 도착했다.. */
	if ( (unsigned int)(this->m_iCurIndexNum) >= this->m_vecMovePath.size() )
	{
		//m_pBackground->UpdateUnitPosData( this->m_pGameEntity);
		return 1;
	}

	m_fRestReFindPath -= GET_TIME;
	if ( m_fRestReFindPath <= 0.f )
		this->ReFindPath();

	if ( this->m_pGameEntity->GetIsCollision() && m_bCanRestReFindPath )
	{
		this->ReFindPath();
		//m_bCanRestReFindPath = false;
		return 0;
	}

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
	//m_pBackground->UpdateUnitPosData( this->m_pGameEntity, true );
	this->m_vDestination = _vDestination;

	this->DecidePathFindMethod();
}

void CMove::PathFind()
{
	m_vecMovePath.clear();

	/* AStar 로 최단거리 타일 인덱스 찾기.. */
	int iEndIndex = m_pBackground->GetTileIndex( m_vDestination );

	if ( !this->m_pAStar->AStarStartPos( this->m_pGameEntity->GetPos(), m_vDestination, this->m_vecMovePath ) )
	{
		//ERROR_MSG( L"이동 경로를 못찾음.." );
		this->m_fRestReFindPath = RANDOM_FLOAT( 0.2f, 0.5f );
		return;
	}

	this->m_vecMovePath.push_back( m_vDestination );

	this->m_iCurIndexNum = 0;
	this->SettingMoveData();

	this->m_bCanRestReFindPath = true;
	this->m_fRestReFindPath = RANDOM_FLOAT( 5.f, 10.f );
}

void CMove::ReFindPath()
{
	this->PathFind();
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
