#include "stdafx.h"
#include "Effect_ChaseTarget.h"

#include "TimeMgr.h"

#include "GameEntity.h"
#include "Weapon.h"
#include "Effect.h"
#include "Animation.h"


CEffect_ChaseTarget::CEffect_ChaseTarget()
{
}


CEffect_ChaseTarget::~CEffect_ChaseTarget()
{
}

void CEffect_ChaseTarget::SetWeapon( CWeapon * _pWeapon )
{
	this->m_pWeapon = _pWeapon;
}

void CEffect_ChaseTarget::SetIsUseFollowDustEffect( const bool & _bUseFollowDustEffect )
{
	m_bUseFollowDustEffect = _bUseFollowDustEffect;
}

void CEffect_ChaseTarget::Initialize()
{
	if ( !this->m_bFirstInitialize )
	{
		this->m_pTarget = const_cast<CGameEntity*>(this->m_pWeapon->GetTarget());
	}

	CEffectBridge::Initialize();

	FRAME tFrame = FRAME( 0.f, this->m_vecDrawTexture.size(), this->m_vecDrawTexture.size(), 0.f );
	this->m_pAnimation->AddAnimation( this->m_pEffect->GetStateKey(), tFrame, CAnimation::Anim_Loop );

	this->m_pAnimation->ChangeAnimation( this->m_pEffect->GetStateKey() );
}

int CEffect_ChaseTarget::Update()
{
	if(this->m_pTarget->GetIsDie() )
		return CEffectBridge::Event_DestroyObject; 

	D3DXVECTOR3 vTempDir;
	D3DXVec3Normalize( &vTempDir, &(this->m_pTarget->GetPos() - this->m_pEffect->GetPos()) );

	this->m_pEffect->SetDir( vTempDir );

	this->m_pEffect->Translate( 100.f * CTimeMgr::GetInstance()->GetTime() );

	if ( this->m_pAnimation )
	{
		this->m_pAnimation->UpdateAnim();
	}

	if ( D3DXVec3Length( &(this->m_pEffect->GetPos() - this->m_pTarget->GetPos()) ) <= 5.f )
	{
		this->m_pWeapon->HitTarget();
		return CEffectBridge::Event_DestroyObject;
	}

	return CEffectBridge::Event_None;
}

void CEffect_ChaseTarget::Render()
{
	CEffectBridge::Render();
}

void CEffect_ChaseTarget::Release()
{
}
