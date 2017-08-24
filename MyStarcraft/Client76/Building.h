#pragma once
#include "GameEntity.h"

class CPlayer;

class CBuilding :
	public CGameEntity
{
protected:
	/* 
	 * �ǹ��� �Ǽ� ���� �ִϸ��̼��� ������ ����..
	 * 
	 */
	static const TEX_INFO*	m_pBasicBuildTextureArr[3][3];	// �ǹ��� ���� �� �ع��� �ִϸ��̼�..

	D3DXVECTOR3				m_vTileOccupy;					// �����ϴ� Ÿ�� ����..
	map<wstring, vector<const TEX_INFO*>>	m_mapAllTexture;
	const TEX_INFO*			m_pCurDrawTexture;

	CPlayer*				m_pPlayer;

	bool					m_bSuccessBuild;				// �� ������ �ǹ��ΰ�??..
	bool					m_bApplyCol;
	bool					m_bCanBuild;

	const TEX_INFO*			m_pBuildRectTexture[2];

public:
	CBuilding();
	virtual ~CBuilding();

public:
	void SetApplyCol( const bool& _bApplyCol );

public:
	bool GetIsSuccessBuild() const;
	bool GetIsCanBuild() const;

public:
	virtual HRESULT Initialize( void );
	virtual int Update( void );
	virtual void Render( void );

	void FrameRender( const D3DXMATRIX& _matWorld );
	void RectRender( const RECT& _rcDraw );
	void BuildStart();

public:
	virtual void UpdateLookAnimIndex() override;
	virtual void SuccessBuild();

protected:
	virtual void DecideTexture() PURE;
	virtual void InitTexture() PURE;
	virtual bool CheckCanBuild( const int& _iIndex );

protected:
	D3DXVECTOR3 CalcNearEmptySpace();

public:
	static void InitBasicBuildTexture();

};

