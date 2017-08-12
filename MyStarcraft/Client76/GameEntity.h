#pragma once
#include "GameObject.h"

class CAnimation;
class CEntityPattern;
class CAStar;
class CWeapon;
class CCorps;
class CBackground;

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
		Pattern_ChaseTarget,
		Pattern_Hit,
	};

	typedef struct tagAttackData
	{
		CWeapon*		pWeapon;
		BYTE			byAttackNum;
		CGameEntity*	pAttackEntity;
	}ATTACK_DATA;

protected:
	COMMON_DATA				m_tInfoData;				// ���� �ɷ�ġ ������..

	wstring					m_wstrStateKey;				// ���� ���� Ű ( �̹��� ����� ���� )..

	ATTACK_DATA				m_tGroundAttWeapon;			// ���� ������ ������ ����..
	ATTACK_DATA				m_tAirAttWeapon;			// ���� ������ ������ ����..

	vector<const TEX_INFO*> m_vecTexture;				// ������ �ִϸ��̼�..
	const TEX_INFO*			m_pSelectTexture[2];		// ���õǾ��� �� �׸� �� �̹���..
	CAnimation*				m_pAnimCom;					// �ִϸ��̼� ������Ʈ..

	eGameEntityPattern		m_curActPatternKind;		// ���� ���� ���� ����..

	CCorps*					m_pEntityBelongToCorps;		// ���� �δ�..

	BYTE					m_byDirAnimIndex;

	CAStar*					m_pAStar;

	map<wstring, CEntityPattern*>	m_mapPatterns;
	CEntityPattern*					m_pCurActPattern;

	RECT					m_tColRect;
	RECT					m_tOriginColRect;

	bool					m_bCollision;

	static CBackground*				m_pBackground;

	list<pair<int, BYTE>>		m_standTileIndexList;

public:
	CGameEntity();
	virtual ~CGameEntity();

public:
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind ) PURE;
	void SetCurHp( const float& fHp );
	void SetEntityBelongToCorps( CCorps* _pEntityBelongToCorps );
	static void SetBackground(CBackground* pBackground)
	{
		m_pBackground = pBackground;
	}
	void SetStandTileIndexList( const list<pair<int, BYTE>>& _standTileIndexList );

public:
	float GetCurHp() const;
	float GetSpeed() const;
	int GetScope() const;
	const CCorps* GetEntityBelongToCorps() const;
	CAStar*	GetAStar() const { return m_pAStar; }
	RECT GetColRect() const;
	RECT GetOriginColRect() const;
	bool GetIsCollision() const;
	const list<pair<int, BYTE>>* GetStandTileIndexList();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	virtual void UpdatePosition( const D3DXVECTOR3& vPrevPos ) override;

public:
	bool CheckAlertEntity( const eObjectType& _eObjectType, vector<CGameEntity*>* pVecEntitys = NULL );
	void MoveEntity();
	void UpdateDirAnimIndex();
	void LookPos( const D3DXVECTOR3& _vPos );

	void RenderSelectTexture( bool _bPlayer );

protected:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

	/* �׶� �ǹ� �� �̻��� �ͷ��� �ʿ�� �ϱ� ������ ???? */
	/*
	 * PushDirAnimTexture : ���� ������Ʈ�� ���⿡ ���� �ؽ��ĸ� �־���..
	 * UpdateDirAnimIndex : ���� ������Ʈ�� ���⿡ ���� �ε����� ��������..
	 */
	void ChangeDirAnimTexture();
	void CollisionUpdate();
	void CollisionCheck();

};

