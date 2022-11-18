#include "GreenMushroom.h"
#include "../../../../Collider/ColliderRect.h"

CGreenMushroom::CGreenMushroom()
{
}

CGreenMushroom::CGreenMushroom(const CGreenMushroom & mushroom) :
	CMonster(mushroom)
{
}

CGreenMushroom::~CGreenMushroom()
{
}

bool CGreenMushroom::Init()
{
	CMonster::Init();

	SetSize(56.f, 52.f);
	SetSpeed(50.f);
	SetMonsterType(MT_NORMAL);

	m_iMaxHP = m_iHP = 125;
	m_iMaxMP = m_iMP = 10;
	m_iLevel = 10;
	m_iExp = 17;

	/*CColliderRect* pRC = AddCollider<CColliderRect>("GreenBody");

	pRC->SetRect(-28.f, -52.f, 28.f, 0.f);

	SAFE_RELEASE(pRC);*/

	CAnimation* pAnimation = CreateAnimation("GreenMushroomAnimation");
	SAFE_RELEASE(pAnimation);

	POSITION arrOrigin[3] = { POSITION(27.f, 52.f), POSITION(27.f, 52.f), POSITION(27.f, 52.f) };
	POSITION arrLT[3] = { POSITION(-27.f, -52.f), POSITION(-27.f, -52.f), POSITION(-27.f, -52.f) };
	POSITION arrRB[3] = { POSITION(29.f, 0.f), POSITION(29.f, 0.f), POSITION(29.f, 0.f) };
	float arrDelay[3] = { 0.18f, 0.1f, 0.18f };
	AddAnimationClip("Idle", AO_LOOP, 3, arrOrigin, arrLT, arrRB, arrDelay, "GreenMushroomStand", L"Monster/GreenMushroom/Stand/");

	POSITION arrOrigin2[4] = { POSITION(27.f, 52.f), POSITION(27.f, 51.f), POSITION(27.f, 52.f), POSITION(27.f, 51.f) };
	POSITION arrLT2[4] = { POSITION(-27.f, -52.f), POSITION(-27.f, -51.f), POSITION(-27.f, -52.f), POSITION(-27.f, -51.f) };
	POSITION arrRB2[4] = { POSITION(29.f, 0.f), POSITION(29.f, 0.f), POSITION(29.f, 0.f), POSITION(29.f, 0.f) };
	float arrDelay2[4] = { 0.15f, 0.15f, 0.15f, 0.15f };
	AddAnimationClip("Run", AO_LOOP, 4, arrOrigin2, arrLT2, arrRB2, arrDelay2, "GreenMushroomRun", L"Monster/GreenMushroom/Run/");

	POSITION arrOrigin3[1] = { POSITION(27.f, 56.f) };
	POSITION arrLT3[1] = { POSITION(-27.f, -56.f) };
	POSITION arrRB3[1] = { POSITION(29.f, 0.f) };
	float arrDelay3[1] = { 0.3f };
	AddAnimationClip("Hit", AO_LOOP, 1, arrOrigin3, arrLT3, arrRB3, arrDelay3, "GreenMushroomHit", L"Monster/GreenMushroom/Hit/");

	POSITION arrOrigin4[4] = { POSITION(28.f, 49.f), POSITION(27.f, 48.f), POSITION(28.f, 46.f), POSITION(29.f, 46.f) };
	POSITION arrLT4[4] = { POSITION(-28.f, -49.f), POSITION(-27.f, -48.f), POSITION(-28.f, -46.f), POSITION(-29.f, -46.f) };
	POSITION arrRB4[4] = { POSITION(29.f, 0.f), POSITION(30.f, 0.f), POSITION(29.f, 1.f), POSITION(28.f, 0.f) };
	float arrDelay4[4] = { 0.18f, 0.18f, 0.18f, 0.3f };
	AddAnimationClip("Die", AO_ONCE_DISABLE, 4, arrOrigin4, arrLT4, arrRB4, arrDelay4, "GreenMushroomDie", L"Monster/GreenMushroom/Die/");

	return true;
}

//void CGreenMushroom::Input(float fDeltaTime)
//{
//	CMonster::Input(fDeltaTime);
//}

int CGreenMushroom::Update(float fDeltaTime)
{
	CMonster::Update(fDeltaTime);

	/*if (m_bIsDead)
		return 0;

	int randomN = rand() % 3;

	if (m_pAggressiveObj)
	{
		if (m_fAggressiveMoveTime >= 1.f)
		{
			m_fAggressiveMoveTime -= 1.f;
			POSITION tPos = m_pAggressiveObj->GetPos();
			if (!m_bJumping && !m_bIsHit)
			{
				m_pAnimation->ChangeClip("Run");
				if (tPos.x > m_tPos.x)
					m_eDir = MD_RIGHT;
				else
					m_eDir = MD_LEFT;
			}
		}
	}
	else
	{
		m_fRandomMoveTime += fDeltaTime;
		if (m_fRandomMoveTime >= m_fRandomMoveLimitTime)
		{
			m_fRandomMoveTime -= m_fRandomMoveLimitTime;

			if (!m_bJumping && !m_bIsHit)
			{
				switch (randomN)
				{
				case 0:
					SetSpeed(0.f);
					m_pAnimation->ChangeClip("Stand");
					break;
				case 1:
					SetSpeed(50.f);
					m_eDir = MD_RIGHT;
					m_pAnimation->ChangeClip("Run");
					break;
				case 2:
					SetSpeed(50.f);
					m_eDir = MD_LEFT;
					m_pAnimation->ChangeClip("Run");
					break;
				default:
					break;
				}
			}
		}
	}

	if (!m_bIsHit)
		MoveX(fDeltaTime, m_eDir);*/

	return 0;
}

int CGreenMushroom::LateUpdate(float fDeltaTime)
{
	CMonster::LateUpdate(fDeltaTime);
	return 0;
}

void CGreenMushroom::Collision(float fDeltaTime)
{
	CMonster::Collision(fDeltaTime);
}

void CGreenMushroom::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CMonster::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CGreenMushroom * CGreenMushroom::Clone()
{
	return new CGreenMushroom(*this);
}