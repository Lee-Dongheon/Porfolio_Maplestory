#include "ColliderPoint.h"
#include "ColliderRect.h"

CColliderPoint::CColliderPoint()
{
}

CColliderPoint::CColliderPoint(const CColliderPoint & coll):
	CCollider(coll)
{
	m_tInfo = coll.m_tInfo;
}

CColliderPoint::~CColliderPoint()
{
}

void CColliderPoint::SetPoint(float x, float y)
{
	m_tInfo.x = x;
	m_tInfo.y = y;
}

bool CColliderPoint::Init()
{
	return true;
}

void CColliderPoint::Input(float fDeltaTime)
{
	CCollider::Input(fDeltaTime);
}

int CColliderPoint::Update(float fDeltaTime)
{
	CCollider::Update(fDeltaTime);
	return 0;
}

int CColliderPoint::LateUpdate(float fDeltaTime)
{
	CCollider::LateUpdate(fDeltaTime);
	return 0;
}

bool CColliderPoint::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToPoint(((CColliderRect*)pDest)->GetWorldInfo(), m_tInfo);
	}
	return false;
}

void CColliderPoint::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CCollider::Render( pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CColliderPoint * CColliderPoint::Clone()
{
	return new CColliderPoint(*this);
}
