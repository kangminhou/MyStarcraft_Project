#pragma once
#include "Unit.h"

class CBuilding;
class CPlayer;

class CSCV :
	public CUnit
{
private:
	CGameObject*					m_pGatherObject;

	const TEX_INFO*					m_pAreaTexture;
	CBuilding*						m_pDrawBuilding;
	CGameEntity*					m_pBuildEntity;

	CPlayer*						m_pPlayer;

	bool	m_bCanBuild;					// 
	bool	m_bBuildAdvancedStructure;		// Advanced Structure 의 건물을 볼 것인가 ( UI 때문에 필요 )..
	bool	m_bRenderBuildingArea;			// 건물 완성본 출력하는가?? ( 플레이어의 SCV 가 사용 )..
	bool	m_bUnder_Construction;			// 건설중인가??..
	bool	m_bMineralGather;

	static vector<BUTTON_DATA*>*	m_pVecStructureButton;
	static vector<BUTTON_DATA*>*	m_pVecAdvancedStructureButton;

public:
	CSCV();
	virtual ~CSCV();

public:
	CGameObject* GetGatherObject() const;

public:
	CGameEntity* GetBuildEntity() const;
	bool GetIsUnderConstruction() const;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	// CUnit을(를) 통해 상속됨
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool& _bPrevPattern = FALSE ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;
	virtual bool UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget ) override;
	virtual void MouseEvent() override;

public:
	void RenderBuildingArea( CGameEntity* pBuilding );
	void BuildBuilding( CGameEntity* pEntity, const D3DXVECTOR3& _vPos );
	void BuildStart();
	void SuccessBuild();

	void GatherResource( const bool& _bMineral, CGameObject* _pObject );

	static void InitButton( vector<BUTTON_DATA*>*& _pVecButtonData, vector<BUTTON_DATA*>*& _pVecAdvancedStructureButton );
	static void DeleteButton();

};

