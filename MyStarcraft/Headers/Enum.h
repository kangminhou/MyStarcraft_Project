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
	Effect_Show_None,						// ÀÌÆåÆ® ¾È ¶ç¿ò..
	Effect_Show_Owner_Position,				// ÀÌÆåÆ®°¡ °ø°ÝÇÑ °´Ã¼ ÂÊ¿¡¼­ ¶ç¿ò..
	Effect_Show_Target_Position,			// ÀÌÆåÆ®°¡ °ø°Ý ´çÇÑ °´Ã¼ ÂÊ¿¡¼­ ¶ç¿ò..
	Effect_Show_Hit_Target,					// ÀÌÆåÆ®°¡ °ø°Ý ´çÇÑ °´Ã¼ÀÇ ÁÂÇ¥·Î ³¯¾Æ°¨ ( ¹ß»çÇßÀ» ¶§ °´Ã¼ÀÇ ÁÂÇ¥·Î (À¯µµX,Ã¼·ÂÀº ±ðÀÓ) )..
	Effect_Show_Chase_Target,				// ÀÌÆåÆ®°¡ °ø°Ý ´çÇÑ °´Ã¼¸¦ ÂÑ¾Æ°¡ ¸ÂÈû ( À¯µµO )..
	Effect_Show_Target_AND_Owner_Position,	// ÀÌÆåÆ®°¡ °ø°Ý ´çÇÑ °´Ã¼¿Í °ø°ÝÇÑ °´Ã¼ ÁÂÇ¥¿¡¼­ ¶ç¿ò..
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