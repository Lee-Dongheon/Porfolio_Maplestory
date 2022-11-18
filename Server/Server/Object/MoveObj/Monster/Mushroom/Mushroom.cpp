#include "Mushroom.h"
#include "../../../../Collider/ColliderRect.h"
#include "../../../StaticObj/Item/Item.h"

CMushroom::CMushroom()
{
}

CMushroom::CMushroom(const CMushroom & mushroom) :
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
	pRC->AddCollisionFunction(CS_ENTER, this, &CMushroom::Enter);
	pRC->AddCollisionFunction(CS_STAY, this, &CMushroom::Stay);
	pRC->AddCollisionFunction(CS_LEAVE, this, &CMushroom::Leave);

	SAFE_RELEASE(pRC);*/

	CColliderRect* pRC = AddCollider<CColliderRect>("OrangeFoot");

	pRC->SetRect(-32.f, -20.f, 31.f, 0.f);
	pRC->AddCollisionFunction(CS_ENTER, (CMonster*)this, &CMonster::FootEnter);
	pRC->AddCollisionFunction(CS_STAY, (CMonster*)this, &CMonster::FootStay);
	pRC->AddCollisionFunction(CS_LEAVE, (CMonster*)this, &CMonster::FootLeave);

	SAFE_RELEASE(pRC);

	SetForce(300.f);
	SetKnockBackForce(60.f);

	CItem* pItem = new CItem;
	if (!pItem->Init())
		return false;
	pItem->SetTag("OrangeHead");
	pItem->SetNum(1);
	pItem->SetItemType(IT_ETC);
	m_ItemList.push_back(pItem);

	return true;
}

//void CMushroom::Input(float fDeltaTime)
//{
//	CMonster::Input(fDeltaTime);
//}

int CMushroom::Update(float fDeltaTime)
{
	CMonster::Update(fDeltaTime);

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
					SetObjState(OBJ_STATE_JUMP);
					Jump();
					break;
				case 2:
				case 3:
				default:
					SetSpeed(50.f);
					SetObjState(OBJ_STATE_RUN);
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

	if (!m_bIsHit)
		MoveX(fDeltaTime, m_eDir);

	return 0;
}

int CMushroom::LateUpdate(float fDeltaTime)
{
	CMonster::LateUpdate(fDeltaTime);
	return 0;
}

//void CMushroom::Collision(float fDeltaTime)
//{
//	CMonster::Collision(fDeltaTime);
//}

CMushroom * CMushroom::Clone()
{
	return new CMushroom(*this);
}