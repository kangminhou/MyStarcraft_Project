#include "stdafx.h"
#include "EffectMgr.h"

#include "EffectFactory.h"
#include "ObjMgr.h"

#include "Weapon.h"
#include "Effect_Basic.h"
#include "Effect.h"
#include "GameEntity.h"
#include "Effect_HitTarget.h"
#include "Effect_ChaseTarget.h"


IMPLEMENT_SINGLETON( CEffectMgr )


CEffectMgr::CEffectMgr()
{
}


CEffectMgr::~CEffectMgr()
{
	this->Release();
}

void CEffectMgr::Initialize( void )
{
	m_pEffectList = CObjMgr::GetInstance()->GetList( OBJ_TYPE_EFFECT );

	for ( int i = 0; i < 100; ++i )
	{
		this->AddEffect( CEffectMgr::Effect_Kind_Basic );
	}

	for ( int i = 0; i < 100; ++i )
	{
		this->AddEffect( CEffectMgr::Effect_Kind_HitTarget );
	}
}

void CEffectMgr::Release( void )
{
	for ( int i = 0; i < Effect_Kind_End; ++i )
	{
		while ( !this->m_queueEffectArr[i].empty() )
		{
			safe_delete( this->m_queueEffectArr[i].front() );
			this->m_queueEffectArr[i].pop();
		}
	}
}

void CEffectMgr::ShowEffect( CWeapon* _pWeapon, const CGameEntity* _pEntity )
{
	CEffect*	pShowEffect = NULL;
	const WEAPON_DATA*	pWeaponData = _pWeapon->GetWeaponData();

	if ( pWeaponData->eEffectShowKind != Effect_Show_None )
	{
		if ( pWeaponData->pImagePath )
		{
			pShowEffect = this->PopEffect( CEffectMgr::Effect_Kind_Basic );

			/* Entity 객체의 방향에 영향을 받는지 확인.. */
			if ( pWeaponData->bImagePathInfluenceEntityDir )
			{
				int iCount = pWeaponData->pImagePath->iCount / 17;
				pShowEffect->SetVecTexture(
					pWeaponData->pImagePath->wstrObjKey, pWeaponData->pImagePath->wstrStateKey,
					_pWeapon->GetEntity()->GetLookAnimIndex() * iCount, iCount );
			}
			else
			{
				pShowEffect->SetVecTexture(
					pWeaponData->pImagePath->wstrObjKey, pWeaponData->pImagePath->wstrStateKey, 0, pWeaponData->pImagePath->iCount );
			}

			pShowEffect->SetStateKey( pWeaponData->pImagePath->wstrStateKey );
		}

		/* Effect 가 어디서 나가는지 결정.. */
		switch ( pWeaponData->eEffectShowKind )
		{
			case Effect_Show_Owner_Position:
				pShowEffect->SetPos( _pWeapon->GetEntity()->GetEffectShowPos() );
				break;
			case Effect_Show_Target_Position:
				pShowEffect->SetPos( _pEntity->GetHitShowPos() );
				break;
			case Effect_Show_Target_AND_Owner_Position:
			{
				CEffect* pEffect = this->PopEffect( CEffectMgr::Effect_Kind_Basic );

				if ( pWeaponData->bImagePath2InfluenceEntityDir )
				{
					int iCount = pWeaponData->pImagePath2->iCount / 17;
					pShowEffect->SetVecTexture(
						pWeaponData->pImagePath2->wstrObjKey, pWeaponData->pImagePath2->wstrStateKey, 
						_pWeapon->GetEntity()->GetLookAnimIndex() * iCount, iCount );
				}
				else
				{
					pEffect->SetVecTexture(
						pWeaponData->pImagePath2->wstrObjKey, 
						pWeaponData->pImagePath2->wstrStateKey, 0, 
						pWeaponData->pImagePath2->iCount );
				}

				pEffect->SetStateKey( pWeaponData->pImagePath2->wstrStateKey );

				pEffect->SetPos( _pEntity->GetHitShowPos() );
				pEffect->Initialize();
				m_pEffectList->push_back( pEffect );

				pShowEffect->SetPos( _pWeapon->GetEntity()->GetEffectShowPos() );
			}

				break;
			case Effect_Show_Hit_Target:
			{
				CEffect* pEffect = this->PopEffect( CEffectMgr::Effect_Kind_HitTarget );
				dynamic_cast<CEffect_HitTarget*>(const_cast<CEffectBridge*>(pEffect->GetEffectBridge()))->SetWeapon( _pWeapon );

				pEffect->SetVecTexture(
					pWeaponData->pBulletData->pImagePath->wstrObjKey, 
					pWeaponData->pBulletData->pImagePath->wstrStateKey, 0, 
					pWeaponData->pBulletData->pImagePath->iCount );

				pEffect->SetStateKey( pWeaponData->pBulletData->pImagePath->wstrStateKey );

				pEffect->Initialize();
				pEffect->SetPos( _pWeapon->GetEntity()->GetEffectShowPos() );
				m_pEffectList->push_back( pEffect );

				pShowEffect->SetPos( _pWeapon->GetEntity()->GetEffectShowPos() );
			}
				break;
			case Effect_Show_Chase_Target:
			{
				CEffect* pEffect = this->PopEffect( CEffectMgr::Effect_Kind_ChaseTarget );

				CEffect_ChaseTarget* pEffectBridge = dynamic_cast<CEffect_ChaseTarget*>(const_cast<CEffectBridge*>(pEffect->GetEffectBridge()));
				pEffectBridge->SetWeapon( _pWeapon );
				pEffectBridge->SetIsUseFollowDustEffect( pWeaponData->pBulletData->bUseFollowDustEffect );

				pEffect->SetVecTexture(
					pWeaponData->pBulletData->pImagePath->wstrObjKey, 
					pWeaponData->pBulletData->pImagePath->wstrStateKey, 0, 
					pWeaponData->pBulletData->pImagePath->iCount );

				pEffect->SetStateKey( pWeaponData->pBulletData->pImagePath->wstrStateKey );

				pEffect->Initialize();
				pEffect->SetPos( _pWeapon->GetEntity()->GetEffectShowPos() );
				m_pEffectList->push_back( pEffect );

				if ( pShowEffect )
					pShowEffect->SetPos( _pWeapon->GetEntity()->GetEffectShowPos() );
			}
				break;

		}
	}

	pShowEffect->Initialize();
	m_pEffectList->push_back( pShowEffect );
}

void CEffectMgr::PushEffect( CEffect *_pPushEffect, const eEffectKind & _eKind )
{
	this->m_queueEffectArr[_eKind].push( _pPushEffect );
}

CEffect * CEffectMgr::PopEffect( const eEffectKind & _eKind )
{
	if ( this->m_queueEffectArr[_eKind].empty() )
	{
		for ( int i = 0; i < 50; ++i )
			this->AddEffect( _eKind );
	}

	CEffect* pEffect = this->m_queueEffectArr[_eKind].front();
	pEffect->SetEffectType( _eKind );
	this->m_queueEffectArr[_eKind].pop();
	return pEffect;
}

void CEffectMgr::AddEffect( const CEffectMgr::eEffectKind & _eEffectKind )
{
	CEffect*	pEffect = NULL;

	switch ( _eEffectKind )
	{
		case Effect_Kind_Basic:
			pEffect = CEffectFactory<CEffect_Basic>::CreateEffect();
			break;

		case Effect_Kind_HitTarget:
			pEffect = CEffectFactory<CEffect_HitTarget>::CreateEffect();
			break;

		default:
			return;
	}

	pEffect->Initialize();
	this->m_queueEffectArr[_eEffectKind].push( pEffect );
}
