#include "stdafx.h"
#include "GameEntity.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "UIMgr.h"
#include "EffectMgr.h"
#include "UpgradeMgr.h"
#include "Mouse.h"
#include "Random.h"
#include "SoundMgr.h"

#include "Animation.h"
#include "EntityPattern.h"
#include "Astar.h"
#include "Background.h"
#include "Weapon.h"
#include "Corps.h"
#include "Player.h"
#include "UI.h"
#include "MiniMap.h"

CBackground* CGameEntity::m_pBackground = NULL;


CGameEntity::CGameEntity()
	: m_wstrStateKey( L"" )
	, m_pAnimCom( NULL )
	, m_pCurActPattern( NULL )
	, m_bCollision( false )
	, m_pEntityBelongToCorps( NULL )
	, m_pTarget( NULL )
	, m_bDie( false )
	, m_bDestoryEntity( false )
	, m_bInfluenceLook( true )
	, m_bUseDirAnimIndex( false )
	, m_vHitShowPos( 0.f, 0.f, 0.f ), m_vEffectShowPos( 0.f, 0.f, 0.f )
	, m_byKillEnemy(0)
	, m_bDrawHpBarUI(false)
	, m_pMiniMap(NULL)
	, m_iMinimapSpaceDataKey(-1)
	, m_bUseDeathEffect(true)
	, m_bCheckEntityTile(true)
	, m_bHideEntity(false)
	, m_bUseDestination(false)
	, m_byProgressRatio(0)
	, m_bUpdateOrderData(false)
	, m_bClocking(false)
	, m_fIntervalSoundTime(1.f)
	, m_fSoundPlayGlobalTime(0.f)
{
	ZeroMemory( &this->m_tInfoData, sizeof( COMMON_DATA ) );
	ZeroMemory( &this->m_tGroundAttWeapon, sizeof( ATTACK_DATA ) );
	ZeroMemory( &this->m_tAirAttWeapon, sizeof( ATTACK_DATA ) );

	this->m_tGroundAttWeapon.pAttackEntity = this->m_tAirAttWeapon.pAttackEntity = this;

	this->m_pTimeMgr = CTimeMgr::GetInstance();
	this->m_pRandom = CRandom::GetInstance();
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
				this->SetPattern( this->m_vecActPatterns[i], TRUE );
				//this->m_vecActPatterns.pop_back();
				return;
			}

			if ( i <= 0 )
				break;
		}
	}

	this->SetPattern( eGameEntityPattern::Pattern_Idle );
}

void CGameEntity::SetEntityType( const CEntityMgr::eEntityType & _eEntityType )
{
	this->m_eEntityType = _eEntityType;
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

void CGameEntity::SetGenerateData( const UNIT_GENERATE_DATA * _pGenData )
{
	this->m_pGenerateData = _pGenData;
}

void CGameEntity::SetPlayer( CPlayer * _pPlayer )
{
	this->m_pPlayer = _pPlayer;
}

void CGameEntity::SetUpdateOrderData( const bool & _bUpdateOrderData )
{
	this->m_bUpdateOrderData = _bUpdateOrderData;
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

void CGameEntity::SetSelectShowData( const SELECT_UNIT_SHOW_DATA* _pSelectShowData )
{
	this->m_pSelectShowData = _pSelectShowData;
}

void CGameEntity::SetButtonData( vector<BUTTON_DATA*>* _pVecButtonData )
{
	this->m_pVecActButton = _pVecButtonData;
}

void CGameEntity::ChangeAnimation( const wstring & _wstrName )
{
	if ( this->m_pAnimCom->ChangeAnimation( _wstrName ) )
	{
		this->m_wstrStateKey = _wstrName;
		this->ChangeLookAnimTexture();
	}
}

void CGameEntity::SetHpBarData( HP_BAR_MATRIX_DATA& _tHpBarMatrixData )
{
	this->m_pHpBarMatrixData = &_tHpBarMatrixData;
}

void CGameEntity::SetMinimapSpaceDataKey( const int & _iMinimapSpaceKey )
{
	this->m_iMinimapSpaceDataKey = _iMinimapSpaceKey;
}

void CGameEntity::SetEntityMgrListIndex( const int & _iEntityMgrListIndex )
{
	this->m_iEntityMgrListIndex = _iEntityMgrListIndex;
}

void CGameEntity::SetIsCheckEntityTile( const bool & _bCheckEntityTile )
{
	m_bCheckEntityTile = _bCheckEntityTile;
}

void CGameEntity::SetHideUnit( const bool & _bHideUnit )
{
	this->m_bHideEntity = _bHideUnit;
}

void CGameEntity::SetUseDestination( const bool & _bUseDestination )
{
	this->m_bUseDestination = _bUseDestination;
}

void CGameEntity::SetDestination( const D3DXVECTOR3 & _vDestination )
{
	this->m_vDestination = _vDestination;
}

void CGameEntity::SetProgressRatio( const BYTE & _byProgressRatio )
{
	this->m_byProgressRatio = _byProgressRatio;
}

bool CGameEntity::AddOrderIcon( const SHORT & _nIcon, const int& _iMessage, void* _pData, const int& _iDataType )
{
	if ( this->GetIsFullOrderVector() )
		return false;

	ORDER_DATA tOrderData;
	tOrderData.nIconNum = _nIcon;
	tOrderData.iMessage = _iMessage;
	tOrderData.pData = _pData;
	tOrderData.iDataType = _iDataType;

	this->m_vecOrderData.push_back( tOrderData );
	return true;
}

void CGameEntity::SetStopTIme( const float & _fStopTime )
{
	if ( _fStopTime > 0.f )
		this->m_bStopAct = true;

	this->m_fStopTime = _fStopTime;
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

int CGameEntity::GetEntitySpaceDataKey() const
{
	return this->m_iEntitySpaceDataKey;
}

int CGameEntity::GetMinimapSpaceDataKey() const
{
	return this->m_iMinimapSpaceDataKey;
}

int CGameEntity::GetEntityMgrListIndex() const
{
	return this->m_iEntityMgrListIndex;
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

bool CGameEntity::GetIsCheckEntityTile() const
{
	return this->m_bCheckEntityTile;
}

bool CGameEntity::GetIsUseDestination() const
{
	return this->m_bUseDestination;
}

bool CGameEntity::GetIsFullOrderVector() const
{
	if ( this->m_vecOrderData.size() >= 5 )
		return true;

	return false;
}

bool CGameEntity::GetIsClocking() const
{
	return this->m_bClocking;
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

BYTE CGameEntity::GetDirAnimIndex() const
{
	return this->m_byDirAnimIndex;
}

BYTE CGameEntity::GetLookAnimIndex() const
{
	return this->m_byLookAnimIndex;
}

BYTE CGameEntity::GetFaceFrameNum() const
{
	return this->m_byFaceFrameNum;
}

BYTE CGameEntity::GetProgressRatio() const
{
	return this->m_byProgressRatio;
}

D3DXVECTOR3 CGameEntity::GetHitShowPos() const
{
	return this->GetPos() + this->m_vHitShowPos;
}

D3DXVECTOR3 CGameEntity::GetEffectShowPos() const
{
	return this->GetPos() + this->m_vEffectShowPos;
}

D3DXVECTOR3 CGameEntity::GetDestination() const
{
	return this->m_vDestination;
}

UNIT_GENERATE_DATA CGameEntity::GetGenerateData() const
{
	return (*this->m_pGenerateData);
}

wstring CGameEntity::GetWireFrameKey() const
{
	return this->m_wstrWireFrameKey;
}

wstring CGameEntity::GetFaceKey() const
{
	return this->m_wstrFaceKey;
}

COMMON_DATA CGameEntity::GetCommanData() const
{
	return this->m_tInfoData;
}

const SELECT_UNIT_SHOW_DATA* CGameEntity::GetSelectShowData() const
{
	return this->m_pSelectShowData;
}

BYTE CGameEntity::GetKillCount() const
{
	return this->m_byKillEnemy;
}

vector<BUTTON_DATA*>* CGameEntity::GetButtonData() const
{
	return this->m_pVecActButton;
}

D3DXVECTOR3 CGameEntity::GetImageSize()
{
	if ( this->m_vecTexture.empty() || !this->m_vecTexture.front() )
		return D3DXVECTOR3( 0.f, 0.f, 0.f );

	return D3DXVECTOR3((float)this->m_vecTexture.front()->ImageInfo.Width, (float)this->m_vecTexture.front()->ImageInfo.Height, 0.f);
}

int CGameEntity::GetOrderVectorSize() const
{
	return this->m_vecOrderData.size();
}

CGameEntity::ORDER_DATA CGameEntity::GetOrderData( const int & _iIndex ) const
{
	if ( this->m_vecOrderData.size() <= _iIndex )
		return ORDER_DATA();

	return this->m_vecOrderData[_iIndex];
}

bool CGameEntity::GetCheckUnitInformation( const eEntityInformation & _eEntityInfo )
{
	for ( size_t i = 0; i < this->m_vecEntityInformation.size(); ++i )
	{
		if ( m_vecEntityInformation[i] == _eEntityInfo )
			return true;

	}

	return false;
}

void CGameEntity::KillEnemy()
{
	if ( this->m_byKillEnemy >= 255 )
		return;

	++this->m_byKillEnemy;
}

void CGameEntity::PushMessage( const BUTTON_DATA* _pButtonData )
{
	m_pPushData = _pButtonData;
}

void CGameEntity::SuccessOrder( const CGameEntity::eGameEntityPattern& _ePatternKind )
{
	this->m_bUpdateOrderData = true;
}

HRESULT CGameEntity::Initialize( void )
{
	this->m_pMouse = CMouse::GetInstance();
	this->m_pObjMgr = CObjMgr::GetInstance();

	this->m_pUpgradeMgr = CUpgradeMgr::GetInstance();
	this->m_pUpgradeMgr->AddArmorUpgradeObserver( this, this->m_tInfoData.eArmorUpgradeType );

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

	this->m_pUIMgr = CUIMgr::GetInstance();
	this->m_pHpBarUI = m_pUIMgr->GetHpBarUI();
	this->m_pHpBarBackUI = m_pUIMgr->GetHpBarBackUI();

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
	this->UpdateLookAnimIndex();

	this->CollisionUpdate();

	this->SetPattern( CGameEntity::Pattern_Idle );

	this->m_tColRect = this->m_tOriginColRect;

	this->m_pMiniMap = CUIMgr::GetInstance()->GetMiniMap();

	this->m_pEntityMgr = CEntityMgr::GetInstance();

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

	if ( this->m_bStopAct )
	{
		this->m_fStopTime -= GET_TIME;
		if ( this->m_fStopTime <= 0.f )
			this->m_bStopAct = false;
	}
	else if ( this->m_pCurActPattern )
	{
		this->m_pCurActPattern->Update();
	}

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

	if ( this->m_bDrawHpBarUI )
	{
		this->RenderHpUI();
	}

	//D3DXMATRIX matFont;
	//D3DXMatrixTranslation( &matFont, this->GetPos().x, this->GetPos().y + 100.f, this->GetPos().z );
	//
	//TCHAR str[128];
	//swprintf_s( str, L"HP : %f", this->GetCurHp() );
	//this->DrawFont( matFont, str );

	RECT rcDraw = { (LONG)(this->m_tColRect.left - m_vScroll.x), (LONG)(this->m_tColRect.top - m_vScroll.y),
					(LONG)(this->m_tColRect.right - m_vScroll.x), (LONG)(this->m_tColRect.bottom - m_vScroll.y) };

	this->DrawRect( rcDraw );

}

void CGameEntity::Release( void )
{
	this->m_vecTexture.clear();
	this->m_vecActPatterns.clear();

	for ( size_t i = 0; i < m_vecOrderData.size(); ++i )
	{
		if ( this->m_vecOrderData[i].iDataType == CGameEntity::Pattern_Make_Unit )
		{
			UNIT_GENERATE_DATA* pDelete = ((UNIT_GENERATE_DATA*)(this->m_vecOrderData[i].pData));
			safe_delete( pDelete );
		}
		else if ( this->m_vecOrderData[i].iDataType == CGameEntity::Pattern_Research )
		{
			RESEARCH_DATA* pDelete = ((RESEARCH_DATA*)(this->m_vecOrderData[i].pData));
			safe_delete( pDelete );
		}
	}

	for ( int i = 0; i < Sound_End; ++i )
	{
		for ( size_t j = 0; j < m_vecSoundName[i].size(); ++j )
		{
			delete[] this->m_vecSoundName[i][j];
			this->m_vecSoundName[i][j] = nullptr;
		}

		this->m_vecSoundName[i].clear();
	}
}

void CGameEntity::UpdatePosition( const D3DXVECTOR3& vPrevPos )
{
	this->CollisionUpdate();
	//this->CollisionCheck();

	m_pBackground->EraseUnitData( m_vecSpaceDataKey );
	m_pBackground->UpdateUnitData( this, m_vecSpaceDataKey );

	this->m_pObjMgr->EraseEntitySpaceData( this, this->m_iEntitySpaceDataKey );
	this->m_pObjMgr->InsertEntitySpaceData( this );

	CGameObject::UpdatePosition( vPrevPos );
	m_iMinimapSpaceDataKey = this->m_pMiniMap->MoveEntity( this );
}

void CGameEntity::MouseEvent()
{
}

bool CGameEntity::UseSkill( const eGameEntitySkillKind & _eSkillKind, CGameEntity * _pTarget )
{

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

		if ( !bOut && this->m_pObjMgr->CheckNearEntitys( pVecEntitys, this, (eObjectType)i, iVecLimitSize ) )
		{
			bOut = true;
		}
	}

	if ( bOut && pVecEntitys )
	{
		for ( size_t i = 0; i < pVecEntitys->size(); ++i )
		{
			if ( (*pVecEntitys)[i]->GetIsClocking() )
			{
				pVecEntitys->erase( pVecEntitys->begin() + i );
			}
		}

		if ( pVecEntitys->empty() )
			bOut = false;
	}

	return bOut;
}

bool CGameEntity::CheckRangeAlertEnemy( const float & _fRange, vector<CGameEntity*>* pVecEntitys, const int & iVecLimitSize )
{
	/* eObjectType 타입의 주변 오브젝트를 검사.. */
	bool bOut = false;

	for ( int i = OBJ_TYPE_USER; i <= OBJ_TYPE_USER2; ++i )
	{
		if ( i == this->GetObjectType() )
			continue;

		if ( !bOut && this->m_pObjMgr->CheckNearEntitys( pVecEntitys, this, _fRange, (eObjectType)i, iVecLimitSize ) )
		{
			bOut = true;
		}
	}

	if ( bOut && pVecEntitys )
	{
		for ( size_t i = 0; i < pVecEntitys->size(); ++i )
		{
			if ( (*pVecEntitys)[i]->GetIsClocking() )
			{
				pVecEntitys->erase( pVecEntitys->begin() + i );
			}
		}

		if ( pVecEntitys->empty() )
			bOut = false;
	}

	return bOut;
}

bool CGameEntity::CheckAlertOurForces( vector<CGameEntity*>* pVecEntitys, const int & iVecLimitSize )
{
	/* eObjectType 타입의 주변 오브젝트를 검사.. */
	bool bOut = false;
	vector<CGameEntity*> vecEntitys;

	this->m_pObjMgr->CheckNearEntitys( &vecEntitys, this, this->GetObjectType() );

	int iTempVecLimitSize = iVecLimitSize;

	size_t iLength = vecEntitys.size();
	for ( size_t i = 0; i < iLength; ++i )
	{
		if ( (vecEntitys[i]->m_tInfoData.fMaxHp - vecEntitys[i]->m_tInfoData.fCurHp) <= EPSILON )
			continue;

		if ( !vecEntitys[i]->GetCheckUnitInformation( CGameEntity::Entity_Bionic ) )
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

void CGameEntity::UpdateLookAnimIndex()
{
	D3DXVECTOR3 vUp( 0.f, -1.f, 0.f );
	D3DXVECTOR3 vTempDir = this->GetTransform()->GetLook();
	float fAngle = D3DXVec3Dot( &vUp, &(vTempDir) );
	fAngle = acosf( fAngle );

	if ( this->m_bInfluenceLook )
	{
		if ( vTempDir.x < 0.f )
			this->SetSize( -1.f, 1.f );
		else
			this->SetSize( 1.f, 1.f );
	}
	else
		this->SetSize( 1.f, 1.f );

	fAngle /= (D3DX_PI / 17.f);

	BYTE byLookAnimIndex = BYTE( fAngle );

	if ( byLookAnimIndex == 17 )
		byLookAnimIndex -= 1;
	else if ( byLookAnimIndex > 17 || byLookAnimIndex < 0 )
	{
		ERROR_MSG( L"byDirAnimIndex Error" );
		return;
	}

	if ( this->m_bUseDirAnimIndex )
	{
		vTempDir = this->GetTransform()->GetDir();
		fAngle = D3DXVec3Dot( &vUp, &(vTempDir) );
		fAngle = acosf( fAngle );

		fAngle /= (D3DX_PI / 17.f);

		BYTE byDIrAnimIndex = BYTE( fAngle );

		if ( byDIrAnimIndex == 17 )
			byDIrAnimIndex -= 1;
		else if ( byDIrAnimIndex > 17 || byDIrAnimIndex < 0 )
		{
			ERROR_MSG( L"byDIrAnimIndex Error" );
			return;
		}

		if ( byDIrAnimIndex != this->m_byDirAnimIndex )
		{
			this->m_byDirAnimIndex = byDIrAnimIndex;
			this->ChangeDirAnimIndex();
		}
	}

	if ( byLookAnimIndex != this->m_byLookAnimIndex )
	{
		this->m_byLookAnimIndex = byLookAnimIndex;
		this->ChangeLookAnimTexture();
	}
}

void CGameEntity::LookPos( const D3DXVECTOR3 & _vPos, const bool& _bDirUpdate /*= TRUE*/ )
{
	D3DXVECTOR3 vTempDir;
	D3DXVec3Normalize( &vTempDir, &(_vPos - this->GetPos()) );
	
	this->SetLook( vTempDir );
	this->UpdateLookAnimIndex();

	if ( _bDirUpdate )
		this->SetDir( vTempDir );
}

void CGameEntity::RenderSelectTexture( bool _bPlayer )
{
	if ( this->m_bDie )
		return;

	if ( _bPlayer )
	{
		if ( m_pSelectTexture[0] )
		{
			float fX = this->m_pSelectTexture[0]->ImageInfo.Width * 0.5f;
			float fY = this->m_pSelectTexture[0]->ImageInfo.Height * 0.5f;
			this->DrawTexture( this->m_pSelectTexture[0], this->GetWorldMatrix(), D3DXVECTOR3( fX, fY, 0.f ) );
		}
	}
	else
	{
		if ( m_pSelectTexture[1] )
		{
			float fX = this->m_pSelectTexture[1]->ImageInfo.Width * 0.5f;
			float fY = this->m_pSelectTexture[1]->ImageInfo.Height * 0.5f;
			this->DrawTexture( this->m_pSelectTexture[1], this->GetWorldMatrix(), D3DXVECTOR3( fX, fY, 0.f ) );
		}
	}
}

void CGameEntity::HitEntity( CGameEntity* _pAttackedObject, float _fDamage )
{
	if ( _fDamage >= 0.f )
	{
		_fDamage -= (float)(this->m_tInfoData.iDefense + this->m_tInfoData.iUpgradeDefense);

		if ( _fDamage <= 0.f )
			_fDamage = 0.5f;

		this->m_tInfoData.fCurHp -= _fDamage;
	}

	if ( this->m_tInfoData.fCurHp <= 0.f )
	{
		this->SetPattern( CGameEntity::Pattern_Die );
		return;
	}

	if ( this->m_curActPatternKind == CGameEntity::Pattern_Idle ||
		 this->m_curActPatternKind == CGameEntity::Pattern_MoveAlert ||
		 this->m_curActPatternKind == CGameEntity::Pattern_Patrol ||
		 this->m_curActPatternKind == CGameEntity::Pattern_ChaseTarget )
	{
		this->m_pTarget = _pAttackedObject;
		this->SetPattern( CGameEntity::Pattern_Hit );
	}
}

void CGameEntity::DieEntity()
{
	/* 기계 유닛일 경우 터지는 이펙트.. */
	if ( this->m_bUseDeathEffect )
	{
		/*## 이펙트 터지는 부분.. */
		/*## 이펙트 터지는 부분.. */
		D3DXVECTOR3 vSize( 1.f, 1.f, 1.f );
		vSize.x = (this->m_tColRect.right - this->m_tColRect.left) / 8 * 0.1f;
		vSize.y = (this->m_tColRect.bottom - this->m_tColRect.top) / 8 * 0.1f;

		if ( this->GetCheckUnitInformation( CGameEntity::Entity_Building ) )
		{
			vSize.x *= 0.5f;
			vSize.y *= 0.5f;
		}

		CEffectMgr::GetInstance()->ShowEffect( L"Effect", L"DeathUnit", 0, 14, this->GetPos(), vSize );
		this->m_bDestoryEntity = true;
	}

	this->m_bDie = true;
	this->SetDir( D3DXVECTOR3( 0.f, 0.f, 0.f ) );
	this->m_byLookAnimIndex = 0;

	if ( this->m_pEntityBelongToCorps )
		this->m_pEntityBelongToCorps->EraseUnit( this );

	if ( this->GetObjectType() == OBJ_TYPE_USER )
	{
		m_pPlayer->ShowEntityUI();
	}

	this->m_pObjMgr->EraseEntitySpaceData( this, this->m_iEntitySpaceDataKey );
	this->m_pBackground->EraseUnitData( m_vecSpaceDataKey );
	this->m_pMiniMap->EraseEntity( this );

	this->m_pUpgradeMgr->EraseArmorUpgradeObserver( this, this->m_tInfoData.eArmorUpgradeType );

	this->SoundPlay( CGameEntity::Sound_Death );

}

void CGameEntity::OnRenderHpBar()
{
	this->m_bDrawHpBarUI = true;
}

void CGameEntity::OffRenderHpBar()
{
	this->m_bDrawHpBarUI = false;
}

void CGameEntity::RenderHpUI()
{
	this->m_pHpBarBackUI->Render( 0, this->m_pHpBarMatrixData->hpBarBackWordMatrix );
	for ( size_t j = 0; j < this->m_pHpBarMatrixData->vecHpBarData.size(); ++j )
	{
		this->m_pHpBarUI->Render( 
			this->m_pHpBarMatrixData->vecHpBarData[j].first, this->m_pHpBarMatrixData->vecHpBarData[j].second );
	}
}

void CGameEntity::UpgradeArmor( const int & _iUpgradeArmor )
{
	this->m_tInfoData.iUpgradeDefense = _iUpgradeArmor;
}

bool CGameEntity::GetOrderIconData( vector<SHORT>& _vecGet )
{
	if ( this->m_bUpdateOrderData )
	{
		vector<SHORT> vecIcon;

		for ( size_t i = 0; i < m_vecOrderData.size(); ++i )
			_vecGet.push_back( m_vecOrderData[i].nIconNum );

		this->m_bUpdateOrderData = false;

		return true;
	}

	return false;
}

void CGameEntity::SoundPlay( const eUnit_Sound_Kind & _eSoundKind )
{
	if ( m_vecSoundName[_eSoundKind].empty() )
		return;

	size_t iSoundNum = m_vecSoundName[_eSoundKind].size();
	size_t iSelectSoundNum = m_pRandom->GetValue( iSoundNum );

	this->SoundPlay( _eSoundKind, iSelectSoundNum );
}

void CGameEntity::SoundPlay( const eUnit_Sound_Kind & _eSoundKind, const int & _iIndex )
{
	if ( _eSoundKind == CGameEntity::Sound_Click || _eSoundKind == CGameEntity::Sound_ActPattern )
	{
		this->m_fCurTime = this->m_pTimeMgr->GetGlobalTime();

		if(this->m_fCurTime - this->m_fIntervalSoundTime < this->m_fSoundPlayGlobalTime )
			return;
	}

	if ( m_vecSoundName[_eSoundKind].empty() )
		return;

	this->m_pSoundMgr->PlaySound( m_vecSoundName[_eSoundKind][_iIndex], CSoundMgr::Channel_Player );

	this->m_fSoundPlayGlobalTime = this->m_fCurTime;
}

void CGameEntity::SoundPlay( const eUnit_Sound_Kind & _eSoundKind, const int & _iStartIndex, const int & _iSize )
{
	if ( m_vecSoundName[_eSoundKind].empty() )
		return;

	size_t iSelectSoundNum = m_pRandom->GetValue( _iStartIndex, _iStartIndex + _iSize );

	this->SoundPlay( _eSoundKind, iSelectSoundNum );
}

void CGameEntity::AddSound( TCHAR * pStr, const eUnit_Sound_Kind & _eSoundKind )
{
	TCHAR* pPushString = new TCHAR[MAX_PATH];

	lstrcpy( pPushString, pStr );

	this->m_vecSoundName[_eSoundKind].push_back( pPushString );
}

void CGameEntity::ChangeLookAnimTexture()
{
	const FRAME* pTempCurFrame = this->m_pAnimCom->GetCurAnimation();
	if ( pTempCurFrame )
	{
		m_vecTexture.clear();

		int iAnimLength = int( pTempCurFrame->fMax );
		int iStart = ((this->m_bInfluenceLook) ? iAnimLength * this->m_byLookAnimIndex : 0);
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
	if ( !this->m_bCheckEntityTile )
		return;
	//return;
	vector<CGameEntity*> vColEntity;
	
	this->m_pObjMgr->CheckEntitysCol( &vColEntity, this );
	
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

		int iTileIndex = this->m_pBackground->GetTileIndex( this->GetPos() );
		if ( !this->m_pBackground->CheckCanGoTile( iTileIndex, 0, this, false ) )
			this->Translate( -vMove );

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

void CGameEntity::ChangeDirAnimIndex()
{
}
