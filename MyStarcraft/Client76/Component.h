#pragma once

class CGameObject;
class CComponent
{
private:
	CGameObject*	m_pGameObject;
	bool			m_bActive;

public:
	CComponent();
	virtual ~CComponent();

public:
	void SetActive( const bool& _bActive );
	void SetGameObject( CGameObject* pGameObject );

public:
	bool GetActive() const;
	const CGameObject* GetGameObject() const;

public:
	virtual void Initialize() PURE;
	virtual void Release() PURE;

};

