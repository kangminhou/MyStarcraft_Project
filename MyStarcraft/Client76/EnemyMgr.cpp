#include "stdafx.h"
#include "EnemyMgr.h"

#include "TimeMgr.h"
#include "ObjMgr.h"

#include "GameEntity.h"
#include "KeyMgr.h"

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
	m_pObjMgr = CObjMgr::GetInstance();

	for ( int i = 0; i < CEntityMgr::Entity_UnitEnd; ++i )
	{
		m_pAllEntityList[i] = this->m_pEntityMgr->GetEntityList( OBJ_TYPE_USER2, CEntityMgr::eEntityType( i ) );
	}

	D3DXVECTOR3 vStart( 0.f, 0.f, 0.f );
	int iLength = 0;

	vStart = D3DXVECTOR3( 800.f, TILECY * TILEY - 1000.f, 0.f );
	
	iLength = 1;
	for ( int i = 0; i < iLength; ++i )
	{
		for ( int j = 0; j < iLength; ++j )
		{
			CGameEntity* pEntity = this->m_pEntityMgr->MakeUnit( CEntityMgr::Entity_Vulture,
																 vStart + D3DXVECTOR3( j * 50.f, i * 50.f, 0.f ), OBJ_TYPE_USER2 );

			this->AddEntityToLevel( 0, pEntity );
		}
	}

	vStart = D3DXVECTOR3( 672.f, 1600.f, 0.f );

	iLength = 24;
	for ( int i = 0; i < iLength; ++i )
	{
		CGameEntity* pEntity = this->m_pEntityMgr->MakeUnit( CEntityMgr::Entity_Vulture,
															 vStart + D3DXVECTOR3( i % 7 * 100.f, i / 7 * 100.f, 0.f ), OBJ_TYPE_USER2 );

		this->AddEntityToLevel( 1, pEntity );
	}

	vStart = D3DXVECTOR3( 2500.f, 2050.f, 0.f );
	iLength = 25;
	for ( int i = 0; i < iLength; ++i )
	{
		CGameEntity* pEntity = this->m_pEntityMgr->MakeUnit( CEntityMgr::Entity_Marine,
															 vStart + D3DXVECTOR3( (i % 5) * 50.f, (i / 5) * 50.f, 0.f ), OBJ_TYPE_USER2 );

		this->AddEntityToLevel( 2, pEntity );
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
			if ( CKeyMgr::GetInstance()->GetKeyStayDown( VK_CONTROL ) )
			{
				for ( BYTE i = VK_F1; i <= VK_F12; ++i )
				{
					if ( CKeyMgr::GetInstance()->GetKeyOnceDown( i ) )
					{
						BYTE byIndex = i - VK_F1;

						if ( !this->m_vecRushData[byIndex].bRush )
						{
							CGameEntity* pEntity = nullptr;
							for ( int j = 0; j < this->m_vecRushData[byIndex].vecEntity.size(); ++j )
							{
								pEntity = this->m_vecRushData[byIndex].vecEntity[j];
								pEntity->SetDestination( D3DXVECTOR3( 700.f, 2500.f, 0.f ) );
								pEntity->SetPattern( CGameEntity::Pattern_MoveAlert );

								this->m_pObjMgr->AddGameObject( pEntity, OBJ_TYPE_USER2 );
							}

							this->m_vecRushData[byIndex].bRush = true;
						}

					}
				}
			}
		}
			break;
	}

}

void CEnemyMgr::Release()
{
}

void CEnemyMgr::AddEntityToLevel( const int & _iLevel, CGameEntity * _pEntity )
{
	if ( this->m_vecRushData.size() <= _iLevel )
	{
		this->m_vecRushData.resize( _iLevel + 1 );
	}

	this->m_vecRushData[_iLevel].vecEntity.push_back( _pEntity );
}
