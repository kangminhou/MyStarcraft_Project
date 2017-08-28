#include "stdafx.h"
#include "Effect_Nuclear.h"

#include "TimeMgr.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "Effect.h"
#include "Animation.h"
#include "GameEntity.h"


CEffect_Nuclear::CEffect_Nuclear()
{
}


CEffect_Nuclear::~CEffect_Nuclear()
{
}

void CEffect_Nuclear::SetFireEntity( CGameEntity * _pFireEntity )
{
	this->m_pFireEntity = _pFireEntity;
}

void CEffect_Nuclear::Initialize()
{
	this->m_iLevel = 0;

	if ( this->m_bFirstInitialize )
	{
		for ( int i = 0; i < 17; ++i )
		{
			const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Unit", L"NukeMissile", i );
			m_vecNuclearTrunkTexture.push_back( pTexture );
		}

		for ( int i = 0; i < 2; ++i )
		{
			const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Effect", L"NuClearReady", i );
			m_vecReadyTexture.push_back( pTexture );
		}

		for ( int i = 0; i < 26; ++i )
		{
			const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Effect", L"Nuke", i );
			m_vecBombTexture.push_back( pTexture );
		}

		this->m_pObjMgr = CObjMgr::GetInstance();
	}
	else
	{
		this->m_pAnimation->AddAnimation( L"Wait", FRAME( 0.f, 5.f, 2.f, 0.f ), CAnimation::Anim_Loop );
		this->m_pAnimation->AddAnimation( L"Fire", FRAME( 0.f, 1.f, 1.f, 17.f ), CAnimation::Anim_ClampForever );
		this->m_pAnimation->AddAnimation( L"Bomb", FRAME( 0.f, 26.f, 26.f, 0.f ), CAnimation::Anim_ClampForever );

		this->m_pAnimation->ChangeAnimation( L"Wait" );
		this->m_vecDrawTexture = this->m_vecReadyTexture;

		this->m_fWaitTime = 10.f;
		this->m_pFireEntity->SoundPlay( CGameEntity::Sound_ETC, 1 );
		this->m_bNuclearLunchSoundPlay = false;
		this->m_fNuclearLunchWaitTime = 1.3f;
	}

	CEffectBridge::Initialize();

}

int CEffect_Nuclear::Update()
{
	if ( this->m_pAnimation )
	{
		this->m_pAnimation->UpdateAnim();
	}

	//return CEffectBridge::Event_None;

	switch ( this->m_iLevel )
	{
		case 0:
		{
			if ( !this->m_bNuclearLunchSoundPlay )
			{
				this->m_fNuclearLunchWaitTime -= GET_TIME;
				if ( this->m_fNuclearLunchWaitTime <= 0.f )
				{
					this->m_bNuclearLunchSoundPlay = true;
					this->m_pFireEntity->SoundPlay( CGameEntity::Sound_ETC, 2 );
				}
			}
		
			this->m_fWaitTime -= GET_TIME;

			if ( this->m_fWaitTime <= 0.f )
			{
				this->m_iLevel = 1;

				this->m_pAnimation->ChangeAnimation( L"Fire" );
				this->m_vecDrawTexture = this->m_vecNuclearTrunkTexture;

				this->m_vNuclearPos = this->m_pEffect->GetPos();
				this->m_vNuclearPos.y = 0.f;
			}
		}
		break;

		case 1:
		{
			D3DXVECTOR3 vPos = this->m_pEffect->GetPos();
			vPos.y -= this->m_pEffect->GetScroll().y;

			this->m_vNuclearPos.y += GET_TIME * 40.f;
			if ( this->m_vNuclearPos.y >= vPos.y )
			{
				this->m_vNuclearPos.y = vPos.y;

				this->m_iLevel = 2;

				this->m_pAnimation->ChangeAnimation( L"Bomb" );
				this->m_vecDrawTexture = this->m_vecBombTexture;

				vector<CGameEntity*> vecEntity;

				for ( int i = OBJ_TYPE_USER; i <= OBJ_TYPE_USER2; ++i )
				{
					this->m_pObjMgr->CheckNearEntitys( &vecEntity, this->m_pEffect->GetPos(), 2000.f, (eObjectType)i );

					for ( size_t j = 0; j < vecEntity.size(); ++j )
					{
						vecEntity[j]->HitEntity( nullptr, INFINITY );
					}
				}
			}

			D3DXMatrixTranslation( &m_matNuclearWorld, this->m_vNuclearPos.x, this->m_vNuclearPos.y, 0.f );

			//if ( this->m_vNuclearPos.z <= 0.f )
			//{
			//	this->m_iLevel = 2;
			//
			//	this->m_pAnimation->ChangeAnimation( L"Bomb" );
			//	this->m_vecDrawTexture = this->m_vecBombTexture;
			//
			//	vector<CGameEntity*> vecEntity;
			//
			//	for ( int i = OBJ_TYPE_USER; i <= OBJ_TYPE_USER2; ++i )
			//	{
			//		this->m_pObjMgr->CheckNearEntitys( &vecEntity, this->m_pEffect->GetPos(), 300.f, (eObjectType)i );
			//
			//		for ( size_t j = 0; j < vecEntity.size(); ++j )
			//		{
			//			vecEntity[j]->HitEntity( nullptr, 500.f );
			//		}
			//	}
			//}
		}

		break;

		case 2:
		{
			if ( this->m_pAnimation->GetIsAnimEnd() )
			{
				return Event_DestoryObject;
			}
		}
		break;
	}

	return Event_None;
}

void CEffect_Nuclear::Render()
{
	D3DXMATRIX matDraw;
	if ( this->m_iLevel != 1 )
	{
		matDraw = this->m_pEffect->GetWorldMatrix();
	}
	else
	{
		matDraw = m_matNuclearWorld;
	}

	if ( this->m_pAnimation )
	{
		const FRAME* pCurFrame = this->m_pAnimation->GetCurAnimation();
		const TEX_INFO*	pDrawTexture = NULL;

		if(this->m_vecDrawTexture.size() > (unsigned int)pCurFrame->fIndex )
			pDrawTexture = this->m_vecDrawTexture[pCurFrame->fIndex];

		if ( pDrawTexture )
		{
			this->m_pSprite->SetTransform( &matDraw );

			this->m_pSprite->Draw(
				pDrawTexture->pTexture,
				NULL,
				&D3DXVECTOR3( pDrawTexture->ImageInfo.Width * 0.5f, pDrawTexture->ImageInfo.Height * 0.5f, 0.f ),
				NULL,
				D3DCOLOR_ARGB( 254, 255, 255, 255 )
			);
		}
	}
}

void CEffect_Nuclear::Release()
{
	
}
