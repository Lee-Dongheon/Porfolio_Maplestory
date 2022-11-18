#include "Mushmom.h"
#include "../../../../Collider/ColliderRect.h"
#include "../../../StaticObj/Effect/EffectObj.h"

CMushmom::CMushmom():
	m_bIsAttack(false),
	m_pEffectObj(NULL)
{
}

CMushmom::CMushmom(const CMushmom & mushmom) :
	CMonster(mushmom)
{
	m_bIsAttack = false;
	if (mushmom.m_pEffectObj)
		m_pEffectObj = mushmom.m_pEffectObj->Clone();
	m_pEffectObj->SetObj(this);
}

CMushmom::~CMushmom()
{
	SAFE_RELEASE(m_pEffectObj);
}

bool CMushmom::Init()
{
	CMonster::Init();

	SetSize(120.f, 180.f);
	SetSpeed(125.f);
	SetMonsterType(MT_KNOCKBACKBOSS);

	/*m_iMaxHP = m_iHP = 17500;
	m_iMaxMP = m_iMP = 2000;
	m_iLevel = 18;
	m_iExp = 1650;*/

	m_iMaxHP = m_iHP = 300;
	m_iMaxMP = m_iMP = 20;
	m_iLevel = 18;
	m_iExp = 30;

	/*CColliderRect* pRC = AddCollider<CColliderRect>("MushmomBody");

	pRC->SetRect(-60.f, -110.f, 60.f, 0.f);

	SAFE_RELEASE(pRC);*/

	SetForce(400.f);
	SetKnockBackForce(40.f);

	CAnimation* pAnimation = CreateAnimation("MushmomAnimation");
	SAFE_RELEASE(pAnimation);

	POSITION arrOrigin[1] = { POSITION(59.f, 110.f) };
	POSITION arrLT[1] = { POSITION(-59.f, -110.f) };
	POSITION arrRB[1] = { POSITION(60.f, 0.f) };
	float arrDelay[1] = { 0.1f };
	AddAnimationClip("Idle", AO_LOOP, 1, arrOrigin, arrLT, arrRB, arrDelay, "MushmomStand", L"Monster/Mushmom/Stand/");

	POSITION arrOrigin2[5] = { POSITION(59.f, 110.f), POSITION(60.f, 105.f), POSITION(59.f, 102.f), POSITION(59.f, 180.f), POSITION(59.f, 160.f) };
	POSITION arrLT2[5] = { POSITION(-59.f, -110.f), POSITION(-60.f, -105.f), POSITION(-59.f, -103.f), POSITION(-59.f, -180.f), POSITION(-59.f, -160.f) };
	POSITION arrRB2[5] = { POSITION(60.f, 0.f), POSITION(60.f, -3.f), POSITION(59.f, 0.f), POSITION(59.f, -55.f), POSITION(59.f, -35.f) };
	float arrDelay2[5] = { 0.5f, 0.1f, 0.1f, 0.3f, 0.1f };
	AddAnimationClip("Run", AO_LOOP, 5, arrOrigin2, arrLT2, arrRB2, arrDelay2, "MushmomRun", L"Monster/Mushmom/Run/");

	POSITION arrOrigin3[1] = { POSITION(59.f, 125.f) };
	POSITION arrLT3[1] = { POSITION(-59.f, -125.f) };
	POSITION arrRB3[1] = { POSITION(59.f, 0.f) };
	float arrDelay3[1] = { 0.1f };
	AddAnimationClip("Jump", AO_LOOP, 1, arrOrigin3, arrLT3, arrRB3, arrDelay3, "MushmomJump", L"Monster/Mushmom/Jump/");

	POSITION arrOrigin4[1] = { POSITION(59.f, 140.f) };
	POSITION arrLT4[1] = { POSITION(-59.f, -140.f) };
	POSITION arrRB4[1] = { POSITION(59.f, 0.f) };
	float arrDelay4[1] = { 0.3f };
	AddAnimationClip("Hit", AO_LOOP, 1, arrOrigin4, arrLT4, arrRB4, arrDelay4, "MushmomHit", L"Monster/Mushmom/Hit/");

	POSITION arrOrigin5[6] = { POSITION(60.f, 140.f), POSITION(82.f, 61.f),
								POSITION(90.f, 61.f), POSITION(90.f, 61.f),
								POSITION(104.f, 61.f), POSITION(96.f, 61.f) };
	POSITION arrLT5[6] = { POSITION(-60.f, -140.f), POSITION(-82.f, -61.f),
							POSITION(-90.f, -61.f), POSITION(-90.f, -61.f),
							POSITION(-104.f, -61.f), POSITION(-96.f, -61.f) };
	POSITION arrRB5[6] = { POSITION(60.f, 0.f), POSITION(84.f, 3.f),
							POSITION(84.f, 1.f), POSITION(94.f, 0.f),
							POSITION(93.f, 0.f), POSITION(81.f, 0.f) };
	float arrDelay5[6] = { 0.2f, 0.15f, 0.15f, 0.15f, 0.15f, 0.3f };
	AddAnimationClip("Die", AO_ONCE_DISABLE, 1, arrOrigin5, arrLT5, arrRB5, arrDelay5, "MushmomDie", L"Monster/Mushmom/Die/");

	POSITION arrOrigin6[5] = { POSITION(59.f, 102.f), POSITION(59.f, 213.f),
								POSITION(60.f, 195.f), POSITION(60.f, 133.f), POSITION(58.f, 105.f) };
	POSITION arrLT6[5] = { POSITION(-59.f, -102.f), POSITION(-60.f, -214.f),
							POSITION(-60.f, -195.f), POSITION(-60.f, -133.f), POSITION(-58.f, -105.f) };
	POSITION arrRB6[5] = { POSITION(59.f, 0.f), POSITION(58.f, -89.f),
							POSITION(57.f, -71.f), POSITION(57.f, -9.f), POSITION(62.f, 0.f) };
	float arrDelay6[5] = { 0.8f, 0.2f, 0.05f, 0.05f, 0.75f };
	AddAnimationClip("Attack", AO_LOOP, 5, arrOrigin6, arrLT6, arrRB6, arrDelay6, "MushmomAttack", L"Monster/Mushmom/Attack/");
	
	/*CItem* pItem = CreateItem<CItem>("MushmomPoja", IT_ETC);
	pItem->SetTexture("MushmomPojaRaw", L"Item/Mushmom/Icon_MushmomPojaRaw.png", POSITION(16.f, 31.f), POSITION(-16.f, -31.f), POSITION(16.f, 0.f));
	pItem->LoadTextureIcon("MushmomPoja", L"Item/Mushmom/Icon_MushmomPoja.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(32.f, 32.f));
	SAFE_RELEASE(pItem);*/

	m_pEffectObj = new CEffectObj;
	m_pEffectObj->SetObj(this);
	m_pEffectObj->SetEnable(false);
	CAnimation* pEffectAni = m_pEffectObj->CreateAnimation("MushmomEffect");
	SAFE_RELEASE(pEffectAni);

	float arrEffectDelay[15] = { 0.1f, 0.1f, 0.1f,
								0.1f, 0.1f, 0.1f,
								0.2f, 0.3f, 0.1f,
								0.1f, 0.1f, 0.1f,
								0.1f, 0.2f, 0.05f, };

	POSITION arrEffectOrigin[15] = { POSITION(46.f, 63.f), POSITION(51.f, 69.f), POSITION(58.f, 78.f), 
									POSITION(59.f, 80.f), POSITION(53.f, 75.f), POSITION(19.f, 77.f), 
									POSITION(19.f, 78.f), POSITION(0.f, 0.f), POSITION(108.f, 50.f), 
									POSITION(122.f, 54.f), POSITION(129.f, 58.f), POSITION(101.f, 60.f), 
									POSITION(102.f, 61.f), POSITION(103.f, 64.f), POSITION(0.f, 0.f) };
	POSITION arrEffectLT[15] = { POSITION(-46.f, -63.f), POSITION(-51.f, -69.f), POSITION(-58.f, -78.f),
									POSITION(-59.f, -80.f), POSITION(-53.f, -75.f), POSITION(-19.f, -77.f),
									POSITION(-19.f, -78.f), POSITION(0.f, 0.f), POSITION(-108.f, -50.f),
									POSITION(-122.f, -54.f), POSITION(-129.f, -58.f), POSITION(-101.f, -60.f),
									POSITION(-102.f, -61.f), POSITION(-103.f, -64.f), POSITION(0.f, 0.f) };
	POSITION arrEffectRB[15] = { POSITION(-14.f, -21.f), POSITION(-10.f, -16.f), POSITION(24.f, -7.f),
									POSITION(29.f, -5.f), POSITION(37.f, -4.f), POSITION(40.f, -1.f),
									POSITION(42.f, 1.f), POSITION(1.f, 1.f), POSITION(109.f, 1.f),
									POSITION(120.f, 0.f), POSITION(125.f, -2.f), POSITION(108.f, -7.f),
									POSITION(107.f, -18.f), POSITION(106.f, -17.f), POSITION(1.f, 1.f) };
	m_pEffectObj->AddAnimationClip("AttackEffect", AO_ONCE_DISABLE, 15, arrEffectOrigin, arrEffectLT, arrEffectRB, arrEffectDelay, "MushmomEffect", L"Monster/Mushmom/Attack/Effect/");

	return true;
}

//void CMushmom::Input(float fDeltaTime)
//{
//	CMonster::Input(fDeltaTime);
//}

int CMushmom::Update(float fDeltaTime)
{
	CMonster::Update(fDeltaTime);

	/*if (m_bIsDead)
		return 0;

	int randomN = rand() % 4;

	if (m_pAggressiveObj)
	{
		if (m_fAggressiveMoveTime >= 1.f && !m_bIsAttack)
		{
			m_fAggressiveMoveTime -= 1.f;
			POSITION tPos = m_pAggressiveObj->GetPos();
			if (!m_bJumping && !m_bIsHit)
			{
				float fDist = (tPos.x - m_tPos.x > 0 ? tPos.x - m_tPos.x : m_tPos.x - tPos.x);
				if (fDist < 200.f)
				{
					Attack();
				}
				else
					m_pAnimation->ChangeClip("Run");

				if (tPos.x > m_tPos.x)
					SetDir(MD_RIGHT);
				else
					SetDir(MD_LEFT);
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

	if (!m_bIsHit && !m_bIsAttack)
		MoveX(fDeltaTime, m_eDir);*/

	/*if (m_bIsAttack && m_pAnimation->GetMotionEnd())
		m_bIsAttack = false;*/

	if (m_pEffectObj)
	{
		if (!m_pEffectObj->GetEnable() && m_eObjState == OBJ_STATE_ATTACK)
			m_pEffectObj->SetEnable(true);
		if (m_pEffectObj->GetEnable())
			m_pEffectObj->Update(fDeltaTime);
	}	

	return 0;
}

int CMushmom::LateUpdate(float fDeltaTime)
{
	CMonster::LateUpdate(fDeltaTime);

	return 0;
}

void CMushmom::Collision(float fDeltaTime)
{
	CMonster::Collision(fDeltaTime);
}

void CMushmom::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CMonster::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	if (m_pEffectObj)
	{
		if (m_pEffectObj->GetEnable())
			m_pEffectObj->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}
}

CMushmom * CMushmom::Clone()
{
	return new CMushmom(*this);
}

//void CMushmom::Attack()
//{
//	m_bIsAttack = true;
//	if (m_pAnimation)
//	{
//		m_pAnimation->ChangeClip("Attack");
//		m_pEffectObj->SetEnable(true);
//	}
//}
