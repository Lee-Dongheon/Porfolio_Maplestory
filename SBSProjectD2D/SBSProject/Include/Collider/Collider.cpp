#include "Collider.h"
#include "../Object/Obj.h"

CCollider::CCollider()
{
}

CCollider::CCollider(const CCollider & coll)
{
	*this = coll;
}

CCollider::~CCollider()
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_CollisionList.end();

	for (iter = m_CollisionList.begin(); iter != iterEnd; iter++)
	{
		(*iter)->EraseCollisionList(this);
	}
}

void CCollider::Input(float fDeltaTime)
{
}

int CCollider::Update(float fDeltaTime)
{
	return 0;
}

int CCollider::LateUpdate(float fDeltaTime)
{
	return 0;
}

bool CCollider::Collision(CCollider * pDest)
{
	return false;
}

void CCollider::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
}

bool CCollider::CollisionRectToRect(const RECTANGLE & src, const RECTANGLE & dest)
{
	if (src.l > dest.r)
		return false;
	else if (src.r < dest.l)
		return false;
	else if (src.t > dest.b)
		return false;
	else if (src.b < dest.t)
		return false;

	return true;
}

bool CCollider::CollisionRectToPoint(const RECTANGLE & src, const POSITION & dest)
{
	if (dest.x < src.l)
		return false;
	else if (dest.x > src.r)
		return false;
	else if (dest.y < src.t)
		return false;
	else if (dest.y > src.b)
		return false;

	return true;
}

bool CCollider::CollisionRectToPixel(const RECTANGLE & src, const POSITION& tPixelPos, const vector<PIXEL>& vecPixel, int iWidth, int iHeight, PIXEL& rgb)
{
	m_tHitPoint.x = -1;
	m_tHitPoint.y = -1;

	int iStartX, iEndX;
	int iStartY, iEndY;

	iStartX = src.l < (int)tPixelPos.x ? (int)tPixelPos.x : src.l;
	iEndX = src.r >= iWidth + (int)tPixelPos.x ? iWidth + (int)tPixelPos.x - 1 : src.r;

	iStartY = src.t < (int)tPixelPos.y ? (int)tPixelPos.y : src.t;
	iEndY = src.b >= iHeight + (int)tPixelPos.y ? iHeight + (int)tPixelPos.y - 1 : src.b;

	for (int i = iStartY; i <= iEndY; ++i)
	{
		for (int j = iStartX; j <= iEndX; ++j)
		{
			int idx = (i - (int)tPixelPos.y) * iWidth + j - (int)tPixelPos.x;
			const PIXEL& pixel = vecPixel[idx];
			if (pixel.r == rgb.r && pixel.g == rgb.g && pixel.b == rgb.b)
			{
				m_tHitPoint.x = j;
				m_tHitPoint.y = i;

				return true;
			}
		}
	}

	return false;
}
