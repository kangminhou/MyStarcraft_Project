#include "stdafx.h"
#include "EntityMgr.h"

#include "Factory.h"

#include "Marine.h"
#include "SCV.h"
#include "Medic.h"
#include "Firebat.h"
#include "Vulture.h"
#include "Tank.h"
#include "Goliath.h"

#include "CommandCenter.h"
#include "Barrack.h"
#include "Academy.h"
#include "Factory_Building.h"
#include "Armory.h"


IMPLEMENT_SINGLETON(CEntityMgr)


CEntityMgr::CEntityMgr()
{
	for ( int i = 0; i < Entity_End; ++i )
	{
		this->m_pEntityGenDataArr[i].second = NULL;
		this->m_pEntitySelectShowDataArr[i] = NULL;
		this->m_pVecEntityActButton[i] = NULL;
	}
}


CEntityMgr::~CEntityMgr()
{
	Release();
}

UNIT_GENERATE_DATA CEntityMgr::GetEntityGenData( const eEntityType & _eEntityType )
{
	return *(this->m_pEntityGenDataArr[_eEntityType].second);
}

void CEntityMgr::Initialize()
{
	/* Entity 객체가 선택했을 때 띄울 정보들.. */
	m_pEntitySelectShowDataArr[Entity_Marine] = new SELECT_UNIT_SHOW_DATA( L"Terran Marine", L"Private" );
	m_pEntitySelectShowDataArr[Entity_Firebat] = new SELECT_UNIT_SHOW_DATA( L"Terran Firebat", L"Corporal" );
	m_pEntitySelectShowDataArr[Entity_Medic] = new SELECT_UNIT_SHOW_DATA( L"Terran Medic", L"1st Lieutenant" );
	m_pEntitySelectShowDataArr[Entity_Vulture] = new SELECT_UNIT_SHOW_DATA( L"Terran Vulture", L"Sergeant" );
	m_pEntitySelectShowDataArr[Entity_Tank] = new SELECT_UNIT_SHOW_DATA( L"Terran Tank", L"Master Sergeant" );
	m_pEntitySelectShowDataArr[Entity_Goliath] = new SELECT_UNIT_SHOW_DATA( L"Terran Goliath", L"First Sergeant" );
	m_pEntitySelectShowDataArr[Entity_SCV] = new SELECT_UNIT_SHOW_DATA( L"Terran SCV", L"Private" );

	m_pEntitySelectShowDataArr[Entity_Control] = new SELECT_UNIT_SHOW_DATA( L"Terran Command Center", L"" );
	m_pEntitySelectShowDataArr[Entity_SupplyDepot] = new SELECT_UNIT_SHOW_DATA( L"Terran Supply Depot", L"" );
	m_pEntitySelectShowDataArr[Entity_Refinery] = new SELECT_UNIT_SHOW_DATA( L"Terran Refinery", L"" );
	m_pEntitySelectShowDataArr[Entity_Barracks] = new SELECT_UNIT_SHOW_DATA( L"Terran Barracks", L"" );
	m_pEntitySelectShowDataArr[Entity_Engineering_Bay] = new SELECT_UNIT_SHOW_DATA( L"Terran Engineering Bay", L"" );
	m_pEntitySelectShowDataArr[Entity_Missile_Turret] = new SELECT_UNIT_SHOW_DATA( L"Terran Missile Turret", L"" );
	m_pEntitySelectShowDataArr[Entity_Academy] = new SELECT_UNIT_SHOW_DATA( L"Terran Academy", L"" );
	m_pEntitySelectShowDataArr[Entity_Bunker] = new SELECT_UNIT_SHOW_DATA( L"Terran Bunker", L"" );

	m_pEntitySelectShowDataArr[Entity_Factory] = new SELECT_UNIT_SHOW_DATA( L"Terran Factory", L"" );
	m_pEntitySelectShowDataArr[Entity_Starport] = new SELECT_UNIT_SHOW_DATA( L"Terran Starport", L"" );
	m_pEntitySelectShowDataArr[Entity_Science_Facility] = new SELECT_UNIT_SHOW_DATA( L"Terran Science Facility", L"" );
	m_pEntitySelectShowDataArr[Entity_Armory] = new SELECT_UNIT_SHOW_DATA( L"Terran Armory", L"" );
	
	/* Entity 객체가 사용할 버튼 변수 생성.. */
	/* 공용 기능 버튼들.. */
	m_mapAllEntityActButton.insert( 
		make_pair( L"Move", new BUTTON_DATA( CGameEntity::Pattern_Move, 228, 'M', 0, false, true ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Stop", new BUTTON_DATA( CGameEntity::Pattern_Stop, 229, 'S', 0, false, true ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Attack", new BUTTON_DATA( CGameEntity::Pattern_MoveAlert, 230, 'A', 0, false, true ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Patrol", new BUTTON_DATA( CGameEntity::Pattern_MoveAlert, 254, 'P', 1, false, true ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Hold", new BUTTON_DATA( CGameEntity::Pattern_MoveAlert, 255, 'H', 1, false, true ) ) );

	/* 유닛의 고유 기능 버튼들.. */
	m_mapAllEntityActButton.insert( 
		make_pair( L"Repair", new BUTTON_DATA( CGameEntity::Pattern_MoveAlert, 232, 'R', 1, false, true ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Gather", new BUTTON_DATA( CGameEntity::Pattern_MoveAlert, 231, 'G', 1, false, true ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Steampack", new BUTTON_DATA( CGameEntity::Skill_SteamPack, 237, 'T', 2, false, false ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Heal", new BUTTON_DATA( CGameEntity::Skill_Heal, 365, 'A', 3, true, true ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Build Structure", new BUTTON_DATA( CGameEntity::Skill_BuildStructure, 234, 'B', 2, true, true ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Build Advanced Structure", new BUTTON_DATA( CGameEntity::Skill_BuildAdvancedStructure, 235, 'V', 2, true, true ) ) );
	
	/* 유닛 생산 관련 버튼.. */
	m_mapAllEntityActButton.insert( 
		make_pair( L"Build SCV", new BUTTON_DATA( CGameEntity::Pattern_Make_Unit, 7, 'S', 0, false, true, CEntityMgr::Entity_SCV ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Train Marine", new BUTTON_DATA( CGameEntity::Pattern_Make_Unit, 0, 'M', 0, false, true, CEntityMgr::Entity_Marine ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Train Firebat", new BUTTON_DATA( CGameEntity::Pattern_Make_Unit, 10, 'F', 0, false, false, CEntityMgr::Entity_Firebat ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Train Medic", new BUTTON_DATA( CGameEntity::Pattern_Make_Unit, 34, 'C', 1, false, false, CEntityMgr::Entity_Medic ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Train Vulture", new BUTTON_DATA( CGameEntity::Pattern_Make_Unit, 2, 'V', 0, false, true, CEntityMgr::Entity_Vulture ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Train Tank", new BUTTON_DATA( CGameEntity::Pattern_Make_Unit, 5, 'T', 0, false, false, CEntityMgr::Entity_Tank ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Train Goliath", new BUTTON_DATA( CGameEntity::Pattern_Make_Unit, 3, 'G', 0, false, false, CEntityMgr::Entity_Goliath ) ) );

	/* 건물 건설 관련 버튼들.. */
	m_mapAllEntityActButton.insert(
		make_pair( L"Build Command Center", new BUTTON_DATA( CGameEntity::Pattern_Build, 106, 'C', 0, false, true, CEntityMgr::Entity_Control ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Supply Depot", new BUTTON_DATA( CGameEntity::Pattern_Build, 109, 'S', 0, false, true, CEntityMgr::Entity_SupplyDepot) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Refinery", new BUTTON_DATA( CGameEntity::Pattern_Build, 110, 'R', 0, false, true, CEntityMgr::Entity_Refinery ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Barracks", new BUTTON_DATA( CGameEntity::Pattern_Build, 111, 'B', 1, false, true, CEntityMgr::Entity_Barracks ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Engineering Bay", new BUTTON_DATA( CGameEntity::Pattern_Build, 122, 'E', 1, false, true, CEntityMgr::Entity_Engineering_Bay ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Missile Turret", new BUTTON_DATA( CGameEntity::Pattern_Build, 124, 'T', 1, false, false, CEntityMgr::Entity_Missile_Turret ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Academy", new BUTTON_DATA( CGameEntity::Pattern_Build, 112, 'A', 2, false, false, CEntityMgr::Entity_Academy) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Bunker", new BUTTON_DATA( CGameEntity::Pattern_Build, 125, 'U', 2, false, false, CEntityMgr::Entity_Bunker ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Factory", new BUTTON_DATA( CGameEntity::Pattern_Build, 113, 'F', 0, false, false, CEntityMgr::Entity_Factory ) ));

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Starport", new BUTTON_DATA( CGameEntity::Pattern_Build, 114, 'S', 0, false, false, CEntityMgr::Entity_Starport ) ));

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Science Facility", new BUTTON_DATA( CGameEntity::Pattern_Build, 116, 'I', 0, false, false, CEntityMgr::Entity_Science_Facility ) ));

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Armory", new BUTTON_DATA( CGameEntity::Pattern_Build, 123, 'A', 1, false, false, CEntityMgr::Entity_Armory ) ) );

	m_mapAllEntityActButton.insert( 
		make_pair( L"Build Cancel2", new BUTTON_DATA( CGameEntity::Pattern_Cancel, 236, VK_ESCAPE, 3, false, true ) ) );

	m_mapAllEntityActButton.insert(
		make_pair( L"Build Cancel", new BUTTON_DATA( CGameEntity::Pattern_Cancel, 236, VK_ESCAPE, 2, false, true ) ) );

	/* Entity 객체의 생성 시 필요한 정보들 초기화.. */
	m_pEntityGenDataArr[Entity_Marine] = make_pair( nullptr, new UNIT_GENERATE_DATA( 15, 50, 0, 1, 1 ) );
	m_pEntityGenDataArr[Entity_Firebat] = make_pair( m_mapAllEntityActButton.find( L"Train Firebat" )->second, new UNIT_GENERATE_DATA( 15, 50, 25, 1, 1 ) );
	m_pEntityGenDataArr[Entity_Firebat].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Academy );
	m_pEntityGenDataArr[Entity_Medic] = make_pair( m_mapAllEntityActButton.find( L"Train Medic" )->second, new UNIT_GENERATE_DATA( 15, 50, 25, 1, 1 ) );
	m_pEntityGenDataArr[Entity_Medic].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Academy );
	m_pEntityGenDataArr[Entity_Vulture] = make_pair( nullptr, new UNIT_GENERATE_DATA( 15, 75, 0, 1, 1 ) );
	m_pEntityGenDataArr[Entity_Tank] = make_pair( m_mapAllEntityActButton.find( L"Train Tank" )->second, new UNIT_GENERATE_DATA( 15, 150, 100, 1, 1 ) );
	m_pEntityGenDataArr[Entity_Tank].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Armory );
	m_pEntityGenDataArr[Entity_Goliath] = make_pair( m_mapAllEntityActButton.find( L"Train Goliath" )->second, new UNIT_GENERATE_DATA( 15, 100, 50, 1, 1 ) );
	m_pEntityGenDataArr[Entity_Goliath].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Armory );
	m_pEntityGenDataArr[Entity_SCV] = make_pair( nullptr, new UNIT_GENERATE_DATA( 15, 50, 0, 1, 1 ) );

	m_pEntityGenDataArr[Entity_Control] = make_pair( nullptr, new UNIT_GENERATE_DATA( 15, 400, 0, 0, 1 ));
	m_pEntityGenDataArr[Entity_SupplyDepot] = make_pair( nullptr, new UNIT_GENERATE_DATA( 5, 100, 0, 0, 1 ));
	m_pEntityGenDataArr[Entity_Refinery] = make_pair( nullptr, new UNIT_GENERATE_DATA( 5, 100, 0, 0, 1 ));
	m_pEntityGenDataArr[Entity_Barracks] = make_pair( nullptr, new UNIT_GENERATE_DATA( 5, 150, 0, 0, 1 ));
	m_pEntityGenDataArr[Entity_Engineering_Bay] = make_pair( nullptr, new UNIT_GENERATE_DATA( 5, 125, 0, 0, 1 ));
	m_pEntityGenDataArr[Entity_Missile_Turret] = make_pair( m_mapAllEntityActButton.find( L"Build Missile Turret" )->second, new UNIT_GENERATE_DATA( 5, 75, 0, 0, 1 ));
	m_pEntityGenDataArr[Entity_Missile_Turret].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Engineering_Bay );
	m_pEntityGenDataArr[Entity_Academy] = make_pair( m_mapAllEntityActButton.find( L"Build Academy" )->second, new UNIT_GENERATE_DATA( 5, 150, 0, 0, 1 ));
	m_pEntityGenDataArr[Entity_Academy].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Barracks );
	m_pEntityGenDataArr[Entity_Bunker] = make_pair( m_mapAllEntityActButton.find( L"Build Bunker" )->second, new UNIT_GENERATE_DATA( 5, 100, 0, 0, 1 ));
	m_pEntityGenDataArr[Entity_Bunker].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Barracks );

	m_pEntityGenDataArr[Entity_Factory] = make_pair( m_mapAllEntityActButton.find(L"Build Factory")->second, new UNIT_GENERATE_DATA( 5, 200, 100, 0, 1 ));
	m_pEntityGenDataArr[Entity_Factory].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Barracks );
	m_pEntityGenDataArr[Entity_Starport] = make_pair( m_mapAllEntityActButton.find(L"Build Starport")->second, new UNIT_GENERATE_DATA( 5, 150, 100, 0, 1 ));
	m_pEntityGenDataArr[Entity_Starport].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Factory );
	m_pEntityGenDataArr[Entity_Science_Facility] = make_pair( m_mapAllEntityActButton.find(L"Build Science Facility")->second, new UNIT_GENERATE_DATA( 5, 100, 150, 0, 1 ));
	m_pEntityGenDataArr[Entity_Science_Facility].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Starport );
	m_pEntityGenDataArr[Entity_Armory] = make_pair( m_mapAllEntityActButton.find(L"Build Armory")->second, new UNIT_GENERATE_DATA( 5, 100, 50, 0, 1 ));
	m_pEntityGenDataArr[Entity_Armory].second->vecRequireBuilding.push_back( CEntityMgr::Entity_Factory );

	for(int i = 0; i < Entity_End; ++i )
		m_pVecEntityActButton[i] = new vector<BUTTON_DATA*>;

	m_pVecEntityActButton[Entity_Marine]->push_back( m_mapAllEntityActButton.find( L"Move" )->second );
	m_pVecEntityActButton[Entity_Marine]->push_back( m_mapAllEntityActButton.find( L"Stop" )->second );
	m_pVecEntityActButton[Entity_Marine]->push_back( m_mapAllEntityActButton.find( L"Attack" )->second );
	m_pVecEntityActButton[Entity_Marine]->push_back( m_mapAllEntityActButton.find( L"Patrol" )->second );
	m_pVecEntityActButton[Entity_Marine]->push_back( m_mapAllEntityActButton.find( L"Hold" )->second );
	m_pVecEntityActButton[Entity_Marine]->push_back( m_mapAllEntityActButton.find( L"Steampack" )->second );

	m_pVecEntityActButton[Entity_Medic]->push_back( m_mapAllEntityActButton.find( L"Move" )->second );
	m_pVecEntityActButton[Entity_Medic]->push_back( m_mapAllEntityActButton.find( L"Stop" )->second );
	m_pVecEntityActButton[Entity_Medic]->push_back( m_mapAllEntityActButton.find( L"Patrol" )->second );
	m_pVecEntityActButton[Entity_Medic]->push_back( m_mapAllEntityActButton.find( L"Hold" )->second );

	m_pVecEntityActButton[Entity_Firebat]->push_back( m_mapAllEntityActButton.find( L"Move" )->second );
	m_pVecEntityActButton[Entity_Firebat]->push_back( m_mapAllEntityActButton.find( L"Stop" )->second );
	m_pVecEntityActButton[Entity_Firebat]->push_back( m_mapAllEntityActButton.find( L"Attack" )->second );
	m_pVecEntityActButton[Entity_Firebat]->push_back( m_mapAllEntityActButton.find( L"Patrol" )->second );
	m_pVecEntityActButton[Entity_Firebat]->push_back( m_mapAllEntityActButton.find( L"Hold" )->second );
	m_pVecEntityActButton[Entity_Firebat]->push_back( m_mapAllEntityActButton.find( L"Steampack" )->second );

	m_pVecEntityActButton[Entity_SCV]->push_back( m_mapAllEntityActButton.find( L"Move" )->second );
	m_pVecEntityActButton[Entity_SCV]->push_back( m_mapAllEntityActButton.find( L"Stop" )->second );
	m_pVecEntityActButton[Entity_SCV]->push_back( m_mapAllEntityActButton.find( L"Attack" )->second );
	m_pVecEntityActButton[Entity_SCV]->push_back( m_mapAllEntityActButton.find( L"Gather" )->second );
	m_pVecEntityActButton[Entity_SCV]->push_back( m_mapAllEntityActButton.find( L"Repair" )->second );
	m_pVecEntityActButton[Entity_SCV]->push_back( m_mapAllEntityActButton.find( L"Build Structure" )->second );
	m_pVecEntityActButton[Entity_SCV]->push_back( m_mapAllEntityActButton.find( L"Build Advanced Structure" )->second );

	m_pVecEntityActButton[Entity_Vulture]->push_back( m_mapAllEntityActButton.find( L"Move" )->second );
	m_pVecEntityActButton[Entity_Vulture]->push_back( m_mapAllEntityActButton.find( L"Stop" )->second );
	m_pVecEntityActButton[Entity_Vulture]->push_back( m_mapAllEntityActButton.find( L"Attack" )->second );
	m_pVecEntityActButton[Entity_Vulture]->push_back( m_mapAllEntityActButton.find( L"Patrol" )->second );
	m_pVecEntityActButton[Entity_Vulture]->push_back( m_mapAllEntityActButton.find( L"Hold" )->second );

	m_pVecEntityActButton[Entity_Tank]->push_back( m_mapAllEntityActButton.find( L"Move" )->second );
	m_pVecEntityActButton[Entity_Tank]->push_back( m_mapAllEntityActButton.find( L"Stop" )->second );
	m_pVecEntityActButton[Entity_Tank]->push_back( m_mapAllEntityActButton.find( L"Attack" )->second );
	m_pVecEntityActButton[Entity_Tank]->push_back( m_mapAllEntityActButton.find( L"Patrol" )->second );
	m_pVecEntityActButton[Entity_Tank]->push_back( m_mapAllEntityActButton.find( L"Hold" )->second );

	m_pVecEntityActButton[Entity_Goliath]->push_back( m_mapAllEntityActButton.find( L"Move" )->second );
	m_pVecEntityActButton[Entity_Goliath]->push_back( m_mapAllEntityActButton.find( L"Stop" )->second );
	m_pVecEntityActButton[Entity_Goliath]->push_back( m_mapAllEntityActButton.find( L"Attack" )->second );
	m_pVecEntityActButton[Entity_Goliath]->push_back( m_mapAllEntityActButton.find( L"Patrol" )->second );
	m_pVecEntityActButton[Entity_Goliath]->push_back( m_mapAllEntityActButton.find( L"Hold" )->second );

	m_pVecEntityActButton[Entity_Control]->push_back( m_mapAllEntityActButton.find( L"Build SCV" )->second );

	m_pVecEntityActButton[Entity_Barracks]->push_back( m_mapAllEntityActButton.find( L"Train Marine" )->second );
	m_pVecEntityActButton[Entity_Barracks]->push_back( m_mapAllEntityActButton.find( L"Train Firebat" )->second );
	m_pVecEntityActButton[Entity_Barracks]->push_back( m_mapAllEntityActButton.find( L"Train Medic" )->second );

	m_pVecEntityActButton[Entity_Factory]->push_back( m_mapAllEntityActButton.find( L"Train Vulture" )->second );
	m_pVecEntityActButton[Entity_Factory]->push_back( m_mapAllEntityActButton.find( L"Train Tank" )->second );
	m_pVecEntityActButton[Entity_Factory]->push_back( m_mapAllEntityActButton.find( L"Train Goliath" )->second );

	/* SCV 에게 줄 건물 초기화.. */
	vector<BUTTON_DATA*>* pVecStructureBtnData = new vector<BUTTON_DATA*>;
	vector<BUTTON_DATA*>* pVecAdvancedStructureBtnData = new vector<BUTTON_DATA*>;
	pVecStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Command Center" )->second );
	pVecStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Supply Depot" )->second );
	pVecStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Refinery" )->second );
	pVecStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Barracks" )->second );
	pVecStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Engineering Bay" )->second );
	pVecStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Missile Turret" )->second );
	pVecStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Academy" )->second );
	pVecStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Bunker" )->second );
	pVecStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Cancel" )->second );

	pVecAdvancedStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Factory" )->second );
	pVecAdvancedStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Starport" )->second );
	pVecAdvancedStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Science Facility" )->second );
	pVecAdvancedStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Armory" )->second );
	pVecAdvancedStructureBtnData->push_back( m_mapAllEntityActButton.find( L"Build Cancel2" )->second );

	CSCV::InitButton( pVecStructureBtnData, pVecAdvancedStructureBtnData );

}

void CEntityMgr::Release()
{
	for ( int i = 0; i < Entity_End; ++i )
	{
		if ( this->m_pEntityGenDataArr[i].second )
			safe_delete( this->m_pEntityGenDataArr[i].second );

		if ( this->m_pEntitySelectShowDataArr[i] )
			safe_delete( this->m_pEntitySelectShowDataArr[i] );

		if ( this->m_pVecEntityActButton[i] )
			safe_delete( this->m_pVecEntityActButton[i] );
	}

	for ( auto iter = this->m_mapAllEntityActButton.begin(); iter != this->m_mapAllEntityActButton.end(); ++iter )
	{
		safe_delete( iter->second );
	}

	this->m_mapAllEntityActButton.clear();

	CSCV::DeleteButton();
}

void CEntityMgr::BuildBuilding( const eEntityType & _eEntityType )
{
	for ( int i = 0; i < Entity_End; ++i )
	{
		vector<int>& vecRequireBuilding = this->m_pEntityGenDataArr[i].second->vecRequireBuilding;
		for ( size_t j = 0; j < vecRequireBuilding.size(); ++j )
		{
			if ( vecRequireBuilding[j] == _eEntityType )
			{
				vecRequireBuilding.erase( 
					vecRequireBuilding.begin() + j );

				if ( vecRequireBuilding.empty() )
				{
					if ( this->m_pEntityGenDataArr[i].first )
					{
						this->m_pEntityGenDataArr[i].first->bCanUse = true;
					}
				}

				break;
			}
		}
	}
}

CGameEntity* CEntityMgr::MakeUnit( const eEntityType& _eEntityType, const D3DXVECTOR3 & _vPos, const eObjectType & _eType )
{
	CGameEntity* pEntity = nullptr;

	switch ( _eEntityType )
	{
		case CEntityMgr::Entity_Marine:
			pEntity = new CMarine;
			break;

		case CEntityMgr::Entity_Firebat:
			pEntity = new CFirebat;
			break;

		case CEntityMgr::Entity_Medic:
			pEntity = new CMedic;
			break;

		case CEntityMgr::Entity_Vulture:
			pEntity = new CVulture;
			break;

		case CEntityMgr::Entity_Tank:
			pEntity = new CTank;
			break;

		case CEntityMgr::Entity_Goliath:
			pEntity = new CGoliath;
			break;

		case CEntityMgr::Entity_SCV:
			pEntity = new CSCV;
			break;

		case CEntityMgr::Entity_Control:
			pEntity = new CCommandCenter;
			break;

		case CEntityMgr::Entity_Barracks:
			pEntity = new CBarrack;
			break;

		case CEntityMgr::Entity_Academy:
			pEntity = new CAcademy;
			break;

		case CEntityMgr::Entity_Factory:
			pEntity = new CFactory_Building;
			break;

		case CEntityMgr::Entity_Armory:
			pEntity = new CArmory;
			break;

	}

	pEntity->SetGenerateData( this->m_pEntityGenDataArr[_eEntityType].second );
	pEntity->SetSelectShowData( this->m_pEntitySelectShowDataArr[_eEntityType] );
	pEntity->SetButtonData( this->m_pVecEntityActButton[_eEntityType] );
	pEntity->Initialize();
	pEntity->SetPos( _vPos );
	pEntity->SetObjectType( _eType );
	pEntity->SetEntityType( _eEntityType );
	pEntity->CollisionCheck();

	return pEntity;
}
