#include "StdAfx.h"
#include "SceneMgr.h"

#include "Logo.h"
#include "Stage.h"
#include "TestScene.h"
#include "FileCopyScene.h"
#include "AStarTestScene.h"

IMPLEMENT_SINGLETON(CSceneMgr);

void CSceneMgr::SetScene(eSceneType eType)
{
	safe_delete(m_pScene);

	switch (eType)
	{
	case SCENE_LOGO:
		m_pScene = new CLogo;
		break;

	case SCENE_STAGE:
		m_pScene = new CStage;
		break;

	case SCENE_TEST:
		m_pScene = new CTestScene;
		break;

	case SCENE_FILECOPY:
		m_pScene = new CFileCopyScene;
		break;

	case SCENE_ASTAR_TEST:
		m_pScene = new CAStarTestScene;
		break;
	}

	m_pScene->Initialize();
}

void CSceneMgr::SetChangeScene(eSceneType eType)
{
	m_bChangeScene = true;
	m_eType		   = eType;
}

HRESULT CSceneMgr::Initialize(void)
{
	return S_OK;
}

int CSceneMgr::Update(void)
{
	if(m_bChangeScene)
	{
		m_bChangeScene = false;
		SetScene(m_eType);
	}


	if(m_pScene)
		m_pScene->Update();

	return 0;
}

void CSceneMgr::Render(void)
{
	if(m_pScene)
		m_pScene->Render();
}

void CSceneMgr::Release(void)
{
	safe_delete(m_pScene);
}

CSceneMgr::CSceneMgr(void)
{
	m_pScene		= NULL;
	m_bChangeScene	= false;
	m_eType			= SCENE_MAX;
}

CSceneMgr::~CSceneMgr(void)
{
	Release();
}
