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

	vector<CGameEntity*>	m_vecUseEntity;
	int			m_iMaxGatherNum;

public:
	CMineral();
	virtual ~CMineral();

public:
	void SetUseEntity( CGameEntity* _pEntity );
	void SetMaxGatherNum( const int& _iMaxGatherNum );

public:
	bool GetIsCanGather() const;
	int GetMaxGatherNum() const;

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

