#include "stdafx.h"
#include "WeaponMgr.h"

#include "TextureMgr.h"

#include "Weapon.h"


CWeaponMgr::CWeaponMgr()
{
}


CWeaponMgr::~CWeaponMgr()
{
}

void CWeaponMgr::Initialize()
{
	/* ������ ���� �ʱ�ȭ.. ( XML �����ͷ� ó���� ����???? ) */
	this->LoadWeaponData();

	this->m_tWeaponDataArr[Weapon_GaussRifle].bInfluenceEntityDir = false;

	this->m_tWeaponDataArr[Weapon_FragmentationGrenade].bInfluenceEntityDir = false;

	this->m_tWeaponDataArr[Weapon_FlameThrower].bInfluenceEntityDir = true;
}

CWeapon * CWeaponMgr::GetNewWeapon( const eWeaponKind& _eWeaponKind )
{
	CWeapon* pOutWeapon = nullptr;

	if ( _eWeaponKind < 0 || _eWeaponKind >= Weapon_End )
		return pOutWeapon;

	pOutWeapon = new CWeapon;
	pOutWeapon->SetWeaponData( &this->m_tWeaponDataArr[_eWeaponKind] );
	pOutWeapon->Initialize();

	return pOutWeapon;
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
		ERROR_MSG(L"������ ������ �ε� ����!");
		return;
	}

	//���(Element)�� Ž������.
	MSXML2::IXMLDOMNodeListPtr	pElementList = NULL;


	//xml������ ������ �ӽ÷� ������ ����.
	wstring strTemp = L"";

	//Ž���� ���� ����
	int iCount = 0;
	int iTotal = 0;

	pElementList = pDoc->selectNodes(L"//WEAPON");

	iTotal = pElementList->Getlength();		//ITEM Element�� �� ������ �޾ƿ´�.

	for(int i = 0; i < iTotal; ++i)
	{
		IMAGE_PATH*	pImagePath = new IMAGE_PATH;

		ZeroMemory(pImagePath, sizeof(IMAGE_PATH));

		this->m_tWeaponDataArr[i].pBulletData = new BULLET_DATA;

		/* ���� ���� �ޱ�.. */
		this->m_tWeaponDataArr[i].wstrWeaponName 
			= pElementList->Getitem(i)->selectNodes(L"//NAME")->Getitem(i)->Gettext();

		this->m_tWeaponDataArr[i].fDamage
			= atof( pElementList->Getitem( i )->selectNodes( L"//DAMAGE" )->Getitem(i)->Gettext() );

		this->m_tWeaponDataArr[i].fAttInterval
			= Calc_Weapon_Interval( atof( pElementList->Getitem( i )->selectNodes( L"//ATTACK_INTERVAL" )->Getitem( i )->Gettext() ) );

		this->m_tWeaponDataArr[i].eUpgradeType
			= (eWeaponUpgradeType)atoi( pElementList->Getitem( i )->selectNodes( L"//UPGRADE_TYPE" )->Getitem(i)->Gettext() );

		this->m_tWeaponDataArr[i].iUpgradePlus
			= atoi( pElementList->Getitem( i )->selectNodes( L"//UPGRADE_PLUS" )->Getitem(i)->Gettext() );

		this->m_tWeaponDataArr[i].iMinAttRange
			= atoi( pElementList->Getitem( i )->selectNodes( L"//MIN_ATTACK_RANGE" )->Getitem(i)->Gettext() );

		this->m_tWeaponDataArr[i].iMaxAttRange
			= atoi( pElementList->Getitem( i )->selectNodes( L"//MAX_ATTACK_RANGE" )->Getitem(i)->Gettext() );

		this->m_tWeaponDataArr[i].eWeaponAttType
			= (eWeaponAttType)atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_TYPE" )->Getitem(i)->Gettext() );

		this->m_tWeaponDataArr[i].iInside
			= atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_INSIDE" )->Getitem(i)->Gettext() );

		this->m_tWeaponDataArr[i].iMiddle
			= atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_MIDDLE" )->Getitem(i)->Gettext() );

		this->m_tWeaponDataArr[i].iOutSide
			= atoi( pElementList->Getitem( i )->selectNodes( L"//SPLASH_OUTSIDE" )->Getitem(i)->Gettext() );

		/* �̹��� ��� �ޱ�.. */
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
			CTextureMgr::GetInstance()->InsertTexture( pImagePath->wstrPath.c_str(), pImagePath->wstrObjKey.c_str(), TEX_MULTI, pImagePath->wstrStateKey.c_str(), pImagePath->iCount,
													   true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) );

			this->m_tWeaponDataArr[i].pImagePath = pImagePath;
		}
		else
			this->m_tWeaponDataArr[i].pImagePath = NULL;

		this->m_tWeaponDataArr[i].eEffectShowKind
			= (eEffectShowKind)atoi( pElementList->Getitem( i )->selectNodes( L"//SHOW_EFFECT_KIND" )->Getitem( i )->Gettext() );

		/* ����ü ���� �ޱ�.. */
		if ( this->m_tWeaponDataArr[i].eEffectShowKind == Effect_Show_Hit_Target ||
			 this->m_tWeaponDataArr[i].eEffectShowKind == Effect_Show_Chase_Target )
		{
			/* ����ü ������ �ޱ�.. */
			this->m_tWeaponDataArr[i].pBulletData->fCastingTime
				= atof( pElementList->Getitem( i )->selectNodes( L"//BULLET_CASTING_TIME" )->Getitem( i )->Gettext() );

			this->m_tWeaponDataArr[i].pBulletData->fSpeed
				= atof( pElementList->Getitem( i )->selectNodes( L"//BULLET_SPEED" )->Getitem( i )->Gettext() );

			/* ����ü �̹��� ��� �ޱ�.. */
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


			/* ����ü �̹��� ��� �ޱ�.. */
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


			/* ����ü �̹��� ��� �ޱ�.. */
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


	}

}
