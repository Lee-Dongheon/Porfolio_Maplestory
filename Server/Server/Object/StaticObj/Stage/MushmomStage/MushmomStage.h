#pragma once
#include "../Stage.h"
class CMushmomStage :
	public CStage
{
private:
	friend class CObj;
	friend class CScene;

protected:
	CMushmomStage();
	CMushmomStage(const CMushmomStage& stage);
	virtual ~CMushmomStage();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CStage* Clone();
};

