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
	COMMON_DATA				m_tInfoData;				// ���� �ɷ�ġ ������..

	wstring					m_wstrStateKey;				// ���� ���� Ű ( �̹��� ����� ���� )..

	ATTACK_DATA				m_tGroundAttWeapon;			// ���� ������ ������ ����..
	ATTACK_DATA				m_tAirAttWeapon;			// ���� ������ ������ ����..

	vector<const TEX_INFO*> m_vecTexture;				// ������ �ִϸ��̼�..
	const TEX_INFO*			m_pSelectTexture[2];		// ���õǾ��� �� �׸� �� �̹���..
	CAnimation*				m_pAnimCom;					// �ִϸ��̼� ������Ʈ..

	eGameEntityPattern		m_curActPatternKind;		// ���� ���� ���� ����..

	BYTE					m_byDirAnimIndex;

	CAStar*					m_pAStar;

	map<wstring, CEntityPattern*>	m_mapPatterns;
	CEntityPattern*					m_pCurActPattern;

	RECT							m_tColRect;
	RECT							m_tOriginColRect;

	D3DXVECTOR3						m_vPrevIdlePos;		// Idle Pos ���� ���� ��ǥ ( AStar �� ������ �ִ� ������ ��ǥ�� ���� ������ �ϱ� ���� )..
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
	// CGameObject��(��) ���� ��ӵ�
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

	/* �׶� �ǹ� �� �̻��� �ͷ��� �ʿ�� �ϱ� ������ ???? */
	/*
	 * PushDirAnimTexture : ���� ������Ʈ�� ���⿡ ���� �ؽ��ĸ� �־���..
	 * UpdateDirAnimIndex : ���� ������Ʈ�� ���⿡ ���� �ε����� ��������..
	 */
	void ChangeDirAnimTexture();
	void CollisionUpdate();
	void CollisionCheck();

};

