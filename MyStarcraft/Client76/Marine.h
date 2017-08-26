#pragma once
#include "Unit.h"

class CMarine :
	public CUnit
{
private:
	float	m_fSteampackDuration;	// ������ ���� �ð�..
	bool	m_bUseSteampack;

public:
	CMarine();
	virtual ~CMarine();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	// CUnit��(��) ���� ��ӵ�
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool& _bPrevPattern = FALSE ) override;
	virtual bool UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

};

