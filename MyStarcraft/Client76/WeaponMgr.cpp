#include "stdafx.h"
#include "WeaponMgr.h"

#include "TextureMgr.h"

#include "Weapon.h"


CWeaponMgr::CWeaponMgr()
{
}


CWeaponMgr::~CWeaponMgr()
{
	Release();
}

void CWeaponMgr::Initialize()
{
	/* 무기의 정보 초기화.. ( XML 데이터로 처리할 예정???? ) */
	this->LoadWeaponData();

	for ( int i = 0; i < Weapon_End; ++i )
	{
		this->m_tWeaponDataArr[i].strImageName = new TCHAR[MIN_STR];
		this->m_tWeaponDataArr[i].strBulletSoundName = new TCHAR[MIN_STR];
	}

	this->m_tWeaponDataArr[Weapon_GaussRifle].bImagePathInfluenceEntityDir = false;
	this->m_tWeaponDataArr[Weapon_GaussRifle].nIconFrame = 323;
	lstrcpy( this->m_tWeaponDataArr[Weapon_GaussRifle].strImageName, L"tmafir00.wav" );

	this->m_tWeaponDataArr[Weapon_FragmentationGrenade].bImagePathInfluenceEntityDir = false;
	this->m_tWeaponDataArr[Weapon_FragmentationGrenade].nIconFrame = 325;
	lstrcpy( this->m_tWeaponDataArr[Weapon_FragmentationGrenade].strImageName, L"tvufir00.wav" );
	lstrcpy( this->m_tWeaponDataArr[Weapon_FragmentationGrenade].strBulletSoundName, L"tvuhit01.wav" );

	this->m_tWeaponDataArr[Weapon_FlameThrower].bImagePathInfluenceEntityDir = true;
	this->m_tWeaponDataArr[Weapon_FlameThrower].nIconFrame = 335;
	lstrcpy( this->m_tWeaponDataArr[Weapon_FlameThrower].strImageName, L"tfbfir00.wav" );

	this->m_tWeaponDataArr[Weapon_FusionCutter].bImagePathInfluenceEntityDir = false;
	this->m_tWeaponDataArr[Weapon_FusionCutter].nIconFrame = 329;
	lstrcpy( this->m_tWeaponDataArr[Weapon_FusionCutter].strImageName, L"edrrep01.wav" );

	this->m_tWeaponDataArr[Weapon_ArcliteCannon].bImagePathInfluenceEntityDir = true;
	this->m_tWeaponDataArr[Weapon_ArcliteCannon].bImagePath2InfluenceEntityDir = false;
	this->m_tWeaponDataArr[Weapon_ArcliteCannon].nIconFrame = 328;
	lstrcpy( this->m_tWeaponDataArr[Weapon_ArcliteCannon].strImageName, L"ttafir00.wav" );

	this->m_tWeaponDataArr[Weapon_ArcliteShockCannon].bImagePathInfluenceEntityDir = true;
	this->m_tWeaponDataArr[Weapon_ArcliteShockCannon].bImagePath2InfluenceEntityDir = false;
	this->m_tWeaponDataArr[Weapon_ArcliteShockCannon].nIconFrame = 336;
	lstrcpy( this->m_tWeaponDataArr[Weapon_ArcliteShockCannon].strImageName, L"ttafi200.wav" );

	this->m_tWeaponDataArr[Weapon_HelfireMissilePack].bImagePathInfluenceEntityDir = false;
	this->m_tWeaponDataArr[Weapon_HelfireMissilePack].nIconFrame = 327;
	lstrcpy( this->m_tWeaponDataArr[Weapon_HelfireMissilePack].strImageName, L"tgofir00.wav" );

	if ( this->m_tWeaponDataArr[Weapon_HelfireMissilePack].pBulletData )
		this->m_tWeaponDataArr[Weapon_HelfireMissilePack].pBulletData->bUseFollowDustEffect = true;

	this->m_tWeaponDataArr[Weapon_TwinAutocannons].bImagePathInfluenceEntityDir = false;
	this->m_tWeaponDataArr[Weapon_TwinAutocannons].nIconFrame = 326;
	lstrcpy( this->m_tWeaponDataArr[Weapon_TwinAutocannons].strImageName, L"tmafir00.wav" );

	this->m_tWeaponDataArr[Weapon_C10_ConcussionRifle].bImagePathInfluenceEntityDir = false;
	this->m_tWeaponDataArr[Weapon_C10_ConcussionRifle].nIconFrame = 324;
	lstrcpy( this->m_tWeaponDataArr[Weapon_C10_ConcussionRifle].strImageName, L"tghfir00.wav" );

	for ( int i = 0; i < Weapon_End; ++i )
	{
		m_tWeaponDataArr2[i] = m_tWeaponDataArr[i];
	}

}

void CWeaponMgr::Release()
{
	for ( int i = 0; i < Weapon_End; ++i )
	{
		if ( this->m_tWeaponDataArr[i].pBulletData )
		{
			if ( this->m_tWeaponDataArr[i].pBulletData->pImagePath )
				safe_delete( this->m_tWeaponDataArr[i].pBulletData->pImagePath );

			if ( this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath )
				safe_delete( this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath );

			if ( this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath )
				safe_delete( this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath );
			
			safe_delete( this->m_tWeaponDataArr[i].pBulletData );
		}

		if ( this->m_tWeaponDataArr[i].pImagePath )
			safe_delete( this->m_tWeaponDataArr[i].pImagePath );

		if ( this->m_tWeaponDataArr[i].pImagePath2 )
			safe_delete( this->m_tWeaponDataArr[i].pImagePath2 );

		delete[] this->m_tWeaponDataArr[i].strImageName;
		this->m_tWeaponDataArr[i].strImageName = nullptr;

		delete[] this->m_tWeaponDataArr[i].strBulletSoundName;
		this->m_tWeaponDataArr[i].strBulletSoundName = nullptr;
	}
}

CWeapon * CWeaponMgr::GetNewWeapon( const eObjectType& _eObjType, const eWeaponKind& _eWeaponKind )
{
	CWeapon* pOutWeapon = nullptr;

	if ( _eWeaponKind < 0 || _eWeaponKind >= Weapon_End )
		return pOutWeapon;

	pOutWeapon = new CWeapon;
	if ( _eObjType == OBJ_TYPE_USER )
	{
		pOutWeapon->SetWeaponData( &this->m_tWeaponDataArr[_eWeaponKind] );
	}
	else
	{
		pOutWeapon->SetWeaponData( &this->m_tWeaponDataArr2[_eWeaponKind] );
	}

	pOutWeapon->Initialize();

	return pOutWeapon;
}

void CWeaponMgr::UpgradeWeapon( const eObjectType & _eObjType, const eWeaponUpgradeType & _eWeaponUpgradeType )
{
	if ( _eObjType != OBJ_TYPE_USER && _eObjType != OBJ_TYPE_USER2 )
	{
		ERROR_MSG( L"CWeaponMgr UpgradeWeapon Failed ( Out Range _eObjType Param )" );
		return;
	}

	vector<WEAPON_DATA*> vecUpgradeWeapon;

	if ( _eObjType == OBJ_TYPE_USER )
	{
		//auto& iter = m_vecAllWeapon[0][_eWeaponUpgradeType];
		//for ( size_t i = 0; i < iter.size(); ++i )
		//{
		//	iter[i]->UpgradeWeapon();
		//}

		switch ( _eWeaponUpgradeType )
		{
			case Upgrade_Terran_Vehicle_Weapons:
			{
				vecUpgradeWeapon.push_back( &(this->m_tWeaponDataArr[Weapon_ArcliteCannon]) );
				vecUpgradeWeapon.push_back( &(this->m_tWeaponDataArr[Weapon_ArcliteShockCannon]) );
				vecUpgradeWeapon.push_back( &(this->m_tWeaponDataArr[Weapon_FragmentationGrenade]) );
				vecUpgradeWeapon.push_back( &(this->m_tWeaponDataArr[Weapon_HelfireMissilePack]) );
				vecUpgradeWeapon.push_back( &(this->m_tWeaponDataArr[Weapon_TwinAutocannons]) );
			}
			break;

			case Upgrade_Terran_Infantry_Weapons:
			{
				vecUpgradeWeapon.push_back( &(this->m_tWeaponDataArr[Weapon_GaussRifle]) );
				vecUpgradeWeapon.push_back( &(this->m_tWeaponDataArr[Weapon_FlameThrower]) );
			}
			break;

			default:
			{
				ERROR_MSG( L"CWeaponMgr UpgradeWeapon Failed ( Out Range _eWeaponUpgradeType Param )" );
			}
			return;

		}

		for ( size_t i = 0; i < vecUpgradeWeapon.size(); ++i )
		{
			vecUpgradeWeapon[i]->fUpgradeDamage += (float)vecUpgradeWeapon[i]->iUpgradePlus;
			++vecUpgradeWeapon[i]->iUpgradeNum;
		}
	}
}

void CWeaponMgr::LoadWeaponData()
{
	CoInitialize(NULL);

	MSXML2::IXMLDOMDocument2Ptr	pDoc = NULL;

	pDoc.CreateInstance(  __uuidof(MSXML2::DOMDocument40) );

	pDoc->async				= VARIANT_FALSE;
	pDoc->validateOnParse	= VARIANT_TRUE;
	pDoc->resolveExternals  = VARIANT_TRUE;

	if(pDoc->load( (_variant_t)L"..\\Data\\UnitWeaponData.xml" ) == E_FAIL )
	{
		ERROR_MSG(L"아이템 데이터 로드 실패!");
		return;
	}

	//노드(Element)를 탐색하자.
	MSXML2::IXMLDOMNodeListPtr	pElementList = NULL;


	//xml문서의 내용을 임시로 보관할 변수.
	wstring strTemp = L"";

	//탐색을 위한 변수
	int iCount = 0;
	int iTotal = 0;

	pElementList = pDoc->selectNodes(L"//WEAPON");

	iTotal = pElementList->Getlength();		//ITEM Element의 총 개수를 받아온다.

	for ( int i = 0; i < iTotal; ++i )
	{
		IMAGE_PATH*	pImagePath = new IMAGE_PATH;

		this->m_tWeaponDataArr[i].pBulletData = new BULLET_DATA;

		/* 무기 정보 받기.. */
		this->m_tWeaponDataArr[i].wstrWeaponName
			= pElementList->Getitem( i )->selectNodes( L"//NAME" )->Getitem( i )->Gettext();

		this->m_tWeaponDataArr[i].fDamage
			= atof( pElementList->Getitem( i )->selectNodes( L"//DAMAGE" )->Getitem( i )->Gettext() );

		this->m_tWeaponDataArr[i].fAttInterval
			= Calc_Weapon_Interval( atof( pElementList->Getitem( i )->selectNodes( L"//ATTACK_INTERVAL" )->Getitem( i )->Gettext() ) );

		this->m_tWeaponDataArr[i].eUpgradeType
			= (eWeaponUpgradeType)atoi( pElementList->Getitem( i )->selectNodes( L"//UPGRADE_TYPE" )->Getitem( i )->Gettext() );

		this->m_tWeaponDataArr[i].iUpgradePlus
			= atoi( pElementList->Getitem( i )->selectNodes( L"//UPGRADE_PLUS" )->Getitem( i )->Gettext() );

		this->m_tWeaponDataArr[i].iMinAttRange
			= atoi( pElementList->Getitem( i )->selectNodes( L"//MIN_ATTACK_RANGE" )->Getitem( i )->Gettext() );

		this->m_tWeaponDataArr[i].iMaxAttRange
			= atoi( pElementList->Getitem( i )->selectNodes( L"//MAX_ATTACK_RANGE" )->Getitem( i )->Gettext() );

		this->m_tWeaponDataArr[i].eWeaponAttType
			= (eWeaponAttType)atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_TYPE" )->Getitem( i )->Gettext() );

		this->m_tWeaponDataArr[i].iInside
			= atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_INSIDE" )->Getitem( i )->Gettext() );

		this->m_tWeaponDataArr[i].iMiddle
			= atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_MIDDLE" )->Getitem( i )->Gettext() );

		this->m_tWeaponDataArr[i].iOutSide
			= atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_OUTSIDE" )->Getitem( i )->Gettext() );

		/* 이미지 경로 받기.. */
		pImagePath->wstrObjKey
			= pElementList->Getitem( i )->selectNodes( L"//IMAGE_OBJ_KEY" )->Getitem( i )->Gettext();
		
		pImagePath->wstrStateKey
			= pElementList->Getitem( i )->selectNodes( L"//IMAGE_STATE_KEY" )->Getitem( i )->Gettext();
		
		pImagePath->wstrPath
			= pElementList->Getitem( i )->selectNodes( L"//IMAGE_PATH" )->Getitem( i )->Gettext();
		
		pImagePath->iCount
			= atoi( pElementList->Getitem( i )->selectNodes( L"//IMAGE_COUNT" )->Getitem( i )->Gettext() );

		if ( pImagePath->wstrObjKey != L"NULL" )
		{
			D3DCOLOR color = D3DCOLOR_ARGB( 255, 0, 0, 0 );
			//if ( pImagePath->wstrObjKey == L"Effect" &&
			//	 pImagePath->wstrStateKey == L"SiegeTankFire" )
			//	color = D3DCOLOR_ARGB( 255, 0, 255, 0 );

			CTextureMgr::GetInstance()->InsertTexture( pImagePath->wstrPath.c_str(),
													   pImagePath->wstrObjKey.c_str(), TEX_MULTI,
													   pImagePath->wstrStateKey.c_str(),
													   pImagePath->iCount,
													   true, color );

			this->m_tWeaponDataArr[i].pImagePath = pImagePath;
		}
		else
		{
			this->m_tWeaponDataArr[i].pImagePath = NULL;
			safe_delete( pImagePath );
		}

		this->m_tWeaponDataArr[i].eEffectShowKind
			= (eEffectShowKind)atoi( pElementList->Getitem( i )->selectNodes( L"//SHOW_EFFECT_KIND" )->Getitem( i )->Gettext() );

		/* 투사체 정보 받기.. */
		if ( this->m_tWeaponDataArr[i].eEffectShowKind == Effect_Show_Hit_Target ||
			 this->m_tWeaponDataArr[i].eEffectShowKind == Effect_Show_Chase_Target )
		{
			/* 투사체 데이터 받기.. */
			this->m_tWeaponDataArr[i].pBulletData->fCastingTime
				= atof( pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_TIME" )->Getitem( i )->Gettext() );

			this->m_tWeaponDataArr[i].pBulletData->fSpeed
				= atof( pElementList->Getitem( i )->selectNodes( L"//BULLET_SPEED" )->Getitem( i )->Gettext() );

			/* 투사체 이미지 경로 받기.. */
			this->m_tWeaponDataArr[i].pBulletData->pImagePath = new IMAGE_PATH;

			this->m_tWeaponDataArr[i].pBulletData->pImagePath->wstrObjKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_OBJ_KEY" )->Getitem( i )->Gettext();

			this->m_tWeaponDataArr[i].pBulletData->pImagePath->wstrStateKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_STATE_KEY" )->Getitem( i )->Gettext();

			this->m_tWeaponDataArr[i].pBulletData->pImagePath->wstrPath
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_IMAGE_PATH" )->Getitem( i )->Gettext();

			this->m_tWeaponDataArr[i].pBulletData->pImagePath->iCount
				= atoi( pElementList->Getitem( i )->selectNodes( L"//BULLET_IMAGE_COUNT" )->Getitem( i )->Gettext() );

			if ( this->m_tWeaponDataArr[i].pBulletData->pImagePath->wstrObjKey == L"NULL" )
				safe_delete( this->m_tWeaponDataArr[i].pBulletData->pImagePath );


			/* 투사체 이미지 경로 받기.. */
			this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath = new IMAGE_PATH;

			this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath->wstrObjKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_OBJ_KEY" )->Getitem( i )->Gettext();

			this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath->wstrStateKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_STATE_KEY" )->Getitem( i )->Gettext();

			this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath->wstrPath
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_IMAGE_PATH" )->Getitem( i )->Gettext();

			this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath->iCount
				= atoi( pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_IMAGE_COUNT" )->Getitem( i )->Gettext() );

			if ( this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath->wstrObjKey == L"NULL" )
				safe_delete( this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath );


			/* 투사체 이미지 경로 받기.. */
			this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath = new IMAGE_PATH;

			this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath->wstrObjKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_EFFECT_OBJ_KEY" )->Getitem( i )->Gettext();

			this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath->wstrStateKey
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_EFFECT_STATE_KEY" )->Getitem( i )->Gettext();

			this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath->wstrPath
				= pElementList->Getitem( i )->selectNodes( L"//BULLET_EFFECT_IMAGE_PATH" )->Getitem( i )->Gettext();

			this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath->iCount
				= atoi( pElementList->Getitem( i )->selectNodes( L"//BULLET_EFFECT_IMAGE_COUNT" )->Getitem( i )->Gettext() );

			if ( this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath->wstrObjKey == L"NULL" )
				safe_delete( this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath );

			if ( this->m_tWeaponDataArr[i].pBulletData->pImagePath )
			{
				CTextureMgr::GetInstance()->InsertTexture(
					this->m_tWeaponDataArr[i].pBulletData->pImagePath->wstrPath.c_str(),
					this->m_tWeaponDataArr[i].pBulletData->pImagePath->wstrObjKey.c_str(), TEX_MULTI,
					this->m_tWeaponDataArr[i].pBulletData->pImagePath->wstrStateKey.c_str(),
					this->m_tWeaponDataArr[i].pBulletData->pImagePath->iCount,
					true, D3DCOLOR_ARGB( 255, 0, 0, 0 )
				);
			}

			if ( this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath )
			{
				CTextureMgr::GetInstance()->InsertTexture(
					this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath->wstrPath.c_str(),
					this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath->wstrObjKey.c_str(), TEX_MULTI,
					this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath->wstrStateKey.c_str(),
					this->m_tWeaponDataArr[i].pBulletData->pEffectImaePath->iCount,
					true, D3DCOLOR_ARGB( 255, 0, 0, 0 )
				);
			}

			if ( this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath )
			{
				CTextureMgr::GetInstance()->InsertTexture(
					this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath->wstrPath.c_str(),
					this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath->wstrObjKey.c_str(), TEX_MULTI,
					this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath->wstrStateKey.c_str(),
					this->m_tWeaponDataArr[i].pBulletData->pCastingImagePath->iCount,
					true, D3DCOLOR_ARGB( 255, 0, 0, 0 )
				);
			}

		}
		else
			safe_delete( this->m_tWeaponDataArr[i].pBulletData );

		/*
		pElement_Image2ObjKey = pDoc->createElement( L"IMAGE2_OBJ_KEY" );
		pElement_Image2StateKey = pDoc->createElement( L"IMAGE2_STATE_KEY" );
		pElement_Image2Path = pDoc->createElement( L"IMAGE2_PATH" );
		pElement_Image2Count = pDoc->createElement( L"IMAGE2_COUNT" );
		*/

		/* 2번째 이미지 데이터 불러오기.. */
		this->m_tWeaponDataArr[i].pImagePath2 = new IMAGE_PATH;

		this->m_tWeaponDataArr[i].pImagePath2->wstrObjKey
			= pElementList->Getitem( i )->selectNodes( L"//IMAGE2_OBJ_KEY" )->Getitem( i )->Gettext();

		this->m_tWeaponDataArr[i].pImagePath2->wstrStateKey
			= pElementList->Getitem( i )->selectNodes( L"//IMAGE2_STATE_KEY" )->Getitem( i )->Gettext();

		this->m_tWeaponDataArr[i].pImagePath2->wstrPath
			= pElementList->Getitem( i )->selectNodes( L"//IMAGE2_PATH" )->Getitem( i )->Gettext();

		this->m_tWeaponDataArr[i].pImagePath2->iCount
			= atoi( pElementList->Getitem( i )->selectNodes( L"//IMAGE2_COUNT" )->Getitem( i )->Gettext() );

		if ( this->m_tWeaponDataArr[i].pImagePath2->wstrObjKey == L"NULL" )
			safe_delete( this->m_tWeaponDataArr[i].pImagePath2 );
		else
		{
			CTextureMgr::GetInstance()->InsertTexture( this->m_tWeaponDataArr[i].pImagePath2->wstrPath.c_str(),
													   this->m_tWeaponDataArr[i].pImagePath2->wstrObjKey.c_str(),
													   TEX_MULTI,
													   this->m_tWeaponDataArr[i].pImagePath2->wstrStateKey.c_str(),
													   this->m_tWeaponDataArr[i].pImagePath2->iCount,
													   true,
													   D3DCOLOR_ARGB( 255, 0, 0, 0 ) );
		}
	}

}
