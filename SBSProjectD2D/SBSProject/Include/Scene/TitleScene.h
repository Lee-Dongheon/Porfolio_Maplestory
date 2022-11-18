#pragma once
#include "Scene.h"
class CTitleScene :
	public CScene
{
private:
	friend class CSceneManager;

private:
	CTitleScene();
	~CTitleScene();

public:
	virtual bool Init();
};

