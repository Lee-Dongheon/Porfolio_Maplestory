#include "Bolt.h"

CBolt::CBolt()
{
}

CBolt::CBolt(const CBolt & bolt) :
	CActiveSkill(bolt)
{
}

CBolt::~CBolt()
{
}

bool CBolt::Init()
{
	SetType(ST_MOVE);
	//SetSize(284.f, 280.f);
	SetLevel(0);
	SetStepLevel(1);
	SetDamage(0); // 500 + level * 50
	SetCost(10);
	SetMaxLevel(5);

	SetSkillName(L"º¼Æ®");

	m_fDelay = 0.f;

	CAnimation* pAni = CreateAnimation("BoltAnimation");
	SAFE_RELEASE(pAni);

	float arrDelay[18];
	fill_n(arrDelay, 18, 0.06f);

	POSITION arrOrigin[18] = { POSITION(98.f, 26.f), POSITION(186.f, 114.f),
		POSITION(187.f, 138.f), POSITION(188.f, 122.f),
		POSITION(189.f, 125.f), POSITION(189.f, 126.f),
		POSITION(189.f, 127.f), POSITION(190.f, 129.f),
		POSITION(187.f, 127.f), POSITION(185.f, 127.f),
		POSITION(183.f, 126.f), POSITION(161.f, 111.f),
		POSITION(141.f, 87.f), POSITION(118.f, 81.f),
		POSITION(118.f, 78.f), POSITION(118.f, 51.f),
		POSITION(118.f, 45.f), POSITION(118.f, 44.f) };
	POSITION arrLT[18] = { POSITION(-98.f, -26.f), POSITION(-186.f, -114.f),
		POSITION(-187.f, -138.f), POSITION(-188.f, -122.f),
		POSITION(-189.f, -125.f), POSITION(-189.f, -126.f),
		POSITION(-189.f, -127.f), POSITION(-190.f, -129.f),
		POSITION(-187.f, -127.f), POSITION(-185.f, -127.f),
		POSITION(-183.f, -126.f), POSITION(-161.f, -111.f),
		POSITION(-141.f, -87.f), POSITION(-118.f, -81.f),
		POSITION(-118.f, -78.f), POSITION(-118.f, -51.f),
		POSITION(-118.f, -45.f), POSITION(-118.f, -44.f) };
	POSITION arrRB[18] = { POSITION(122.f, 10.f), POSITION(210.f, 82.f),
		POSITION(221.f, 126.f), POSITION(232.f, 86.f),
		POSITION(239.f, 87.f), POSITION(243.f, 86.f),
		POSITION(243.f, 89.f), POSITION(250.f, 86.f),
		POSITION(249.f, 85.f), POSITION(251.f, 85.f),
		POSITION(249.f, 82.f), POSITION(235.f, 73.f),
		POSITION(227.f, 53.f), POSITION(206.f, 63.f),
		POSITION(206.f, 58.f), POSITION(206.f, 49.f),
		POSITION(186.f, 31.f), POSITION(186.f, 28.f) };
	AddAnimationClip("Bolt", AO_ONCE_DESTROY, 18, arrOrigin, arrLT, arrRB, arrDelay,
		"Bolt", L"Player/Skill/Adel/Bolt/Bolt");

	LoadButton("BoltIcon", L"Player/Skill/Adel/Bolt/Icon/", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(32.f, 32.f));
	SetIconFunction((CActiveSkill*)this, &CActiveSkill::UseSkill);

	return true;
}

void CBolt::Input(float fDeltaTime)
{
	CActiveSkill::Input(fDeltaTime);
}

int CBolt::Update(float fDeltaTime)
{
	CActiveSkill::Update(fDeltaTime);
	return 0;
}

int CBolt::LateUpdate(float fDeltaTime)
{
	CActiveSkill::LateUpdate(fDeltaTime);
	return 0;
}

void CBolt::Collision(float fDeltaTime)
{
	CActiveSkill::Collision(fDeltaTime);
}

void CBolt::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CActiveSkill::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CBolt * CBolt::Clone()
{
	return new CBolt(*this);
}

void CBolt::ActiveSkill(CObj * pObj)
{
}
