#pragma once
#include "../Monster.h"
class CMushroom :
	public CMonster
{
public:
	CMushroom();
	CMushroom(const CMushroom& mushroom);
	virtual ~CMushroom();

public:
	virtual bool Init();
	//virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CMushroom* Clone();
};

