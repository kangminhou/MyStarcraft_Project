#pragma once
#include "Component.h"
#include "Include.h"

class CAnimation :
	public CComponent
{
public:
	enum eAnim_Kind {	Anim_Loop, 			// ��� �ݺ� ���..
						Anim_ClampForever 	// ������ ���� ����..
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

public:
	CAnimation();
	virtual ~CAnimation();

public:
	void SetAnimationData( const wstring& _wstrName, const FRAME& _tFrame );

public:
	const FRAME*	GetCurAnimation();

public:
	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	void UpdateAnim();
	virtual void Release() override;

public:
	void AddAnimation( const wstring& _wstrName, const FRAME& _tFrame, const eAnim_Kind& eAnimKind );
	bool ChangeAnimation( const wstring& _wstrName );

};

