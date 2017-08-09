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
	COMMON_DATA				m_tInfoData;				// 유닛 능력치 데이터..

	wstring					m_wstrStateKey;				// 현재 상태 키 ( 이미지 출력을 위한 )..

	vector<const TEX_INFO*> m_vecTexture;				// 렌더할 애니메이션..
	CAnimation*				m_pAnimCom;					// 애니메이션 컴포넌트..

	eGameEntityPattern		m_curActPatternKind;		// 현재 실행 중인 패턴..

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
	// CGameObject을(를) 통해 상속됨
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

	/* 테란 건물 중 미사일 터렛도 필요로 하기 때문에 ???? */
	/*
	 * PushDirAnimTexture : 현재 오브젝트의 방향에 따라 텍스쳐를 넣어줌..
	 * UpdateDirAnimIndex : 현재 오브젝트의 방향에 따른 인덱스를 결정해줌..
	 */
	void ChangeDirAnimTexture();

};

