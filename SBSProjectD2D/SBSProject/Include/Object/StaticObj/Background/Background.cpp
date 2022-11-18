#include "Background.h"
#include "../../../Core/BackCam.h"
#include "../../../Resources/Texture.h"

CBackground::CBackground()
{
}

CBackground::CBackground(const CBackground & bg)
{
}

CBackground::~CBackground()
{
}

bool CBackground::Init()
{
	SetPos(0.f, 0.f);

	return true;
}

void CBackground::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

int CBackground::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);
	return 0;
}

int CBackground::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void CBackground::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);
}

void CBackground::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	if (m_pTexture)
	{
		ID2D1Bitmap* pD2DBitmap = m_pTexture->GetBitmap();
		POSITION tLT = m_pTexture->GetLT();
		POSITION tRB = m_pTexture->GetRB();

		// 카메라 위치를 고려하여 위치 조정
		POSITION tPos = m_tPos - GET_SINGLE(CBackCam)->GetPos();

		// tPos 기준 texture 사각형 범위
		D2D1_RECT_F tRect;
		tRect.left = tPos.x + tLT.x;
		tRect.top = tPos.y + tLT.y;
		tRect.right = tPos.x + tRB.x;
		tRect.bottom = tPos.y + tRB.y;

		pRenderTarget->DrawBitmap(pD2DBitmap, &tRect);
	}
}

CBackground * CBackground::Clone()
{
	return new CBackground(*this);
}
