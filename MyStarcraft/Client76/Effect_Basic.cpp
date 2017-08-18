#include "stdafx.h"
#include "Effect_Basic.h"

#include "Animation.h"
#include "Device.h"

#include "Effect.h"


CEffect_Basic::CEffect_Basic()
{
}


CEffect_Basic::~CEffect_Basic()
{
}

void CEffect_Basic::Initialize()
{
	CEffectBridge::Initialize();

	FRAME tFrame = FRAME( 0.f, this->m_vecDrawTexture.size(), this->m_vecDrawTexture.size(), 0.f );
	this->m_pAnimation->AddAnimation( this->m_pEffect->GetStateKey(), tFrame, CAnimation::Anim_ClampForever );

	this->m_pAnimation->ChangeAnimation( this->m_pEffect->GetStateKey() );
}

int CEffect_Basic::Update()
{
	if ( this->m_pAnimation )
	{
		this->m_pAnimation->UpdateAnim();

		if ( this->m_pAnimation->GetIsAnimEnd() )
			return CEffectBridge::Event_DestroyObject;
	}

	return CEffectBridge::Event_None;
}

void CEffect_Basic::Render()
{
	CEffectBridge::Render();
}

void CEffect_Basic::Release()
{
	
}
