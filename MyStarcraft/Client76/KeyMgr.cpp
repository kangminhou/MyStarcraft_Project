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
	/* 키를 땐 후에 그 다음 업데이트 시 false 로 바꿔주기 위한 코드.. */
	while ( !m_queueUpKeyDownIndex.empty() )
	{
		int iUpKeyDownIndex = m_queueUpKeyDownIndex.back();

		if ( iUpKeyDownIndex == VK_LBUTTON )
			int a = 10;
		this->m_bUpKey[iUpKeyDownIndex] = false;

		m_queueUpKeyDownIndex.pop();

	}

	/* 키 체크 */
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
		if ( this->m_bStayDownKey[vKey] ) // 전에 이미 누른 키라면..
			this->m_bOnceDownKey[vKey] = false;
		else{ // 전에 안 누른 키라면..
			this->m_bStayDownKey[vKey] = true;
			this->m_bOnceDownKey[vKey] = true;
		}
	}
	else // 키 안 눌렀을 때..
	{
		if(this->m_bStayDownKey[vKey]) // 현재 키가 눌렸다가 땐 상태라면..
		{
			this->m_bUpKey[vKey] = true;
			this->m_bStayDownKey[vKey] = false;
			this->m_bOnceDownKey[vKey] = false;

			this->m_queueUpKeyDownIndex.push( vKey );
		}
	}
}
