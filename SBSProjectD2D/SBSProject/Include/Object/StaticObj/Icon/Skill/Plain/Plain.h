#pragma once
#include "../ActiveSkill.h"
class CPlain :
	public CActiveSkill
{
public:
	CPlain();
	CPlain(const CPlain& plain);
	~CPlain();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CPlain* Clone();

public:
	virtual void ActiveSkill(CObj* pObj);
};

