#pragma once
#include "../Stage.h"
class CMushroomStage :
	public CStage
{
private:
	friend class CObj;
	friend class CScene;

protected:
	CMushroomStage();
	CMushroomStage(const CMushroomStage& stage);
	virtual ~CMushroomStage();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CStage* Clone();
};

