#include "GreenMushroom.h"
#include "../../../../Collider/ColliderRect.h"
#include "../../../StaticObj/Item/Item.h"

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
	pRC->AddCollisionFunction(CS_ENTER, this, &CGreenMushroom::Enter);
	pRC->AddCollisionFunction(CS_STAY, this, &CGreenMushroom::Stay);
	pRC->AddCollisionFunction(CS_LEAVE, this, &CGreenMushroom::Leave);

	SAFE_RELEASE(pRC);*/

	CColliderRect* pRC = AddCollider<CColliderRect>("GreenFoot");

	pRC->SetRect(-28.f, -20.f, 28.f, 0.f);
	pRC->AddCollisionFunction(CS_ENTER, (CMonster*)this, &CMonster::FootEnter);
	pRC->AddCollisionFunction(CS_STAY, (CMonster*)this, &CMonster::FootStay);
	pRC->AddCollisionFunction(CS_LEAVE, (CMonster*)this, &CMonster::FootLeave);

	SAFE_RELEASE(pRC);

	SetForce(0.f); // 점프 불가
	SetKnockBackForce(60.f);

	m_bIsNoJump = true;
	SetPhysics(false);

	CItem* pItem = new CItem;
	if (!pItem->Init())
		return false;
	pItem->SetTag("GreenHead");
	pItem->SetNum(1);
	pItem->SetItemType(IT_ETC);
	m_ItemList.push_back(pItem);

	return true;
}

//void CGreenMushroom::Input(float fDeltaTime)
//{
//	CMonster::Input(fDeltaTime);
//}

int CGreenMushroom::Update(float fDeltaTime)
{
	CMonster::Update(fDeltaTime);

	if (m_bIsDead)
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
				SetObjState(OBJ_STATE_RUN);
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
					SetSpeed(50.f);
					m_eDir = MD_RIGHT;
					SetObjState(OBJ_STATE_RUN);
					break;
				case 2:
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

int CGreenMushroom::LateUpdate(float fDeltaTime)
{
	CMonster::LateUpdate(fDeltaTime);
	return 0;
}

//void CGreenMushroom::Collision(float fDeltaTime)
//{
//	CMonster::Collision(fDeltaTime);
//}

CGreenMushroom * CGreenMushroom::Clone()
{
	return new CGreenMushroom(*this);
}