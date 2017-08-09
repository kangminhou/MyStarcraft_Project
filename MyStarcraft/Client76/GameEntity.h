#pragma once
#include "GameObject.h"

class CAnimation;
class CEntityPattern;
class CAStar;

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

protected:
	COMMON_DATA				m_tInfoData;				// ���� �ɷ�ġ ������..

	wstring					m_wstrStateKey;				// ���� ���� Ű ( �̹��� ����� ���� )..

	vector<const TEX_INFO*> m_vecTexture;				// ������ �ִϸ��̼�..
	CAnimation*				m_pAnimCom;					// �ִϸ��̼� ������Ʈ..

	eGameEntityPattern		m_curActPatternKind;		// ���� ���� ���� ����..

	BYTE					m_byDirAnimIndex;

	CAStar*					m_pAStar;

	map<wstring, CEntityPattern*>	m_mapPatterns;
	CEntityPattern*					m_pCurActPattern;

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

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	bool CheckAlertEntity( const eObjectType& eObjectType, vector<CGameEntity*>* pVecEntitys = NULL );
	void MoveEntity();
	void UpdateDirAnimIndex();
	void LookPos( const D3DXVECTOR3& _vPos );

protected:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

	/* �׶� �ǹ� �� �̻��� �ͷ��� �ʿ�� �ϱ� ������ ???? */
	/*
	 * PushDirAnimTexture : ���� ������Ʈ�� ���⿡ ���� �ؽ��ĸ� �־���..
	 * UpdateDirAnimIndex : ���� ������Ʈ�� ���⿡ ���� �ε����� ��������..
	 */
	void ChangeDirAnimTexture();

};

