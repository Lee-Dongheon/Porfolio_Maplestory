#include "Divide.h"
#include "../../../../MoveObj/Monster/Monster.h"

CDivide::CDivide() :
	m_iRand(0)
{
}

CDivide::CDivide(const CDivide & divide) :
	CActiveSkill(divide)
{
	m_iRand = 0;
}

CDivide::~CDivide()
{
}

bool CDivide::Init()
{
	SetType(ST_ATTACK);
	// Divide1 : 756 x 436
	// Divide2 : 676 x 357
	// Divide3 : 695 x 501
	//SetSize(485.f, 340.f);
	SetLT(-450.f, -205.f);
	SetRB(35.f, 135.f);
	SetLevel(0);
	SetStepLevel(4);
	SetDamage(10); // 500 + level * 50
	SetCost(0);
	SetMaxLevel(30);

	SetSkillName(L"디바이드");

	m_fDelay = 0.7f;

	CAnimation* pAni = CreateAnimation("DivideAnimation");
	SAFE_RELEASE(pAni);
	///////////////////////////////////////////
	float arrDelay[10];
	fill_n(arrDelay, 10, 0.09f);

	POSITION arrOrigin[10] = { POSITION(36.f, 163.f), POSITION(71.f, 172.f),
								POSITION(84.f, 231.f), POSITION(434.f, 253.f),
								POSITION(595.f, 256.f), POSITION(595.f, 259.f),
								POSITION(558.f, 262.f), POSITION(558.f, 264.f),
								POSITION(579.f, 244.f), POSITION(572.f, 196.f) };
	POSITION arrLT[10] = { POSITION(-36.f, -163.f), POSITION(-71.f, -172.f),
							POSITION(-84.f, -231.f), POSITION(-434.f, -253.f),
							POSITION(-595.f, -256.f), POSITION(-595.f, -259.f),
							POSITION(-558.f, -262.f), POSITION(-558.f, -264.f),
							POSITION(-579.f, -244.f), POSITION(-572.f, -196.f) };
	POSITION arrRB[10] = { POSITION(124.f, -3.f), POSITION(125.f, 72.f),
							POSITION(116.f, 79.f), POSITION(154.f, 137.f),
							POSITION(159.f, 164.f), POSITION(105.f, 169.f),
							POSITION(106.f, 170.f), POSITION(142.f, 172.f),
							POSITION(105.f, 172.f), POSITION(-28.f, 124.f) };
	AddAnimationClip("Divide1", AO_ONCE_DISABLE, 10, arrOrigin, arrLT, arrRB, arrDelay,
		"Divide1", L"Player/Skill/Adel/Divide/Divide1/");

	CEffectObj* pEffect = new CEffectObj;
	pEffect->Init();
	pAni = pEffect->CreateAnimation("DivideHitAnimation1");
	SAFE_RELEASE(pAni);

	float arrHitDelay[7];
	fill_n(arrHitDelay, 7, 0.06f);

	POSITION arrHitOrigin[7] = { POSITION(92.f, 87.f),
								POSITION(85.f, 104.f), POSITION(88.f, 107.f),
								POSITION(89.f, 113.f), POSITION(89.f, 113.f),
								POSITION(78.f, 66.f), POSITION(80.f, 32.f) };
	POSITION arrHitLT[7] = { POSITION(-92.f, -87.f),
								POSITION(-85.f, -104.f), POSITION(-88.f, -107.f),
								POSITION(-89.f, -113.f), POSITION(-89.f, -113.f),
								POSITION(-78.f, -66.f), POSITION(-80.f, -32.f) };
	POSITION arrHitRB[7] = { POSITION(88.f, 85.f),
								POSITION(83.f, 96.f), POSITION(92.f, 101.f),
								POSITION(91.f, 103.f), POSITION(91.f, 103.f),
								POSITION(78.f, 66.f), POSITION(80.f, 36.f) };
	pEffect->AddAnimationClip("DivideHit1", AO_ONCE_DESTROY, 7, arrHitOrigin, arrHitLT, arrHitRB, arrHitDelay, "DivideHit1", L"Player/Skill/Adel/Divide/Hit/1/");
	m_vecHitEffectObj.push_back(pEffect);

	/////////////////////////////////////////
	_SIZE arrSize2[10] = { _SIZE(280.f, 252.f), _SIZE(608.f, 304.f),
							_SIZE(608.f, 344.f), _SIZE(644.f, 352.f),
							_SIZE(656.f, 348.f), _SIZE(596.f, 356.f),
							_SIZE(596.f, 356.f), _SIZE(600.f, 352.f),
							_SIZE(492.f, 348.f), _SIZE(496.f, 344.f) };
	POSITION arrOrigin2[10] = { POSITION(382.f, 167.f), POSITION(513.f, 192.f),
								POSITION(487.f, 217.f), POSITION(485.f, 220.f),
								POSITION(493.f, 220.f), POSITION(488.f, 225.f),
								POSITION(487.f, 224.f), POSITION(484.f, 221.f),
								POSITION(485.f, 220.f), POSITION(487.f, 216.f) };
	POSITION arrLT2[10] = { POSITION(-382.f, -167.f), POSITION(-513.f, -192.f),
							POSITION(-487.f, -217.f), POSITION(-485.f, -220.f),
							POSITION(-493.f, -220.f), POSITION(-488.f, -225.f),
							POSITION(-487.f, -224.f), POSITION(-484.f, -221.f),
							POSITION(-485.f, -220.f), POSITION(-487.f, -216.f) };
	POSITION arrRB2[10] = { POSITION(-102.f, 85.f), POSITION(95.f, 112.f),
							POSITION(121.f, 127.f), POSITION(159.f, 132.f),
							POSITION(163.f, 128.f), POSITION(108.f, 131.f),
							POSITION(109.f, 132.f), POSITION(116.f, 131.f),
							POSITION(7.f, 128.f), POSITION(9.f, 128.f) };
	AddAnimationClip("Divide2", AO_ONCE_DISABLE, 10, arrOrigin2, arrLT2, arrRB2, arrDelay,
		"Divide2", L"Player/Skill/Adel/Divide/Divide2/");

	pEffect = new CEffectObj;
	pEffect->Init();
	pAni = pEffect->CreateAnimation("DivideHitAnimation2");
	SAFE_RELEASE(pAni);

	float arrHitDelay2[6];
	fill_n(arrHitDelay2, 6, 0.06f);

	POSITION arrHitOrigin2[6] = { POSITION(115.f, 92.f), POSITION(104.f, 103.f),
		POSITION(135.f, 134.f), POSITION(140.f, 139.f),
		POSITION(137.f, 136.f), POSITION(118.f, 117.f) };
	POSITION arrHitLT2[6] = { POSITION(-115.f, -92.f), POSITION(-104.f, -103.f),
		POSITION(-135.f, -134.f), POSITION(-140.f, -139.f),
		POSITION(-137.f, -136.f), POSITION(-118.f, -117.f) };
	POSITION arrHitRB2[6] = { POSITION(109.f, 92.f), POSITION(100.f, 101.f),
		POSITION(133.f, 134.f), POSITION(136.f, 137.f),
		POSITION(135.f, 136.f), POSITION(114.f, 115.f) };
	pEffect->AddAnimationClip("DivideHit2", AO_ONCE_DESTROY, 6, arrHitOrigin2, arrHitLT2, arrHitRB2, arrHitDelay2, "DivideHit2", L"Player/Skill/Adel/Divide/Hit/2/");
	m_vecHitEffectObj.push_back(pEffect);
	///////////////////////////////////////////
	_SIZE arrSize3[10] = { _SIZE(216.f, 284.f), _SIZE(256.f, 364.f),
							_SIZE(272.f, 416.f), _SIZE(260.f, 356.f),
							_SIZE(652.f, 488.f), _SIZE(632.f, 488.f),
							_SIZE(624.f, 484.f), _SIZE(632.f, 472.f),
							_SIZE(620.f, 428.f), _SIZE(608.f, 404.f) };
	POSITION arrOrigin3[10] = { POSITION(63.f, 222.f), POSITION(69.f, 266.f),
								POSITION(72.f, 303.f), POSITION(74.f, 287.f),
								POSITION(490.f, 290.f), POSITION(495.f, 290.f),
								POSITION(495.f, 288.f), POSITION(481.f, 287.f),
								POSITION(480.f, 250.f), POSITION(465.f, 237.f) };
	POSITION arrLT3[10] = { POSITION(-63.f, -222.f), POSITION(-69.f, -266.f),
							POSITION(-72.f, -303.f), POSITION(-74.f, -287.f),
							POSITION(-490.f, -290.f), POSITION(-495.f, -290.f),
							POSITION(-495.f, -288.f), POSITION(-481.f, -287.f),
							POSITION(-480.f, -250.f), POSITION(-465.f, -237.f) };
	POSITION arrRB3[10] = { POSITION(153.f, 62.f), POSITION(187.f, 98.f),
							POSITION(200.f, 113.f), POSITION(186.f, 69.f),
							POSITION(162.f, 198.f), POSITION(137.f, 198.f),
							POSITION(129.f, 196.f), POSITION(151.f, 185.f),
							POSITION(140.f, 178.f), POSITION(143.f, 167.f) };

	AddAnimationClip("Divide3", AO_ONCE_DISABLE, 10, arrOrigin3, arrLT3, arrRB3, arrDelay,
		"Divide3", L"Player/Skill/Adel/Divide/Divide3/");

	pEffect = new CEffectObj;
	pEffect->Init();
	pAni = pEffect->CreateAnimation("DivideHitAnimation3");
	SAFE_RELEASE(pAni);
	float arrHitDelay3[6];
	fill_n(arrHitDelay3, 6, 0.06f);

	POSITION arrHitOrigin3[6] = { POSITION(63.f, 95.f), POSITION(87.f, 84.f),
		POSITION(104.f, 85.f), POSITION(113.f, 90.f),
		POSITION(117.f, 89.f), POSITION(80.f, 90.f) };
	POSITION arrHitLT3[6] = { POSITION(-63.f, -95.f), POSITION(-87.f, -84.f),
		POSITION(-104.f, -85.f), POSITION(-113.f, -90.f),
		POSITION(-117.f, -89.f), POSITION(-80.f, -90.f) };
	POSITION arrHitRB3[6] = { POSITION(81.f, 93.f), POSITION(97.f, 96.f),
		POSITION(116.f, 95.f), POSITION(131.f, 98.f),
		POSITION(151.f, 87.f), POSITION(88.f, 86.f) };
	pEffect->AddAnimationClip("DivideHit3", AO_ONCE_DESTROY, 6, arrHitOrigin3, arrHitLT3, arrHitRB3, arrHitDelay3, "DivideHit3", L"Player/Skill/Adel/Divide/Hit/3/");
	m_vecHitEffectObj.push_back(pEffect);

	LoadButton("DivideIcon", L"Player/Skill/Adel/Divide/Icon/", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(32.f, 32.f));
	SetIconFunction((CActiveSkill*)this, &CActiveSkill::UseSkill);

	return true;
}

void CDivide::Input(float fDeltaTime)
{
	CActiveSkill::Input(fDeltaTime);
}

int CDivide::Update(float fDeltaTime)
{
	CActiveSkill::Update(fDeltaTime);

	if (m_bUsed)
	{
		m_iRand = rand() % 3;

		{
			switch (m_iRand)
			{
			case 0:
				m_pAnimation->ChangeClip("Divide1");
				break;
			case 1:
				m_pAnimation->ChangeClip("Divide2");
				break;
			case 2:
				m_pAnimation->ChangeClip("Divide3");
				break;
			default:
				break;
			}
		}
	}

	return 0;
}

int CDivide::LateUpdate(float fDeltaTime)
{
	CActiveSkill::LateUpdate(fDeltaTime);
	return 0;
}

void CDivide::Collision(float fDeltaTime)
{
	CActiveSkill::Collision(fDeltaTime);
}

void CDivide::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CActiveSkill::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CDivide * CDivide::Clone()
{
	return new CDivide(*this);
}

void CDivide::ActiveSkill(CObj * pObj)
{
	if (m_iDamage > 0)
	{
		CMonster* pMonster = (CMonster*)pObj;
		pMonster->SetDir(m_eDir);
		pMonster->GetDamage(m_iDamage, false, 1);
		pMonster->GetDamage(m_iDamage, false, 2);
		pMonster->GetDamage(m_iDamage, true, 3);
		pMonster->GetDamage(m_iDamage, false, 4);
		pMonster->GetDamage(m_iDamage, true, 5);
		pMonster->GetDamage(m_iDamage, true, 6);

		// 6타이므로 6개의 hit effect를 복사해준다.
		POSITION tPos = pMonster->GetPos();
		_SIZE tSize = pMonster->GetSize();
		POSITION tPivot = pMonster->GetPivot();
		float fPosY = (tPos.y - tSize.y * tPivot.y + tPos.y + tSize.y * (1 - tPivot.y)) / 2.f;
		for (int i = 0; i < 6; i++)
		{
			CEffectObj* pEffect = m_vecHitEffectObj[m_iRand]->Clone();
			pEffect->SetMaxDelay(0.f + (float)i*0.09f);
			POSITION tRandPos;
			tRandPos.x = (float)(rand() % 40 - 20);
			tRandPos.y = (float)(rand() % 40 - 20);
			pEffect->SetPos(tPos.x + tRandPos.x, fPosY + tRandPos.y);
			pMonster->AddHitEffect(pEffect);
		}
	}
}
