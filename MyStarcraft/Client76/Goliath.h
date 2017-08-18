#pragma once
#include "Unit.h"

class CGoliath :
	public CUnit
{
private:
	CAnimation*				m_pCannonAnim;
	vector<const TEX_INFO*>	m_vecCannonTexture[2];
	vector<const TEX_INFO*>	m_vecDrawCannonTexture;
	D3DXMATRIX				m_matCannonWorld;

	ATTACK_DATA				m_tNormalTankGroundAttData;
	ATTACK_DATA				m_tSiegeTankGroundAttData;

	int						m_iModeChangeLevel;

	bool					m_bChangeMode;
	bool					m_bSiegeMode;

public:
	CGoliath();
	virtual ~CGoliath();

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

public:
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern = FALSE ) override;

};

