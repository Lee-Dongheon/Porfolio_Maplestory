#include "ShardBullet.h"
#include "../../../../MoveObj/Player/Player.h"
#include "Shard.h"
#include "../../../../../Scene/Scene.h"
#include "../../../../../Collider/ColliderRect.h"
#include "../../../../../Math.h"
#include "../../../../MoveObj/Monster/Monster.h"
#include "../../../Effect/EffectObj.h"

CShardBullet::CShardBullet() :
	m_tLT(POSITION(0.f, 0.f)),
	m_tRB(POSITION(0.f, 0.f)),
	m_pTarget(NULL),
	m_pShard(NULL),
	m_pEffectObj(NULL),
	m_fExistTime(0.f),
	m_fExistLimitTime(2.f),
	m_fDelayTime(0.f),
	m_fDelayLimitTime(-1.f)
{
}

CShardBullet::CShardBullet(const CShardBullet & bullet) :
	CMoveObj(bullet)
{
	m_iRef = 1;

	m_tLT = bullet.m_tLT;
	m_tRB = bullet.m_tRB;
	
	m_TargetList.clear();
	m_pShard = bullet.m_pShard;
	m_pEffectObj = NULL;
	if (bullet.m_pEffectObj)
		m_pEffectObj = bullet.m_pEffectObj->Clone();
	m_pTarget = NULL;

	m_fExistTime = 0.f;
	m_fExistLimitTime = bullet.m_fExistLimitTime;

	m_fDelayTime = 0.f;
	m_fDelayLimitTime = -1.f;
}

CShardBullet::~CShardBullet()
{
	SAFE_RELEASE(m_pEffectObj);
}

void CShardBullet::SetShard(CShard * pShard)
{
	m_pShard = pShard;
}

void CShardBullet::SetTarget()
{
	float fDist = 500.f;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_TargetList.end();

	for (iter = m_TargetList.begin(); iter != iterEnd; iter++)
	{
		if (!(*iter)->GetEnable())
			continue;

		float fTempDist = GetDistance((*iter)->GetPos(), m_tPos);
		if (fTempDist < fDist)
		{
			m_pTarget = (*iter);
			fDist = fTempDist;
		}
	}
}

bool CShardBullet::Init()
{
	SetPivot(0.5f, 0.5f);
	SetLT(-300.f, -200.f);
	SetRB(300.f, 200.f);
	SetSize(100.f, 80.f);
	SetSpeed(400.f);
	SetTag("ShardBullet");
	SetObjType(OT_SKILL);

	/*CColliderRect* pRC = AddCollider<CColliderRect>("ShardBullet");
	pRC->SetRect(-50.f, -40.f, 50.f, 40.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &CShardBullet::ShardCollFunc);
	SAFE_RELEASE(pRC);*/

	CAnimation* pAni = CreateAnimation("ShardBulletAnimation");
	SAFE_RELEASE(pAni);

	float arrDelay[7];
	fill_n(arrDelay, 7, 0.03f);

	POSITION arrOrigin[7] = { POSITION(50.f, 10.f),
		POSITION(52.f, 30.f), POSITION(63.f, 50.f),
		POSITION(73.f, 50.f), POSITION(49.f, 50.f),
		POSITION(49.f, 50.f), POSITION(43.f, 34.f) };
	POSITION arrLT[7] = { POSITION(-50.f, -10.f),
		POSITION(-52.f, -30.f), POSITION(-63.f, -50.f),
		POSITION(-73.f, -50.f), POSITION(-49.f, -50.f),
		POSITION(-49.f, -50.f), POSITION(-43.f, -34.f) };
	POSITION arrRB[7] = { POSITION(54.f, 14.f),
		POSITION(52.f, 34.f), POSITION(65.f, 54.f),
		POSITION(75.f, 54.f), POSITION(55.f, 54.f),
		POSITION(55.f, 54.f), POSITION(33.f, 38.f) };

	AddAnimationClip("ShardBulletStart", AO_ONCE_RETURN, 7, arrOrigin, arrLT, arrRB, arrDelay, "ShardBulletStart", L"Player/Skill/Adel/Shard/Bullet/Start/");
	//m_pAnimation->SetCurrentClip("ShardBulletStart");

	float arrDelay2[6];
	fill_n(arrDelay2, 6, 0.06f);

	POSITION arrOrigin2[6] = { POSITION(47.f, 42.f), POSITION(54.f, 42.f),
							POSITION(62.f, 42.f), POSITION(61.f, 42.f),
							POSITION(60.f, 42.f), POSITION(57.f, 42.f) };
	POSITION arrLT2[6] = { POSITION(-47.f, -42.f), POSITION(-54.f, -42.f),
							POSITION(-62.f, -42.f), POSITION(-61.f, -42.f),
							POSITION(-60.f, -42.f), POSITION(-57.f, -42.f) };
	POSITION arrRB2[6] = { POSITION(65.f, 46.f), POSITION(62.f, 46.f),
							POSITION(54.f, 46.f), POSITION(47.f, 46.f),
							POSITION(48.f, 46.f), POSITION(47.f, 46.f) };

	AddAnimationClip("ShardBulletMove", AO_LOOP, 6, arrOrigin2, arrLT2, arrRB2, arrDelay2, "ShardBulletMove", L"Player/Skill/Adel/Shard/Bullet/Move/");
	m_pAnimation->SetDefaultClip("ShardBulletMove");

	float arrDelay3[8];
	fill_n(arrDelay3, 8, 0.06f);

	POSITION arrOrigin3[8] = { POSITION(93.f, 78.f), POSITION(71.f, 105.f),
		POSITION(69.f, 61.f), POSITION(72.f, 59.f),
		POSITION(58.f, 56.f), POSITION(61.f, 59.f),
		POSITION(61.f, 59.f), POSITION(62.f, 60.f) };
	POSITION arrLT3[8] = { POSITION(-93.f, -78.f), POSITION(-71.f, -105.f),
		POSITION(-69.f, -61.f), POSITION(-72.f, -59.f),
		POSITION(-58.f, -56.f), POSITION(-61.f, -59.f),
		POSITION(-61.f, -59.f), POSITION(-62.f, -60.f) };
	POSITION arrRB3[8] = { POSITION(83.f, 70.f), POSITION(69.f, 75.f),
		POSITION(67.f, 71.f), POSITION(68.f, 69.f),
		POSITION(66.f, 64.f), POSITION(59.f, 61.f),
		POSITION(63.f, 65.f), POSITION(62.f, 64.f) };

	AddAnimationClip("ShardBulletEnd1", AO_ONCE_DESTROY, 8, arrOrigin3, arrLT3, arrRB3, arrDelay3, "ShardBulletEnd1", L"Player/Skill/Adel/Shard/Bullet/End1/");

	float arrDelay4[4];
	fill_n(arrDelay4, 4, 0.06f);

	POSITION arrOrigin4[4] = { POSITION(58.f, 56.f), POSITION(61.f, 59.f),
		POSITION(61.f, 59.f), POSITION(62.f, 60.f) };
	POSITION arrLT4[4] = { POSITION(-58.f, -56.f), POSITION(-61.f, -59.f),
		POSITION(-61.f, -59.f), POSITION(-62.f, -60.f) };
	POSITION arrRB4[4] = { POSITION(66.f, 64.f), POSITION(59.f, 61.f),
		POSITION(63.f, 65.f), POSITION(62.f, 64.f) };

	AddAnimationClip("ShardBulletEnd2", AO_ONCE_DESTROY, 4, arrOrigin4, arrLT4, arrRB4, arrDelay4, "ShardBulletEnd2", L"Player/Skill/Adel/Shard/Bullet/End2/");

	// move effect
	m_pEffectObj = new CEffectObj;
	m_pEffectObj->SetObj(this);
	pAni = m_pEffectObj->CreateAnimation("ShardBulletMoveAnimation");
	SAFE_RELEASE(pAni);

	float arrEffDelay[2] = { 0.06f, 0.06f };
	POSITION arrEffOrigin[2] = { POSITION(60.f, 34.f), POSITION(92.f, 34.f) };
	POSITION arrEffLT[2] = { POSITION(-60.f, -34.f), POSITION(-92.f, -34.f) };
	POSITION arrEffRB[2] = { POSITION(36.f, 34.f), POSITION(32.f, 34.f) };
	m_pEffectObj->AddAnimationClip("ShardBulletMoveEff", AO_ONCE_DISABLE, 2, arrEffOrigin, arrEffLT, arrEffRB, arrEffDelay, "ShardBulletMoveEff", L"Player/Skill/Adel/Shard/Bullet/MoveEff/");

	return true;
}

void CShardBullet::Input(float fDeltaTime)
{
	CMoveObj::Input(fDeltaTime);
}

int CShardBullet::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);

	if (m_pEffectObj && m_pEffectObj->GetEnable())
		m_pEffectObj->Update(fDeltaTime);

	// 몬스터 추적하기
	if (m_fDelayLimitTime > 0.f)
	{
		m_fDelayTime += fDeltaTime;
		if (m_fDelayTime >= m_fDelayLimitTime)
		{
			m_fDelayTime = 0.f;
			m_fDelayLimitTime = -1.f;
		}
		return 0;
	}

	// 타겟이 없으면 우선 목록을 가져와서 타겟 설정을 한다.
	if (!m_pTarget)
	{
		if (m_pEffectObj)
			m_pEffectObj->SetEnable(false);

		RECTANGLE tRect;
		tRect.l = m_tPos.x + m_tLT.x;
		tRect.t = m_tPos.y + m_tLT.y;
		tRect.r = m_tPos.x + m_tRB.x;
		tRect.b = m_tPos.y + m_tRB.y;

		m_pScene->GetObjList(OT_MONSTER, m_TargetList, tRect);
		SetTarget();

		// 그래도 없으면 exist limit time 만큼만 존재하다가 사라진다.
		// 있으면 추적함수를 따라 이동한다.
		m_fExistTime += fDeltaTime;
		if (m_fExistTime >= m_fExistLimitTime)
			m_pAnimation->ChangeClip("ShardBulletEnd1");
	}
	else // 타겟이 있다면
	{
		if (m_pEffectObj)
			m_pEffectObj->SetEnable(true);

		m_fExistTime = 0.f; // 존재 시간부터 초기화
		// 만약 추적하던 적이 없어지면(disable)
		if (!m_pTarget->GetEnable())
		{
			// 새 적을 찾기 위해 초기화 및 리턴
			m_pTarget = NULL;
			return 0;
		}
		// 타겟 추적 > 각도변화 후 + move시켜주기
		ChangeAngle(m_pTarget->GetPos(), m_tPos, m_fAngle, fDeltaTime);
	}

	return 0;
}

int CShardBullet::LateUpdate(float fDeltaTime)
{
	CMoveObj::LateUpdate(fDeltaTime);

	if(m_pTarget)
		MoveAngle(fDeltaTime);

	return 0;
}

void CShardBullet::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);

	if (m_pTarget)
	{
		RECTANGLE src(m_tPos.x - m_tPivot.x * m_tSize.x, m_tPos.y - m_tPivot.y * m_tSize.y,
			m_tPos.x + (1 - m_tPivot.x) *m_tSize.x, m_tPos.y + (1 - m_tPivot.y) * m_tSize.y);

		POSITION tPos = m_pTarget->GetPos();
		_SIZE tSize = m_pTarget->GetSize();
		POSITION tPivot = m_pTarget->GetPivot();

		RECTANGLE dest(tPos.x - tPivot.x * tSize.x, tPos.y - tPivot.y * tSize.y,
			tPos.x + (1 - tPivot.x) *tSize.x, tPos.y + (1 - tPivot.y) * tSize.y);

		if (RectToRect(src, dest) && GetDelayLimitTime() == -1)
		{
			//pBullet->Attack();
			// 몬스터에게 데미지 입히고
			m_pTarget->SetDir(m_pShard->GetDir());
			((CMonster*)m_pTarget)->GetDamage(m_pShard->GetDamage(), true, 1);
			// 사라지기 애니메이션 (실제로 애니메이션 후에 die)
			m_pAnimation->ChangeClip("ShardBulletEnd2");
			m_pTarget = NULL;
		}
	}	
}

void CShardBullet::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	if (m_pEffectObj && m_pEffectObj->GetEnable())
		m_pEffectObj->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	// 기본 사진이 좌측을 향하므로 90도돌린 각 전달
	float fTempAngle = m_fAngle + 90.f;
	if (fTempAngle > 360.f)
		fTempAngle -= 360.f;

	if (m_pTexture)
		m_pTexture->Render(m_tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime, 1.f, true, MD_LEFT, fTempAngle);
}

CShardBullet * CShardBullet::Clone()
{
	return new CShardBullet(*this);
}