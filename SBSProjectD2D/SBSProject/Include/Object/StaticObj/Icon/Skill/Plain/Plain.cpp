#include "Plain.h"
#include "../../../../MoveObj/Monster/Monster.h"

CPlain::CPlain()
{
}

CPlain::CPlain(const CPlain & plain):
	CActiveSkill(plain)
{
}

CPlain::~CPlain()
{
}

bool CPlain::Init()
{
	SetType(ST_ATTACK);
	//SetSize(300.f, 145.f);
	SetLT(-285.f, -130.f);
	SetRB(15.f, 15.f);
	SetLevel(0);
	SetStepLevel(1);
	SetDamage(500); // 500 + level * 50
	SetCost(0);
	SetMaxLevel(10);

	SetSkillName(L"ÇÃ·¹ÀÎ");

	m_fDelay = 0.7f;

	CAnimation* pAni = CreateAnimation("PlainAnimation");
	SAFE_RELEASE(pAni);

	float arrDelay[11] = { 0.09f, 0.09f, 0.09f, 0.09f, 0.09f, 0.09f,
		0.06f, 0.06f, 0.06f, 0.06f, 0.06f };

	POSITION arrOrigin[11] = { POSITION(-9.f, 43.f),
		POSITION(10.f, 43.f), POSITION(323.f, 182.f),
		POSITION(328.f, 203.f), POSITION(333.f, 180.f),
		POSITION(339.f, 185.f), POSITION(340.f, 177.f),
		POSITION(342.f, 178.f), POSITION(342.f, 173.f),
		POSITION(340.f, 163.f), POSITION(288.f, 153.f) };
	POSITION arrLT[11] = { POSITION(9.f, -43.f),
		POSITION(-10.f, -43.f), POSITION(-323.f, -182.f),
		POSITION(-328.f, -203.f), POSITION(-333.f, -180.f),
		POSITION(-339.f, -185.f), POSITION(-340.f, -177.f),
		POSITION(-342.f, -178.f), POSITION(-342.f, -173.f),
		POSITION(-340.f, -163.f), POSITION(-288.f, -153.f) };
	POSITION arrRB[11] = { POSITION(113.f, 5.f),
		POSITION(114.f, 13.f), POSITION(121.f, 58.f),
		POSITION(116.f, 65.f), POSITION(111.f, 60.f),
		POSITION(105.f, 63.f), POSITION(108.f, 59.f),
		POSITION(110.f, 54.f), POSITION(110.f, 57.f),
		POSITION(112.f, 41.f), POSITION(116.f, 39.f) };
	AddAnimationClip("Plain", AO_ONCE_DISABLE, 11, arrOrigin, arrLT, arrRB, arrDelay,
		"Plain", L"Player/Skill/Adel/Plain/Plain/");

	LoadButton("PlainIcon", L"Player/Skill/Adel/Plain/Icon/", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(32.f, 32.f));
	SetIconFunction((CActiveSkill*)this, &CActiveSkill::UseSkill);

	return true;
}

void CPlain::Input(float fDeltaTime)
{
	CActiveSkill::Input(fDeltaTime);
}

int CPlain::Update(float fDeltaTime)
{
	CActiveSkill::Update(fDeltaTime);
	return 0;
}

int CPlain::LateUpdate(float fDeltaTime)
{
	CActiveSkill::LateUpdate(fDeltaTime);
	return 0;
}

void CPlain::Collision(float fDeltaTime)
{
	CActiveSkill::Collision(fDeltaTime);
}

void CPlain::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CActiveSkill::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CPlain * CPlain::Clone()
{
	return new CPlain(*this);
}

void CPlain::ActiveSkill(CObj * pObj)
{
	if (m_iDamage > 0)
	{
		//((CMonster*)pObj)->SetDir(m_eDir);
		//((CMonster*)pObj)->AddHP(-m_iDamage);
		//((CMonster*)pObj)->SetAggressiveObj((CObj*)m_pPlayer);
	}
}