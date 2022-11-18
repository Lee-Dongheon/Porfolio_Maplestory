#include "Mushroom.h"
#include "../../../../Collider/ColliderRect.h"

CMushroom::CMushroom()
{
}

CMushroom::CMushroom(const CMushroom & mushroom):
	CMonster(mushroom)
{
}

CMushroom::~CMushroom()
{
}

bool CMushroom::Init()
{
	CMonster::Init();
	
	SetSize(63.f, 63.f);
	SetSpeed(100.f);
	SetMonsterType(MT_NORMAL);

	m_iMaxHP = m_iHP = 125;
	m_iMaxMP = m_iMP = 0;
	m_iLevel = 6;
	m_iExp = 17;

	/*CColliderRect* pRC = AddCollider<CColliderRect>("OrangeBody");

	pRC->SetRect(-32.f, -63.f, 31.f, 0.f);

	SAFE_RELEASE(pRC);*/

	CAnimation* pAnimation = CreateAnimation("OrangeMushroomAnimation");
	SAFE_RELEASE(pAnimation);

	POSITION arrOrigin[2] = { POSITION(27.f, 58.f), POSITION(27.f, 55.f) };
	POSITION arrLT[2] = { POSITION(-27.f, -58.f), POSITION(-27.f, -55.f) };
	POSITION arrRB[2] = { POSITION(36.f, 0.f), POSITION(36.f, 0.f) };
	float arrDelay[2] = { 0.18f, 0.18f };
	AddAnimationClip("Idle", AO_LOOP, 2, arrOrigin, arrLT, arrRB, arrDelay, "OrangeMushroomStand", L"Monster/OrangeMushroom/Stand/");

	POSITION arrOrigin2[3] = { POSITION(32.f, 55.f),POSITION(31.f, 67.f), POSITION(31.f, 55.f) };
	POSITION arrLT2[3] = { POSITION(-32.f, -55.f),POSITION(-31.f, -67.f), POSITION(-31.f, -55.f) };
	POSITION arrRB2[3] = { POSITION(32.f, 0.f), POSITION(31.f, -3.f), POSITION(32.f, 0.f) };
	float arrDelay2[3] = { 0.18f, 0.12f, 0.18f };
	AddAnimationClip("Run", AO_LOOP, 3, arrOrigin2, arrLT2, arrRB2, arrDelay2, "OrangeMushroomRun", L"Monster/OrangeMushroom/Run/");

	POSITION arrOrigin3[1] = { POSITION(28.f, 64.f) };
	POSITION arrLT3[1] = { POSITION(-28.f, -64.f) };
	POSITION arrRB3[1] = { POSITION(34.f, 0.f) };
	float arrDelay3[1] = { 0.12f };
	AddAnimationClip("Jump", AO_LOOP, 1, arrOrigin3, arrLT3, arrRB3, arrDelay3, "OrangeMushroomJump", L"Monster/OrangeMushroom/Jump/");
	
	POSITION arrOrigin4[1] = { POSITION(28.f, 65.f) };
	POSITION arrLT4[1] = { POSITION(-28.f, -65.f) };
	POSITION arrRB4[1] = { POSITION(34.f, 0.f) };
	float arrDelay4[1] = { 0.3f };
	AddAnimationClip("Hit", AO_LOOP, 1, arrOrigin4, arrLT4, arrRB4, arrDelay4, "OrangeMushroomHit", L"Monster/OrangeMushroom/Hit/");

	POSITION arrOrigin5[3] = { POSITION(30.f, 59.f),POSITION(32.f, 52.f), POSITION(34.f, 44.f) };
	POSITION arrLT5[3] = { POSITION(-30.f, -59.f),POSITION(-32.f, -52.f), POSITION(-34.f, -44.f) };
	POSITION arrRB5[3] = { POSITION(31.f, 0.f),POSITION(27.f, 0.f), POSITION(25.f, 0.f) };
	float arrDelay5[3] = { 0.18f, 0.18f, 0.3f };
	AddAnimationClip("Die", AO_ONCE_DISABLE, 1, arrOrigin5, arrLT5, arrRB5, arrDelay5, "OrangeMushroomDie", L"Monster/OrangeMushroom/Die/");

	//CItem* pItem = CreateItem<CItem>("OrangeHead", IT_ETC);
	//pItem->SetTexture("OrangeHeadRaw", L"Item/OrangeMushroom/Icon_OrangeHeadRaw.png", POSITION(15.f, 26.f), POSITION(-15.f, -26.f), POSITION(15.f, 0.f));
	//pItem->LoadTextureIcon("OrangeHead", L"Item/OrangeMushroom/Icon_OrangeHead.png", POSITION(-1.f, -4.f), POSITION(1.f, 4.f), POSITION(31.f, 32.f));
	//// itemobj의 pivot은 (0.5f, 1.f)이므로 texture origin, lt, rb 다 고려하자
	//SAFE_RELEASE(pItem);

	return true;
}

//void CMushroom::Input(float fDeltaTime)
//{
//	CMonster::Input(fDeltaTime);
//}

int CMushroom::Update(float fDeltaTime)
{
	CMonster::Update(fDeltaTime);
/*
	if (m_bIsDead)
		return 0;

	int randomN = rand() % 4;

	if (m_pAggressiveObj)
	{
		if (m_fAggressiveMoveTime >= 1.f)
		{
			m_fAggressiveMoveTime -= 1.f;
			POSITION tPos = m_pAggressiveObj->GetPos();
			if (!m_bJumping && !m_bIsHit)
			{
				switch (randomN)
				{
				case 0:
				case 1:
					SetSpeed(50.f);
					m_pAnimation->ChangeClip("Jump");
					Jump();
					break;
				case 2:
				case 3:
				default:
					SetSpeed(50.f);
					m_pAnimation->ChangeClip("Run");
					break;
				}
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
					m_pAnimation->ChangeClip("Jump");
					Jump();
					break;
				case 2:
					SetSpeed(50.f);
					m_eDir = MD_RIGHT;
					m_pAnimation->ChangeClip("Run");
					break;
				case 3:
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

int CMushroom::LateUpdate(float fDeltaTime)
{
	CMonster::LateUpdate(fDeltaTime);
	return 0;
}

void CMushroom::Collision(float fDeltaTime)
{
	CMonster::Collision(fDeltaTime);
}

void CMushroom::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CMonster::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CMushroom * CMushroom::Clone()
{
	return new CMushroom(*this);
}