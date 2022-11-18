#include "Monster.h"
#include "../Player/Player.h"
#include "../../../Core/Core.h"
#include "../../../Collider/ColliderRect.h"
#include "../../StaticObj/Item/Item.h"
#include "../../../Scene/Scene.h"

CMonster::CMonster() :
	m_fHitTime(0.f),
	m_fHitLimitTime(0.5f),
	m_fJumpTime(0.f),
	m_fJumpLimitTime(2.f),
	m_fRandomMoveTime(0.f),
	m_fRandomMoveLimitTime(0.2f),
	m_pAggressiveObj(NULL),
	m_fAggressiveTime(0.f),
	m_fAggressiveMoveTime(1.f),
	m_fAggressiveLimitTime(10.f),
	m_bIsNoJump(false)
{
}

CMonster::CMonster(const CMonster & monster) :
	CMoveObj(monster)
{
	m_fHitTime = 0;
	m_fHitLimitTime = monster.m_fHitLimitTime;
	m_fJumpTime = 0;
	m_fJumpLimitTime = monster.m_fJumpLimitTime;
	m_fRandomMoveTime = 0;
	m_fRandomMoveLimitTime = monster.m_fRandomMoveLimitTime;

	m_bIsHit = false;

	m_pAggressiveObj = NULL;
	m_fAggressiveTime = 0.f;
	m_fAggressiveMoveTime = 1.f;
	m_fAggressiveLimitTime = monster.m_fAggressiveLimitTime;

	m_ItemList.clear();

	list<CItem*>::const_iterator iter;
	list<CItem*>::const_iterator iterEnd = monster.m_ItemList.end();

	for (iter = monster.m_ItemList.begin(); iter != iterEnd; iter++)
	{
		m_ItemList.push_back((*iter)->Clone());
	}

}

CMonster::~CMonster()
{
	SAFE_RELEASE(m_pAggressiveObj);
	Safe_Release_VecList(m_ItemList);
}

void CMonster::SetAggressiveObj(CObj * pObj)
{
	SAFE_RELEASE(m_pAggressiveObj);
	pObj->AddRef();
	m_pAggressiveObj = pObj;
	m_fAggressiveTime = 0.f;
}

bool CMonster::GetDamage(int iDamage)
{
	// 체력이 0 이하가 되어 죽으면 true
	// 아니면 false;
	m_bIsHit = true;
	m_iHP -= iDamage;

	return (m_iHP <= 0);
}

bool CMonster::Init()
{
	SetPivot(0.5f, 1.0f);
	SetObjType(OT_MONSTER);
	SetObjState(OBJ_STATE_IDLE);
	SetPhysics(false);

	return true;
}

int CMonster::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);

	if (m_pAggressiveObj)
	{
		m_fAggressiveTime += fDeltaTime;
		m_fAggressiveMoveTime += fDeltaTime;
		// 어그로는 10초가 넘어가면 풀린다.
		// 맞을때마다 갱신됨
		if (m_fAggressiveTime >= m_fAggressiveLimitTime)
		{
			m_fAggressiveTime = 0.f;
			m_fAggressiveMoveTime = 1.f;
			SAFE_RELEASE(m_pAggressiveObj);
			SetObjState(OBJ_STATE_IDLE);
		}
	}

	if (m_bIsHit)
	{
		if (m_iHP <= 0)
		{
			if (!m_bIsDead)
			{
				m_bIsDead = true;

				// 몬스터가 죽을 때 할 것들
				SetEnable(false);
				m_iHP = 0;
				SetSpeed(0.f);
				SetForce(0.f);

				// 아이템을 해당 scene에 추가한다
				list<CItem*>::iterator iter;
				list<CItem*>::iterator iterEnd = m_ItemList.end();

				for (iter = m_ItemList.begin(); iter != iterEnd; iter++)
				{
					(*iter)->SetPos(m_tPos);
					m_pScene->AddObject((*iter));
				}
			}
		}
		m_bIsHit = false;
	}

	return 0;
}

int CMonster::LateUpdate(float fDeltaTime)
{
	CMoveObj::LateUpdate(fDeltaTime);

	return 0;
}

//void CMonster::Collision(float fDeltaTime)
//{
//	CMoveObj::Collision(fDeltaTime);
//}

void CMonster::FootEnter(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	if (pDest->GetObj()->GetObjType() == OT_MAP)
	{
		POSITION tHitPoint = pSrc->GetHitPoint();

		if (m_bFalling) // 떨어지는 경우
		{
			ClearGravity();
			JumpEnd();
			SetPhysics(false);
			m_tPos.y = tHitPoint.y;
		}

		// 벽은 따로 처리해주기
		if (pDest->GetTag().find("Wall") != string::npos)
		{
			// 대신 옆에서 벽에 부딪힌 경우만 체크하기 위해서
			// HitPoint와 Pos의 거리가 짧지 않은 경우에만 벽 충돌체크를 하도록 짜자.

			if (m_tPos.x > tHitPoint.x) // 왼쪽 벽에 부딪힘
			{
				if (m_tSize.x * m_tPivot.x - (m_tPos.x - tHitPoint.x) < 5.f)
					m_tPos.x = tHitPoint.x + m_tSize.x * (1 - m_tPivot.x);
			}
			else // 오른쪽 벽
			{
				if (m_tSize.x * (1 - m_tPivot.x) - (tHitPoint.x - m_tPos.x) < 5.f)
					m_tPos.x = tHitPoint.x - m_tSize.x * m_tPivot.x;
			}

			m_eDir = (MOVE_DIR)(-1 * (int)m_eDir);
		}
	}
}

void CMonster::FootStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	if (pDest->GetObj()->GetObjType() == OT_MAP)
	{
		POSITION tHitPoint = pSrc->GetHitPoint();

		// 벽이라면
		if (pDest->GetTag().find("Wall") != string::npos)
		{
			if (m_tPos.x > tHitPoint.x) // 왼쪽 벽에 부딪힘
			{
				if (m_tSize.x * m_tPivot.x - (m_tPos.x - tHitPoint.x) < 5.f)
					m_tPos.x = tHitPoint.x + m_tSize.x * (1 - m_tPivot.x);
			}
			else // 오른쪽 벽
			{
				if (m_tSize.x * (1 - m_tPivot.x) - (tHitPoint.x - m_tPos.x) < 5.f)
					m_tPos.x = tHitPoint.x - m_tSize.x * m_tPivot.x;
			}
		}
	}
}

void CMonster::FootLeave(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	if (pDest->GetObj()->GetObjType() == OT_MAP)
	{
		// 바닥에서 떠날때만 true
		if (pDest->GetTag().find("Floor") != string::npos)
			SetPhysics(true);
	}
}