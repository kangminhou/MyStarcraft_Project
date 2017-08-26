#include "stdafx.h"
#include "Effect_HitTarget.h"

#include "Animation.h"
#include "Device.h"
#include "TimeMgr.h"

#include "Effect.h"
#include "GameEntity.h"
#include "Weapon.h"


CEffect_HitTarget::CEffect_HitTarget()
{
}


CEffect_HitTarget::~CEffect_HitTarget()
{
}

void CEffect_HitTarget::SetWeapon( CWeapon* _pWeapon )
{
	this->m_pWeapon = _pWeapon;
}

void CEffect_HitTarget::Initialize()
{
	if ( !this->m_bFirstInitialize )
	{
		this->m_pTarget = const_cast<CGameEntity*>(this->m_pWeapon->GetTarget());
		this->m_bLookDestination = false;
	}

	CEffectBridge::Initialize();

	FRAME tFrame = FRAME( 0.f, (float)this->m_vecDrawTexture.size(), (float)this->m_vecDrawTexture.size(), 0.f );
	this->m_pAnimation->AddAnimation( this->m_pEffect->GetStateKey(), tFrame, CAnimation::Anim_Loop );

	this->m_pAnimation->ChangeAnimation( this->m_pEffect->GetStateKey() );

	this->m_fSpeed = 400.f;
}

int CEffect_HitTarget::Update()
{
	if(this->m_pTarget->GetIsDie() )
		return CEffectBridge::Event_DestroyObject; 

	if ( !m_bLookDestination )
	{
		this->m_vDestination = this->m_pTarget->GetPos();
		D3DXVECTOR3 vTempDir;
		D3DXVec3Normalize( &vTempDir, &(this->m_pTarget->GetPos() - this->m_pEffect->GetPos()) );

		this->m_pEffect->SetDir( vTempDir );

		this->m_bLookDestination = true;
	}

	float fSpeed = m_fSpeed * GET_TIME;
	this->m_pEffect->Translate( fSpeed );

	if ( this->m_pAnimation )
	{
		this->m_pAnimation->UpdateAnim();
	}

	if ( D3DXVec3Length( &(this->m_pEffect->GetPos() - this->m_vDestination) ) <= fSpeed )
	{
		this->m_pWeapon->HitTarget();
		return CEffectBridge::Event_DestroyObject;
	}

	cout << "Effect Pos : " << this->m_pEffect->GetPos().x << ", " << this->m_pEffect->GetPos().y << ", 0.f" << endl;

	return CEffectBridge::Event_None;
}

void CEffect_HitTarget::Render()
{
	CEffectBridge::Render();
}

void CEffect_HitTarget::Release()
{
}
