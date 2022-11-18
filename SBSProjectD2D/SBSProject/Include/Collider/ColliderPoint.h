#pragma once
#include "Collider.h"

class CColliderPoint :
	public CCollider
{
protected:
	friend class CObj;

protected:
	CColliderPoint();
	CColliderPoint(const CColliderPoint& coll);
	virtual ~CColliderPoint();

private:
	POSITION m_tInfo;

public:
	void SetPoint(float x, float y);

public:
	POSITION GetInfo() const { return m_tInfo; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual bool Collision(CCollider* pDest);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CColliderPoint* Clone();
};

