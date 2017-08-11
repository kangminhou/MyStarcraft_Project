#include "StdAfx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr(void)
{
	for ( BYTE i = 0; i < MAX_KEY_NUM; ++i )
	{
		m_bStayDownKey[i] = m_bOnceDownKey[i] = m_bUpKey[i] = false;
	}
}

CKeyMgr::~CKeyMgr(void)
{
}

void CKeyMgr::Initialize()
{
	//this->m_vecCheckKey.push_back( VK_UP );
	//this->m_vecCheckKey.push_back( VK_DOWN );
	//this->m_vecCheckKey.push_back( VK_RIGHT );
	//this->m_vecCheckKey.push_back( VK_LEFT );
	//
	//this->m_vecCheckKey.push_back( VK_LBUTTON );
	//this->m_vecCheckKey.push_back( VK_RBUTTON );

	for ( int i = 0; i < 255; ++i )
		this->m_vecCheckKey.push_back( i );

	//this->m_vecCheckKey.push_back( 'S' );

	int a = 10;
}

void CKeyMgr::Update()
{
	/* Ű�� �� �Ŀ� �� ���� ������Ʈ �� false �� �ٲ��ֱ� ���� �ڵ�.. */
	while ( !m_queueUpKeyDownIndex.empty() )
	{
		int iUpKeyDownIndex = m_queueUpKeyDownIndex.back();

		if ( iUpKeyDownIndex == VK_LBUTTON )
			int a = 10;
		this->m_bUpKey[iUpKeyDownIndex] = false;

		m_queueUpKeyDownIndex.pop();

	}

	/* Ű üũ */
	size_t totalCheckKeyNum = this->m_vecCheckKey.size();
	//for ( BYTE i = 0; i < MAX_KEY_NUM; ++i )
	for ( size_t i = 0; i < totalCheckKeyNum; ++i )
	{
		this->KeyCheck( i );
	}

}

void CKeyMgr::KeyCheck( int vKey )
{
	if(GetAsyncKeyState(vKey))
	{
		if ( this->m_bStayDownKey[vKey] ) // ���� �̹� ���� Ű���..
			this->m_bOnceDownKey[vKey] = false;
		else{ // ���� �� ���� Ű���..
			this->m_bStayDownKey[vKey] = true;
			this->m_bOnceDownKey[vKey] = true;
		}
	}
	else // Ű �� ������ ��..
	{
		if(this->m_bStayDownKey[vKey]) // ���� Ű�� ���ȴٰ� �� ���¶��..
		{
			this->m_bUpKey[vKey] = true;
			this->m_bStayDownKey[vKey] = false;
			this->m_bOnceDownKey[vKey] = false;

			this->m_queueUpKeyDownIndex.push( vKey );
		}
	}
}
