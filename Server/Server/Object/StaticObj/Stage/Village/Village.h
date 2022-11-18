#pragma once
#include "../Stage.h"
class CVillage :
	public CStage
{
protected:
	friend class CObj;
	friend class CScene;

protected:
	CVillage();
	CVillage(const CVillage& village);
	virtual ~CVillage();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CStage* Clone();
};

