#include "stdafx.h"
#include "UI.h"

#include "TextureMgr.h"


CUI::CUI()
	: m_iDrawIndex(0)
	, m_pAnim(NULL)
{
}


CUI::~CUI()
{
	if ( this->m_pAnim )
		safe_delete( this->m_pAnim );

	m_vecDrawTexture.clear();
	
	for ( auto iter = m_mapAllTexture.begin(); iter != m_mapAllTexture.end(); ++iter )
	{
		iter->second.clear();
	}
	m_mapAllTexture.clear();
}

void CUI::SetDrawIndex( const int & _iIndex )
{
	this->m_iDrawIndex = _iIndex;
}

void CUI::AddFrame( const wstring & _wstrName, const FRAME & tFrame, const CAnimation::eAnim_Kind& eKind )
{
	this->m_pAnim->AddAnimation( _wstrName, tFrame, eKind );
}

bool CUI::ChangeFrame( const wstring & _wstrName )
{
	return this->m_pAnim->ChangeAnimation( _wstrName );
}

void CUI::DecideFrame( const wstring & _wstrName, const FRAME & tFrame, const CAnimation::eAnim_Kind & eKind )
{
	this->m_pAnim->ResetFrame();
	this->m_pAnim->AddAnimation( _wstrName, tFrame, eKind );
	this->m_pAnim->ChangeAnimation( _wstrName );
}

void CUI::SetUIKind( const eUI_Kind & _eKind )
{
	m_eKind = _eKind;
}

void CUI::AddTexture( const wstring & _wstrTextureKey, const wstring & _wstrObjKey, const wstring & _wstrStateKey, const int & _iCount )
{
	vector<const TEX_INFO*> vecTexture;

	for ( int i = 0; i < _iCount; ++i )
	{
		vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture(
			_wstrObjKey.c_str(), _wstrStateKey.c_str(), i ) );
	}

	this->m_mapAllTexture.insert( make_pair( _wstrTextureKey, vecTexture ) );

}

bool CUI::DecideDrawTexture( const wstring & _wstrTextureKey )
{
	auto& iter = this->m_mapAllTexture.find( _wstrTextureKey );

	if ( iter == this->m_mapAllTexture.end() )
		return false;

	this->m_vecDrawTexture = iter->second;
	return true;
}

void CUI::ChangeDrawTexture( const wstring & _wstrObjKey, const wstring & _wstrStateKey, const int & _iCount )
{
	this->m_vecDrawTexture.clear();

	for ( int i = 0; i < _iCount; ++i )
	{
		this->m_vecDrawTexture.push_back( CTextureMgr::GetInstance()->GetTexture(
			_wstrObjKey.c_str(), _wstrStateKey.c_str(), i ) );
	}
}

void CUI::ChangeDrawTexture( const vector<const TEX_INFO*>& vecDrawTexture )
{
	this->m_vecDrawTexture.clear();

	for ( size_t i = 0; i < vecDrawTexture.size(); ++i )
	{
		this->m_vecDrawTexture.push_back( vecDrawTexture[i] );
	}
}

HRESULT CUI::Initialize( void )
{
	if ( this->m_eKind == CUI::UI_Anim_Loop )
	{
		m_pAnim = new CAnimation;
		m_pAnim->Initialize();
	}

	CGameObject::Initialize();

	return S_OK;
}

int CUI::Update( void )
{
	if ( this->m_eKind == CUI::UI_Anim_Loop )
	{

	}

	return 0;
}

void CUI::Render( void )
{
	const TEX_INFO* pDrawTexture = this->m_vecDrawTexture[m_iDrawIndex];

	this->DrawTexture( pDrawTexture,
					   this->GetWorldMatrix() );
}

void CUI::Render( const int & _iDrawIndex, const D3DXMATRIX& _vDrawWorldMatrix )
{
	const TEX_INFO* pDrawTexture = this->m_vecDrawTexture[_iDrawIndex];

	this->DrawTexture( pDrawTexture,
					   _vDrawWorldMatrix );
}

void CUI::Release( void )
{
	
}
