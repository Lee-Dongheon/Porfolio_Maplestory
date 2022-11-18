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
	// ü���� 0 ���ϰ� �Ǿ� ������ true
	// �ƴϸ� false;
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
		// ��׷δ� 10�ʰ� �Ѿ�� Ǯ����.
		// ���������� ���ŵ�
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

				// ���Ͱ� ���� �� �� �͵�
				SetEnable(false);
				m_iHP = 0;
				SetSpeed(0.f);
				SetForce(0.f);

				// �������� �ش� scene�� �߰��Ѵ�
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

		if (m_bFalling) // �������� ���
		{
			ClearGravity();
			JumpEnd();
			SetPhysics(false);
			m_tPos.y = tHitPoint.y;
		}

		// ���� ���� ó�����ֱ�
		if (pDest->GetTag().find("Wall") != string::npos)
		{
			// ��� ������ ���� �ε��� ��츸 üũ�ϱ� ���ؼ�
			// HitPoint�� Pos�� �Ÿ��� ª�� ���� ��쿡�� �� �浹üũ�� �ϵ��� ¥��.

			if (m_tPos.x > tHitPoint.x) // ���� ���� �ε���
			{
				if (m_tSize.x * m_tPivot.x - (m_tPos.x - tHitPoint.x) < 5.f)
					m_tPos.x = tHitPoint.x + m_tSize.x * (1 - m_tPivot.x);
			}
			else // ������ ��
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

		// ���̶��
		if (pDest->GetTag().find("Wall") != string::npos)
		{
			if (m_tPos.x > tHitPoint.x) // ���� ���� �ε���
			{
				if (m_tSize.x * m_tPivot.x - (m_tPos.x - tHitPoint.x) < 5.f)
					m_tPos.x = tHitPoint.x + m_tSize.x * (1 - m_tPivot.x);
			}
			else // ������ ��
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
		// �ٴڿ��� �������� true
		if (pDest->GetTag().find("Floor") != string::npos)
			SetPhysics(true);
	}
}