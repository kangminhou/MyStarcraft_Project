#include "stdafx.h"
#include "GameEntity.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "TimeMgr.h"

#include "Animation.h"
#include "EntityPattern.h"
#include "Astar.h"


CGameEntity::CGameEntity()
	: m_wstrStateKey(L"")
	, m_pAnimCom(NULL)
	, m_pCurActPattern(NULL)
{
	ZeroMemory( &this->m_tInfoData, sizeof( COMMON_DATA ) );
	ZeroMemory( &this->m_tGroundAttWeapon, sizeof( ATTACK_DATA ) );
	ZeroMemory( &this->m_tAirAttWeapon, sizeof( ATTACK_DATA ) );

	this->m_tGroundAttWeapon.pAttackEntity = this->m_tAirAttWeapon.pAttackEntity = this;
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

RECT CGameEntity::GetColRect() const
{
	return this->m_tColRect;
}

HRESULT CGameEntity::Initialize( void )
{
	//this->m_pAStar = new CAStar;

	this->m_pAnimCom = new CAnimation;
	this->m_pAnimCom->Initialize();
	this->AddComponent( this->m_pAnimCom );

	this->InitAnimation();
	this->InitPattern();

	for each (auto pPattern in m_mapPatterns)
	{
		if ( pPattern.second )
		{
			pPattern.second->SetGameEntity( this );
			this->AddComponent( pPattern.second );
		}
	}

	this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey );
	this->UpdateDirAnimIndex();

	this->CollisionUpdate();

	CGameObject::Initialize();

	return S_OK;
}

int CGameEntity::Update( void )
{
	if ( this->m_bScrollMove )
		this->UpdateMatrix();

	if ( this->m_pCurActPattern )
		this->m_pCurActPattern->Update();

	if ( this->m_pAnimCom )
		this->m_pAnimCom->UpdateAnim();
	
	this->CollisionUpdate();
	//this->CollisionCheck();

	return 0;
}

void CGameEntity::Render( void )
{
	/* 그릴 텍스쳐를 찾아냄.. */
	const TEX_INFO* pDrawTexture = NULL;
	const FRAME* pCurAnimation = this->m_pAnimCom->GetCurAnimation();

	size_t u = (size_t)pCurAnimation->fIndex;
	if ( pCurAnimation && this->m_vecTexture.size() > (size_t)pCurAnimation->fIndex )
		pDrawTexture = this->m_vecTexture[(unsigned int)(pCurAnimation->fIndex)];

	/* 그림이 중앙이 객체의 좌표가 되도록 설정.. */
	if ( pDrawTexture )
	{
		float fX = pDrawTexture->ImageInfo.Width * 0.5f;
		float fY = pDrawTexture->ImageInfo.Height * 0.5f;
		this->DrawTexture( pDrawTexture, this->GetWorldMatrix(), D3DXVECTOR3( fX, fY, 0.f ) );
	}

	this->DrawRect( this->m_tColRect );

}

void CGameEntity::Release( void )
{
}

void CGameEntity::UpdatePosition()
{
	this->CollisionUpdate();
	this->CollisionCheck();

	CGameObject::UpdatePosition();
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
	this->Translate( this->m_tInfoData.fSpeed * GET_TIME );
}

void CGameEntity::UpdateDirAnimIndex()
{
	D3DXVECTOR3 vUp( 0.f, -1.f, 0.f );
	D3DXVECTOR3 vTempDir = this->GetTransform()->GetDir();
	float fAngle = D3DXVec3Dot( &vUp, &(vTempDir) );
	fAngle = acosf( fAngle );

	if ( vTempDir.x < 0.f )
		this->SetSize( -1.f, 1.f );
	else
		this->SetSize( 1.f, 1.f );

	fAngle /= (D3DX_PI / 17.f);

	BYTE byDirAnimIndex = BYTE( fAngle );

	if ( byDirAnimIndex == 17 )
		byDirAnimIndex -= 1;
	else if ( byDirAnimIndex > 17 )
	{
		int a = 10;
	}
	else if ( byDirAnimIndex < 0 )
	{
		int a = 10;
	}

	if ( byDirAnimIndex != this->m_byDirAnimIndex )
	{
		this->m_byDirAnimIndex = byDirAnimIndex;
		this->ChangeDirAnimTexture();
	}
}

void CGameEntity::LookPos( const D3DXVECTOR3 & _vPos )
{
	D3DXVECTOR3 vTempDir;
	D3DXVec3Normalize( &vTempDir, &(_vPos - this->GetPos()) );
	
	this->SetDir( vTempDir );
	this->UpdateDirAnimIndex();
}

void CGameEntity::RenderSelectTexture( bool _bPlayer )
{
	if ( _bPlayer )
	{
		float fX = this->m_pSelectTexture[0]->ImageInfo.Width * 0.5f;
		float fY = this->m_pSelectTexture[0]->ImageInfo.Height * 0.5f;
		this->DrawTexture( this->m_pSelectTexture[0], this->GetWorldMatrix(), D3DXVECTOR3( fX, fY, 0.f ) );
	}
	else
	{
		float fX = this->m_pSelectTexture[1]->ImageInfo.Width * 0.5f;
		float fY = this->m_pSelectTexture[1]->ImageInfo.Height * 0.5f;
		this->DrawTexture( this->m_pSelectTexture[1], this->GetWorldMatrix(), D3DXVECTOR3( fX, fY, 0.f ) );
	}
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

void CGameEntity::CollisionUpdate()
{
	this->m_tColRect.left = (LONG)(this->GetPos().x + m_tOriginColRect.left);
	this->m_tColRect.top = (LONG)(this->GetPos().y + m_tOriginColRect.top);
	this->m_tColRect.right = (LONG)(this->GetPos().x + m_tOriginColRect.right);
	this->m_tColRect.bottom = (LONG)(this->GetPos().y + m_tOriginColRect.bottom);
}

void CGameEntity::CollisionCheck()
{
	vector<CGameEntity*> vColEntity;
	
	CObjMgr::GetInstance()->CheckNearEntitys(&vColEntity, this);
	
	if (vColEntity.empty())
		return;
	
	auto SortEntityToPos = [&](CGameEntity* _pDstEntity, CGameEntity* _pSrcEntity) {
		return D3DXVec3Length(&(_pDstEntity->GetPos() - this->GetPos())) > D3DXVec3Length(&(_pSrcEntity->GetPos() - this->GetPos()));
	};
	
	sort(vColEntity.begin(), vColEntity.end(), SortEntityToPos);
	
	CGameEntity* pEntity = vColEntity.front();
	RECT rcCheckEntity = pEntity->GetColRect();

	D3DXVECTOR3 vDir = -this->GetTransform()->GetDir();
	D3DXVECTOR3 vColDir, vMove;

	D3DXVec3Normalize(&vColDir, &(this->GetPos() - pEntity->GetPos()));
	D3DXVec3Normalize(&vMove, &(vDir + vColDir));
	
	//D3DXVECTOR3 vMove(0.f, 0.f, 0.f);
	//
	//if (rcCheckEntity.left < m_tColRect.left)
	//	vMove.x = rcCheckEntity.right - m_tColRect.left;
	//else
	//	vMove.x = m_tColRect.right - rcCheckEntity.left;
	//
	//if(rcCheckEntity.top < m_tColRect.top)
	//	vMove.y = rcCheckEntity.bottom - m_tColRect.top;
	//else
	//	vMove.y = m_tColRect.bottom - rcCheckEntity.top;
	
	this->Translate(vMove * GET_TIME);

}
