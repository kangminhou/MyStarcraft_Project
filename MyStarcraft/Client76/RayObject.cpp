#include "stdafx.h"
#include "RayObject.h"

#include "TextureMgr.h"
#include "KeyMgr.h"
#include "Mouse.h"
#include "TimeMgr.h"


CRayObject::CRayObject()
{
}


CRayObject::~CRayObject()
{
}

HRESULT CRayObject::Initialize( void )
{
	m_pTex = CTextureMgr::GetInstance()->GetTexture( L"Marine", L"Idle", 0 );

	CGameObject::Initialize();

	fSpeed = 100.f;

	return S_OK;
}

int CRayObject::Update( void )
{
	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_RBUTTON ) )
	{
		m_vDestination = CMouse::GetInstance()->GetPos() + m_vScroll;
		if ( !m_bMove )
			m_bMove = true;

		D3DXVECTOR3 vDir;
		D3DXVec3Normalize( &vDir, &(m_vDestination - this->GetPos()) );
		const_cast<CTransform*>(this->GetTransform())->SetDir( vDir );
	}

	if ( m_bMove )
	{
		this->Translate( fSpeed * GET_TIME );

		if ( D3DXVec3Length( &(this->m_vDestination - this->GetPos()) ) <= 1.f )
			m_bMove = false;

	}

	return 0;
}

void CRayObject::Render( void )
{
	DrawTexture( m_pTex, this->GetWorldMatrix(), D3DXVECTOR3( m_pTex->ImageInfo.Width * 0.5f, m_pTex->ImageInfo.Height * 0.5f, 0.f ) );
}

void CRayObject::Release( void )
{
}
