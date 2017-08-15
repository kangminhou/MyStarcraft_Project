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
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	// CUnit��(��) ���� ��ӵ�
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind ) override;

public:
	virtual bool UseSkill( const eGameEntitySkillKind& wstrSkillName, CGameEntity* _pTarget ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

};

