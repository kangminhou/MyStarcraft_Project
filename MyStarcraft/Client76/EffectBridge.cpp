#include "stdafx.h"
#include "EffectBridge.h"

#include "Animation.h"
#include "TextureMgr.h"
#include "Device.h"

#include "Effect.h"


CEffectBridge::CEffectBridge()
	: m_bFirstInitialize(true)
{
}


CEffectBridge::~CEffectBridge()
{
	this->Release();
}

void CEffectBridge::SetVecTexture( const wstring & _wstrObjKey, const wstring & _wstrStateKey, const int& _iStart, const int & _iCount )
{
	this->m_vecDrawTexture.clear();

	for ( int i = _iStart; i < _iStart + _iCount; ++i )
	{
		const TEX_INFO* pPushTexture = CTextureMgr::GetInstance()->GetTexture( _wstrObjKey.c_str(), _wstrStateKey.c_str(), i );
		this->m_vecDrawTexture.push_back( pPushTexture );
	}
}

void CEffectBridge::SetVecTexture( const vector<const TEX_INFO*>* _pVecTexture )
{
	for ( int i = 0; i < _pVecTexture->size(); ++i )
	{
		this->m_vecDrawTexture.push_back( (*_pVecTexture)[i] );
	}
}

void CEffectBridge::Initialize()
{
	if ( m_bFirstInitialize )
	{
		this->m_pEffect = const_cast<CEffect*>(dynamic_cast<const CEffect*>(this->GetGameObject()));

		if ( !this->m_pEffect )
		{
			ERROR_MSG( L"Effect °´Ã¼°¡ ¾øÀ½ ( CEffectBridge Class - Initialize Function )" );
			return;
		}

		this->m_pEffectTransform = const_cast<CTransform*>(this->m_pEffect->GetTransform());

		this->m_pAnimation = new CAnimation;
		this->m_pAnimation->Initialize();
		this->m_pAnimation->SetGameObject( this->m_pEffect );

		this->m_bFirstInitialize = false;
	}
	else
	{
		this->m_pAnimation->ResetFrame();
	}
}

void CEffectBridge::Render()
{
	if ( this->m_pAnimation )
	{
		const FRAME* pCurFrame = this->m_pAnimation->GetCurAnimation();
		const TEX_INFO*	pDrawTexture = NULL;

		if(this->m_vecDrawTexture.size() > (unsigned int)pCurFrame->fIndex )
			pDrawTexture = this->m_vecDrawTexture[pCurFrame->fIndex];

		if ( pDrawTexture )
		{
			CDevice::GetInstance()->GetSprite()->SetTransform( &this->m_pEffect->GetWorldMatrix() );

			CDevice::GetInstance()->GetSprite()->Draw(
				pDrawTexture->pTexture,
				NULL,
				&D3DXVECTOR3( pDrawTexture->ImageInfo.Width * 0.5f, pDrawTexture->ImageInfo.Height * 0.5f, 0.f ),
				NULL,
				D3DCOLOR_ARGB( 255, 255, 255, 255 )
			);
		}
	}
}

void CEffectBridge::Release()
{
	safe_delete( this->m_pAnimation );
	this->m_vecDrawTexture.clear();
}
