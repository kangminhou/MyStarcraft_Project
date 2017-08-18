#include "stdafx.h"
#include "Weapon.h"

#include "GameEntity.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "EffectMgr.h"


CWeapon::CWeapon()
{
}


CWeapon::~CWeapon()
{
	this->Release();
}

void CWeapon::SetAttInterval( const float & _fAttInterval )
{
	if ( this->m_pWeaonData )
		this->m_pWeaonData->fAttInterval = _fAttInterval;
}

void CWeapon::SetWeaponData( WEAPON_DATA * _pWeaponData )
{
	if ( _pWeaponData )
		this->m_pWeaonData = _pWeaponData;
}

void CWeapon::SetWeaponOwner( CGameEntity * _pOwnerEntity )
{
	this->m_pOwnerEntity = _pOwnerEntity;
}

const WEAPON_DATA * CWeapon::GetWeaponData() const
{
	if ( this->m_pWeaonData )
		return this->m_pWeaonData;

	return NULL;
}

const CGameEntity * CWeapon::GetEntity() const
{
	return this->m_pOwnerEntity;
}

const CGameEntity * CWeapon::GetTarget() const
{
	return this->m_pTarget;
}

void CWeapon::Initialize()
{
	this->m_fRestInterval = 0.f;
}

void CWeapon::Release()
{
}

#include "Marine.h"

void CWeapon::Attack( CGameEntity * _pAttTarget )
{
	this->m_fRestInterval = this->m_pWeaonData->fAttInterval;
	this->m_pTarget = _pAttTarget;
	
	/* 즉시 공격인지 투사체 공격인지 판별.. */
	if ( !this->m_pWeaonData->pBulletData )
		this->HitTarget();

	/* 이펙트 발사.. */
	CEffectMgr::GetInstance()->ShowEffect( this, _pAttTarget );

}

void CWeapon::HitTarget()
{
	/* 공격 타입에 따라 갈리게 할 예정.. */
	switch ( this->m_pWeaonData->eWeaponAttType )
	{
		case Weapon_Normal:
			NormalAttack();
			break;
		case Weapon_Splash_Noraml:
			NormalSplashAttack();
			break;
		case Weapon_Splash_Circle:
			CircleSplashAttack();
			break;
	}
}

bool CWeapon::AttackCoolTimeUpdate()
{
	if ( !this->IsCanAttack() )
	{
		this->m_fRestInterval -= GET_TIME;
		return false;
	}

	return true;
}

bool CWeapon::IsCanAttack()
{
	return ((m_fRestInterval <= 0.f) ? true : false);
}

void CWeapon::NormalAttack()
{
	this->m_pTarget->HitEntity( this->m_pOwnerEntity, this->m_pWeaonData->fDamage );
}

void CWeapon::NormalSplashAttack()
{
	this->m_pTarget->HitEntity( this->m_pOwnerEntity, this->m_pWeaonData->fDamage );

	vector<CGameEntity*> vecNearEntity;
	for ( int i = OBJ_TYPE_USER; i <= OBJ_TYPE_USER2; ++i )
	{
		if ( i == this->m_pOwnerEntity->GetObjectType() )
			continue;

		vecNearEntity.clear();
		CObjMgr::GetInstance()->CheckNearEntitys( &vecNearEntity, this->m_pTarget, this->m_pWeaonData->iOutSide, (eObjectType)i );

		size_t iLength = vecNearEntity.size();
		for ( size_t i = 0; i < iLength; ++i )
		{
			float fDistToTarget = D3DXVec3Length( &(vecNearEntity[i]->GetPos() - this->m_pTarget->GetPos()) );
			if ( fDistToTarget <= this->m_pWeaonData->iInside )
			{
				vecNearEntity[i]->HitEntity( this->m_pOwnerEntity, this->m_pWeaonData->fDamage );
			}
			else if ( fDistToTarget <= this->m_pWeaonData->iMiddle )
			{
				vecNearEntity[i]->HitEntity( this->m_pOwnerEntity, this->m_pWeaonData->fDamage * 0.5f );
			}
			else
			{
				vecNearEntity[i]->HitEntity( this->m_pOwnerEntity, this->m_pWeaonData->fDamage * 0.25f );
			}
		}
	}
}

void CWeapon::CircleSplashAttack()
{
	this->m_pTarget->HitEntity( this->m_pOwnerEntity, this->m_pWeaonData->fDamage );
}
