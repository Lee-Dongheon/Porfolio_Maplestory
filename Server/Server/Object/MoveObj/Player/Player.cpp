#include "Player.h"
#include "../Monster/Monster.h"
#include "../../../Core/Input.h"
#include "../../../Collider/ColliderRect.h"
//#include "../../../Math.h"
#include "../../StaticObj/Item/Item.h"
#include "../../StaticObj/Skill/Skill.h"

#include "../../../Scene/Scene.h"
#include "../../../Core/DBManager.h"
#include "../../../Core/ClientManager.h"

CPlayer::CPlayer() :
	m_bDoubleJump(false),
	m_fHitTime(0.f),
	m_bInvincible(false),
	m_bOnRope(false),
	m_bOnLadder(false),
	m_bSceneChange(false),
	m_bInertia(false),
	m_bStatusChange(true),
	m_bIsGet(false),
	m_bProne(false),
	m_bProneAttack(false),
	m_bJumpBelow(false),
	m_eJumpDir(MD_LEFT),
	m_pMaxExp(NULL),
	m_iMeso(0),
	m_fSkillDelay(0.f),
	m_fSkillTime(0.f),
	m_strLastLeaveFloor("")
{
}

CPlayer::CPlayer(const CPlayer & player) :
	CMoveObj(player)
{
	// player 복사할 일이 없다..
}

CPlayer::~CPlayer()
{
	SAFE_DELETE_ARR(m_pMaxExp);
	Safe_Release_VecList(m_ItemList);
	Safe_Release_VecList(m_SkillList);
	Safe_Release_VecList(m_EquipList);
}

void CPlayer::SetAttackRect(float l, float t, float r, float b)
{
	m_tAttackRect.l = l;
	m_tAttackRect.t = t;
	m_tAttackRect.r = r;
	m_tAttackRect.b = b;
}

int CPlayer::GetMaxExp()
{
	if (m_iLevel >= 10) // max level : 10
		return 0;
	return m_pMaxExp[m_iLevel - 1];
}

CItem * CPlayer::FindItem(const string& strTag)
{
	list<CItem*>::iterator iter;
	list<CItem*>::iterator iterEnd = m_ItemList.end();

	for (iter = m_ItemList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return (*iter);
		}
	}

	return NULL;
}

CSkill * CPlayer::FindSkill(const string& strTag)
{
	list<CSkill*>::iterator iter;
	list<CSkill*>::iterator iterEnd = m_SkillList.end();

	for (iter = m_SkillList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return (*iter);
		}
	}

	return NULL;
}

CItem* CPlayer::CreateItem(const string & strTag, ITEM_TYPE eItemType, POSITION tItemPos, int iNum)
{
	CItem* pItem = new CItem;

	if (!pItem->Init())
	{
		delete pItem;
		return NULL;
	}
	pItem->SetTag(strTag);
	pItem->SetItemType(eItemType);
	pItem->SetCoord(tItemPos);
	pItem->SetNum(iNum);

	pItem->AddRef();
	m_ItemList.push_back(pItem);

	return pItem;
}

CSkill* CPlayer::CreateSkill(const string& strTag, int iStepLevel, int iSkillLevel)
{
	CSkill* pSkill = new CSkill;
	if (!pSkill->Init())
		return NULL;
	pSkill->SetTag(strTag);
	pSkill->SetSkillStep(iStepLevel);
	pSkill->SetLevel(iSkillLevel);

	m_SkillList.push_back(pSkill);

	return pSkill;
}

CItem * CPlayer::CreateEquip(const string & strTag, EQUIP_TYPE eEquipType)
{
	CItem* pItem = new CItem;

	if (!pItem->Init())
		return NULL;

	pItem->SetTag(strTag);
	pItem->SetItemType(IT_EQUIP);
	pItem->SetEquipType(eEquipType);

	pItem->AddRef();
	m_EquipList.push_back(pItem);

	return pItem;
}

void CPlayer::AddItem(const string& strTag, ITEM_TYPE eItemType, POSITION tItemPos, int iNum)
{
	//CItem* pItem = CreateItem(strTag, eItemType, tItemPos, iNum);
	CItem* pItem = FindItem(strTag);
	if (pItem)
		pItem->SetNum(pItem->GetNum() + iNum);
	else
		pItem = CreateItem(strTag, eItemType, tItemPos, iNum);

	// DB에도 추가하기
	GET_SINGLE(CDBManager)->AddItem(m_strTag, pItem);
	SAFE_RELEASE(pItem);
}

void CPlayer::ChangeItem(const string& strTag, ITEM_TYPE eItemType, POSITION tItemPos, int iNum, bool bErased)
{
	if (bErased)
		EraseItem(strTag);
	else
	{
		CItem* pItem = FindItem(strTag);
		
		// 없던 아이템이라면 아이템 추가
		if (!pItem)
			AddItem(strTag, eItemType, tItemPos, iNum);
		else // 있었다면 아이템 개수 or 위치 변화 적용
		{
			pItem->SetCoord(tItemPos);
			pItem->SetNum(iNum);

			SAFE_RELEASE(pItem);
		}
	}
}

void CPlayer::ChangeSkill(const string & strTag, int iSkillLevel)
{
	CSkill* pSkill = FindSkill(strTag);
	if (!pSkill)
		return;

	pSkill->SetLevel(iSkillLevel);

	SAFE_RELEASE(pSkill);
}

void CPlayer::EraseItem(const string & strTag)
{
	list<CItem*>::iterator iter;
	list<CItem*>::iterator iterEnd = m_ItemList.end();

	for (iter = m_ItemList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetTag() == strTag)
		{
			// DB에서도 지워주기
			GET_SINGLE(CDBManager)->EraseItem(strTag, (*iter));
			SAFE_RELEASE((*iter));
			m_ItemList.erase(iter);
			return;
		}
	}
}

void CPlayer::EraseEquip(EQUIP_TYPE eType)
{
	list<CItem*>::iterator iter;
	list<CItem*>::iterator iterEnd = m_EquipList.end();

	for (iter = m_EquipList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetEquipType() == eType)
		{
			// DB에서도 지워주기
			GET_SINGLE(CDBManager)->EraseEquip(m_strTag, (*iter));
			SAFE_RELEASE((*iter));
			m_EquipList.erase(iter);
			return;
		}
	}
}

void CPlayer::ExpInit()
{
	m_pMaxExp = new int[9];
	m_pMaxExp[0] = 15;
	m_pMaxExp[1] = 34;
	m_pMaxExp[2] = 57;
	m_pMaxExp[3] = 92;
	m_pMaxExp[4] = 135;
	m_pMaxExp[5] = 372;
	m_pMaxExp[6] = 560;
	m_pMaxExp[7] = 840;
	m_pMaxExp[8] = 1242;
}

void CPlayer::AddExp(int iExp)
{
	if (m_iLevel < 10)
	{
		m_iExp += iExp;
		if (m_iExp >= m_pMaxExp[m_iLevel - 1])
		{
			m_iExp -= m_pMaxExp[m_iLevel - 1];
			LevelUp();
			if (m_iLevel == 10)
				m_iExp = 0;
		}
	}
}

bool CPlayer::Init()
{
	SetPos(600.f, 606.f);
	SetSize(43.f, 66.f);
	SetPivot(0.5f, 1.f);
	SetObjType(OT_PLAYER);

	SetSpeed(150.f);
	SetSpeedY(100.f);

	SetLevel(1);
	ExpInit();
	SetAttackRect(0.f, -76.f, 100.f, 0.f);

	m_iMaxHP = m_iHP = 200;
	m_iMaxMP = m_iMP = 150;

	// 점프할 힘
	SetForce(350.f);

	return true;
}

void CPlayer::Input(float fDeltaTime)
{
	CMoveObj::Input(fDeltaTime);
}

int CPlayer::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);

	return 0;
}

int CPlayer::LateUpdate(float fDeltaTime)
{
	CMoveObj::LateUpdate(fDeltaTime);

	return 0;
}

//void CPlayer::Collision(float fDeltaTime)
//{
//	CMoveObj::Collision(fDeltaTime);
//}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::DoubleJump(int level)
{
	if (m_bJumping && !m_bDoubleJump)
	{
		ClearGravity();
		m_bDoubleJump = true;
		m_fForce = m_fForceOrigin;
		m_eJumpDir = m_eDir;
		SetSpeed(100.f + level * 50.f);
	}
}

void CPlayer::LevelUp()
{
	m_iLevel++;

	m_iMaxHP += 100;
	m_iMaxMP += 20;
	m_iHP = m_iMaxHP;
	m_iMP = m_iMaxMP;

	// 레벨업 플레이어에게 전송
	GET_SINGLE(CClientManager)->SendLevelUp(this);
}