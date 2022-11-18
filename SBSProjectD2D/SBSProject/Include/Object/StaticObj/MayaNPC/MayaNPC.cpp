#include "MayaNPC.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../../../Core/Camera.h"
#include "../../../Core/Core.h"

CMayaNPC::CMayaNPC():
	m_bClicked(false),
	m_fEyeTime(0.f),
	m_fEyeLimitTime(0.f)
{
	//printf("Create NPC!");
}

CMayaNPC::CMayaNPC(const CMayaNPC & obj) :
	CStaticObj(obj)
{
}

CMayaNPC::~CMayaNPC()
{
}

bool CMayaNPC::Init()
{
	SetSize(44.f, 71.f);
	SetPivot(0.5f, 1.f);

	CAnimation* pAnimation = CreateAnimation("MayaAnimation");
	SAFE_RELEASE(pAnimation);

	POSITION arrOrigin[1] = { POSITION(21.f, 71.f) };
	POSITION arrLT[1] = { POSITION(-21.f, -71.f) };
	POSITION arrRB[1] = { POSITION(23.f, 0.f) };
	float arrDelay[1] = { 0.3f };
	AddAnimationClip("MayaStand", AO_LOOP, 1, arrOrigin, arrLT, arrRB, arrDelay, "MayaStand", L"NPC/Stand/");
	
	POSITION arrOrigin2[4] = { POSITION(21.f, 71.f), POSITION(21.f, 71.f),
		POSITION(21.f, 71.f), POSITION(21.f, 71.f) };
	POSITION arrLT2[4] = { POSITION(-21.f, -71.f), POSITION(-21.f, -71.f), 
		POSITION(-21.f, -71.f), POSITION(-21.f, -71.f) };
	POSITION arrRB2[4] = { POSITION(23.f, 0.f), POSITION(23.f, 0.f), 
		POSITION(23.f, 0.f), POSITION(23.f, 0.f) };
	float arrDelay2[4] = { 0.1f, 0.1f, 0.1f, 0.1f };
	AddAnimationClip("MayaEye", AO_ONCE_RETURN, 4, arrOrigin2, arrLT2, arrRB2, arrDelay2, "MayaEye", L"NPC/Eye/");

	return true;
}

void CMayaNPC::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

int CMayaNPC::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);

	if (m_fEyeLimitTime == 0.f)
	{
		int iRand = rand() % 3 + 1;
		m_fEyeLimitTime = (float)iRand; // 1~3ÃÊ ÁÖ±â
	}
	m_fEyeTime += fDeltaTime;

	if (m_fEyeTime >= m_fEyeLimitTime)
	{
		m_fEyeTime -= m_fEyeLimitTime;
		m_fEyeLimitTime = 0.f;
		m_pAnimation->ChangeClip("MayaEye");
	}

	return 0;
}

int CMayaNPC::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void CMayaNPC::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);
}

void CMayaNPC::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CStaticObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CMayaNPC * CMayaNPC::Clone()
{
	return new CMayaNPC(*this);
}
