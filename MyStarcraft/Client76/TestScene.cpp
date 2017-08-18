#include "stdafx.h"
#include "TestScene.h"

#include "ObjMgr.h"
#include "Factory.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

#include "Unit.h"

#include <chrono>

class CTest
{
public:
	static int iSang;
	static int iBock;
	static int iMove;
	static int iSo;

public:
	CTest()
	{
		++iSang;
	}

	CTest(CTest& _test)
	{
		++iBock;
	}

	CTest(CTest&& _test)
	{
		++iMove;
	}

	~CTest()
	{
		++iSo;
	}
};

int CTest::iSang = 0;
int CTest::iBock = 0;
int CTest::iMove = 0;
int CTest::iSo = 0;

CTestScene::CTestScene()
{
}


CTestScene::~CTestScene()
{
}

HRESULT CTestScene::Initialize( void )
{
	CObjMgr::GetInstance()->Initialize();

	list<CTest>			intList;
	queue<CTest>		queueInt;

	{
		auto start = chrono::high_resolution_clock::now();
		auto end = chrono::high_resolution_clock::now();

		start = chrono::high_resolution_clock::now();
		for ( int i = 0; i < 1000; ++i )
		{
			intList.push_back( CTest() );
		}
		end = chrono::high_resolution_clock::now();
		cout << "List push_back Cnt (*1000) : " << (end - start).count() << endl;

		cout << endl;

		start = chrono::high_resolution_clock::now();
		for ( int i = 0; i < 1000; ++i )
		{
			intList.pop_back();
		}
		end = chrono::high_resolution_clock::now();
		cout << "List pop_back Cnt (*1000) : " << (end - start).count() << endl;
	}

	cout << endl;

	cout << "생성자 호출 개수 : " << CTest::iSang << endl;
	cout << "복사 생성자 호출 개수 : " << CTest::iBock << endl;
	cout << "이동 생성자 호출 개수 : " << CTest::iMove << endl;
	cout << "소멸자 호출 개수 : " << CTest::iSo << endl;

	CTest::iSang = 0;
	CTest::iBock = 0;
	CTest::iMove = 0;
	CTest::iSo = 0;

	cout << endl;

	{
		auto start = chrono::high_resolution_clock::now();
		auto end = chrono::high_resolution_clock::now();

		start = chrono::high_resolution_clock::now();
		for ( int i = 0; i < 1000; ++i )
		{
			queueInt.push( CTest() );
		}
		end = chrono::high_resolution_clock::now();
		cout << "Queue push Cnt (*1000) : " << (end - start).count() << endl;

		cout << endl;

		start = chrono::high_resolution_clock::now();
		for ( int i = 0; i < 1000; ++i )
		{
			queueInt.pop();
		}
		end = chrono::high_resolution_clock::now();
		cout << "Queue pop Cnt (*1000) : " << (end - start).count() << endl;
	}

	cout << endl;

	cout << "생성자 호출 개수 : " << CTest::iSang << endl;
	cout << "복사 생성자 호출 개수 : " << CTest::iBock << endl;
	cout << "이동 생성자 호출 개수 : " << CTest::iMove << endl;
	cout << "소멸자 호출 개수 : " << CTest::iSo << endl;

	return S_OK;
}

int CTestScene::Update( void )
{
	CObjMgr::GetInstance()->Update();

	return 0;
}

void CTestScene::Render( void )
{
	CObjMgr::GetInstance()->Render();
}

void CTestScene::Release( void )
{
	
}
