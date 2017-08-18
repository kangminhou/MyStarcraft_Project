#include "stdafx.h"
#include "Effect.h"

#include "EffectBridge.h"


CEffect::CEffect()
{
}


CEffect::~CEffect()
{
}

void CEffect::SetEffectBridge( CEffectBridge* _pEffectBridge )
{
	this->m_pEffectBridge = _pEffectBridge;
}

void CEffect::SetStateKey( const wstring& _wstrStateKey )
{
	this->m_wstrStateKey = _wstrStateKey;
}

void CEffect::SetVecTexture( const wstring& _wstrObjKey, const wstring& _wstrStateKey, const int& _iStart, const int & _iCount )
{
	if ( this->m_pEffectBridge )
		this->m_pEffectBridge->SetVecTexture( _wstrObjKey, _wstrStateKey, _iStart, _iCount );
}

void CEffect::SetVecTexture( const vector<const TEX_INFO*>* _pVecTexture )
{
	if ( this->m_pEffectBridge && _pVecTexture )
		this->m_pEffectBridge->SetVecTexture( _pVecTexture );
}

wstring CEffect::GetStateKey() const
{
	return this->m_wstrStateKey;
}

const CEffectBridge * CEffect::GetEffectBridge() const
{
	return this->m_pEffectBridge;
}

HRESULT CEffect::Initialize( void )
{
	this->m_pEffectBridge->SetGameObject( this );
	this->m_pEffectBridge->Initialize();

	CGameObject::Initialize();

	return S_OK;
}

int CEffect::Update( void )
{
	int iEvent = this->m_pEffectBridge->Update();

	return iEvent;
}

void CEffect::Render( void )
{
	this->m_pEffectBridge->Render();
}

void CEffect::Release( void )
{
	safe_delete( this->m_pEffectBridge );
}
