#pragma once
#include "Unit.h"

class CTank :
	public CUnit
{
private:
	CAnimation*				m_pCannonAnim;
	vector<const TEX_INFO*>	m_vecTankTrunkTexture;
	D3DXMATRIX				m_matCannonWorld;

	wstring					m_wstrCannonObjKey;
	wstring					m_wstrCannonStateKey;

	ATTACK_DATA				m_tNormalTankGroundAttData;
	ATTACK_DATA				m_tSiegeTankGroundAttData;

	int						m_iModeChangeLevel;

	bool					m_bChangeMode;
	bool					m_bSiegeMode;
	bool					m_bInfluenceDir;

public:
	CTank();
	virtual ~CTank();

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

	virtual void ChangeDirAnimIndex() override;

public:
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern = FALSE ) override;
	virtual bool UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget ) override;

private:
	void UpdateModeChange();
	void ModeChangeEnd();

};

