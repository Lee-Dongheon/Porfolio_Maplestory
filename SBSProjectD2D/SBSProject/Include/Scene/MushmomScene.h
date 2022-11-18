#pragma once
#include "Scene.h"
class CMushmomScene :
	public CScene
{
private:
	friend class CSceneManager;

private:
	CMushmomScene();
	~CMushmomScene();

public:
	virtual bool Init();
};

