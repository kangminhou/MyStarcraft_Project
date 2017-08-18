#pragma once
#include "Scene.h"

class CFileCopyScene :
	public CScene
{
private:
	FILE*	m_pOpenFile;
	FILE*	m_pCopyFile;

	char	m_szImageFileName[128];
	char	m_szCopyFileName[128];

public:
	CFileCopyScene();
	virtual ~CFileCopyScene();

	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

private:
	errno_t FileOpen( FILE** ppFp, char szFileName[], char szMode[] );

	bool FileCopy();

};

