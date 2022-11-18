#pragma once
#include "Collider.h"

class CColliderRect :
	public CCollider
{
protected:
	friend class CObj;

protected:
	CColliderRect();
	CColliderRect(const CColliderRect& coll);
	virtual ~CColliderRect();

private:
	RECTANGLE m_tInfo;
	RECTANGLE m_tWorldInfo;
	RECTANGLE m_tWorldInfoFixed;

public:
	void SetRect(RECTANGLE tInfo) { m_tInfo = tInfo; }
	void SetRect(float l, float t, float r, float b);
	void SetWorldInfo(float l, float t, float r, float b);
	
public:
	RECTANGLE GetInfo() const { return m_tInfo; }
	RECTANGLE GetWorldInfo() const { return m_tWorldInfo; }
	RECTANGLE GetWorldInfoFixed() const { return m_tWorldInfoFixed; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual bool Collision(CCollider* pDest);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CColliderRect* Clone();
};

