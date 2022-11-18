#include "Mushmom.h"
#include "../../../../Collider/ColliderRect.h"
#include "../../../StaticObj/Item/Item.h"

CMushmom::CMushmom() :
	m_bIsAttack(false)
{
}

CMushmom::CMushmom(const CMushmom & mushmom) :
	CMonster(mushmom)
{
	m_bIsAttack = false;
}

CMushmom::~CMushmom()
{
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
	pRC->AddCollisionFunction(CS_ENTER, this, &CMushmom::Enter);
	pRC->AddCollisionFunction(CS_STAY, this, &CMushmom::Stay);
	pRC->AddCollisionFunction(CS_LEAVE, this, &CMushmom::Leave);

	SAFE_RELEASE(pRC);*/

	CColliderRect* pRC = AddCollider<CColliderRect>("MushmomFoot");

	pRC->SetRect(-60.f, -8.f, 60.f, 0.f);
	pRC->AddCollisionFunction(CS_ENTER, (CMonster*)this, &CMonster::FootEnter);
	pRC->AddCollisionFunction(CS_STAY, (CMonster*)this, &CMonster::FootStay);
	pRC->AddCollisionFunction(CS_LEAVE, (CMonster*)this, &CMonster::FootLeave);

	SAFE_RELEASE(pRC);

	SetForce(400.f);
	SetKnockBackForce(40.f);

	CItem* pItem = new CItem;
	if (!pItem->Init())
		return false;
	pItem->SetTag("MushmomPoja");
	pItem->SetNum(1);
	pItem->SetItemType(IT_ETC);
	m_ItemList.push_back(pItem);

	return true;
}

int CMushmom::Update(float fDeltaTime)
{
	CMonster::Update(fDeltaTime);

	if (m_bIsDead)
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
					SetObjState(OBJ_STATE_RUN);

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
					SetObjState(OBJ_STATE_IDLE);
					break;
				case 1:
					SetObjState(OBJ_STATE_JUMP);
					Jump();
					break;
				case 2:
					SetSpeed(50.f);
					m_eDir = MD_RIGHT;
					SetObjState(OBJ_STATE_RUN);
					break;
				case 3:
					SetSpeed(50.f);
					m_eDir = MD_LEFT;
					SetObjState(OBJ_STATE_RUN);
					break;
				default:
					break;
				}
			}
		}
	}

	if (!m_bIsHit && !m_bIsAttack)
		MoveX(fDeltaTime, m_eDir);

	return 0;
}

int CMushmom::LateUpdate(float fDeltaTime)
{
	CMonster::LateUpdate(fDeltaTime);

	return 0;
}

//void CMushmom::Collision(float fDeltaTime)
//{
//	CMonster::Collision(fDeltaTime);
//}

CMushmom * CMushmom::Clone()
{
	return new CMushmom(*this);
}

void CMushmom::Attack()
{
	m_bIsAttack = true;
	SetObjState(OBJ_STATE_ATTACK);
}
