#pragma once

//
//enum eOBJECT_TYPE
enum eObjectType
{
	//eObjectType.
	OBJ_TYPE_BACKGROUND,
	OBJ_TYPE_EFFECT,
	OBJ_TYPE_USER,
	OBJ_TYPE_USER2,
	OBJ_TYPE_MAX,
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
	SCENE_MAX,
};

enum eWeaponUpgradeType
{
	Upgrade_Terran_Vehicle_Weapons,
	Upgrade_Terran_Infantry_Weapons,
};

//enum eItemType
//{
//	ITEM_TYPE_WEAPON = 3,
//	ITEM_TYPE_ARMOR  = 2,
//	ITEM_TYPE_POTION
//	ITEM_TYPE_MAX,
//};