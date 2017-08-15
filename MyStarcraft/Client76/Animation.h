#pragma once
#include "Component.h"
#include "Include.h"

class CAnimation :
	public CComponent
{
public:
	enum eAnim_Kind {	Anim_Loop, 			// 계속 반복 재생..
						Anim_ClampForever 	// 끝까지 가면 멈춤..
	};

private:
	typedef struct tagAnimData
	{
		FRAME		tFrame;
		eAnim_Kind	eKind;
	}ANIM_DATA;

private:
	map<wstring, ANIM_DATA*>	m_mapAnimation;
	ANIM_DATA*					m_pCurAnimation;

	bool						m_bAnimEnd;

public:
	CAnimation();
	virtual ~CAnimation();

public:
	void SetAnimationData( const wstring& _wstrName, const FRAME& _tFrame );
	void ChangeAnimationSpeed( const wstring& _wstrName, const float& _fSpeed );
	void ResetFrame();

public:
	const FRAME*	GetCurAnimation();
	const bool		GetIsAnimEnd() const;

public:
	// CComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	void UpdateAnim();
	virtual void Release() override;

public:
	void AddAnimation( const wstring& _wstrName, const FRAME& _tFrame, const eAnim_Kind& eAnimKind );
	bool ChangeAnimation( const wstring& _wstrName );

};

