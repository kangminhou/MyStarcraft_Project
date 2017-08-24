#pragma once
#include "GameEntity.h"

class CRefinery;

class CGas :
	public CGameEntity
{
private:
	vector<const TEX_INFO*>	m_vecGasaTexture;
	vector<const TEX_INFO*>	m_vecGasShadTexture;

	int			m_iSelectGasIndex;

	D3DXMATRIX	m_shadWorldMatrix;

	CRefinery*	m_pRefinery;

	bool		m_bCanGather;

public:
	CGas();
	virtual ~CGas();

public:
	bool GetIsCanGather() const;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void )override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	void BuildRefinery( CRefinery* _pBuilding );
	void UpdateTileData();

public:
	virtual void UpdatePosition( const D3DXVECTOR3& vPrevPos ) override;

public:
	// CGameEntity��(��) ���� ��ӵ�
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern = FALSE ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

};

