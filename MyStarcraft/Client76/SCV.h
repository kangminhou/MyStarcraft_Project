#pragma once
#include "Unit.h"

class CBuilding;
class CPlayer;

class CSCV :
	public CUnit
{
private:
	bool	m_bCanBuild;
	bool	m_bBuildAdvancedStructure;
	bool	m_bRenderBuildingArea;

	const TEX_INFO*					m_pAreaTexture;
	CBuilding*						m_pDrawBuilding;

	CPlayer*						m_pPlayer;

	static vector<BUTTON_DATA*>*	m_pVecStructureButton;
	static vector<BUTTON_DATA*>*	m_pVecAdvancedStructureButton;

public:
	CSCV();
	virtual ~CSCV();

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

	static void InitButton( vector<BUTTON_DATA*>* _pVecButtonData, vector<BUTTON_DATA*>* _pVecAdvancedStructureButton );
	static void DeleteButton();

};

