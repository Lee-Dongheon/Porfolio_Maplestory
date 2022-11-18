#include "Player.h"
#include "../Monster/Monster.h"
#include "../../../Core/Input.h"
#include "../../../Collider/ColliderRect.h"
#include "../../../Core/Camera.h"
#include "../../../Core/Animation.h"
#include "../../../Math.h"
#include "../../StaticObj/Effect/EffectObj.h"
#include "../../../Core/Core.h"

#include "../../StaticObj/UI/UI.h"
#include "../../StaticObj/UI/UIMain.h"

#include "../../StaticObj/Icon/Icon.h"
#include "../../StaticObj/Icon/Skill/Divide/Divide.h"
#include "../../StaticObj/Icon/Skill/Expert/Expert.h"
#include "../../StaticObj/Icon/Skill/Shard/Shard.h"
#include "../../StaticObj/Icon/Skill/Bolt/Bolt.h"
#include "../../StaticObj/Icon/Skill/Plain/Plain.h"

#include "../../../ItemManager/ItemManager.h"

#include "../../../Scene/Scene.h"
#include "../../../Core/ServerManager.h"

CPlayer::CPlayer() :
	m_bDoubleJump(false),
	m_fHitTime(0.f),
	m_bInvincible(false),
	m_bOnRope(false),
	m_bOnLadder(false),
	m_bSceneChange(false),
	m_bInertia(false),
	m_bProne(false),
	m_bProneAttack(false),
	m_bJumpBelow(false),
	m_eJumpDir(MD_LEFT),
	m_pMaxExp(NULL),
	m_iMeso(0),
	m_fSkillDelay(0.f),
	m_fSkillTime(0.f),
	m_strLastLeaveFloor(""),
	m_pLevelUpEffectObj(NULL),
	m_bChatMode(false)
{
}

CPlayer::CPlayer(const CPlayer & player) :
	CMoveObj(player)
{
	// player 복사할 일이 없다..

	for (int i = 0; i < (int)IT_END; i++)
	{
		m_mapItem[i].clear();

		map<POSITION, CItem*>::const_iterator iter;
		map<POSITION, CItem*>::const_iterator iterEnd = player.m_mapItem[i].end();

		for (iter = player.m_mapItem[i].begin(); iter != iterEnd; iter++)
		{
			m_mapItem[i].insert(make_pair(iter->first, iter->second));
		}
	}
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pLevelUpEffectObj);
	SAFE_DELETE_ARR(m_pMaxExp);

	for (int i = 0; i < (int)IT_END; i++)
	{
		map<POSITION, CItem*>::iterator iter;
		map<POSITION, CItem*>::iterator iterEnd = m_mapItem[i].end();

		for (iter = m_mapItem[i].begin(); iter != iterEnd; iter++)
		{
			CObj::EraseObj(iter->second);
			SAFE_RELEASE(iter->second);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		Safe_Release_VecList(m_vecSkill[i]);
	}

	Safe_Release_Map(m_mapEquip);
}

void CPlayer::SetAttackRect(float l, float t, float r, float b)
{
	m_tAttackRect.l = l;
	m_tAttackRect.t = t;
	m_tAttackRect.r = r;
	m_tAttackRect.b = b;
}

void CPlayer::SetAlertMotion()
{
}

int CPlayer::GetMaxExp()
{
	if (m_iLevel >= 10) // max level : 10
		return 0;
	return m_pMaxExp[m_iLevel - 1];
}

bool CPlayer::AddItem(CItem* pItem, POSITION tPos)
{
	// 만약 있던 아이템이라면 개수를 증가
	CItem* pSameItem = FindItem(pItem->GetTag());
	if (pSameItem)
	{
		pSameItem->AddNum(pItem->GetNum());
		SAFE_RELEASE(pSameItem);

		return true;
	}
	else // 아니라면
	{
		pItem->AddRef();
		pItem->SetObj(this);
		GET_SINGLE(CItemManager)->AddItem(pItem->GetTag(), pItem->GetItemType());

		// 위치가 정해져있지 않다면 비어있는 가장 앞 좌표를 찾아 넣어주기
		if (tPos == POSITION(-1.f, -1.f))
		{
			POSITION tFrontPos = FindItemCoord(pItem->GetItemType());
			m_mapItem[pItem->GetItemType()].insert(make_pair(tFrontPos, pItem));
		}
		else // 정해진 위치가 있다면 해당 위치에 넣어주기
		{
			m_mapItem[pItem->GetItemType()].insert(make_pair(tPos, pItem));
		}

		return false;
	}
}

void CPlayer::ChangeItemPos(POSITION tUIPos, POSITION tPos, CItem * pItem)
{
	// 특정 좌표에 아이템을 옮기는 함수인데
	// 이 행위는 마우스로 옮기는 것 말고는 불가능하다
	// 인자로 받아온 pItem도 기존에 아이템에 들어있었다는 점이 중요

	// 옮길 위치 아이템 가져오기
	CItem* pBeforeItem = FindItem(tPos, pItem->GetItemType());
	ITEM_TYPE eType = pItem->GetItemType();

	// tCoord는 pItem의 기존 좌표
	POSITION tCoord;
	tCoord.x = (pItem->GetIconPos().x - tUIPos.x - 13.f) / 42.f;
	tCoord.y = (pItem->GetIconPos().y - tUIPos.y - 54.f) / 42.f;

	// 해당 좌표에 기존 아이템이 있었다면
	if (pBeforeItem)
	{
		// 같은 타입 아니면 함수 종료		
		if (eType != pBeforeItem->GetItemType())
			return;

		// 기존 아이템을 현재 아이템의 위치와 맞바꿔야 한다.

		// tCoord는 pItem의 기존 좌표(before가 가야하는 좌표)
		// tPos는 before의 기존 좌표(pitem이 목표하는 좌표)
		map<POSITION, CItem*>::iterator iter = m_mapItem[eType].find(tCoord);
		// 제대로 찾았다면 위치 교체
		if (iter != m_mapItem[eType].end())
		{
			if (iter->second == pItem)
				iter->second = pBeforeItem;
			// 기존 아이템 위치 교체를 서버에 알림
			GET_SINGLE(CServerManager)->SendChangeItem(pBeforeItem->GetTag(), pBeforeItem->GetItemType(), pBeforeItem->GetNum(), iter->first);
		}
		if (iter != m_mapItem[eType].end())
		{
			iter = m_mapItem[eType].find(tPos);
			if (iter->second == pBeforeItem)
				iter->second = pItem;
		}
		SAFE_RELEASE(pBeforeItem);
	}
	else // 아이템이 없었다면
	{
		// 새 위치에 아이템을 넣고
		m_mapItem[eType].insert(make_pair(tPos, pItem));
		// 기존 좌표는 없애주기
		map<POSITION, CItem*>::iterator iter = m_mapItem[eType].find(tCoord);
		// 제대로 찾았다면 삭제
		if (iter != m_mapItem[eType].end())
		{
			if (iter->second == pItem)
				m_mapItem[eType].erase(iter);
		}
	}
	// 아이템 위치 교체를 서버에 알림
	GET_SINGLE(CServerManager)->SendChangeItem(pItem->GetTag(), pItem->GetItemType(), pItem->GetNum(), tPos);

	//SAFE_RELEASE(pBeforeItem);
}

void CPlayer::EraseItem(CItem * pItem)
{
	ITEM_TYPE eType = pItem->GetItemType();

	map<POSITION, CItem*>::iterator iter;
	map<POSITION, CItem*>::iterator iterEnd = m_mapItem[eType].end();

	for (iter = m_mapItem[eType].begin(); iter != iterEnd; iter++)
	{
		if (iter->second == pItem)
		{
			// 서버에 전송
			GET_SINGLE(CServerManager)->SendChangeItem(pItem->GetTag(), eType, pItem->GetNum(), iter->first, true);
			CObj::EraseObj(iter->second);
			SAFE_RELEASE(iter->second);
			m_mapItem[eType].erase(iter);
			break;
		}
	}
}

void CPlayer::AddEquip(CItem * pItem)
{
	// 장비 맵에 추가하고
	m_mapEquip.insert(make_pair(pItem->GetEquipType(), pItem));
	pItem->AddRef();
	// 서버에 알림
	GET_SINGLE(CServerManager)->SendAddEquip(m_strTag, pItem->GetTag(), pItem->GetEquipType());
	// 아이템 창에서는 삭제(함수 내부에서 서버에 알림)
	EraseItem(pItem);
}

void CPlayer::EraseEquip(CItem * pItem)
{
	// 이건 뺀다고 반드시 아이템이 아이템맵으로 들어간다는 보장이 없음
	map<EQUIP_TYPE, CItem*>::iterator iter = m_mapEquip.find(pItem->GetEquipType());

	if (iter == m_mapEquip.end())
		return;

	// 서버에 알림
	GET_SINGLE(CServerManager)->SendEraseEquip(m_strTag, pItem->GetEquipType());
	// 지움
	SAFE_RELEASE(iter->second);
	m_mapEquip.erase(iter);
}

CItem* CPlayer::FindItem(const string& strTag)
{
	ITEM_TYPE eType = GET_SINGLE(CItemManager)->FindItemType(strTag);
	if (eType == IT_DEFAULT)
		return NULL;

	map<POSITION, CItem*>::iterator iter;
	map<POSITION, CItem*>::iterator iterEnd = m_mapItem[eType].end();

	for (iter = m_mapItem[eType].begin(); iter != iterEnd; iter++)
	{
		if (iter->second->GetTag() == strTag)
		{
			iter->second->AddRef();
			return iter->second;
		}
	}

	return nullptr;
}

CItem * CPlayer::FindItem(POSITION tPos, ITEM_TYPE eType)
{
	map<POSITION, CItem*>::iterator iter = m_mapItem[eType].find(tPos);

	if (iter == m_mapItem[eType].end())
		return NULL;

	iter->second->AddRef();
	return iter->second;
}

POSITION CPlayer::FindItemCoord(ITEM_TYPE eType)
{
	POSITION tPos = POSITION(0.f, 0.f);
	map<POSITION, CItem*>::iterator iter = m_mapItem[eType].find(tPos);
	while (iter != m_mapItem[eType].end())
	{
		// 아이템 좌표는 x 넓이가 4, 다음 순서에 아이템 있는지 체크
		tPos.Next(4);
		iter = m_mapItem[eType].find(tPos);
	}

	return tPos;
}

CSkill * CPlayer::FindSkill(int iStepLevel, const string & strTag)
{
	for (int i = 0; i < m_vecSkill[iStepLevel].size(); i++)
	{
		if (m_vecSkill[iStepLevel][i]->GetTag() == strTag)
		{
			m_vecSkill[iStepLevel][i]->AddRef();
			return m_vecSkill[iStepLevel][i];
		}
	}
	return NULL;
}

CItem * CPlayer::FindEquip(EQUIP_TYPE eType)
{
	map<EQUIP_TYPE, CItem*>::iterator iter = m_mapEquip.find(eType);

	if (iter == m_mapEquip.end())
		return NULL;

	iter->second->AddRef();
	return iter->second;
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

	CColliderRect* pRC = AddCollider<CColliderRect>("PlayerFoot");

	pRC->SetRect(-21.f, -8.f, 22.f, 0.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &CPlayer::FootEnter);
	pRC->AddCollisionFunction(CS_STAY, this, &CPlayer::FootStay);
	pRC->AddCollisionFunction(CS_LEAVE, this, &CPlayer::FootLeave);

	SAFE_RELEASE(pRC);

	m_iMaxHP = m_iHP = 200;
	m_iMaxMP = m_iMP = 150;

	// 중력 설정
	SetPhysics(true);
	// 점프할 힘
	SetForce(350.f);
	// 넉백 힘
	SetKnockBackForce(80.f);

	CAnimation* pAnimation = CreateAnimation("PlayerAnimation");
	SAFE_RELEASE(pAnimation);

	// 43 x 66
	POSITION arrOrigin[4] = { POSITION(21.f, 66.f), POSITION(21.f, 66.f), POSITION(21.f, 66.f), POSITION(21.f, 66.f) };
	POSITION arrLT[4] = { POSITION(-21.f, -66.f), POSITION(-21.f, -66.f), POSITION(-21.f, -66.f), POSITION(-21.f, -66.f) };
	POSITION arrRB[4] = { POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f) };
	float arrDelay[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	AddAnimationClip("Idle", AO_LOOP, 4, arrOrigin, arrLT, arrRB, arrDelay, "PlayerStand", L"Player/Stand/");

	// 51 x 66
	POSITION arrOrigin2[4] = { POSITION(29.f, 66.f), POSITION(29.f, 66.f), POSITION(29.f, 66.f), POSITION(29.f, 66.f) };
	POSITION arrLT2[4] = { POSITION(-29.f, -66.f), POSITION(-29.f, -66.f), POSITION(-29.f, -66.f), POSITION(-29.f, -66.f) };
	POSITION arrRB2[4] = { POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f) };
	float arrDelay2[4] = { 0.18f, 0.18f, 0.18f, 0.18f };
	AddAnimationClip("Run", AO_LOOP, 4, arrOrigin2, arrLT2, arrRB2, arrDelay2, "PlayerRun", L"Player/Run/");

	// 48 x 64
	POSITION arrOrigin3[1] = { POSITION(20.f, 64.f) };
	POSITION arrLT3[1] = { POSITION(-20.f, -64.f) };
	POSITION arrRB3[1] = { POSITION(28.f, 0.f) };
	float arrDelay3[1] = { 0.1f };
	AddAnimationClip("Jump", AO_LOOP, 1, arrOrigin3, arrLT3, arrRB3, arrDelay3, "PlayerJump", L"Player/Jump/");

	// 47 x 65
	POSITION arrOrigin4[4] = { POSITION(25.f, 65.f), POSITION(25.f, 65.f), POSITION(25.f, 65.f), POSITION(25.f, 65.f) };
	POSITION arrLT4[4] = { POSITION(-25.f, -65.f), POSITION(-25.f, -65.f), POSITION(-25.f, -65.f), POSITION(-25.f, -65.f) };
	POSITION arrRB4[4] = { POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f) };
	float arrDelay4[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	AddAnimationClip("Alert", AO_TIME_RETURN, 4, arrOrigin4, arrLT4, arrRB4, arrDelay4, "PlayerAlert", L"Player/Alert/");

	// 엎드리기
	// 67 x 38
	POSITION arrOrigin5[1] = { POSITION(33.f, 38.f) };
	POSITION arrLT5[1] = { POSITION(-33.f, -38.f) };
	POSITION arrRB5[1] = { POSITION(34.f, 0.f) };
	float arrDelay5[1] = { 0.1f };
	AddAnimationClip("Prone", AO_LOOP, 1, arrOrigin5, arrLT5, arrRB5, arrDelay5, "PlayerProne", L"Player/Prone/");

	// 41 x 66
	POSITION arrOrigin6[1] = { POSITION(20.f, 66.f) };
	POSITION arrLT6[1] = { POSITION(-20.f, -66.f) };
	POSITION arrRB6[1] = { POSITION(21.f, 0.f) };
	float arrDelay6[1] = { 0.1f };
	AddAnimationClip("Rope1", AO_LOOP, 1, arrOrigin6, arrLT6, arrRB6, arrDelay6, "PlayerRope1", L"Player/Rope/Rope1/");
	AddAnimationClip("Rope2", AO_LOOP, 1, arrOrigin6, arrLT6, arrRB6, arrDelay6, "PlayerRope2", L"Player/Rope/Rope2/");

	// 44 x 65
	POSITION arrOrigin7[1] = { POSITION(22.f, 65.f) };
	POSITION arrLT7[1] = { POSITION(-22.f, -65.f) };
	POSITION arrRB7[1] = { POSITION(22.f, 0.f) };
	float arrDelay7[1] = { 0.1f };
	AddAnimationClip("Ladder1", AO_LOOP, 1, arrOrigin7, arrLT7, arrRB7, arrDelay7, "PlayerLadder1", L"Player/Ladder/Ladder1/");
	AddAnimationClip("Ladder2", AO_LOOP, 1, arrOrigin7, arrLT7, arrRB7, arrDelay7, "PlayerLadder2", L"Player/Ladder/Ladder2/");

	// 39 x 63
	POSITION arrOrigin8[1] = { POSITION(19.f, 63.f) };
	POSITION arrLT8[1] = { POSITION(-19.f, -63.f) };
	POSITION arrRB8[1] = { POSITION(20.f, 0.f) };
	float arrDelay8[1] = { 0.1f };
	AddAnimationClip("Die", AO_LOOP, 1, arrOrigin8, arrLT8, arrRB8, arrDelay8, "PlayerDie", L"Player/Die/");

	// swing attack
	// 88 x 65
	POSITION arrOrigin9[3] = { POSITION(66.f, 65.f), POSITION(66.f, 65.f), POSITION(66.f, 65.f) };
	POSITION arrLT9[3] = { POSITION(-66.f, -65.f), POSITION(-66.f, -65.f), POSITION(-66.f, -65.f) };
	POSITION arrRB9[3] = { POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f) };
	float arrDelay9[3] = { 0.3f, 0.15f, 0.35f };
	AddAnimationClip("Attack", AO_ONCE_RETURN, 3, arrOrigin9, arrLT9, arrRB9, arrDelay9, "PlayerAttack1", L"Player/Attack/0/");

	// ProneAttack
	// 82 x 38
	POSITION arrOrigin10[2] = { POSITION(48.f, 38.f), POSITION(48.f, 38.f) };
	POSITION arrLT10[2] = { POSITION(-48.f, -38.f), POSITION(-48.f, -38.f) };
	POSITION arrRB10[2] = { POSITION(34.f, 0.f), POSITION(34.f, 0.f) };
	float arrDelay10[2] = { 0.3f, 0.15f };
	AddAnimationClip("ProneAttack", AO_ONCE_RETURN, 2, arrOrigin10, arrLT10, arrRB10, arrDelay10, "PlayerProneAttack", L"Player/ProneAttack/");

	CPlain* pPlain = AddSkill<CPlain>("Plain");
	SAFE_RELEASE(pPlain);
	CBolt* pBolt = AddSkill<CBolt>("Bolt");
	SAFE_RELEASE(pBolt);

	CShard* pShard = AddSkill<CShard>("Shard");
	SAFE_RELEASE(pShard);

	CExpert* pExpert = AddSkill<CExpert>("Expert");
	SAFE_RELEASE(pExpert);

	CDivide* pDiv = AddSkill<CDivide>("Divide");
	SAFE_RELEASE(pDiv);

	m_pLevelUpEffectObj = new CEffectObj;
	m_pLevelUpEffectObj->SetObj(this);
	m_pLevelUpEffectObj->SetEnable(false);
	CAnimation* pEffectAni = m_pLevelUpEffectObj->CreateAnimation("LevelUpEffect");
	SAFE_RELEASE(pEffectAni);

	float arrEffectDelay[21];
	fill_n(arrEffectDelay, 21, 0.09f);
	arrEffectDelay[0] = 0.5f;

	POSITION arrEffectOrigin[21] = { POSITION(0.f, 0.f), POSITION(131.f, 564.f), POSITION(105.f, 613.f),
									POSITION(113.f, 599.f), POSITION(121.f, 599.f), POSITION(165.f, 535.f),
									POSITION(192.f, 522.f), POSITION(190.f, 296.f), POSITION(193.f, 306.f),
									POSITION(198.f, 310.f), POSITION(202.f, 311.f), POSITION(423.f, 486.f),
									POSITION(423.f, 526.f), POSITION(423.f, 543.f), POSITION(423.f, 606.f),
									POSITION(448.f, 643.f), POSITION(448.f, 643.f), POSITION(448.f, 643.f),
									POSITION(448.f, 643.f), POSITION(448.f, 643.f), POSITION(353.f, 549.f) };
	POSITION arrEffectLT[21] = { POSITION(0.f, 0.f), POSITION(-131.f, -564.f), POSITION(-105.f, -613.f),
									POSITION(-113.f, -599.f), POSITION(-121.f, -599.f), POSITION(-165.f, -535.f),
									POSITION(-192.f, -522.f), POSITION(-190.f, -296.f), POSITION(-193.f, -306.f),
									POSITION(-198.f, -310.f), POSITION(-202.f, -311.f), POSITION(-423.f, -486.f),
									POSITION(-423.f, -526.f), POSITION(-423.f, -543.f), POSITION(-423.f, -606.f),
									POSITION(-448.f, -643.f), POSITION(-448.f, -643.f), POSITION(-448.f, -643.f),
									POSITION(-448.f, -643.f), POSITION(-448.f, -643.f), POSITION(-353.f, -549.f) };
	POSITION arrEffectRB[21] = { POSITION(0.f, 0.f), POSITION(143.f, 30.f), POSITION(143.f, 30.f),
									POSITION(143.f, 42.f), POSITION(128.f, 51.f), POSITION(215.f, 56.f),
									POSITION(214.f, 60.f), POSITION(211.f, 62.f), POSITION(207.f, 18.f),
									POSITION(204.f, -16.f), POSITION(201.f, -22.f), POSITION(376.f, 106.f),
									POSITION(373.f, 145.f), POSITION(376.f, 163.f), POSITION(418.f, 226.f),
									POSITION(456.f, 261.f), POSITION(456.f, 261.f), POSITION(456.f, 261.f),
									POSITION(456.f, 261.f), POSITION(456.f, 261.f), POSITION(362.f, 166.f) };

	m_pLevelUpEffectObj->AddAnimationClip("LevelUp", AO_ONCE_DISABLE, 21, arrEffectOrigin, arrEffectLT, arrEffectRB,
		arrEffectDelay, "LevelUpEffect", L"Player/LevelUp/");

	return true;
}

void CPlayer::Input(float fDeltaTime)
{
	CMoveObj::Input(fDeltaTime);

	if (m_fSkillDelay > 0.f)
	{
		m_fSkillTime += fDeltaTime;
		if (m_fSkillTime > m_fSkillDelay)
		{
			m_fSkillDelay = 0.f;
			m_fSkillTime = 0.f;
		}
		return;
	}
	
	// 채팅모드에선 입력 받지 않음
	/*if (m_bChatMode)
		return;*/

	if (KEYPRESS("MoveLeft") && !m_bIsAttack)
		m_eDir = MD_LEFT;
	else if (KEYPRESS("MoveRight") && !m_bIsAttack)
		m_eDir = MD_RIGHT;

	if (KEYDOWN("MoveLeft") && !m_bIsAttack && !m_bOnRope && !m_bOnLadder && !m_bProne)
	{
		m_bMove = true;
		MoveX(fDeltaTime, m_eDir);
		m_pAnimation->ChangeClip("Run");
		SetObjState(OBJ_STATE_RUN);
	}

	if (KEYDOWN("MoveRight") && !m_bIsAttack && !m_bOnRope && !m_bOnLadder && !m_bProne)
	{
		m_bMove = true;
		MoveX(fDeltaTime, m_eDir);
		m_pAnimation->ChangeClip("Run");
		SetObjState(OBJ_STATE_RUN);
	}

	if (KEYDOWN("Up") && (m_bOnRope || m_bOnLadder))
	{
		MoveY(fDeltaTime, MD_LEFT);
	}

	if (KEYDOWN("Down") && !m_bJumping && !m_bIsAttack)
	{
		if (m_bOnRope || m_bOnLadder)
			MoveY(fDeltaTime, MD_RIGHT);
		else
		{
			m_bProne = true;
			SetObjState(OBJ_STATE_PRONE);
		}
	}

	if (KEYPRESS("Alt") && !m_bJumping && !m_bIsAttack)
	{
		if (m_bProne)
			JumpBelow();
		else
		{
			if (!m_bOnRope && !m_bOnLadder)
				Jump();
			else
			{
				if (KEYDOWN("MoveLeft") || KEYDOWN("MoveRight"))
				{
					m_bOnRope = false;
					m_bOnLadder = false;
					m_bMove = true;
					MoveX(fDeltaTime, m_eDir);
					Jump();
				}
			}
		}
		SetObjState(OBJ_STATE_JUMP);
	}

	if (KEYPRESS("Ctrl") && !m_bIsAttack && !m_bProneAttack)
	{
		if (m_bProne)
			m_bProneAttack = true;
		else
			m_bIsAttack = true;
		Attack();
		SetObjState(OBJ_STATE_ATTACK);
	}
}

int CPlayer::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);

	if (!m_bMove && !m_bIsAttack && !m_bOnRope && !m_bJumping)
	{
		m_pAnimation->ChangeClip("Idle");
		SetObjState(OBJ_STATE_IDLE);
	}

	if (m_bOnRope)
	{
		int i = (int)(m_tPos.y);
		if ((i / 30) % 2 == 0)
			m_pAnimation->ChangeClip("Rope1");
		else
			m_pAnimation->ChangeClip("Rope2");
		SetObjState(OBJ_STATE_ROPE);
	}
	if (m_bOnLadder)
	{
		int i = (int)(m_tPos.y);
		if ((i / 30) % 2 == 0)
			m_pAnimation->ChangeClip("Ladder1");
		else
			m_pAnimation->ChangeClip("Ladder2");
		SetObjState(OBJ_STATE_LADDER);
	}
	if (m_bProne && !m_bProneAttack && !m_bIsAttack)
	{
		m_pAnimation->ChangeClip("Prone");
		SetObjState(OBJ_STATE_PRONE);
	}

	if (m_bJumping)
	{
		m_pAnimation->ChangeClip("Jump");
		SetObjState(OBJ_STATE_JUMP);
	}

	if (m_bProneAttack && !m_bIsAttack)
	{
		if (m_pAnimation->GetMotionEnd())
		{
			m_bProneAttack = false;
			SetObjState(OBJ_STATE_IDLE);
		}
		else
			m_pAnimation->ChangeClip("ProneAttack");
	}
	if (m_bIsAttack && !m_bProneAttack)
	{
		if (m_pAnimation->GetMotionEnd())
		{
			m_bIsAttack = false;
			SetObjState(OBJ_STATE_IDLE);
		}
		else
			m_pAnimation->ChangeClip("Attack");
	}

	if (m_bIsAttack && m_pAnimation->GetMotionEnd())
	{
		m_bIsAttack = false;
		SetObjState(OBJ_STATE_IDLE);
	}

	if (m_iLevel < 10)
	{
		if (m_pMaxExp[m_iLevel - 1] <= m_iExp)
		{
			m_iExp -= m_pMaxExp[m_iLevel - 1];
			LevelUp();
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < (int)m_vecSkill[i].size(); j++)
		{
			if (!m_vecSkill[i][j]->GetEnable())
				continue;
			m_vecSkill[i][j]->Update(fDeltaTime);
		}
	}

	m_bMove = false;
	m_bProne = false;

	if (m_pLevelUpEffectObj)
	{
		if (m_pLevelUpEffectObj->GetEnable())
			m_pLevelUpEffectObj->Update(fDeltaTime);
	}

	map<EQUIP_TYPE, CItem*>::iterator iter;
	map<EQUIP_TYPE, CItem*>::iterator iterEnd = m_mapEquip.end();

	for (iter = m_mapEquip.begin(); iter != iterEnd; iter++)
	{
		iter->second->Update(fDeltaTime);
	}

	return 0;
}

int CPlayer::LateUpdate(float fDeltaTime)
{
	CMoveObj::LateUpdate(fDeltaTime);

	RESOLUTION tRS = m_pScene->GetSceneRS();
	// 현재 위치한 scene 크기 밖으로 안나가게 위치보정

	if (m_tPos.x - m_tSize.x * m_tPivot.x < 0.f)
		m_tPos.x = m_tSize.x + m_tPivot.x;
	else if (m_tPos.x + m_tSize.x * (1 - m_tPivot.x) >(float)tRS.iW)
		m_tPos.x = (float)tRS.iW - m_tSize.x * (1 - m_tPivot.x);

	if (m_tPos.y - m_tSize.y *m_tPivot.y < 0.f)
		m_tPos.y = m_tSize.y *m_tPivot.y;
	if (m_tPos.y > m_pScene->GetYPosMax())
	{
		m_tPos.y = m_pScene->GetYPosMax();
		JumpEnd();
		ClearGravity();
		SetPhysics(false);
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < (int)m_vecSkill[i].size(); j++)
		{
			if (!m_vecSkill[i][j]->GetEnable())
				continue;
			m_vecSkill[i][j]->LateUpdate(fDeltaTime);
		}
	}

	GET_SINGLE(CServerManager)->SendMoveCharacter(this);

	return 0;
}

void CPlayer::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < (int)m_vecSkill[i].size(); j++)
		{
			if (!m_vecSkill[i][j]->GetEnable())
				continue;
			m_vecSkill[i][j]->Collision(fDeltaTime);
		}
	}
}

void CPlayer::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CMoveObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	map<EQUIP_TYPE, CItem*>::iterator iter;
	map<EQUIP_TYPE, CItem*>::iterator iterEnd = m_mapEquip.end();

	for (iter = m_mapEquip.begin(); iter != iterEnd; iter++)
	{
		iter->second->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}

	// 오브젝트 하단에 이름 표시
	HDC hDC;
	pGDIRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
	SetBkColor(hDC, RGB(128, 128, 128));
	SetTextColor(hDC, RGB(255, 255, 255));

	POSITION tPos = m_tPos - GET_SINGLE(CCamera)->GetPos();
	TextOutA(hDC, tPos.x - m_tSize.x / 2, tPos.y, m_strTag.c_str(), m_strTag.length());

	pGDIRenderTarget->ReleaseDC(NULL);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < (int)m_vecSkill[i].size(); j++)
		{
			if (!m_vecSkill[i][j]->GetEnable())
				continue;
			m_vecSkill[i][j]->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
		}
	}

	if (m_pLevelUpEffectObj)
	{
		if (m_pLevelUpEffectObj->GetEnable())
			m_pLevelUpEffectObj->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}
}

int CPlayer::Update(PSC_ObjectPacket pPacket)
{
	CMoveObj::Update(pPacket);

	SetExp(pPacket->exp);

	return 0;
}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::Attack()
{
	RECTANGLE tAttackRect;
	tAttackRect.l = m_tPos.x + m_eDir * m_tAttackRect.l;
	tAttackRect.t = m_tPos.y + m_tAttackRect.t;
	tAttackRect.r = m_tPos.x + m_eDir * m_tAttackRect.r;
	tAttackRect.b = m_tPos.y + m_tAttackRect.b;

	if (tAttackRect.l > tAttackRect.r)
	{
		tAttackRect.l += tAttackRect.r;
		tAttackRect.r = tAttackRect.l - tAttackRect.r;
		tAttackRect.l -= tAttackRect.r;
	}

	list<CObj*> monsterList;
	monsterList.clear();
	// 공격 범위 안에 있는 몬스터 리스트 가져오기
	m_pScene->GetObjList(OT_MONSTER, monsterList, tAttackRect);

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = monsterList.end();

	CObj* pMonster = NULL;
	// 몬스터 한마리만 찾기
	for (iter = monsterList.begin(); iter != iterEnd; iter++)
	{
		if (PointInRect(tAttackRect, (*iter)->GetPos()))
		{
			pMonster = (*iter);
			break;
		}
	}
	// 만약 공격 범위에 몬스터 한마리가 있다면 공격 실행
	if (pMonster)
	{
		((CMonster*)pMonster)->GetDamage(10, false, 1);
	}
}

void CPlayer::JumpBelow()
{
	if (!m_bJumping)
	{
		m_bJumping = true;
		m_bJumpBelow = true;
		m_fForce = m_fForceOrigin / 10.f;

		SetPhysics(true);
	}
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

	if (m_pLevelUpEffectObj)
		m_pLevelUpEffectObj->SetEnable(true);
}

void CPlayer::FootEnter(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	if (pDest->GetObj()->GetObjType() == OT_MAP)
	{
		POSITION tHitPoint = pSrc->GetHitPoint();
		if (pDest->GetTag().find("Rope") != string::npos)
		{
			if (KEYPRESS("Up") || KEYDOWN("Up") || KEYPRESS("Down") || KEYDOWN("Down"))
			{
				ClearGravity();
				JumpEnd();
				SetPhysics(false);
				m_tPos.x = tHitPoint.x;
				m_bOnRope = true;
			}
		}
		else if (pDest->GetTag().find("Ladder") != string::npos)
		{
			if (KEYPRESS("Up") || KEYDOWN("Up") || KEYPRESS("Down") || KEYDOWN("Down"))
			{
				ClearGravity();
				JumpEnd();
				SetPhysics(false);
				m_tPos.x = tHitPoint.x;
				m_bOnLadder = true;
			}
		}
		else
		{
			// 떨어지는 경우
			if (m_bFalling)
			{
				// 아랫점프인 경우, 점프 직후에 만나는 첫 발판은 무시하게 해주기
				if (m_bJumpBelow)
				{
					m_bJumpBelow = false;
					return;
				}

				ClearGravity();
				JumpEnd();
				SetPhysics(false);
				m_tPos.y = tHitPoint.y;
			}

			// 벽은 따로 처리해주기
			if (pDest->GetTag().find("Wall") != string::npos &&
				pDest->GetTag().find("Monster") == string::npos) // 몬스터 전용 벽이 아니어야 함
			{
				// 대신 옆에서 벽에 부딪힌 경우만 체크하기 위해서
				// HitPoint와 Pos의 거리가 짧지 않은 경우에만 벽 충돌체크를 하도록 짜자.

				if (m_tPos.x > tHitPoint.x) // 왼쪽 벽에 부딪힘
				{
					if (m_tSize.x * m_tPivot.x - (m_tPos.x - tHitPoint.x) < 3.f)
						m_tPos.x = tHitPoint.x + m_tSize.x * (1 - m_tPivot.x);
				}
				else // 오른쪽 벽
				{
					if (m_tSize.x * (1 - m_tPivot.x) - (tHitPoint.x - m_tPos.x) < 3.f)
						m_tPos.x = tHitPoint.x - m_tSize.x * m_tPivot.x;
				}
			}
		}
	}
}

void CPlayer::FootStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	if (pDest->GetObj()->GetObjType() == OT_MAP)
	{
		POSITION tHitPoint = pSrc->GetHitPoint();
		if (pDest->GetTag().find("Rope") != string::npos)
		{
			if (KEYPRESS("Up") || KEYDOWN("Up") || KEYPRESS("Down") || KEYDOWN("Down"))
			{
				ClearGravity();
				JumpEnd();
				SetPhysics(false);
				m_tPos.x = tHitPoint.x;
				m_bOnRope = true;
			}
		}
		else if (pDest->GetTag().find("Ladder") != string::npos)
		{
			if (KEYPRESS("Up") || KEYDOWN("Up") || KEYPRESS("Down") || KEYDOWN("Down"))
			{
				ClearGravity();
				JumpEnd();
				SetPhysics(false);
				m_tPos.x = tHitPoint.x;
				m_bOnLadder = true;
			}
		}
		else
		{
			// 벽이라면
			if (pDest->GetTag().find("Wall") != string::npos &&
				pDest->GetTag().find("Monster") == string::npos) // 몬스터 전용 벽이 아니어야 함
			{
				if (m_tPos.x > tHitPoint.x) // 왼쪽 벽에 부딪힘
				{
					if (m_tSize.x * m_tPivot.x - (m_tPos.x - tHitPoint.x) < 3.f)
						m_tPos.x = tHitPoint.x + m_tSize.x * (1 - m_tPivot.x);
				}
				else // 오른쪽 벽
				{
					if (m_tSize.x * (1 - m_tPivot.x) - (tHitPoint.x - m_tPos.x) < 3.f)
						m_tPos.x = tHitPoint.x - m_tSize.x * m_tPivot.x;
				}
			}
		}
	}
	if (pDest->GetObj()->GetObjType() == OT_ITEM)
	{
		if (KEYPRESS("z"))
		{
			// 서버에 getitem 패킷 전송
			CItem* pItem = (CItem*)pDest->GetObj();
			POSITION tCoord = FindItemCoord(pItem->GetItemType());
			GET_SINGLE(CServerManager)->SendGetItem(pItem->GetTag(), pItem->GetItemType(), pItem->GetPos(), tCoord);
		}
	}
}

void CPlayer::FootLeave(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	if (pDest->GetObj()->GetObjType() == OT_MAP)
	{
		if ((pDest->GetTag().find("Rope") != string::npos || pDest->GetTag().find("Ladder") != string::npos) &&
			(m_bOnRope || m_bOnLadder))
		{
			m_bOnRope = false;
			m_bOnLadder = false;
			SetPhysics(true);
		}
		else
		{
			// 바닥에서 떠날때만 true
			if (pDest->GetTag().find("Floor") != string::npos)
				SetPhysics(true);
		}
	}
}
