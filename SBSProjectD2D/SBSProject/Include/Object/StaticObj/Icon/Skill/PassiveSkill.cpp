#include "PassiveSkill.h"

CPassiveSkill::CPassiveSkill()
{
}

CPassiveSkill::CPassiveSkill(const CPassiveSkill & skill):
	CSkill(skill)
{
}

CPassiveSkill::~CPassiveSkill()
{
}

void CPassiveSkill::Input(float fDeltaTime)
{
	CSkill::Input(fDeltaTime);
}

int CPassiveSkill::Update(float fDeltaTime)
{
	CSkill::Update(fDeltaTime);
	return 0;
}

int CPassiveSkill::LateUpdate(float fDeltaTime)
{
	CSkill::LateUpdate(fDeltaTime);
	return 0;
}

void CPassiveSkill::Collision(float fDeltaTime)
{
	CSkill::Collision(fDeltaTime);
}

void CPassiveSkill::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CSkill::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}