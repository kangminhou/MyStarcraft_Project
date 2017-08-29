#pragma once
#include "Unit.h"

class CEffectMgr;

class CGhost :
	public CUnit
{
private:
	CEffectMgr*	m_pEffectMgr;

	MOUSE_DRAG_DATA	m_tDragData;

	bool		m_bUseNuclear;

	D3DCOLOR	m_color;

public:
	CGhost();
	~CGhost();

public:
	void NuclearEnd();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	// CUnit을(를) 통해 상속됨
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool& _bPrevPattern = FALSE ) override;
	virtual bool UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

};

