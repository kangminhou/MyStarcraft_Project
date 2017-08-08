#include "stdafx.h"
#include "GameEntity.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"

#include "Animation.h"
#include "EntityPattern.h"


CGameEntity::CGameEntity()
	: m_wstrStateKey(L"")
	, m_pAnimCom(NULL)
	, m_pCurActPattern(NULL)
{
	ZeroMemory( &m_tInfoData, sizeof( COMMON_DATA ) );
}


CGameEntity::~CGameEntity()
{
	this->Release();
}

void CGameEntity::SetCurHp( const float & fHp )
{
	if ( fHp > this->m_tInfoData.fMaxHp )
		this->m_tInfoData.fCurHp = this->m_tInfoData.fMaxHp;
	else if ( fHp < 0.f )
		this->m_tInfoData.fCurHp = 0.f;
	else
		this->m_tInfoData.fCurHp = fHp;
}

float CGameEntity::GetCurHp() const
{
	return this->m_tInfoData.fCurHp;
}

float CGameEntity::GetSpeed() const
{
	return m_tInfoData.fSpeed;
}

HRESULT CGameEntity::Initialize( void )
{
	this->m_pAnimCom = new CAnimation;
	this->m_pAnimCom->Initialize();

	this->InitAnimation();
	this->InitPattern();

	for each ( auto pPattern in m_mapPatterns )
	{
		pPattern.second->SetGameEntity( this );
	}

	return S_OK;
}

int CGameEntity::Update( void )
{
	if ( this->m_pCurActPattern )
		this->m_pCurActPattern->Update();

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
	if ( this->m_pAnimCom )
		safe_delete( this->m_pAnimCom );
}

bool CGameEntity::CheckAlertEntity( const eObjectType & eObjectType, vector<CGameEntity*>* pVecEntitys /*= NULL*/ )
{
	/* eObjectType 타입의 주변 오브젝트를 검사.. */
	bool bOut = false;

	list<CGameObject*>* pCheckList = CObjMgr::GetInstance()->GetList( eObjectType );

	/* 나중에 공간 분할을 통해 연산을 줄이기.. */
	for each (CGameObject* pObject in (*pCheckList))
	{
		float fScope = m_tInfoData.iScope * Object_Scope_Mul;

		D3DXVECTOR3 vMyPos = this->GetTransform()->GetPos();
		D3DXVECTOR3 vCheckObjPos = this->GetTransform()->GetPos();

		/* 플레이어의 시야 안에 있는 객체들을 검사.. */
		if ( D3DXVec3Length( &(vMyPos - vCheckObjPos) ) <= fScope )
		{
			if ( !bOut )
				bOut = true;

			/* 시야 안에 있는 객체들을 넣어주길 원한다면 넣음.. */
			if ( pVecEntitys )
			{
				CGameEntity* pGameEntity = dynamic_cast<CGameEntity*>(pObject);
				if ( pGameEntity )
					pVecEntitys->push_back( pGameEntity );
			}
			else
				return true;

		}
	}

	return bOut;
}

void CGameEntity::MoveEntity()
{
	this->Translate( D3DXVECTOR3( this->m_tInfoData.fSpeed, 0.f, 0.f ) );
}

void CGameEntity::ChangeDirAnimTexture()
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
		this->ChangeDirAnimTexture();
		this->m_byDirAnimIndex = byDirAnimIndex;
	}
}
