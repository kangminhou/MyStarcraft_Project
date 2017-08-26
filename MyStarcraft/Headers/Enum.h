#pragma once

//
//enum eOBJECT_TYPE
enum eObjectType
{
	//eObjectType.
	OBJ_TYPE_BACKGROUND,
	OBJ_TYPE_RESOURCE,
	OBJ_TYPE_USER,
	OBJ_TYPE_USER2,
	OBJ_TYPE_EFFECT,
	OBJ_TYPE_MAX,
	OBJ_TYPE_USER_MAX = 2
};

enum eTextureType
{
	TEX_SINGLE,
	TEX_MULTI,
	TEX_MAX,
};


enum eItemType
{
	ITEM_TYPE_WEAPON = 1 << 0,
	ITEM_TYPE_ARMOR  = 1 << 1,
	ITEM_TYPE_POTION = 1 << 2,
};

enum eSceneType
{
	SCENE_LOGO,
	SCENE_STAGE,
	SCENE_TEST,
	SCENE_FILECOPY,
	SCENE_ASTAR_TEST,
	SCENE_Ray_TEST,
	SCENE_MAX,
};

enum eWeaponUpgradeType
{
	Upgrade_Terran_Infantry_Weapons,
	Upgrade_Terran_Vehicle_Weapons,
	Upgrade_Weapon_End
};

enum eArmorUpgradeType
{
	Upgrade_Terran_Infantry_Armor,
	Upgrade_Armor_End
};

enum eButtonKind
{
	Button_Act_Pattern,
	Button_Act_Skill,
	Button_Act_Upgrade,
	Button_Act_Research,
};

enum eGameObjectEvent
{
	Event_None,
	Event_DestoryObject,
	Event_EraseObjList,
};

enum eWeaponAttType
{
	Weapon_Normal,
	Weapon_Splash_Noraml,
	Weapon_Splash_Circle,
	Weapon_LockDown
};

enum eEffectShowKind
{
	Effect_Show_None,						// ����Ʈ �� ���..
	Effect_Show_Owner_Position,				// ����Ʈ�� ������ ��ü �ʿ��� ���..
	Effect_Show_Target_Position,			// ����Ʈ�� ���� ���� ��ü �ʿ��� ���..
	Effect_Show_Hit_Target,					// ����Ʈ�� ���� ���� ��ü�� ��ǥ�� ���ư� ( �߻����� �� ��ü�� ��ǥ�� (����X,ü���� ����) )..
	Effect_Show_Chase_Target,				// ����Ʈ�� ���� ���� ��ü�� �Ѿư� ���� ( ����O )..
	Effect_Show_Target_AND_Owner_Position,	// ����Ʈ�� ���� ���� ��ü�� ������ ��ü ��ǥ���� ���..
};

enum eResearchKind
{
	//Research_
	Research_SteamPack,
	Research_SiegeTank,
	Research_End
};

//enum eItemType
//{
//	ITEM_TYPE_WEAPON = 3,
//	ITEM_TYPE_ARMOR  = 2,
//	ITEM_TYPE_POTION
//	ITEM_TYPE_MAX,
//};