#pragma once
#include "UI.h"
class CUIStat :
	public CUI
{
private:
	friend class CObj;

protected:
	CUIStat();
	CUIStat(const CUIStat& ui);
	virtual ~CUIStat();

//private:
//	bool m_bStatClicked;
//	POSITION m_tMousePos;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CUIStat* Clone();
};

