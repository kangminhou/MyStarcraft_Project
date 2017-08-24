#include "stdafx.h"
#include "ResourceMgr.h"

#include "ObjMgr.h"

#include "ResourceObj.h"
#include "Mineral.h"
#include "Gas.h"

IMPLEMENT_SINGLETON(CResourceMgr)


CResourceMgr::CResourceMgr()
{
}


CResourceMgr::~CResourceMgr()
{
	Release();
}

void CResourceMgr::Initialize()
{
	for ( int i = 0; i < 50; ++i )
	{
		this->MakeObject();
	}
}

void CResourceMgr::Release()
{
	while ( !this->m_queueResourceObject.empty() )
	{
		safe_delete( this->m_queueResourceObject.front() );
		this->m_queueResourceObject.pop();
	}
}

CResourceObj * CResourceMgr::PopObject( const eResourceType& _eType, CGameEntity * _pEntity )
{
	CResourceObj* pOut = this->Pop();

	pOut->SetParentEntity( _pEntity );

	if ( _eType == CResourceMgr::Resource_Mineral )
		pOut->SetDrawTexture( L"Mineral", L"All", 0, 17 );
	else if ( _eType == CResourceMgr::Resource_Mineral )
		pOut->SetDrawTexture( L"gasorb", L"All", 0, 17 );
	else
		return nullptr;

	pOut->Initialize();

	return pOut;
}

void CResourceMgr::PushObject( CResourceObj * _pObj )
{
	this->m_queueResourceObject.push( _pObj );
}

CGameEntity* CResourceMgr::MakeResource( const D3DXVECTOR3 & _vPos, const eResourceType& _eMakeType, const eObjectType & _eType )
{
	CGameEntity* pOut = NULL;
	D3DXVECTOR3 vRealPos;

	switch ( _eMakeType )
	{
		case CResourceMgr::Resource_Mineral:
		{
			pOut = new CMineral;
			vRealPos = _vPos - D3DXVECTOR3( float( int( _vPos.x ) % TILECX ), float( int( _vPos.y ) % TILECY - 16 ), 0.f );
			//_vPos -= D3DXVECTOR3( float( int( _vPos.x ) % TILECX ), float( int( _vPos.y ) % TILECY ), 0.f );
			//pOut->SetPos( vRealPos );
		}
			break;
	
		case CResourceMgr::Resource_Gas:
		{
			pOut = new CGas;
			vRealPos = _vPos - D3DXVECTOR3( float( int( _vPos.x ) % TILECX ), float( int( _vPos.y ) % TILECY ), 0.f );
			//pOut->SetPos( vRealPos );
		}
		break;

		default:
			return nullptr;

	}

	pOut->Initialize();
	pOut->SetObjectType( _eType );
	pOut->SetPos( vRealPos );

	if ( _eMakeType == CResourceMgr::Resource_Mineral )
		dynamic_cast<CMineral*>(pOut)->UpdateTileData();
	else if ( _eMakeType == CResourceMgr::Resource_Gas )
		dynamic_cast<CGas*>(pOut)->UpdateTileData();
	
	CObjMgr::GetInstance()->GetList( _eType )->push_back( pOut );

	return nullptr;
}

void CResourceMgr::MakeObject()
{
	m_queueResourceObject.push( new CResourceObj );
}

CResourceObj * CResourceMgr::Pop()
{
	if ( m_queueResourceObject.empty() )
	{
		for ( int i = 0; i < 10; ++i )
		{
			this->MakeObject();
		}
	}

	CResourceObj* pOut = m_queueResourceObject.front();
	m_queueResourceObject.pop();
	return pOut;
}
