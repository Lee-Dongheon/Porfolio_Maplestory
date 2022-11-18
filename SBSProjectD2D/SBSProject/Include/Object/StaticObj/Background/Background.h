#pragma once
#include "../StaticObj.h"

class CBackground :
	public CStaticObj
{
public:
	CBackground();
	CBackground(const CBackground& bg);
	~CBackground();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CBackground* Clone();
};

