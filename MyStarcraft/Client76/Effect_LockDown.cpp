#include "stdafx.h"
#include "Effect_LockDown.h"

#include "Animation.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

#include "Effect.h"
#include "GameEntity.h"


CEffect_LockDown::CEffect_LockDown()
{
}


CEffect_LockDown::~CEffect_LockDown()
{
}

void CEffect_LockDown::SetTarget( CGameEntity * _pTarget )
{
	this->m_pTarget = _pTarget;
}

void CEffect_LockDown::Initialize()
{
	if ( this->m_bFirstInitialize )
	{
		for ( int i = 0; i < 17; ++i )
		{
			const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Bullet", L"MissileAtt", i );
			m_vecBulletTexture.push_back( pTexture );
		}
	}
	else
	{
		this->m_pAnimation->AddAnimation( L"LockDown_Begin", FRAME( 0.f, 7.f, 7.f, 0.f ), CAnimation::Anim_ClampForever );
		this->m_pAnimation->AddAnimation( L"LockDown_Loop", FRAME( 0.f, 5.f, 5.f, 7.f ), CAnimation::Anim_Loop );
		this->m_pAnimation->AddAnimation( L"LockDown_End", FRAME( 0.f, 7.f, 7.f, 0.f ), CAnimation::Anim_Reverse_ClampForever );

		this->m_pAnimation->ChangeAnimation( L"LockDown_Begin" );

		wstring wstrLockDownKey = L"LockDownL";

		this->m_vecDrawTexture.clear();

		for ( int i = 0; i < 14; ++i )
		{
			this->m_vecDrawTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"Effect", wstrLockDownKey.c_str(), i ) );
		}

		if ( !this->m_pTarget )
		{
			this->m_bDestory = true;
			return;
		}
		else
			this->m_bDestory = false;

		this->m_fLerpT = 0.f;
		this->m_fDestoryTime = 10.f;
		this->m_bHitTarget = false;
		this->m_byLockDownLevel = 0;

		this->m_vBeginPos = this->m_pEffect->GetPos();
		this->m_pTarget->SetStopTIme( 100.f );
	}

	CEffectBridge::Initialize();
}

int CEffect_LockDown::Update()
{
	if ( m_bDestory || this->m_pTarget->GetIsDie() )
	{
		this->m_pTarget->SetStopTIme( 0.f );
		return CEffectBridge::Event_DestroyObject;
	}

	if ( this->m_bHitTarget )
	{
		if ( this->m_pAnimation )
		{
			this->m_pAnimation->UpdateAnim();

			if ( m_byLockDownLevel == 0 && this->m_pAnimation->GetIsAnimEnd() )
			{
				m_byLockDownLevel = 1;
				this->m_pAnimation->ChangeAnimation( L"LockDown_Loop" );
			}
			else if ( this->m_byLockDownLevel == 1 && this->m_fDestoryTime <= 0.f )
			{
				this->m_byLockDownLevel = 2;
				this->m_pAnimation->ChangeAnimation( L"LockDown_End" );
			}
			else if ( this->m_byLockDownLevel == 2 && this->m_pAnimation->GetIsAnimEnd() )
			{
				this->m_bDestory = true;
			}

		}

		this->m_fDestoryTime -= GET_TIME;
	}
	else
	{
		static D3DXVECTOR3 vUp( 0.f, -1.f, 0.f );

		D3DXVECTOR3 vPos = this->m_pEffectTransform->GetPos();
		D3DXVECTOR3 vTargetPos = this->m_pTarget->GetPos();

		this->m_fLerpT += GET_TIME * 1.f;

		D3DXVec3Lerp( &vPos, &m_vBeginPos, &vTargetPos, this->m_fLerpT );
		this->m_pEffectTransform->SetPos( vPos );

		D3DXVECTOR3 vDir;
		D3DXVec3Normalize( &vDir, &(vTargetPos - vPos) );

		float fCos = D3DXVec3Dot( &vDir, &vUp );
		float fAngle = acosf( fCos );

		fAngle /= (D3DX_PI / 17.f);

		this->m_byBulletDir = BYTE( fAngle );

		if ( this->m_byBulletDir == 17 )
			this->m_byBulletDir -= 1;

		if ( this->m_fLerpT >= 1.f )
		{
			this->m_bHitTarget = true;
		}

	}

	return CEffectBridge::Event_None;
}

void CEffect_LockDown::Render()
{
	if ( this->m_bHitTarget )
	{
		if ( this->m_pAnimation )
		{
			const FRAME* pCurFrame = this->m_pAnimation->GetCurAnimation();
			const TEX_INFO*	pDrawTexture = NULL;

			if(this->m_vecDrawTexture.size() > (unsigned int)pCurFrame->fIndex )
				pDrawTexture = this->m_vecDrawTexture[pCurFrame->fIndex];

			if ( pDrawTexture )
			{
				this->m_pSprite->SetTransform( &this->m_pEffect->GetWorldMatrix() );

				this->m_pSprite->Draw(
					pDrawTexture->pTexture,
					NULL,
					&D3DXVECTOR3( pDrawTexture->ImageInfo.Width * 0.5f, pDrawTexture->ImageInfo.Height * 0.5f, 0.f ),
					NULL,
					D3DCOLOR_ARGB( 125, 255, 255, 255 )
				);
			}
		}
	}
	else
	{
		if ( this->m_vecBulletTexture.size() > this->m_byBulletDir )
		{
			const TEX_INFO*	pDrawTexture = this->m_vecBulletTexture[this->m_byBulletDir];

			if ( pDrawTexture )
			{
				this->m_pSprite->SetTransform( &this->m_pEffect->GetWorldMatrix() );

				this->m_pSprite->Draw(
					pDrawTexture->pTexture,
					NULL,
					&D3DXVECTOR3( pDrawTexture->ImageInfo.Width * 0.5f, pDrawTexture->ImageInfo.Height * 0.5f, 0.f ),
					NULL,
					D3DCOLOR_ARGB( 255, 255, 255, 255 )
				);
			}
		}
	}
}

void CEffect_LockDown::Release()
{
	
}
