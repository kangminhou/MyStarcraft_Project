#pragma once
#include "GameEntity.h"

class CMineral :
	public CGameEntity
{
private:
	vector<const TEX_INFO*>	m_vecMineralTexture[3];
	vector<const TEX_INFO*>	m_vecMineralShadTexture[3];

	int			m_iSelectMineral;
	int			m_iCurMineralIndex;

	D3DXMATRIX	m_shadWorldMatrix;

public:
	CMineral();
	virtual ~CMineral();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void )override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	virtual void UpdatePosition( const D3DXVECTOR3& vPrevPos ) override;

public:
	// CGameEntity��(��) ���� ��ӵ�
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern = FALSE ) override;

public:
	void UpdateTileData();

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;
};

