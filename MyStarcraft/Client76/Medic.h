#pragma once
#include "Unit.h"

class CMedic :
	public CUnit
{
private:
	ENTITY_MANA_DATA	m_tManaData;

public:
	CMedic();
	~CMedic();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	// CUnit을(를) 통해 상속됨
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern = FALSE ) override;

public:
	virtual bool UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget );

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

};

