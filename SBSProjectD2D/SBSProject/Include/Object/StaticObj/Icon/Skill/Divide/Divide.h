#pragma once
#include "../ActiveSkill.h"

class CDivide :
	public CActiveSkill
{
public:
	CDivide();
	CDivide(const CDivide& divide);
	~CDivide();

private:
	int m_iRand;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CDivide* Clone();

public:
	virtual void ActiveSkill(CObj* pObj);
};

