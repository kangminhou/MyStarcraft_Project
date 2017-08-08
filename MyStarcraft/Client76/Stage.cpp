#include "StdAfx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "objmgr.h"
#include "Factory.h"

HRESULT CStage::Initialize(void)
{
	return S_OK;
}

int CStage::Update(void)
{
	CObjMgr::GetInstance()->Update();


	return 0;
}

void CStage::Render(void)
{
	CObjMgr::GetInstance()->Render();
}

void CStage::Release(void)
{
	CObjMgr::GetInstance()->Release();
}

CStage::CStage(void)
{
}

CStage::~CStage(void)
{
	Release();
}
