#include "OtherPlayer.h"

//#include "../../StaticObj/Icon/Skill/Skill.h"
#include "../../StaticObj/Icon/Skill/Divide/Divide.h"
#include "../../StaticObj/Icon/Skill/Expert/Expert.h"
#include "../../StaticObj/Icon/Skill/Shard/Shard.h"
#include "../../StaticObj/Icon/Skill/Bolt/Bolt.h"
#include "../../StaticObj/Icon/Skill/Plain/Plain.h"

COtherPlayer::COtherPlayer()
{
}

COtherPlayer::COtherPlayer(const COtherPlayer & player):
	CMoveObj(player)
{
}

COtherPlayer::~COtherPlayer()
{
	for (int i = 0; i < 5; i++)
	{
		Safe_Release_VecList(m_vecSkill[i]);
	}

	Safe_Release_Map(m_mapEquip);
}

void COtherPlayer::ActivateSkill(int iSkillStep, string strSkillName)
{
	for (int i = 0; i < m_vecSkill[iSkillStep].size(); i++)
	{
		if (m_vecSkill[iSkillStep][i]->GetTag() == strSkillName)
		{
			m_vecSkill[iSkillStep][i]->SetEnable(true);
			((CActiveSkill*)m_vecSkill[iSkillStep][i])->SetUsed(true);
			return;
		}
	}
}

void COtherPlayer::AddEquip(const string & strTag, EQUIP_TYPE eType)
{
	CItem* pItem = new CItem;
	
	pItem->SetObj(this);
	pItem->SetTag(strTag);
	pItem->SetItemType(IT_EQUIP);
	pItem->SetEquipType(eType);

	if (!pItem->Init())
	{
		SAFE_RELEASE(pItem);
		return;
	}
	
	pItem->ItemInit();

	m_mapEquip.insert(make_pair(eType, pItem));
}

void COtherPlayer::EraseEquip(EQUIP_TYPE eType)
{
	map<EQUIP_TYPE, CItem*>::iterator iter = m_mapEquip.find(eType);

	if (iter == m_mapEquip.end())
		return;

	SAFE_RELEASE(iter->second);
	m_mapEquip.erase(iter);
}

bool COtherPlayer::Init()
{
	//SetPos(600.f, 606.f);
	SetSize(43.f, 66.f);
	SetPivot(0.5f, 1.f);
	SetObjType(OT_OTHERPLAYER);

	// 중력은 필요없다
	SetPhysics(false);

	CAnimation* pAnimation = CreateAnimation("OtherPlayerAnimation");
	SAFE_RELEASE(pAnimation);

	// 43 x 66
	POSITION arrOrigin[4] = { POSITION(21.f, 66.f), POSITION(21.f, 66.f), POSITION(21.f, 66.f), POSITION(21.f, 66.f) };
	POSITION arrLT[4] = { POSITION(-21.f, -66.f), POSITION(-21.f, -66.f), POSITION(-21.f, -66.f), POSITION(-21.f, -66.f) };
	POSITION arrRB[4] = { POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f) };
	float arrDelay[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	AddAnimationClip("Idle", AO_LOOP, 4, arrOrigin, arrLT, arrRB, arrDelay, "OtherPlayerStand", L"Player/Stand/");

	// 51 x 66
	POSITION arrOrigin2[4] = { POSITION(29.f, 66.f), POSITION(29.f, 66.f), POSITION(29.f, 66.f), POSITION(29.f, 66.f) };
	POSITION arrLT2[4] = { POSITION(-29.f, -66.f), POSITION(-29.f, -66.f), POSITION(-29.f, -66.f), POSITION(-29.f, -66.f) };
	POSITION arrRB2[4] = { POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f) };
	float arrDelay2[4] = { 0.18f, 0.18f, 0.18f, 0.18f };
	AddAnimationClip("Run", AO_LOOP, 4, arrOrigin2, arrLT2, arrRB2, arrDelay2, "OtherPlayerRun", L"Player/Run/");

	// 48 x 64
	POSITION arrOrigin3[1] = { POSITION(26.f, 64.f) };
	POSITION arrLT3[1] = { POSITION(-26.f, -64.f) };
	POSITION arrRB3[1] = { POSITION(22.f, 0.f) };
	float arrDelay3[1] = { 0.1f };
	AddAnimationClip("Jump", AO_LOOP, 1, arrOrigin3, arrLT3, arrRB3, arrDelay3, "OtherPlayerJump", L"Player/Jump/");

	// 47 x 65
	POSITION arrOrigin4[4] = { POSITION(25.f, 65.f), POSITION(25.f, 65.f), POSITION(25.f, 65.f), POSITION(25.f, 65.f) };
	POSITION arrLT4[4] = { POSITION(-25.f, -65.f), POSITION(-25.f, -65.f), POSITION(-25.f, -65.f), POSITION(-25.f, -65.f) };
	POSITION arrRB4[4] = { POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f) };
	float arrDelay4[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	AddAnimationClip("Alert", AO_TIME_RETURN, 4, arrOrigin4, arrLT4, arrRB4, arrDelay4, "OtherPlayerAlert", L"Player/Alert/");

	// 엎드리기
	// 67 x 38
	POSITION arrOrigin5[1] = { POSITION(33.f, 38.f) };
	POSITION arrLT5[1] = { POSITION(-33.f, -38.f) };
	POSITION arrRB5[1] = { POSITION(34.f, 0.f) };
	float arrDelay5[1] = { 0.1f };
	AddAnimationClip("Prone", AO_LOOP, 1, arrOrigin5, arrLT5, arrRB5, arrDelay5, "OtherPlayerProne", L"Player/Prone/");

	// 41 x 66
	POSITION arrOrigin6[1] = { POSITION(20.f, 66.f) };
	POSITION arrLT6[1] = { POSITION(-20.f, -66.f) };
	POSITION arrRB6[1] = { POSITION(21.f, 0.f) };
	float arrDelay6[1] = { 0.1f };
	AddAnimationClip("Rope1", AO_LOOP, 1, arrOrigin6, arrLT6, arrRB6, arrDelay6, "OtherPlayerRope1", L"Player/Rope/Rope1/");
	AddAnimationClip("Rope2", AO_LOOP, 1, arrOrigin6, arrLT6, arrRB6, arrDelay6, "OtherPlayerRope2", L"Player/Rope/Rope2/");

	// 44 x 65
	POSITION arrOrigin7[1] = { POSITION(22.f, 65.f) };
	POSITION arrLT7[1] = { POSITION(-22.f, -65.f) };
	POSITION arrRB7[1] = { POSITION(22.f, 0.f) };
	float arrDelay7[1] = { 0.1f };
	AddAnimationClip("Ladder1", AO_LOOP, 1, arrOrigin7, arrLT7, arrRB7, arrDelay7, "OtherPlayerLadder1", L"Player/Ladder/Ladder1/");
	AddAnimationClip("Ladder2", AO_LOOP, 1, arrOrigin7, arrLT7, arrRB7, arrDelay7, "OtherPlayerLadder2", L"Player/Ladder/Ladder2/");

	// 39 x 63
	POSITION arrOrigin8[1] = { POSITION(19.f, 63.f) };
	POSITION arrLT8[1] = { POSITION(-19.f, -63.f) };
	POSITION arrRB8[1] = { POSITION(20.f, 0.f) };
	float arrDelay8[1] = { 0.1f };
	AddAnimationClip("Die", AO_LOOP, 1, arrOrigin8, arrLT8, arrRB8, arrDelay8, "OtherPlayerDie", L"Player/Die/");

	// swing attack
	// 88 x 65
	POSITION arrOrigin9[3] = { POSITION(66.f, 65.f), POSITION(66.f, 65.f), POSITION(66.f, 65.f) };
	POSITION arrLT9[3] = { POSITION(-66.f, -65.f), POSITION(-66.f, -65.f), POSITION(-66.f, -65.f) };
	POSITION arrRB9[3] = { POSITION(22.f, 0.f), POSITION(22.f, 0.f), POSITION(22.f, 0.f) };
	float arrDelay9[3] = { 0.3f, 0.15f, 0.35f };
	AddAnimationClip("Attack", AO_ONCE_RETURN, 3, arrOrigin9, arrLT9, arrRB9, arrDelay9, "OtherPlayerAttack1", L"Player/Attack/0/");

	// ProneAttack
	// 82 x 38
	POSITION arrOrigin10[2] = { POSITION(48.f, 38.f), POSITION(48.f, 38.f) };
	POSITION arrLT10[2] = { POSITION(-48.f, -38.f), POSITION(-48.f, -38.f) };
	POSITION arrRB10[2] = { POSITION(34.f, 0.f), POSITION(34.f, 0.f) };
	float arrDelay10[2] = { 0.3f, 0.15f };
	AddAnimationClip("ProneAttack", AO_ONCE_RETURN, 2, arrOrigin10, arrLT10, arrRB10, arrDelay10, "OtherPlayerProneAttack", L"Player/ProneAttack/");

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

	return true;
}

void COtherPlayer::Input(float fDeltaTime)
{
	CMoveObj::Input(fDeltaTime);
}

int COtherPlayer::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);

	SetCriticEnter();

	if (m_pAnimation)
	{
		int tempYPos;
		// objstate는 static obj면 default, moveobj면 그 외의 값
		if (m_eObjState != OBJ_STATE_DEFAULT)
		{
			switch (m_eObjState)
			{
			case OBJ_STATE_IDLE:
				m_pAnimation->ChangeClip("Idle");
				break;
			case OBJ_STATE_RUN:
				m_pAnimation->ChangeClip("Run");
				break;
			case OBJ_STATE_JUMP:
				m_pAnimation->ChangeClip("Jump");
				break;
			case OBJ_STATE_HIT:
				m_pAnimation->ChangeClip("Hit");
				break;
			case OBJ_STATE_ATTACK:
				m_pAnimation->ChangeClip("Attack");
				break;
			case OBJ_STATE_PRONE:
				m_pAnimation->ChangeClip("Prone");
				break;
			case OBJ_STATE_ROPE:
				tempYPos = (int)(m_tPos.y);
				if ((tempYPos / 30) % 2 == 0)
					m_pAnimation->ChangeClip("Rope1");
				else
					m_pAnimation->ChangeClip("Rope2");
				break;
			case OBJ_STATE_LADDER:
				tempYPos = (int)(m_tPos.y);
				if ((tempYPos / 30) % 2 == 0)
					m_pAnimation->ChangeClip("Ladder1");
				else
					m_pAnimation->ChangeClip("Ladder2");
				break;
			default:
				break;
			}
		}
	}

	SetCriticLeave();

	map<EQUIP_TYPE, CItem*>::iterator iter;
	map<EQUIP_TYPE, CItem*>::iterator iterEnd = m_mapEquip.end();

	for (iter = m_mapEquip.begin(); iter != iterEnd; iter++)
	{
		iter->second->Update(fDeltaTime);
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

	return 0;
}

int COtherPlayer::LateUpdate(float fDeltaTime)
{
	CMoveObj::LateUpdate(fDeltaTime);

	return 0;
}

void COtherPlayer::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);
}

void COtherPlayer::Render(ID2D1GdiInteropRenderTarget * pGDIRenderTarget, ID2D1HwndRenderTarget * pRenderTarget, float fDeltaTime)
{
	CMoveObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	// 오브젝트 하단에 이름 표시
	HDC hDC;
	pGDIRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
	SetBkColor(hDC, RGB(128, 128, 128, ));
	//SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));

	POSITION tPos = m_tPos - GET_SINGLE(CCamera)->GetPos();
	TextOutA(hDC, tPos.x - m_tSize.x / 2, tPos.y, m_strTag.c_str(), m_strTag.length());

	pGDIRenderTarget->ReleaseDC(NULL);

	map<EQUIP_TYPE, CItem*>::iterator iter;
	map<EQUIP_TYPE, CItem*>::iterator iterEnd = m_mapEquip.end();

	for (iter = m_mapEquip.begin(); iter != iterEnd; iter++)
	{
		iter->second->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < (int)m_vecSkill[i].size(); j++)
		{
			if (!m_vecSkill[i][j]->GetEnable())
				continue;
			m_vecSkill[i][j]->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
		}
	}
}

COtherPlayer * COtherPlayer::Clone()
{
	return new COtherPlayer(*this);
}
