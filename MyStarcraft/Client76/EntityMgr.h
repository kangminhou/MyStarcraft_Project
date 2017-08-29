#pragma once
#include "Include.h"

class CGameEntity;
class CPlayer;
class CUpgradeMgr;

class CEntityMgr
{
	DECLARE_SINGLETON(CEntityMgr)

public:
	enum eEntityType
	{
		Entity_Marine,
		Entity_Firebat,
		Entity_Medic,
		Entity_Vulture,
		Entity_Tank,
		Entity_Goliath,
		Entity_SCV,
		Entity_Ghost,

		Entity_UnitEnd,

		Entity_Control = Entity_UnitEnd,
		Entity_SupplyDepot,
		Entity_Refinery,
		Entity_Barracks,
		Entity_Engineering_Bay,
		Entity_Missile_Turret,
		Entity_Academy,
		Entity_Bunker,
		Entity_Factory,
		Entity_Starport,
		Entity_Science_Facility,
		Entity_Armory,
		
		Entity_End,
		Entity_TestUnit,
	};

private:
	pair<BUTTON_DATA*, UNIT_GENERATE_DATA*>		m_pEntityGenDataArr[Entity_End];
	SELECT_UNIT_SHOW_DATA*						m_pEntitySelectShowDataArr[Entity_End];
	vector<BUTTON_DATA*>*						m_pVecEntityActButton[Entity_End];
	map<wstring, BUTTON_DATA*>					m_mapAllEntityActButton;

	list<CGameEntity*>							m_entityListArr[2][Entity_End];

	CPlayer*									m_pPlayer;

public:
	CEntityMgr();
	~CEntityMgr();

public:
	UNIT_GENERATE_DATA GetEntityGenData( const eEntityType& _eEntityType );
	list<CGameEntity*>* GetEntityList( const eObjectType& _eObjType, const eEntityType& _eEntityType );

public:
	void Initialize();
	void Release();

	void BuildBuilding( const eEntityType& _eEntityType );
	void SuccessResearch( const wstring& _wstrBtnName );

public:
	CGameEntity* MakeUnit( const eEntityType& _eEntityType, const D3DXVECTOR3& _vPos, const eObjectType& _eType,
						   const bool& _bFree = false);

};

