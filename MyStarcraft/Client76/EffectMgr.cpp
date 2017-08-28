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
#include "Effect_LockDown.h"
#include "Effect_Nuclear.h"


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

	for ( int i = 0; i < 10; ++i )
	{
		this->AddEffect( CEffectMgr::Effect_Kind_LockDown );
	}

	for ( int i = 0; i < 2; ++i )
	{
		this->AddEffect( CEffectMgr::Effect_Kind_Nuclear );
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
				pShowEffect->Initialize();
				pShowEffect->SetPos( _pWeapon->GetEntity()->GetEffectShowPos() );
				break;
			case Effect_Show_Target_Position:
				pShowEffect->Initialize();
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

				pEffect->Initialize();
				pEffect->SetPos( _pEntity->GetHitShowPos() );
				m_pEffectList->push_back( pEffect );

				pShowEffect->Initialize();
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

				pShowEffect->Initialize();
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

				pShowEffect->Initialize();
				if ( pShowEffect )
					pShowEffect->SetPos( _pWeapon->GetEntity()->GetEffectShowPos() );
			}
				break;

		}
	}

	m_pEffectList->push_back( pShowEffect );
}

void CEffectMgr::ShowEffect( const wstring & _wstrObjKey, const wstring & _wstrStateKey, const int & _iStart, const int & _iEnd, 
							 const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vSize )
{
	CEffect*	pShowEffect = NULL;

	pShowEffect = this->PopEffect( CEffectMgr::Effect_Kind_Basic );

	pShowEffect->SetVecTexture(
		_wstrObjKey, _wstrStateKey, _iStart, _iEnd - _iStart );

	pShowEffect->SetStateKey( _wstrStateKey );

	pShowEffect->Initialize();

	pShowEffect->SetPos( _vPos );
	pShowEffect->SetSize( _vSize );

	m_pEffectList->push_back( pShowEffect );
}

void CEffectMgr::PushEffect( CEffect *_pPushEffect, const eEffectKind & _eKind )
{
	this->m_queueEffectArr[_eKind].push( _pPushEffect );
}

void CEffectMgr::ShowLockDown( const CGameEntity * _pFireEntity, CGameEntity * _pHitEntity )
{
	CEffect* pEffect = this->PopEffect( Effect_Kind_LockDown );

	CEffect_LockDown* pLockDown = dynamic_cast<CEffect_LockDown*>(const_cast<CEffectBridge*>(pEffect->GetEffectBridge()));

	if ( pLockDown )
	{
		pEffect->SetPos( _pFireEntity->GetPos() );
		pLockDown->SetTarget( _pHitEntity );
		pEffect->Initialize();
		this->m_pEffectList->push_back( pEffect );
	}
	else
	{
		this->PushEffect( pEffect, Effect_Kind_LockDown );
		ERROR_MSG( L"LockDown Effect is NULL" );
		return;
	}

}

void CEffectMgr::ShowNuclear( CGameEntity* _pFireEntity, const D3DXVECTOR3 & _vPos )
{
	CEffect* pEffect = this->PopEffect( Effect_Kind_Nuclear );

	CEffect_Nuclear* pNuclear = dynamic_cast<CEffect_Nuclear*>(const_cast<CEffectBridge*>(pEffect->GetEffectBridge()));

	if ( pNuclear )
	{
		pNuclear->SetFireEntity( _pFireEntity );
		pEffect->Initialize();
		pEffect->SetPos( _vPos );
		this->m_pEffectList->push_back( pEffect );
	}
	else
	{
		this->PushEffect( pEffect, Effect_Kind_Nuclear );
		ERROR_MSG( L"Nuclear Effect is NULL" );
		return;
	}
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

		case Effect_Kind_LockDown:
			pEffect = CEffectFactory<CEffect_LockDown>::CreateEffect();
			break;

		case Effect_Kind_Nuclear:
			pEffect = CEffectFactory<CEffect_Nuclear>::CreateEffect();
			break;

		default:
			return;
	}

	pEffect->Initialize();
	this->m_queueEffectArr[_eEffectKind].push( pEffect );
}
