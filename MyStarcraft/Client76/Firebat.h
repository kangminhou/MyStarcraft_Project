#pragma once
#include "Unit.h"

class CFirebat :
	public CUnit
{
private:
	float	m_fSteampackDuration;	// Ω∫∆¿∆— ¡ˆº” Ω√∞£..
	bool	m_bUseSteampack;

public:
	CFirebat();
	virtual ~CFirebat();

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Release( void ) override;
	virtual void Render( void ) override;

public:
	// CUnit¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern = FALSE ) override;
	virtual bool UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

};

