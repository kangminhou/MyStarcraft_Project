#include "stdafx.h"
#include "ResourceObj.h"

#include "TextureMgr.h"
#include "ResourceMgr.h"

#include "GameEntity.h"


CResourceObj::CResourceObj()
	: m_pParentEntity(NULL)
	, m_bInit(false)
{
}


CResourceObj::~CResourceObj()
{
	Release();
}

void CResourceObj::SetParentEntity( CGameEntity * _pParentEntity )
{
	this->m_pParentEntity = _pParentEntity;
}

void CResourceObj::SetDrawTexture( const wstring & _wstrObjKey, const wstring & _wstrStateKey, const int & _iStartindex, const int & _iEndIndex )
{
	this->m_vecDrawTexture.clear();

	for ( int i = _iStartindex; i < _iEndIndex - _iStartindex; ++i )
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( _wstrObjKey.c_str(), _wstrStateKey.c_str(), i );
		this->m_vecDrawTexture.push_back( pTexture );
	}
}

void CResourceObj::HideResourceObj()
{
	m_bHideThis = true;
}

HRESULT CResourceObj::Initialize( void )
{
	if ( !m_bInit )
	{
		CGameObject::Initialize();
		this->m_bInit = true;
	}

	this->m_bHideThis = false;
	D3DXMatrixIdentity( &this->m_matWorld );

	return S_OK;
}

int CResourceObj::Update( void )
{
	if ( this->m_bHideThis )
	{
		CResourceMgr::GetInstance()->PushObject( this );
		return Event_EraseObjList;
	}

	m_byDrawIndex = this->m_pParentEntity->GetLookAnimIndex();

	D3DXVECTOR3 vPos = this->m_pParentEntity->GetTransform()->GetLook();
	vPos *= 20.f;
	
	m_matWorld._41 = vPos.x;
	m_matWorld._42 = vPos.y;
	m_matWorld._43 = 0.f;

	//cout << "vPos.x : " << vPos.x << ", vPos.y : " << vPos.y << endl;

	return Event_None;
}

void CResourceObj::Render( void )
{
	const TEX_INFO* pTexture = this->m_vecDrawTexture[m_byDrawIndex];

	if ( pTexture )
	{
		D3DXMATRIX matParentWorld = this->m_pParentEntity->GetWorldMatrix();

		m_matWorld._41 += matParentWorld._41;
		m_matWorld._42 += matParentWorld._42;

		D3DXVECTOR3 vCenter( pTexture->ImageInfo.Width * 0.5f, pTexture->ImageInfo.Height * 0.5f, 0.f );
		this->DrawTexture( pTexture, m_matWorld, vCenter );
	}
}

void CResourceObj::Release( void )
{
	this->m_vecDrawTexture.clear();
}
