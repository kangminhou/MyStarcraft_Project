#pragma once
#include "Unit.h"

class CGoliath :
	public CUnit
{
private:
	CAnimation*				m_pCannonAnim;
	vector<const TEX_INFO*>	m_vecTrunkTexture;
	D3DXMATRIX				m_matCannonWorld;

	wstring					m_wstrCannonObjKey;
	wstring					m_wstrCannonStateKey;

	bool					m_bInfluenceDir;

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

	virtual void ChangeDirAnimIndex() override;

public:
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern = FALSE ) override;

};

