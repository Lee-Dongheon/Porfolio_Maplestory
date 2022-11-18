#include "KeyIcon.h"
#include "../../../../Resources/ResourcesManager.h"

CKeyIcon::CKeyIcon():
	m_strDefaultKey("")
{
}

CKeyIcon::CKeyIcon(const CKeyIcon & key):
	CIcon(key)
{
	m_strDefaultKey = key.m_strDefaultKey;
}

CKeyIcon::~CKeyIcon()
{
}

bool CKeyIcon::Init()
{
	SetIconType(IC_KEY);
	SetSize(32.f, 32.f);

	return true;
}

void CKeyIcon::Input(float fDeltaTime)
{
	CIcon::Input(fDeltaTime);
}

int CKeyIcon::Update(float fDeltaTime)
{
	CIcon::Update(fDeltaTime);
	return 0;
}

int CKeyIcon::LateUpdate(float fDeltaTime)
{
	CIcon::LateUpdate(fDeltaTime);
	return 0;
}

void CKeyIcon::Collision(float fDeltaTime)
{
	CIcon::Collision(fDeltaTime);
}

void CKeyIcon::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CIcon::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CKeyIcon * CKeyIcon::Clone()
{
	return new CKeyIcon(*this);
}
