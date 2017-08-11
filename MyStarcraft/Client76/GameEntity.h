#pragma once
#include "GameObject.h"

class CAnimation;
class CEntityPattern;
class CAStar;
class CWeapon;

class CGameEntity :
	public CGameObject
{
public:
	enum eGameEntityPattern
	{
		Pattern_Idle,
		Pattern_Move,
		Pattern_Stop,
		Pattern_Hold,
		Pattern_Patrol,
		Pattern_Attack,
		Pattern_AttackTarget,
		Pattern_Hit,
	};

	typedef struct tagAttackData
	{
		CWeapon*		pWeapon;
		BYTE			byAttackNum;
		CGameEntity*	pAttackEntity;
	}ATTACK_DATA;

protected:
	COMMON_DATA				m_tInfoData;				// 유닛 능력치 데이터..

	wstring					m_wstrStateKey;				// 현재 상태 키 ( 이미지 출력을 위한 )..

	ATTACK_DATA				m_tGroundAttWeapon;			// 지상 유닛을 공격할 무기..
	ATTACK_DATA				m_tAirAttWeapon;			// 공중 유닛을 공격할 무기..

	vector<const TEX_INFO*> m_vecTexture;				// 렌더할 애니메이션..
	const TEX_INFO*			m_pSelectTexture[2];		// 선택되었을 때 그릴 원 이미지..
	CAnimation*				m_pAnimCom;					// 애니메이션 컴포넌트..

	eGameEntityPattern		m_curActPatternKind;		// 현재 실행 중인 패턴..

	BYTE					m_byDirAnimIndex;

	CAStar*					m_pAStar;

	map<wstring, CEntityPattern*>	m_mapPatterns;
	CEntityPattern*					m_pCurActPattern;

	RECT							m_tColRect;
	RECT							m_tOriginColRect;

	D3DXVECTOR3						m_vPrevIdlePos;		// Idle Pos 였을 때의 좌표 ( AStar 때 가만히 있는 유닛의 좌표의 맵을 못가게 하기 위해 )..
	bool							m_bInitPrevIdlePos;

public:
	CGameEntity();
	virtual ~CGameEntity();

public:
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind ) PURE;
	void SetCurHp( const float& fHp );

public:
	float GetCurHp() const;
	float GetSpeed() const;
	CAStar*	GetAStar() const { return m_pAStar; }
	RECT GetColRect() const;
	RECT GetOriginColRect() const;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	virtual void UpdatePosition() override;

public:
	bool CheckAlertEntity( const eObjectType& eObjectType, vector<CGameEntity*>* pVecEntitys = NULL );
	void MoveEntity();
	void UpdateDirAnimIndex();
	void LookPos( const D3DXVECTOR3& _vPos );

	void RenderSelectTexture( bool _bPlayer );

protected:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

	/* 테란 건물 중 미사일 터렛도 필요로 하기 때문에 ???? */
	/*
	 * PushDirAnimTexture : 현재 오브젝트의 방향에 따라 텍스쳐를 넣어줌..
	 * UpdateDirAnimIndex : 현재 오브젝트의 방향에 따른 인덱스를 결정해줌..
	 */
	void ChangeDirAnimTexture();
	void CollisionUpdate();
	void CollisionCheck();

};

