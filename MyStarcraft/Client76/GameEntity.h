#pragma once
#include "GameObject.h"
#include "EntityMgr.h"

class CAnimation;
class CEntityPattern;
class CAStar;
class CWeapon;
class CCorps;
class CBackground;
class CPlayer;
class CUI;
class CUIMgr;
class CMiniMap;
class CEntityMgr;
class CUpgradeMgr;
class CMouse;
class CObjMgr;
class CSoundMgr;
class CRandom;
class CTimeMgr;

class CGameEntity :
	public CGameObject
{
public:
	enum eUnit_Sound_Kind
	{
		Sound_Born,
		Sound_Click,
		Sound_ActPattern,
		Sound_Death,
		Sound_ETC,
		Sound_End,
	};

	enum eGameEntityPattern
	{
		Pattern_Idle,
		Pattern_Move,
		Pattern_MoveAlert,
		Pattern_Stop,
		Pattern_Hold,
		Pattern_Patrol,
		Pattern_Attack,
		Pattern_ChaseTarget,
		Pattern_Hit,
		Pattern_Build,
		Pattern_Cancel,
		Pattern_Make_Unit,
		Pattern_Gather,
		Pattern_Research,
		Pattern_Die,
	};

	enum eGameEntitySkillKind
	{
		Skill_Heal,
		Skill_SpiderMine,
		Skill_SiegeOnOff,
		Skill_BuildStructure,
		Skill_BuildAdvancedStructure,
		Skill_SteamPack,
		SKill_LockDown,
		Skill_Nuclear,
		Skill_Clocking,
	};

	enum eEntityInformation
	{							
		Entity_AirUnit,			// 공중 유닛이다..
		Entity_Bionic,			// 바이오닉 유닛..
		Entity_Mechanic,		// 메카닉 유닛..
		Entity_MagicUnit,		// 매직 유닛..
		Entity_CanDigResource,	// 자원을 캘 수 있음..
		Entity_Building,		// 건물이다..
		Entity_FlyBuilding,		// 날 수 있는 건물..
		Entity_ProduceUnit,		// 유닛을 생산할 수 있는 건물..
	};

	typedef struct tagAttackData
	{
		CWeapon*		pWeapon;
		BYTE			byAttackNum;
		float			fAttRange;
		CGameEntity*	pAttackEntity;
	}ATTACK_DATA;

	typedef struct tagOrderData
	{
		SHORT	nIconNum;
		int		iMessage;
		void*	pData;

		int		iDataType;

	}ORDER_DATA;

protected:
	CEntityMgr*				m_pEntityMgr;
	CUpgradeMgr*			m_pUpgradeMgr;
	CUIMgr*					m_pUIMgr;
	CMouse*					m_pMouse;
	CObjMgr*				m_pObjMgr;
	CSoundMgr*				m_pSoundMgr;
	CRandom*				m_pRandom;
	CTimeMgr*				m_pTimeMgr;

	vector<TCHAR*>			m_vecSoundName[Sound_End];

	D3DXVECTOR3				m_vHitShowPos;
	D3DXVECTOR3				m_vEffectShowPos;

	vector<ORDER_DATA>			m_vecOrderData;
	BYTE						m_byProgressRatio;

	CEntityMgr::eEntityType	m_eEntityType;

	COMMON_DATA						m_tInfoData;				// 유닛 능력치 데이터..
	const SELECT_UNIT_SHOW_DATA*	m_pSelectShowData;			// 유닛 선택 시 출력할 데이터..
	const UNIT_GENERATE_DATA*		m_pGenerateData;			// 유닛 생성 시 필요한 데이터..

	wstring					m_wstrStateKey;				// 현재 상태 키 ( 이미지 출력을 위한 )..
	wstring					m_wstrWireFrameKey;			// 와이어 프레임 텍스쳐 키..
	wstring					m_wstrFaceKey;				// 얼굴 텍스쳐 키..

	BYTE					m_byFaceFrameNum;			// 얼굴 텍스쳐 이미지 개수..

	ATTACK_DATA				m_tGroundAttWeapon;			// 지상 유닛을 공격할 무기..
	ATTACK_DATA				m_tAirAttWeapon;			// 공중 유닛을 공격할 무기..

	vector<const TEX_INFO*> m_vecTexture;				// 렌더할 애니메이션..
	const TEX_INFO*			m_pSelectTexture[2];		// 선택되었을 때 그릴 원 이미지..
	CAnimation*				m_pAnimCom;					// 애니메이션 컴포넌트..

	eGameEntityPattern		m_curActPatternKind;		// 현재 실행 중인 패턴..

	vector<eGameEntityPattern>	m_vecActPatterns;

	CCorps*					m_pEntityBelongToCorps;		// 현재 부대..
	CPlayer*				m_pPlayer;

	BYTE					m_byDirAnimIndex;
	bool					m_bUseDirAnimIndex;

	BYTE					m_byLookAnimIndex;

	CAStar*					m_pAStar;

	map<wstring, CEntityPattern*>	m_mapPatterns;
	CEntityPattern*					m_pCurActPattern;

	CGameEntity*			m_pTarget;
	CMiniMap*				m_pMiniMap;
	int						m_iMinimapSpaceDataKey;

	RECT					m_tColRect;
	RECT					m_tOriginColRect;

	bool					m_bCollision;
	bool					m_bDie;
	bool					m_bDestoryEntity;
	bool					m_bInfluenceLook;

	static CBackground*		m_pBackground;

	list<pair<int, BYTE>>	m_standTileIndexList;

	vector<_Dlong>			m_vecSpaceDataKey;
	int						m_iEntitySpaceDataKey;

	BYTE					m_byKillEnemy;

	vector<eEntityInformation>	m_vecEntityInformation;
	vector<BUTTON_DATA*>*	m_pVecActButton;
	const BUTTON_DATA*		m_pPushData;

	HP_BAR_MATRIX_DATA*		m_pHpBarMatrixData;

	CUI*					m_pHpBarUI;
	CUI*					m_pHpBarBackUI;

	int						m_iEntityMgrListIndex;

	D3DXVECTOR3				m_vDestination;

	float					m_fStopTime;
	float					m_fIntervalSoundTime;
	float					m_fSoundPlayGlobalTime;
	float					m_fCurTime;

	bool					m_bDrawHpBarUI;
	bool					m_bIgnoreAttack;
	bool					m_bUseDeathEffect;
	bool					m_bCheckEntityTile;
	bool					m_bHideEntity;
	bool					m_bUseDestination;
	bool					m_bUpdateOrderData;
	bool					m_bStopAct;
	bool					m_bClocking;

public:
	CGameEntity();
	virtual ~CGameEntity();

public:
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern = FALSE ) PURE;
	void SetPrevPattern();
	void SetEntityType( const CEntityMgr::eEntityType& _eEntityType );
	void SetCurHp( const float& fHp );
	void SetEntityBelongToCorps( CCorps* _pEntityBelongToCorps );
	void SetTarget( CGameEntity* _pTarget );
	void SetGenerateData( const UNIT_GENERATE_DATA* _pGenData );
	void SetPlayer( CPlayer* _pPlayer );
	void SetUpdateOrderData( const bool& _bUpdateOrderData );
	static void SetBackground(CBackground* pBackground)
	{
		m_pBackground = pBackground;
	}
	void SetStandTileIndexList( const list<pair<int, BYTE>>& _standTileIndexList );
	void SetEntitySpaceDataKey( const int& _iEntitySpaceDataKey );
	void SetSelectShowData( const SELECT_UNIT_SHOW_DATA* _pSelectShowData );
	void SetButtonData( vector<BUTTON_DATA*>* _pVecButtonData );
	void ChangeAnimation( const wstring& _wstrName );
	void SetHpBarData( HP_BAR_MATRIX_DATA& _tHpBarMatrixData );
	void SetMinimapSpaceDataKey( const int& _iMinimapSpaceKey );
	void SetEntityMgrListIndex( const int& _iEntityMgrListIndex );
	void SetIsCheckEntityTile( const bool& _bCheckEntityTile );
	void SetHideUnit( const bool& _bHideUnit );
	void SetUseDestination( const bool& _bUseDestination );
	void SetDestination( const D3DXVECTOR3& _vDestination );
	void SetProgressRatio( const BYTE& _byProgressRatio );
	bool AddOrderIcon( const SHORT& _nIcon, const int& _iMessage, void* _pData, const int& _iDataType );
	void SetStopTIme( const float& _fStopTime );

public:
	float GetCurHp() const;
	float GetMaxHp() const;
	float GetSpeed() const;
	int GetScope() const;
	int GetEntitySpaceDataKey() const;
	int GetMinimapSpaceDataKey() const;
	int GetEntityMgrListIndex() const;
	const CCorps* GetEntityBelongToCorps() const;
	CAStar*	GetAStar() const { return m_pAStar; }
	RECT GetColRect() const;
	RECT GetOriginColRect() const;
	bool GetIsCollision() const;
	bool GetIsDie() const;
	bool GetIsCheckEntityTile() const;
	bool GetIsUseDestination() const;
	bool GetIsFullOrderVector() const;
	bool GetIsClocking() const;
	const list<pair<int, BYTE>>* GetStandTileIndexList();
	ATTACK_DATA GetGroundAttackData() const;
	ATTACK_DATA GetAirAttackData() const;
	float GetGroundWeaponAttRange() const;
	float GetAirWeaponAttRange() const;
	CGameEntity* GetTarget() const;
	BYTE GetDirAnimIndex() const;
	BYTE GetLookAnimIndex() const;
	BYTE GetFaceFrameNum() const;
	BYTE GetProgressRatio() const;
	D3DXVECTOR3 GetHitShowPos() const;
	D3DXVECTOR3 GetEffectShowPos() const;
	D3DXVECTOR3 GetDestination() const;
	UNIT_GENERATE_DATA GetGenerateData() const;
	wstring GetWireFrameKey() const;
	wstring GetFaceKey() const;
	COMMON_DATA GetCommanData() const;
	const SELECT_UNIT_SHOW_DATA* GetSelectShowData() const;
	BYTE GetKillCount() const;
	vector<BUTTON_DATA*>* GetButtonData() const;
	D3DXVECTOR3 GetImageSize();

	int GetOrderVectorSize() const;
	ORDER_DATA GetOrderData(const int& _iIndex) const;

public:
	bool GetCheckUnitInformation( const eEntityInformation& _eEntityInfo );
	void KillEnemy();
	virtual void PushMessage( const BUTTON_DATA* _pButtonData );

	virtual void SuccessOrder( const CGameEntity::eGameEntityPattern& _ePatternKind );

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	virtual void UpdatePosition( const D3DXVECTOR3& vPrevPos ) override;
	virtual void MouseEvent();

public:
	virtual bool UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget );

public:
	bool CheckAlertEnemy( vector<CGameEntity*>* pVecEntitys = NULL, const int& iVecLimitSize = -1 );
	bool CheckRangeAlertEnemy( const float& _fRange, vector<CGameEntity*>* pVecEntitys = NULL, const int& iVecLimitSize = -1 );
	bool CheckAlertOurForces( vector<CGameEntity*>* pVecEntitys = NULL, const int& iVecLimitSize = -1 );
	void MoveEntity();
	virtual void UpdateLookAnimIndex();
	void LookPos( const D3DXVECTOR3& _vPos, const bool& _bDirUpdate = TRUE );

	void RenderSelectTexture( bool _bPlayer );

	void HitEntity( CGameEntity* _pAttackedObject, float _fDamage );

	void DieEntity();
	
	void OnRenderHpBar();
	void OffRenderHpBar();

	void RenderHpUI();

	void UpgradeArmor( const int& _iUpgradeArmor );

	bool GetOrderIconData( vector<SHORT>& _vecGet );

	void SoundPlay( const eUnit_Sound_Kind& _eSoundKind );
	void SoundPlay( const eUnit_Sound_Kind& _eSoundKind, const int& _iIndex );
	void SoundPlay( const eUnit_Sound_Kind& _eSoundKind, const int& _iStartIndex, const int& _iSize );

	void AddSound( TCHAR * pStr, const eUnit_Sound_Kind & _eSoundKind );

protected:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

	/* 테란 건물 중 미사일 터렛도 필요로 하기 때문에 ???? */
	/*
	 * PushDirAnimTexture : 현재 오브젝트의 방향에 따라 텍스쳐를 넣어줌..
	 * UpdateDirAnimIndex : 현재 오브젝트의 방향에 따른 인덱스를 결정해줌..
	 */
	void ChangeLookAnimTexture();
	void CollisionUpdate();

public:
	void CollisionCheck();

protected:
	virtual void ChangeDirAnimIndex();

};

