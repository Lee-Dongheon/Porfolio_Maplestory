#include "Expert.h"

CExpert::CExpert()
{
}

CExpert::CExpert(const CExpert & expert) :
	CPassiveSkill(expert)
{
}

CExpert::~CExpert()
{
}

bool CExpert::Init()
{
	SetType(ST_PASSIVE);
	SetLevel(0);
	SetStepLevel(3);
	SetMaxLevel(30);

	SetSkillName(L"¿¢½ºÆÛÆ®");

	// Atk level * 1, Critical Rate level * 1%

	LoadButton("ExpertIcon", L"Player/Skill/Adel/Expert/Icon/", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(32.f, 32.f));
	
	return true;
}

void CExpert::Input(float fDeltaTime)
{
	CPassiveSkill::Input(fDeltaTime);
}

int CExpert::Update(float fDeltaTime)
{
	CPassiveSkill::Update(fDeltaTime);
	return 0;
}

int CExpert::LateUpdate(float fDeltaTime)
{
	CPassiveSkill::LateUpdate(fDeltaTime);
	return 0;
}

void CExpert::Collision(float fDeltaTime)
{
	CPassiveSkill::Collision(fDeltaTime);
}

void CExpert::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CPassiveSkill::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CExpert * CExpert::Clone()
{
	return new CExpert(*this);
}
