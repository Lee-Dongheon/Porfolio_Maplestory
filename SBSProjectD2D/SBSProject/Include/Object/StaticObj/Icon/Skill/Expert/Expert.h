#pragma once
#include "../PassiveSkill.h"

class CExpert :
	public CPassiveSkill
{
public:
	CExpert();
	CExpert(const CExpert& expert);
	~CExpert();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CExpert* Clone();
};

