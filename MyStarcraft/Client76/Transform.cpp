#include "stdafx.h"
#include "Transform.h"

#include "GameObject.h"

#include "ObjMgr.h"


CTransform::CTransform()
{
}


CTransform::~CTransform()
{
	Release();
}

/* ================================ Setter.. ================================ */
void CTransform::SetPos( const D3DXVECTOR3 & _vPos )
{
	D3DXVECTOR3 vPrevPos = m_tInfo.vPos;

	m_tInfo.vPos = _vPos;

	if ( this->GetGameObject() )
		CObjMgr::GetInstance()->ReAdjustmentSpace( vPrevPos, const_cast<CGameObject*>(this->GetGameObject()) );

	this->UpdateTransform();
}

void CTransform::SetPos( const float & _fX, const float & _fY )
{
	this->SetPos( D3DXVECTOR3( _fX, _fY, 0.f ) );
}

void CTransform::SetLook( const D3DXVECTOR3 & _vLook )
{
	m_tInfo.vLook = _vLook;
}

void CTransform::SetLook( const float & _fX, const float & _fY )
{
	this->SetLook( D3DXVECTOR3( _fX, _fY, 0.f ) );
}

void CTransform::SetDir( const D3DXVECTOR3 & _vDir )
{
	m_tInfo.vDir = _vDir;
}

void CTransform::SetDir( const float & _fX, const float & _fY )
{
	this->SetDir( D3DXVECTOR3( _fX, _fY, 0.f ) );
}

void CTransform::SetSize( const D3DXVECTOR3 & _vSize )
{
	m_tInfo.vSize = _vSize;

	this->UpdateTransform();
}

void CTransform::SetSize( const float & _fX, const float & _fY )
{
	this->SetSize( D3DXVECTOR3( _fX, _fY, 1.f ) );
}



/* ================================ Getter.. ================================ */
const INFO* CTransform::GetInfo()
{
	return &m_tInfo;
}

D3DXVECTOR3 CTransform::GetPos() const
{
	return m_tInfo.vPos;
}

D3DXVECTOR3 CTransform::GetDir() const
{
	return m_tInfo.vDir;
}

D3DXVECTOR3 CTransform::GetLook() const
{
	return m_tInfo.vLook;
}

D3DXVECTOR3 CTransform::GetSize() const
{
	return m_tInfo.vSize;
}




void CTransform::Initialize()
{
	memset( &m_tInfo, 0, sizeof( INFO ) );
	m_tInfo.vLook = D3DXVECTOR3( 1.f, 0.f, 0.f );
	m_tInfo.vSize = D3DXVECTOR3( 1.f, 1.f, 1.f );
}

void CTransform::Release()
{
}

void CTransform::Translate( float _fSpeed, eMoveKind _eMoveKind )
{
	switch ( _eMoveKind )
	{
		case Move_World:
			SetPos(m_tInfo.vPos + D3DXVECTOR3( 1.f, 1.f, 0.f ) * _fSpeed);
			break;
		case Move_Local:
			SetPos(m_tInfo.vPos + m_tInfo.vDir * _fSpeed);
			break;
	}

	this->UpdateTransform();
}

void CTransform::UpdateTransform()
{
	if ( this->GetGameObject() )
		const_cast<CGameObject*>(this->GetGameObject())->UpdateMatrix();

}
