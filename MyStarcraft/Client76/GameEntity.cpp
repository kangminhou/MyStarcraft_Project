#include "stdafx.h"
#include "GameEntity.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"

#include "Animation.h"


CGameEntity::CGameEntity()
{
}


CGameEntity::~CGameEntity()
{
}

HRESULT CGameEntity::Initialize( void )
{
	this->m_pAnimCom = new CAnimation;
	this->m_pAnimCom->Initialize();

	this->InitAnimation();
	this->InitPattern();

	return S_OK;
}

int CGameEntity::Update( void )
{
	return 0;
}

void CGameEntity::Render( void )
{
	const FRAME* pCurPlayAnim = this->m_pAnimCom->GetCurAnimation();

	if ( pCurPlayAnim )
	{
		const TEX_INFO* pDrawTexture = this->m_vecTexture[(unsigned int)(pCurPlayAnim->fIndex)];

		D3DXVECTOR3 vCenter( pDrawTexture->ImageInfo.Width * 0.5f, pDrawTexture->ImageInfo.Height * 0.5f, 0.f );

		CDevice::GetInstance()->GetSprite()->Draw(
			pDrawTexture->pTexture,
			NULL,
			&vCenter,
			NULL,
			D3DCOLOR_ARGB( 255, 255, 255, 255 )
		);

	}
}

void CGameEntity::Release( void )
{
}

void CGameEntity::PushDirAnimTexture()
{
	const FRAME* pTempCurFrame = this->m_pAnimCom->GetCurAnimation();
	if ( pTempCurFrame )
	{
		m_vecTexture.clear();

		int iAnimLength = int( pTempCurFrame->fMax );
		int iStart = iAnimLength * this->m_byDirAnimIndex;
		int iEnd = iAnimLength + iStart;

		for ( ; iStart < iEnd; ++iStart )
		{
			const TEX_INFO* pAnimTex = CTextureMgr::GetInstance()->GetTexture( this->GetObjKey().c_str(), m_wstrStateKey.c_str(), iStart );
			m_vecTexture.push_back( pAnimTex );
		}
	}
}

void CGameEntity::UpdateDirAnimIndex()
{
	D3DXVECTOR3 vUp( 0.f, -1.f, 0.f );
	D3DXVECTOR3 vTempDir = this->GetTransform()->GetDir();
	float fAngle = D3DXVec3Dot( &vUp, &(vTempDir) );
	fAngle = acosf( fAngle );

	if ( vTempDir.x < 0.f )
		this->SetSize( -1.f, 0.f );
	else
		this->SetSize( 1.f, 0.f );

	fAngle /= (D3DX_PI / 17.f);

	BYTE byDirAnimIndex = BYTE( fAngle );

	if ( byDirAnimIndex == 17 )
		byDirAnimIndex -= 1;

	if ( byDirAnimIndex != this->m_byDirAnimIndex )
	{
		this->UpdateDirAnimIndex();
		this->m_byDirAnimIndex = byDirAnimIndex;
	}
}

bool CGameEntity::CheckAlertEntity( const eObjectType& eObjectType )
{
	/* eObjectType 타입의 주변 오브젝트를 검사.. */
	list<CGameObject*>* pCheckList = CObjMgr::GetInstance()->GetList( eObjectType );

	/* 나중에 공간 분할을 통해 연산을 줄이기.. */
	for each (CGameObject* pObject in (*pCheckList))
	{
		float fScope = m_tInfoData.iScope * Object_Scope_Mul;

		D3DXVECTOR3 vMyPos = this->GetTransform()->GetPos();
		D3DXVECTOR3 vCheckObjPos = this->GetTransform()->GetPos();

		if ( D3DXVec3Length( &(vMyPos - vCheckObjPos) ) <= fScope )
			return true;
	}

	return false;
}
