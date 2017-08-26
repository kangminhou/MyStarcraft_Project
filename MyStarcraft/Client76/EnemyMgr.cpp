#include "stdafx.h"
#include "EnemyMgr.h"

#include "TimeMgr.h"
#include "ObjMgr.h"

#include "GameEntity.h"

IMPLEMENT_SINGLETON( CEnemyMgr )


CEnemyMgr::CEnemyMgr()
{
}


CEnemyMgr::~CEnemyMgr()
{
	Release();
}

void CEnemyMgr::Initialize()
{
	m_pEntityMgr = CEntityMgr::GetInstance();

	for ( int i = 0; i < CEntityMgr::Entity_UnitEnd; ++i )
	{
		m_pAllEntityList[i] = this->m_pEntityMgr->GetEntityList( OBJ_TYPE_USER2, CEntityMgr::eEntityType( i ) );
	}

	D3DXVECTOR3 vStart( 2528.f, 2016.f, 0.f );

	int iLength = 7;
	for ( int i = 0; i < iLength; ++i )
	{
		for ( int j = 0; j < iLength; ++j )
		{
			CGameEntity* pEntity = this->m_pEntityMgr->MakeUnit( CEntityMgr::Entity_Marine,
																 vStart + D3DXVECTOR3( j * 50.f, i * 50.f, 0.f ), OBJ_TYPE_USER2 );
	
			CObjMgr::GetInstance()->AddGameObject( pEntity, OBJ_TYPE_USER2 );
		}
	}

	vStart = D3DXVECTOR3( 672.f, 1600.f, 0.f );
	iLength = 7;
	for ( int i = 0; i < iLength; ++i )
	{
		for ( int j = 0; j < iLength; ++j )
		{
			CGameEntity* pEntity = this->m_pEntityMgr->MakeUnit( CEntityMgr::Entity_Marine,
																 vStart + D3DXVECTOR3( j * 50.f, i * 50.f, 0.f ), OBJ_TYPE_USER2 );
	
			CObjMgr::GetInstance()->AddGameObject( pEntity, OBJ_TYPE_USER2 );
		}
	}

	this->m_fStartTime = CTimeMgr::GetInstance()->GetGlobalTime();

	this->m_iLevel = 0;
}

void CEnemyMgr::Update()
{
	this->m_fCurTime = CTimeMgr::GetInstance()->GetGlobalTime();

	switch ( m_iLevel )
	{
		case 0:
		{
			if ( this->m_fCurTime - this->m_fStartTime >= 10.f )
			{
				//for ( auto& iter : (*this->m_pAllEntityList)[CEntityMgr::Entity_Marine] )
				//{
				//	iter->SetDestination( D3DXVECTOR3( 700.f, 2500.f, 0.f ) );
				//	iter->SetPattern( CGameEntity::Pattern_MoveAlert );
				//}

				m_iLevel = 1;
			}
		}
			break;
	}

}

void CEnemyMgr::Release()
{
}
