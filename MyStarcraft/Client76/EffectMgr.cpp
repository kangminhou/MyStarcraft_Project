#include "stdafx.h"
#include "EffectMgr.h"

#include "EffectFactory.h"
#include "ObjMgr.h"

#include "Weapon.h"
#include "Effect_Basic.h"
#include "Effect.h"
#include "GameEntity.h"
#include "Effect_HitTarget.h"


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
		pShowEffect = this->m_queueEffectArr[CEffectMgr::Effect_Kind_Basic].front();


		/* Entity ��ü�� ���⿡ ������ �޴��� Ȯ��.. */
		if ( pWeaponData->bInfluenceEntityDir )
		{
			int iCount = pWeaponData->pImagePath->iCount / 17;
			pShowEffect->SetVecTexture(
				pWeaponData->pImagePath->wstrObjKey, pWeaponData->pImagePath->wstrStateKey, 
				_pWeapon->GetEntity()->GetDirAnimIndex() * iCount, iCount );
		}
		else
		{
			pShowEffect->SetVecTexture(
				pWeaponData->pImagePath->wstrObjKey, pWeaponData->pImagePath->wstrStateKey, 0, pWeaponData->pImagePath->iCount );
		}

		pShowEffect->SetStateKey( pWeaponData->pImagePath->wstrStateKey );

		/* Effect �� ��� �������� ����.. */
		switch ( pWeaponData->eEffectShowKind )
		{
			case Effect_Show_Owner_Position:
				pShowEffect->SetPos( _pWeapon->GetEntity()->GetPos() );
				break;
			case Effect_Show_Target_Position:
				pShowEffect->SetPos( _pEntity->GetPos() );
				break;
			case Effect_Show_Hit_Target:
			{
				CEffect* pEffect = this->m_queueEffectArr[CEffectMgr::Effect_Kind_HitTarget].front();
				dynamic_cast<CEffect_HitTarget*>(const_cast<CEffectBridge*>(pEffect->GetEffectBridge()))->SetWeapon( _pWeapon );

				pEffect->SetVecTexture(
					pWeaponData->pBulletData->pImagePath->wstrObjKey, 
					pWeaponData->pBulletData->pImagePath->wstrStateKey, 0, 
					pWeaponData->pBulletData->pImagePath->iCount );

				pEffect->SetStateKey( pWeaponData->pBulletData->pImagePath->wstrStateKey );

				pEffect->Initialize();
				pEffect->SetPos( _pWeapon->GetEntity()->GetPos() );
				m_pEffectList->push_back( pEffect );

				this->m_queueEffectArr[CEffectMgr::Effect_Kind_HitTarget].pop();

				pShowEffect->SetPos( _pWeapon->GetEntity()->GetPos() );
			}
				break;
			case Effect_Show_Chase_Target:
				pShowEffect->SetPos( _pWeapon->GetEntity()->GetPos() );
				break;

		}

		this->m_queueEffectArr[CEffectMgr::Effect_Kind_Basic].pop();
	}

	pShowEffect->Initialize();
	m_pEffectList->push_back( pShowEffect );
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
