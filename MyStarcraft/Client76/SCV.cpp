#include "stdafx.h"
#include "SCV.h"

#include "Animation.h"
#include "WeaponMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "EntityMgr.h"
#include "Mouse.h"
#include "UIMgr.h"

#include "Pattern_IdleAlert.h"
#include "Pattern_Move.h"
#include "Pattern_MoveAlert.h"
#include "Pattern_ChaseTarget.h"
#include "Pattern_Attack.h"
#include "Pattern_Die.h"
#include "Pattern_Patrol.h"
#include "Pattern_Unit_Build_Building.h"
#include "GatherMineral.h"
#include "GatherGas.h"
#include "Move.h"
#include "Background.h"
#include "Weapon.h"
#include "Corps.h"
#include "Player.h"
#include "Building.h"

vector<BUTTON_DATA*>* CSCV::m_pVecStructureButton = NULL;
vector<BUTTON_DATA*>* CSCV::m_pVecAdvancedStructureButton = NULL;


CSCV::CSCV()
	: m_bRenderBuildingArea(false)
	, m_bCanBuild(false)
	, m_bBuildAdvancedStructure(false)
	, m_bUnder_Construction(false)
{
}


CSCV::~CSCV()
{
	Release();
}

CGameObject* CSCV::GetGatherObject() const
{
	return this->m_pGatherObject;
}

CGameEntity * CSCV::GetBuildEntity() const
{
	return this->m_pBuildEntity;
}

bool CSCV::GetIsUnderConstruction() const
{
	return this->m_bUnder_Construction;
}

HRESULT CSCV::Initialize( void )
{
	/* Add Sound To SoundVector.. */
	this->AddSound( L"tscrdy00.wav", CUnit::Sound_Born );

	this->AddSound( L"tscyes00.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tscyes01.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tscyes02.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tscyes03.wav", CUnit::Sound_ActPattern );

	this->AddSound( L"tscwht00.wav", CUnit::Sound_Click );
	this->AddSound( L"tscwht01.wav", CUnit::Sound_Click );
	this->AddSound( L"tscwht02.wav", CUnit::Sound_Click );
	this->AddSound( L"tscwht03.wav", CUnit::Sound_Click );

	this->AddSound( L"tscdth00.wav", CUnit::Sound_Death );

	this->AddSound( L"tscupd00.wav", CUnit::Sound_ETC );
	this->AddSound( L"taderr00.wav", CUnit::Sound_ETC );
	this->AddSound( L"taderr01.wav", CUnit::Sound_ETC );
	this->AddSound( L"tscerr00.wav", CUnit::Sound_ETC );
	this->AddSound( L"tscerr01.wav", CUnit::Sound_ETC );


	this->SetObjKey( L"Scv" );
	this->m_wstrStateKey = L"Idle";

	this->m_wstrFaceKey = L"FaceScv";
	this->m_wstrWireFrameKey = L"Scv";

	this->m_byFaceFrameNum = 45;

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 60.f;
	//this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 300.f;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(2.8125f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 1;
	this->m_tInfoData.nDefenceIconFrame = 292;

	this->m_vecEntityInformation.push_back( CGameEntity::Entity_Bionic );
	this->m_vecEntityInformation.push_back( CGameEntity::Entity_Mechanic );
	this->m_vecEntityInformation.push_back( CGameEntity::Entity_CanDigResource );
	this->m_vecEntityInformation.push_back( CGameEntity::Entity_ProduceUnit );

	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( this->GetObjectType(), CWeaponMgr::Weapon_FusionCutter );
	this->m_tGroundAttWeapon.byAttackNum = 1;
	this->m_tGroundAttWeapon.fAttRange = 1.f;

	RECT tRect = { -8, -8, 8, 8 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	m_pPlayer = (CPlayer*)this->m_pObjMgr->GetList( OBJ_TYPE_USER )->front();

	return S_OK;
}

int CSCV::Update( void )
{
	return CUnit::Update();
}

void CSCV::Render( void )
{
	if ( this->m_bHideEntity )
		return;

	CUnit::Render();
}

void CSCV::Release( void )
{
	if ( this->m_bRenderBuildingArea )
	{
		if ( this->m_bBuildAdvancedStructure )
		{
			vector<BUTTON_DATA*>* pTemp = this->m_pVecActButton;
			this->m_pVecActButton = this->m_pVecAdvancedStructureButton;
			this->m_pVecAdvancedStructureButton = pTemp;
		}
		else
		{
			vector<BUTTON_DATA*>* pTemp = this->m_pVecActButton;
			this->m_pVecActButton = this->m_pVecStructureButton;
			this->m_pVecStructureButton = pTemp;
		}

		safe_delete( this->m_pDrawBuilding );
		this->m_bRenderBuildingArea = false;
	}
}

void CSCV::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern )
{
	if ( m_curActPatternKind == CGameEntity::Pattern_Die )
		return;

	if ( _ePatternKind == CGameEntity::Pattern_Idle ||
		 _ePatternKind == CGameEntity::Pattern_MoveAlert )
		this->m_vecActPatterns.push_back( _ePatternKind );

	if ( this->m_vecActPatterns.size() > 10 )
		this->m_vecActPatterns.erase( this->m_vecActPatterns.begin() );

	bool bChangeAnimation = false;

	switch ( _ePatternKind )
	{
		case CGameEntity::Pattern_Idle:
		{
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Idle" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Idle" );
			this->m_wstrStateKey = L"Idle";
		}
		break;

		case CGameEntity::Pattern_Move:
			if ( !this->m_bCheckEntityTile )
				this->m_bCheckEntityTile = true;

			if ( !this->m_bCanBuild )
				this->m_pCurActPattern = this->m_mapPatterns.find( L"Move" )->second;
			break;

		case CGameEntity::Pattern_MoveAlert:
			if ( !this->m_bCheckEntityTile )
				this->m_bCheckEntityTile = true;

			if(!this->m_bCanBuild )
				this->m_pCurActPattern = this->m_mapPatterns.find( L"MoveAlert" )->second;
			break;

		case CGameEntity::Pattern_Stop:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Stop" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Idle" );
			this->m_wstrStateKey = L"Idle";
			break;

		case CGameEntity::Pattern_Hold:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Hold" )->second;
			break;

		case CGameEntity::Pattern_Patrol:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Patrol" )->second;
			break;

		case CGameEntity::Pattern_Attack:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Attack" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Attack" );
			this->m_wstrStateKey = L"Attack";
			break;

		case CGameEntity::Pattern_ChaseTarget:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;
			break;

		case CGameEntity::Pattern_Hit:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;
			break;

		case CGameEntity::Pattern_Die:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Die" )->second;;

			if ( this->m_pEntityBelongToCorps )
				this->m_pEntityBelongToCorps->EraseUnit( this );
			break;

		case CGameEntity::Pattern_Build:
		{
			if ( !this->m_bRenderBuildingArea )
			{
				D3DXVECTOR3 vMousePos = this->m_pMouse->GetPos() + m_vScroll;
				UNIT_GENERATE_DATA tUnitGenData = this->m_pEntityMgr->GetEntityGenData( (CEntityMgr::eEntityType)this->m_pPushData->iMessage );

				int iCanBuyCheck = this->m_pPlayer->CheckCanBuyUnit( tUnitGenData );
				if ( iCanBuyCheck )
				{
					if ( iCanBuyCheck == 1 )
					{
						this->SoundPlay( CGameEntity::Sound_ETC, 1 );
						this->m_pUIMgr->ShowFont( CUIMgr::Font_No_Mineral );
					}
					else if ( iCanBuyCheck == 2 )
					{
						this->SoundPlay( CGameEntity::Sound_ETC, 2 );
						this->m_pUIMgr->ShowFont( CUIMgr::Font_No_Gas );
					}

					this->SetPattern( CGameEntity::Pattern_Idle );

					return;
				}

				CGameEntity* pEntity = CEntityMgr::GetInstance()->MakeUnit( CEntityMgr::eEntityType( this->m_pPushData->iMessage ), 
																			vMousePos, this->GetObjectType() );
				
				//if ( !this->MakeEntity( pEntity, (CEntityMgr::eEntityType)this->m_pPushData->iMessage, vMousePos ) )
				//{
				//	//this->m_bCanBuild = false;
				//	this->SetPattern( CGameEntity::Pattern_Idle );
				//	return;
				//}

				this->RenderBuildingArea( pEntity );
			}
			else
			{
				this->m_pCurActPattern = this->m_mapPatterns.find( L"Build" )->second;
			}
		}
			break;

		case CGameEntity::Pattern_Gather:
		{
			if ( this->m_bMineralGather )
			{
				this->m_pCurActPattern = this->m_mapPatterns.find( L"GatherMineral" )->second;
			}
			else
			{
				this->m_pCurActPattern = this->m_mapPatterns.find( L"GatherGas" )->second;
			}
		}
			break;

		case CGameEntity::Pattern_Cancel:
			if ( this->m_bCanBuild )
			{
				if ( this->m_bRenderBuildingArea )
				{
					safe_delete( this->m_pDrawBuilding );
					this->m_bRenderBuildingArea = false;
				}

				if ( this->m_bBuildAdvancedStructure )
				{
					vector<BUTTON_DATA*>* pTemp = this->m_pVecActButton;
					this->m_pVecActButton = this->m_pVecAdvancedStructureButton;
					this->m_pVecAdvancedStructureButton = pTemp;
					this->m_pPlayer->DecideShowButton();
				}
				else
				{
					vector<BUTTON_DATA*>* pTemp = this->m_pVecActButton;
					this->m_pVecActButton = this->m_pVecStructureButton;
					this->m_pVecStructureButton = pTemp;
					this->m_pPlayer->DecideShowButton();
				}

				CUIMgr::GetInstance()->HideFrameBuilding();
				this->m_bCanBuild = false;
			}
			break;

		default:
			return;

	}

	if ( this->m_pCurActPattern )
	{
		this->m_pCurActPattern->SetPrevPattern( _bPrevPattern );
		this->m_pCurActPattern->OnEnable();
	}

	this->m_curActPatternKind = _ePatternKind;

	if ( bChangeAnimation )
		this->ChangeLookAnimTexture();

}

void CSCV::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Attack", FRAME( 0.f, 2.f, 2.f, 0.f ), CAnimation::Anim_ClampForever );
}

void CSCV::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Idle", new CPattern_IdleAlert ) );
	this->m_mapPatterns.insert( make_pair( L"Move", new CPattern_Move ) );
	this->m_mapPatterns.insert( make_pair( L"MoveAlert", new CPattern_MoveAlert ) );
	this->m_mapPatterns.insert( make_pair( L"ChaseTarget", new CPattern_ChaseTarget ) );
	this->m_mapPatterns.insert( make_pair( L"AttackTarget", nullptr) );
	this->m_mapPatterns.insert( make_pair( L"Stop", nullptr) );
	this->m_mapPatterns.insert( make_pair( L"Attack", new CPattern_Attack ) );
	this->m_mapPatterns.insert( make_pair( L"Die", new CPattern_Die ) );
	this->m_mapPatterns.insert( make_pair( L"Patrol", new CPattern_Patrol ) );
	this->m_mapPatterns.insert( make_pair( L"Build", new CPattern_Unit_Build_Building ) );
	this->m_mapPatterns.insert( make_pair( L"GatherMineral", new CGatherMineral ) );
	this->m_mapPatterns.insert( make_pair( L"GatherGas", new CGatherGas ) );
}

bool CSCV::UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget )
{
	switch ( _eSkillKind )
	{
		case Skill_BuildStructure:
			this->m_bCanBuild = true;
			this->m_bBuildAdvancedStructure = false;

			if ( this->GetObjectType() == OBJ_TYPE_USER )
			{
				vector<BUTTON_DATA*>* pTemp = this->m_pVecActButton;
				this->m_pVecActButton = this->m_pVecStructureButton;
				this->m_pVecStructureButton = pTemp;
				this->m_pPlayer->DecideShowButton();
			}

			break;

		case Skill_BuildAdvancedStructure:
			this->m_bCanBuild = true;
			this->m_bBuildAdvancedStructure = true;

			if ( this->GetObjectType() == OBJ_TYPE_USER )
			{
				vector<BUTTON_DATA*>* pTemp = this->m_pVecActButton;
				this->m_pVecActButton = this->m_pVecAdvancedStructureButton;
				this->m_pVecAdvancedStructureButton = pTemp;
				this->m_pPlayer->DecideShowButton();
			}
			break;

	}

	return true;
}

void CSCV::MouseEvent()
{
	if ( this->m_bRenderBuildingArea )
	{
		if ( !this->m_pDrawBuilding->GetIsCanBuild() )
		{
			this->SoundPlay( CGameEntity::Sound_ETC, 3, 2 );
			return;
		}
		//D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos() + m_vScroll;
		//vMousePos -= D3DXVECTOR3( float(int(vMousePos.x) % TILECX), float(int(vMousePos.y) % TILECY), 0.f ) - D3DXVECTOR3( TILECX * 0.5f, TILECY * 0.5f, 0.f );

		D3DXVECTOR3 vScrollPos = CGameObject::GetScroll();
		D3DXVECTOR3 vMousePos = this->m_pMouse->GetPos();
		RECT rcCol = m_pDrawBuilding->GetColRect();

		vScrollPos.x = int( vScrollPos.x ) % TILECX;
		vScrollPos.y = int( vScrollPos.y ) % TILECY;

		D3DXVECTOR3 vPos = vMousePos + D3DXVECTOR3( (float)(rcCol.left % TILECX), (float)(rcCol.top % TILECY), 0.f );
		vPos.x = (int)(vPos.x - int( vPos.x ) % TILECX);
		vPos.y = (int)(vPos.y - int( vPos.y ) % TILECY);

		if ( !m_pBackground )
			m_pBackground = this->m_pObjMgr->FindGameObject<CBackground>();

		int iStartIndex = this->m_pBackground->GetTileIndex( vPos );

		int iXDist = (rcCol.right - rcCol.left) / TILECX;
		int iYDist = (rcCol.bottom - rcCol.top) / TILECY;

		int iMiddleIndex = iStartIndex + int(iXDist * 0.5) + (int(iYDist * 0.5) * TILEX);

		D3DXVECTOR3 vMiddlePos( float( (iMiddleIndex % TILEX) * TILECX ), float( (iMiddleIndex / TILEX) * TILECY ), 0.f );

		vMiddlePos -= vScrollPos;

		if ( iXDist % 2 )
			vMiddlePos.x += TILECX * 0.5f;
		if ( iYDist % 2 )
			vMiddlePos.y += TILECY * 0.5f;

		vMiddlePos += m_vScroll;

		this->BuildBuilding( this->m_pDrawBuilding, vMiddlePos );

		if ( this->m_bBuildAdvancedStructure )
		{
			vector<BUTTON_DATA*>* pTemp = this->m_pVecActButton;
			this->m_pVecActButton = this->m_pVecAdvancedStructureButton;
			this->m_pVecAdvancedStructureButton = pTemp;
			m_pPlayer->DecideShowButton();
		}
		else
		{
			vector<BUTTON_DATA*>* pTemp = this->m_pVecActButton;
			this->m_pVecActButton = this->m_pVecStructureButton;
			this->m_pVecStructureButton = pTemp;
			m_pPlayer->DecideShowButton();
		}

		this->m_pPlayer->EraseMouseClickEventEntity( this );
		this->SetPattern( CGameEntity::Pattern_Build );

		this->m_bRenderBuildingArea = false;
		this->m_bCanBuild = false;

	}
}

void CSCV::RenderBuildingArea( CGameEntity * pBuilding )
{
	this->m_bRenderBuildingArea = true;
	this->m_pDrawBuilding = dynamic_cast<CBuilding*>(pBuilding);
	CUIMgr::GetInstance()->ShowFrameBuilding( m_pDrawBuilding );
	m_pPlayer->AddMouseClickEventEntity( this );
}

void CSCV::BuildBuilding( CGameEntity * pEntity, const D3DXVECTOR3 & _vPos )
{
	pEntity->SetPos( _vPos );
	//dynamic_cast<CBuilding*>(pEntity)->SetApplyCol( true );

	m_pBuildEntity = pEntity;
	dynamic_cast<CBuilding*>(m_pBuildEntity)->BuildStart();

	CUIMgr::GetInstance()->HideFrameBuilding();

	//CObjMgr::GetInstance()->GetList( this->GetObjectType() )->push_back( pEntity );
}

void CSCV::BuildStart()
{
	dynamic_cast<CBuilding*>(m_pBuildEntity)->SetApplyCol( true );
	m_pBuildEntity->UpdateMatrix();

	this->m_bUnder_Construction = true;
	//CObjMgr::GetInstance()->AddGameObject( m_pBuildEntity, this->GetObjectType() );
	this->m_pObjMgr->GetList( this->GetObjectType() )->push_back( m_pBuildEntity );
	this->m_pAnimCom->ChangeAnimation( L"Attack" );
	this->m_wstrStateKey = L"Attack";
	this->ChangeLookAnimTexture();

	this->m_pPlayer->BuyUnit( this->m_pBuildEntity->GetGenerateData() );
}

void CSCV::SuccessBuild()
{
	this->m_bUnder_Construction = false;

	this->SoundPlay( CGameEntity::Sound_ETC, 0 );
}

void CSCV::GatherResource( const bool & _bMineral, CGameObject * _pObject )
{
	this->m_bMineralGather = _bMineral;
	this->m_pGatherObject = _pObject;
}

void CSCV::InitButton(vector<BUTTON_DATA*>*& _pVecButtonData, vector<BUTTON_DATA*>*& _pVecAdvancedStructureButton)
{
	if ( !m_pVecStructureButton )
	{
		m_pVecStructureButton = _pVecButtonData;
	}

	if ( !m_pVecAdvancedStructureButton )
	{
		m_pVecAdvancedStructureButton = _pVecAdvancedStructureButton;
	}
}

void CSCV::DeleteButton()
{
	if ( m_pVecStructureButton )
	{
		m_pVecStructureButton->clear();
		safe_delete( m_pVecStructureButton );
	}
	if ( m_pVecAdvancedStructureButton )
	{
		m_pVecAdvancedStructureButton->clear();
		safe_delete( m_pVecAdvancedStructureButton );
	}
}
