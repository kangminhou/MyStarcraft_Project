#pragma once
#include "GameObject.h"

class CAnimation;
class CGameEntity :
	public CGameObject
{
public:
	enum eGameEntityPattern
	{
		Pattern_Idle,
		Pattern_Move,
		Pattern_Stop,
		Pattern_Hold,
		Pattern_Patrol,
		Pattern_Attack,
	};

protected:
	COMMON_DATA				m_tInfoData;				// ���� �ɷ�ġ ������..

	wstring					m_wstrStateKey;				// ���� ���� Ű ( �̹��� ����� ���� )..

	vector<const TEX_INFO*> m_vecTexture;				// ������ �ִϸ��̼�..
	CAnimation*				m_pAnimCom;

	eGameEntityPattern		m_curActPattern;			// ���� ���� ���� ����..

	BYTE					m_byDirAnimIndex;

public:
	CGameEntity();
	virtual ~CGameEntity();

public:
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind ) PURE;

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

protected:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

	/* �׶� �ǹ� �� �̻��� �ͷ��� �ʿ�� �ϱ� ������ ???? */
	/*
	 * PushDirAnimTexture : ���� ������Ʈ�� ���⿡ ���� �ؽ��ĸ� �־���..
	 * UpdateDirAnimIndex : ���� ������Ʈ�� ���⿡ ���� �ε����� ��������..
	 */
	void PushDirAnimTexture();
	void UpdateDirAnimIndex();
	bool CheckAlertEntity( const eObjectType& eObjectType );

};

