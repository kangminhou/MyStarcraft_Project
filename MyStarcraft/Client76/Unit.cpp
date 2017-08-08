#include "stdafx.h"
#include "Unit.h"

#include "TextureMgr.h"
#include "Device.h"

#include "Animation.h"


CUnit::CUnit()
{
}


CUnit::~CUnit()
{
	Release();
}

HRESULT CUnit::Initialize( void )
{
	this->m_wstrStateKey = L"Idle";

	/* 무기 세팅.. */
	//this->m_pGroundAttWeapon = NULL;
	//this->m_pAirAttWeapon = NULL;

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.iMaxHp = this->m_tInfoData.iCurHp = 50;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = 1.5f;
	this->m_tInfoData.iScope = 7;

	/* 생성 데이터 초기화.. */
	this->m_tGenerateData.fGenerateTime = 1.f;
	this->m_tGenerateData.iRequireMineral = 50;
	this->m_tGenerateData.iRequireGas = 0;
	this->m_tGenerateData.iRequirePopulation = 1;

	CGameEntity::Initialize();

	return S_OK;
}

int CUnit::Update( void )
{
	int iResult = 0;

	return iResult;
}

void CUnit::Render( void )
{
	const TEX_INFO* pDrawTexture = NULL;
	const FRAME* pCurAnimation = this->m_pAnimCom->GetCurAnimation();

	if ( pCurAnimation && this->m_vecTexture.size() < (size_t)pCurAnimation->fIndex )
		pDrawTexture = this->m_vecTexture[pCurAnimation->fIndex];

	if ( pDrawTexture )
	{
		float fX = pDrawTexture->ImageInfo.Width * 0.5f;
		float fY = pDrawTexture->ImageInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(
			pDrawTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fX, fY, 0.f ),
			NULL,
			D3DCOLOR_ARGB( 255, 255, 255, 255 )
		);
	}
}

void CUnit::Release( void )
{
	
}

void CUnit::DecideDirAnimIndex()
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

void CUnit::UpdateDir()
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
		this->DecideDirAnimIndex();
		this->m_byDirAnimIndex = byDirAnimIndex;
	}
}