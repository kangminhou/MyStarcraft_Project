#include "StdAfx.h"
#include "GameObject.h"

#include "Device.h"
#include "TextureMgr.h"

//D3DXVECTOR3 CGameObject::m_vScroll;
D3DXVECTOR3 CGameObject::m_vScroll = D3DXVECTOR3(0.f, 0.f, 0.f);
bool CGameObject::m_bScrollMove = false;


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

void CGameObject::SetObjKey( const wstring & _wstrObjKey )
{
	this->m_wstrObjKey = _wstrObjKey;
}


/* Getter.. */
D3DXVECTOR3 CGameObject::GetPos() const
{
	if ( !this->m_pTransform )
		return D3DXVECTOR3( 0.f, 0.f, 0.f );

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

eObjectType CGameObject::GetObjectType() const
{
	return this->m_eType;
}

D3DXMATRIX CGameObject::GetWorldMatrix() const
{
	return this->m_matWorld;
}



HRESULT CGameObject::Initialize( void )
{
	for ( size_t i = 0; i < m_vecComponent.size(); ++i )
	{
		this->m_vecComponent[i]->SetGameObject( this );
		this->m_vecComponent[i]->Initialize();
	}

	this->m_pDragTexture = CTextureMgr::GetInstance()->GetTexture( L"Drag" );

	this->m_pTempSprite = CDevice::GetInstance()->GetSprite();

	return S_OK;
}

void CGameObject::UpdatePosition( const D3DXVECTOR3& vPrevPos )
{
	this->UpdateMatrix();
}

void CGameObject::AddComponent( CComponent * _pComponent )
{
	this->m_vecComponent.push_back( _pComponent );
}

void CGameObject::Translate( const float& _fSpeed )
{
	this->m_pTransform->Translate( _fSpeed);
}

void CGameObject::Translate(const D3DXVECTOR3& _vMove)
{
	this->m_pTransform->Translate(_vMove);
}

void CGameObject::UpdateMatrix( void )
{
	D3DXMATRIX matTrans, matScale;

	D3DXVECTOR3 vSize = this->m_pTransform->GetSize();
	D3DXVECTOR3 vPos = this->m_pTransform->GetPos();

	D3DXMatrixScaling( &matScale, vSize.x, vSize.y, vSize.z );
	D3DXMatrixTranslation( &matTrans, vPos.x - m_vScroll.x, vPos.y - m_vScroll.y, 0.f );

	m_matWorld = matScale * matTrans;
}

void CGameObject::DrawTexture( const TEX_INFO * _pDrawTexture, const D3DXMATRIX & _matWorld, const D3DXVECTOR3 & _vCenterPos, const D3DCOLOR& _color )
{
	if ( _pDrawTexture )
	{
		this->m_pTempSprite->SetTransform( &_matWorld );

		this->m_pTempSprite->Draw(
			_pDrawTexture->pTexture,
			NULL,
			&_vCenterPos,
			NULL,
			_color
		);
	}
}

void CGameObject::DrawRect( const RECT & _rc, const D3DCOLOR _color )
{
	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXVECTOR3 vPoint[4];

	vPoint[0] = D3DXVECTOR3( FLOAT(_rc.left), FLOAT(_rc.top), 0.f );
	vPoint[1] = D3DXVECTOR3( FLOAT(_rc.left), FLOAT(_rc.top), 0.f );
	vPoint[2] = D3DXVECTOR3( FLOAT(_rc.left), FLOAT(_rc.bottom), 0.f );
	vPoint[3] = D3DXVECTOR3( FLOAT(_rc.right), FLOAT(_rc.top), 0.f );

	for ( int i = 0; i < 4; ++i )
	{
		D3DXMatrixTranslation( &matTrans, vPoint[i].x, vPoint[i].y, vPoint[i].z );
		if ( i % 2 )
			D3DXMatrixScaling( &matScale, 1.f, FLOAT(_rc.bottom - _rc.top), 1.f );
		else
			D3DXMatrixScaling( &matScale, FLOAT( _rc.right - _rc.left), 1.f, 1.f );

		matWorld = matScale * matTrans;	
		CDevice::GetInstance()->GetSprite()->SetTransform( &matWorld );
		CDevice::GetInstance()->GetSprite()->Draw(
			m_pDragTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB( 255, 255, 255, 255 )
		);
	}
}

void CGameObject::DrawFont( const D3DXMATRIX & _matWorld, const wstring & wstrShow, const D3DCOLOR & _color )
{
	CDevice::GetInstance()->GetSprite()->SetTransform( &_matWorld );

	CDevice::GetInstance()->GetFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		wstrShow.c_str(),
		lstrlen( wstrShow.c_str() ),
		NULL,
		NULL,
		_color
	);
}

CGameObject::CGameObject(void)
	: m_wstrObjKey(L"")
{
	m_pTransform = new CTransform;
	m_vecComponent.push_back( m_pTransform );
	D3DXMatrixIdentity( &m_matWorld );
}

CGameObject::~CGameObject(void)
{
	size_t componentLength = this->m_vecComponent.size();
	for ( size_t i = 0; i < componentLength; ++i )
	{
		safe_delete( this->m_vecComponent[i] );
	}
}
