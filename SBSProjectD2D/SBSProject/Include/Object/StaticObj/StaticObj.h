#pragma once
#include "../Obj.h"

class CStaticObj :
	public CObj
{
public:
	CStaticObj();
	CStaticObj(const CStaticObj& obj);
	virtual ~CStaticObj();

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CStaticObj* Clone() = 0;
};

