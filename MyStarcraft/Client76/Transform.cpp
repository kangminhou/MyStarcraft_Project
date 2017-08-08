#include "stdafx.h"
#include "Transform.h"


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
	m_tInfo.vPos = _vPos;

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

D3DXMATRIX CTransform::GetWorldMatrix() const
{
	return m_tInfo.matWorld;
}




void CTransform::Initialize()
{
	memset( &m_tInfo, 0, sizeof( INFO ) );
	D3DXMatrixIdentity( &m_tInfo.matWorld );
	m_tInfo.vLook = D3DXVECTOR3( 1.f, 0.f, 0.f );
	m_tInfo.vSize = D3DXVECTOR3( 1.f, 1.f, 1.f );
}

void CTransform::Release()
{
}

void CTransform::Translate( D3DXVECTOR3 vMove, eMoveKind eMoveKind )
{
	switch ( eMoveKind )
	{
		case Move_World:
			m_tInfo.vPos += vMove;
			break;
		case Move_Local:
			m_tInfo.vPos += D3DXVECTOR3( vMove.x * m_tInfo.vDir.x, vMove.y * m_tInfo.vDir.y, 0.f );
			break;
	}

	this->UpdateTransform();
}

void CTransform::UpdateMatrix()
{
	D3DXMATRIX matTrans, matScale;

	D3DXMatrixScaling( &matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, m_tInfo.vSize.z );
	D3DXMatrixTranslation( &matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z );

	m_tInfo.matWorld = matScale * matTrans;
}

void CTransform::UpdateTransform()
{
}
