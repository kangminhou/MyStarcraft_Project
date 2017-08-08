#pragma once
#include "Scene.h"

class CFileCopyScene :
	public CScene
{
public:
	CFileCopyScene();
	virtual ~CFileCopyScene();

	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

private:
	errno_t FileOpen( FILE** ppFp, char szFileName[], char szMode[] );

};

