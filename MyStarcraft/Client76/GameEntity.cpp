#include "stdafx.h"
#include "GameEntity.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "TimeMgr.h"

#include "Animation.h"
#include "EntityPattern.h"
#include "Astar.h"
#include "Background.h"
#include "Weapon.h"

CBackground* CGameEntity::m_pBackground = NULL;


CGameEntity::CGameEntity()
	: m_wstrStateKey(L"")
	, m_pAnimCom(NULL)
	, m_pCurActPattern(NULL)
	, m_bCollision(false)
	, m_pEntityBelongToCorps(NULL)
	, m_pTarget(NULL)
	, m_bDie(false)
	, m_bDestoryEntity(false)
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

void CGameEntity::SetPrevPattern()
{
	if ( !this->m_vecActPatterns.empty() )
	{
		for ( size_t i = this->m_vecActPatterns.size() - 1; ; )
		{
			if ( this->m_vecActPatterns[i] == CGameEntity::Pattern_ChaseTarget ||
				 this->m_vecActPatterns[i] == CGameEntity::Pattern_Hit )
				--i;
			else
			{
				this->SetPattern( this->m_vecActPatterns[i] );
				//this->m_vecActPatterns.pop_back();
				return;
			}

			if ( i <= 0 )
				break;
		}
	}

	this->SetPattern( eGameEntityPattern::Pattern_Idle );
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

void CGameEntity::SetEntityBelongToCorps( CCorps * _pEntityBelongToCorps )
{
	this->m_pEntityBelongToCorps = _pEntityBelongToCorps;
}

void CGameEntity::SetTarget( CGameEntity * _pTarget )
{
	this->m_pTarget = _pTarget;
}




void CGameEntity::SetStandTileIndexList( const list<pair<int, BYTE>>& _standTileIndexList )
{
	this->m_standTileIndexList.clear();
	this->m_standTileIndexList = _standTileIndexList;
}

void CGameEntity::SetEntitySpaceDataKey( const int & _iEntitySpaceDataKey )
{
	this->m_iEntitySpaceDataKey = _iEntitySpaceDataKey;
}

float CGameEntity::GetCurHp() const
{
	return this->m_tInfoData.fCurHp;
}

float CGameEntity::GetMaxHp() const
{
	return this->m_tInfoData.fMaxHp;
}

float CGameEntity::GetSpeed() const
{
	return m_tInfoData.fSpeed;
}

int CGameEntity::GetScope() const
{
	return this->m_tInfoData.iScope;
}

const CCorps * CGameEntity::GetEntityBelongToCorps() const
{
	return this->m_pEntityBelongToCorps;
}

RECT CGameEntity::GetColRect() const
{
	return this->m_tColRect;
}

RECT CGameEntity::GetOriginColRect() const
{
	return this->m_tOriginColRect;
}

bool CGameEntity::GetIsCollision() const
{
	return this->m_bCollision;
}

bool CGameEntity::GetIsDie() const
{
	return this->m_bDie;
}

const list<pair<int, BYTE>>* CGameEntity::GetStandTileIndexList()
{
	return &this->m_standTileIndexList;
}

CGameEntity::ATTACK_DATA CGameEntity::GetGroundAttackData() const
{
	return this->m_tGroundAttWeapon;
}

CGameEntity::ATTACK_DATA CGameEntity::GetAirAttackData() const
{
	return this->m_tAirAttWeapon;
}

float CGameEntity::GetGroundWeaponAttRange() const
{
	return this->m_tGroundAttWeapon.fAttRange;
}

float CGameEntity::GetAirWeaponAttRange() const
{
	return this->m_tAirAttWeapon.fAttRange;
}

CGameEntity * CGameEntity::GetTarget() const
{
	return this->m_pTarget;
}



HRESULT CGameEntity::Initialize( void )
{
	//this->m_pAStar = new CAStar;

	if ( this->m_tAirAttWeapon.pWeapon )
	{
		m_tAirAttWeapon.pWeapon->SetWeaponOwner( this );
		AddComponent( this->m_tAirAttWeapon.pWeapon );
	}
	if ( this->m_tGroundAttWeapon.pWeapon )
	{
		m_tGroundAttWeapon.pWeapon->SetWeaponOwner( this );
		AddComponent( this->m_tGroundAttWeapon.pWeapon );
	}

	this->m_vecActPatterns.reserve( 10 );

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

	this->SetPattern( CGameEntity::Pattern_Idle );

	CGameObject::Initialize();

	return S_OK;
}

int CGameEntity::Update( void )
{
	if ( m_bDie )
	{
		if ( m_bDestoryEntity )
			return Event_DestoryObject;
		else
		{
			if ( m_pAnimCom->GetIsAnimEnd() )
			{
				m_bDestoryEntity = true;
				return Event_DestoryObject;
			}
		}
	}

	if ( this->m_bScrollMove )
		this->UpdateMatrix();

	if ( this->m_pCurActPattern )
		this->m_pCurActPattern->Update();

	if ( this->m_pAnimCom )
		this->m_pAnimCom->UpdateAnim();
	
	this->CollisionUpdate();
	this->CollisionCheck();

	return Event_None;
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

	//D3DXMATRIX matFont;
	//D3DXMatrixTranslation( &matFont, this->GetPos().x, this->GetPos().y + 100.f, this->GetPos().z );
	//
	//TCHAR str[128];
	//swprintf_s( str, L"HP : %f", this->GetCurHp() );
	//this->DrawFont( matFont, str );

	//this->DrawRect( this->m_tColRect );

}

void CGameEntity::Release( void )
{
	this->m_vecTexture.clear();
	this->m_vecActPatterns.clear();
}

void CGameEntity::UpdatePosition( const D3DXVECTOR3& vPrevPos )
{
	this->CollisionUpdate();
	//this->CollisionCheck();

	m_pBackground->EraseUnitData( m_vecSpaceDataKey );
	m_pBackground->UpdateUnitData( this, m_vecSpaceDataKey );

	CObjMgr::GetInstance()->EraseEntitySpaceData( this, this->m_iEntitySpaceDataKey );
	CObjMgr::GetInstance()->InsertEntitySpaceData( this );

	CGameObject::UpdatePosition( vPrevPos );
}

bool CGameEntity::UseSkill( const eGameEntitySkillKind & _eSkillKind, CGameEntity * _pTarget )
{
	switch ( _eSkillKind )
	{
		case CGameEntity::Skill_Heal:
		{

		}
			break;

		default:
			return false;
	}

	return true;
}

bool CGameEntity::CheckAlertEnemy( vector<CGameEntity*>* pVecEntitys, const int & iVecLimitSize )
{
	/* eObjectType 타입의 주변 오브젝트를 검사.. */
	bool bOut = false;

	for ( int i = OBJ_TYPE_USER; i <= OBJ_TYPE_USER2; ++i )
	{
		if ( i == this->GetObjectType() )
			continue;

		if ( !bOut && CObjMgr::GetInstance()->CheckNearEntitys( pVecEntitys, this, (eObjectType)i, iVecLimitSize ) )
			bOut = true;
	}

	return bOut;
}

bool CGameEntity::CheckAlertOurForces( vector<CGameEntity*>* pVecEntitys, const int & iVecLimitSize )
{
	/* eObjectType 타입의 주변 오브젝트를 검사.. */
	bool bOut = false;
	vector<CGameEntity*> vecEntitys;

	CObjMgr::GetInstance()->CheckNearEntitys( &vecEntitys, this, this->GetObjectType() );

	int iTempVecLimitSize = iVecLimitSize;

	size_t iLength = vecEntitys.size();
	for ( size_t i = 0; i < iLength; ++i )
	{
		if ( (vecEntitys[i]->m_tInfoData.fMaxHp - vecEntitys[i]->m_tInfoData.fCurHp) <= EPSILON )
			continue;
		
		if ( !bOut )
			bOut = true;

		if ( iTempVecLimitSize == 0 || !pVecEntitys )
			break;

		pVecEntitys->push_back( vecEntitys[i] );
		--iTempVecLimitSize;
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
	else if ( byDirAnimIndex > 17 || byDirAnimIndex < 0 )
	{
		ERROR_MSG( L"byDirAnimIndex Error" );
		return;
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
	if ( this->m_bDie )
		return;

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

void CGameEntity::HitEntity( CGameEntity * _pAttackedObject, float _fDamage )
{
	if(_fDamage >= 0.f )
		this->m_tInfoData.fCurHp -= _fDamage;

	if ( this->m_tInfoData.fCurHp <= 0.f )
	{
		this->SetPattern( CGameEntity::Pattern_Die );
		return;
	}

	if ( this->m_curActPatternKind == CGameEntity::Pattern_Idle ||
		 this->m_curActPatternKind == CGameEntity::Pattern_MoveAlert ||
		 this->m_curActPatternKind == CGameEntity::Pattern_Patrol ||
		 this->m_curActPatternKind == CGameEntity::Pattern_ChaseTarget)
	{
		this->m_pTarget = _pAttackedObject;
		this->SetPattern( CGameEntity::Pattern_Hit );
	}
}

void CGameEntity::DieEntity()
{
	/* 기계 유닛일 경우 터지는 이펙트.. */
	if ( false )
	{
		/*## 이펙트 터지는 부분.. */
		/*## 이펙트 터지는 부분.. */

		this->m_bDestoryEntity = true;
	}

	this->m_bDie = true;
	this->SetDir( D3DXVECTOR3( 0.f, 0.f, 0.f ) );
	this->m_byDirAnimIndex = 0;

	CObjMgr::GetInstance()->EraseEntitySpaceData( this, this->m_iEntitySpaceDataKey );
	this->m_pBackground->EraseUnitData( m_vecSpaceDataKey );
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
	//return;
	vector<CGameEntity*> vColEntity;
	
	CObjMgr::GetInstance()->CheckEntitysCol( &vColEntity, this );
	
	if ( !vColEntity.empty() )
	{
		auto SortEntityToPos = [&]( CGameEntity* _pDstEntity, CGameEntity* _pSrcEntity ) {
			return D3DXVec3Length( &(_pDstEntity->GetPos() - this->GetPos()) ) > D3DXVec3Length( &(_pSrcEntity->GetPos() - this->GetPos()) );
		};

		sort( vColEntity.begin(), vColEntity.end(), SortEntityToPos );

		CGameEntity* pEntity = vColEntity.front();
		RECT rcCheckEntity = pEntity->GetColRect();

		D3DXVECTOR3 vDir = -this->GetTransform()->GetDir();
		D3DXVECTOR3 vColDir, vMove;

		D3DXVec3Normalize( &vColDir, &(this->GetPos() - pEntity->GetPos()) );
		//D3DXVec3Normalize( &vMove, &(vDir + vColDir) );
		vMove = vDir + vColDir;

		this->Translate( vMove );

		if ( !m_bCollision )
		{
			m_bCollision = true;
		}
	}
	else
	{
		if ( m_bCollision )
			m_bCollision = false;
	}

}
