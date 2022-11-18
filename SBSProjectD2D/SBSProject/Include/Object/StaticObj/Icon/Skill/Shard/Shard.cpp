#include "Shard.h"
#include "ShardBullet.h"
#include "../../../../MoveObj/Player/Player.h"

CShard::CShard():
	m_pBulletPrototype(NULL),
	m_bCreateBullet(false),
	m_fCreateTime(0.f),
	m_fCreateDelay(0.f),
	m_bCreateDelay(false)
{
}

CShard::CShard(const CShard & shard):
	CActiveSkill(shard)
{
	m_pBulletPrototype = shard.m_pBulletPrototype->Clone();
}

CShard::~CShard()
{
	SAFE_RELEASE(m_pBulletPrototype);
}

bool CShard::Init()
{
	SetType(ST_ATTACK);
	//SetSize(284.f, 280.f);
	SetLT(-500.f, -250.f);
	SetRB(120.f, 200.f);
	SetLevel(0);
	SetStepLevel(2);
	SetDamage(100); // 500 + level * 50
	SetCost(0);
	SetMaxLevel(10);

	SetSkillName(L"샤드");

	m_fDelay = 0.9f;

	CAnimation* pAni = CreateAnimation("ShardAnimation");
	SAFE_RELEASE(pAni);

	m_fCreateDelay = 0.12f;

	m_pBulletPrototype = new CShardBullet;
	m_pBulletPrototype->Init();
	m_pBulletPrototype->SetShard(this);

	float arrDelay[11];
	fill_n(arrDelay, 11, 0.09f);
	arrDelay[0] = 0.45f;

	_SIZE arrSize[11] = { _SIZE(1.f, 1.f),
		_SIZE(164.f, 152.f), _SIZE(256.f, 264.f),
		_SIZE(284.f, 272.f), _SIZE(284.f, 268.f),
		_SIZE(256.f, 264.f), _SIZE(256.f, 280.f),
		_SIZE(240.f, 228.f), _SIZE(240.f, 228.f),
		_SIZE(240.f, 228.f), _SIZE(236.f, 228.f) };
	POSITION arrOrigin[11] = { POSITION(0.f, 0.f),
		POSITION(8.f, 107.f), POSITION(128.f, 212.f),
		POSITION(156.f, 212.f), POSITION(155.f, 220.f),
		POSITION(128.f, 221.f), POSITION(128.f, 223.f),
		POSITION(120.f, 223.f), POSITION(115.f, 223.f),
		POSITION(115.f, 224.f), POSITION(110.f, 224.f) };
	POSITION arrLT[11] = { POSITION(0.f, 0.f),
		POSITION(-8.f, -107.f), POSITION(-128.f, -212.f),
		POSITION(-156.f, -212.f), POSITION(-155.f, -220.f),
		POSITION(-128.f, -221.f), POSITION(-128.f, -223.f),
		POSITION(-120.f, -223.f), POSITION(-115.f, -223.f),
		POSITION(-115.f, -224.f), POSITION(-110.f, -224.f) };
	POSITION arrRB[11] = { POSITION(1.f, 1.f),
		POSITION(156.f, 45.f), POSITION(128.f, 52.f),
		POSITION(128.f, 60.f), POSITION(129.f, 48.f),
		POSITION(128.f, 43.f), POSITION(128.f, 57.f),
		POSITION(120.f, 5.f), POSITION(125.f, 5.f),
		POSITION(125.f, 4.f), POSITION(126.f, 4.f) };

	AddAnimationClip("Shard", AO_ONCE_DISABLE, 11, arrOrigin, arrLT, arrRB, arrDelay,
		"Shard", L"Player/Skill/Adel/Shard/Shard");

	LoadButton("ShardIcon", L"Player/Skill/Adel/Shard/Icon/", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(32.f, 32.f));
	SetIconFunction((CActiveSkill*)this, &CActiveSkill::UseSkill);

	return true;
}

void CShard::Input(float fDeltaTime)
{
	CActiveSkill::Input(fDeltaTime);
}

int CShard::Update(float fDeltaTime)
{
	CActiveSkill::Update(fDeltaTime);
	// 최초사용시 bullet clone을 만들기
	if (m_bUsed && !m_bCreateBullet)
	{
		m_bCreateBullet = true;
	}

	return 0;
}

int CShard::LateUpdate(float fDeltaTime)
{
	CActiveSkill::LateUpdate(fDeltaTime);

	if (m_bCreateBullet)
	{
		// 5개가 만들어지고 위치/각도 모두 설정해주기
		if (m_fCreateTime == 0.f)
		{			
			CShardBullet* pBullet = m_pBulletPrototype->Clone();
			m_pPlayer->GetLayer()->AddObject(pBullet);
			pBullet->SetPos(m_tPos.x, m_tPos.y - 120.f);
			pBullet->SetAngle(270.f);
			pBullet->SetMaxDelay(0.6f);
			SAFE_RELEASE(pBullet);
		}
		m_fCreateTime += fDeltaTime;
		if (m_fCreateTime >= m_fCreateDelay && !m_bCreateDelay)
		{
			m_bCreateDelay = true;

			CShardBullet* pBullet = m_pBulletPrototype->Clone();
			m_pPlayer->GetLayer()->AddObject(pBullet);
			pBullet->SetPos(m_tPos.x - 40.f, m_tPos.y - 100.f);
			pBullet->SetAngle(270.f - 15.f);
			pBullet->SetMaxDelay(0.48f);
			SAFE_RELEASE(pBullet);

			pBullet = m_pBulletPrototype->Clone();
			m_pPlayer->GetLayer()->AddObject(pBullet);
			pBullet->SetPos(m_tPos.x + 40.f, m_tPos.y - 100.f);
			pBullet->SetAngle(270.f + 15.f);
			pBullet->SetMaxDelay(0.48f);
			SAFE_RELEASE(pBullet);
		}
		if (m_fCreateTime >= 2 * m_fCreateDelay)
		{
			m_fCreateTime = 0.f;
			m_bCreateBullet = false;
			m_bCreateDelay = false;

			CShardBullet* pBullet = m_pBulletPrototype->Clone();
			m_pPlayer->GetLayer()->AddObject(pBullet);
			pBullet->SetPos(m_tPos.x - 80.f, m_tPos.y - 80.f);
			pBullet->SetAngle(270.f - 30.f);
			pBullet->SetMaxDelay(0.36f);
			SAFE_RELEASE(pBullet);

			pBullet = m_pBulletPrototype->Clone();
			m_pPlayer->GetLayer()->AddObject(pBullet);
			pBullet->SetPos(m_tPos.x + 80.f, m_tPos.y - 80.f);
			pBullet->SetAngle(270.f + 30.f);
			pBullet->SetMaxDelay(0.36f);
			SAFE_RELEASE(pBullet);
		}
	}

	return 0;
}

void CShard::Collision(float fDeltaTime)
{
	CActiveSkill::Collision(fDeltaTime);
}

void CShard::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CActiveSkill::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CShard * CShard::Clone()
{
	return new CShard(*this);
}

void CShard::ActiveSkill(CObj * pObj)
{
}
