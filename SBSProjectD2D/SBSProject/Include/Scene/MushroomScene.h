#pragma once
#include "Scene.h"
class CMushroomScene :
	public CScene
{
private:
	friend class CSceneManager;

private:
	CMushroomScene();
	~CMushroomScene();

public:
	virtual bool Init();
};

