#include "StdAfx.h"
#include "GameObject.h"

//D3DXVECTOR3 CGameObject::m_vScroll;
D3DXVECTOR3 CGameObject::m_vScroll = D3DXVECTOR3(0.f, 0.f, 0.f);


/* Setter.. */
void CGameObject::SetPos( const D3DXVECTOR3 & _vPos )
{
	m_pTransform->SetPos( _vPos );
}

void CGameObject::SetPos( const float & _fX, const float & _fY )
{
	m_pTransform->SetPos( _fX, _fY );
}

void CGameObject::SetLook( const D3DXVECTOR3 & _vLook )
{
	m_pTransform->SetLook( _vLook );
}

void CGameObject::SetLook( const float & _fX, const float & _fY )
{
	m_pTransform->SetLook( _fX, _fY );
}

void CGameObject::SetDir( const D3DXVECTOR3 & _vDir )
{
	m_pTransform->SetDir( _vDir );
}

void CGameObject::SetDir( const float & _fX, const float & _fY )
{
	m_pTransform->SetDir( _fX, _fY );
}

void CGameObject::SetSize( const D3DXVECTOR3 & _vSize )
{
	this->m_pTransform->SetSize( _vSize );
}

void CGameObject::SetSize( const float & _fX, const float & _fY )
{
	this->m_pTransform->SetSize( _fX, _fY );
}

void CGameObject::SetObjectType( const eObjectType & eType )
{
	this->m_eType = eType;
}


/* Getter.. */
D3DXVECTOR3 CGameObject::GetPos() const
{
	return m_pTransform->GetPos();
}

D3DXVECTOR3 CGameObject::GetScroll()
{
	return m_vScroll;
}

const CTransform* CGameObject::GetTransform() const
{
	return m_pTransform;
}

wstring CGameObject::GetObjKey( void ) const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_wstrObjKey;
}



HRESULT CGameObject::Initialize( void )
{
	for ( size_t i = 0; i < m_vecComponent.size(); ++i )
	{
		m_vecComponent[i]->SetGameObject( this );
		m_vecComponent[i]->Initialize();
	}

	return S_OK;
}

void CGameObject::AddComponent( CComponent * _pComponent )
{
	this->m_vecComponent.push_back( _pComponent );
}

CGameObject::CGameObject(void)
	: m_wstrObjKey(L"")
{
	m_pTransform = new CTransform;
	m_vecComponent.push_back( m_pTransform );
}

CGameObject::~CGameObject(void)
{
	size_t componentLength = this->m_vecComponent.size();
	for ( size_t i = 0; i < componentLength; ++i )
	{
		safe_delete( this->m_vecComponent[i] );
	}
}
