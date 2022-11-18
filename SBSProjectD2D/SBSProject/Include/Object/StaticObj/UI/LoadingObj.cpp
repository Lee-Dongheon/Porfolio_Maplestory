#include "LoadingObj.h"
#include "../../../Resources/Texture.h"

CLoadingObj::CLoadingObj()
{
}

CLoadingObj::CLoadingObj(const CLoadingObj & obj) :
	CStaticObj(obj)
{
}

CLoadingObj::~CLoadingObj()
{
}

bool CLoadingObj::Init()
{
	CAnimation* pAnimation = CreateAnimation("LoadingAnimation");
	SAFE_RELEASE(pAnimation);

	POSITION arrOrigin[16] = { POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f),
								POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f),
								POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f),
								POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f) };
	POSITION arrLT[16] = { POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f),
								POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f),
								POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f),
								POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(0.f, 0.f) };
	POSITION arrRB[16] = { POSITION(80.f, 72.f), POSITION(80.f, 72.f), POSITION(80.f, 72.f), POSITION(80.f, 72.f), 
								POSITION(80.f, 72.f), POSITION(80.f, 72.f), POSITION(80.f, 72.f), POSITION(80.f, 72.f), 
								POSITION(80.f, 72.f), POSITION(80.f, 72.f), POSITION(80.f, 72.f), POSITION(80.f, 72.f), 
								POSITION(80.f, 72.f), POSITION(80.f, 72.f), POSITION(80.f, 72.f), POSITION(80.f, 72.f) };
	float arrDelay[16];
	fill_n(arrDelay, 16, 0.09f);
	AddAnimationClip("Loading", AO_LOOP, 16, arrOrigin, arrLT, arrRB, arrDelay, "Loading", L"UI/NoticeWindow/LoadingCircle/");

	return true;
}

void CLoadingObj::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

int CLoadingObj::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);
	return 0;
}

int CLoadingObj::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void CLoadingObj::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);
}

void CLoadingObj::Render(ID2D1GdiInteropRenderTarget * pGDIRenderTarget, ID2D1HwndRenderTarget * pRenderTarget, float fDeltaTime)
{
	//CStaticObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	if (m_pTexture)
	{
		ID2D1Bitmap* pD2DBitmap = m_pTexture->GetBitmap();
		POSITION tLT = m_pTexture->GetLT();
		POSITION tRB = m_pTexture->GetRB();

		// tPos 기준 texture 사각형 범위
		D2D1_RECT_F tRect;
		tRect.left = m_tPos.x + tLT.x;
		tRect.top = m_tPos.y + tLT.y;
		tRect.right = m_tPos.x + tRB.x;
		tRect.bottom = m_tPos.y + tRB.y;

		pRenderTarget->DrawBitmap(pD2DBitmap, &tRect);
	}
}

CLoadingObj * CLoadingObj::Clone()
{
	return new CLoadingObj(*this);
}
