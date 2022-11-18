#include "ActiveSkill.h"
#include "../../../../Resources/ResourcesManager.h"
#include "../../../../Core/Camera.h"
#include "../../../../Math.h"
#include "../../../Obj.h"
#include "../../../MoveObj/Player/Player.h"
#include "../../../../Scene/Scene.h"
#include "../../../../Core/ServerManager.h"
#include "../../../../Scene/SceneManager.h"

CActiveSkill::CActiveSkill() :
	m_bUsed(false),
	m_iDamage(0)
{
	SetEnable(false);
}

CActiveSkill::CActiveSkill(const CActiveSkill & active) :
	CSkill(active)
{
	m_bUsed = false;
	m_tPos = POSITION(0.f, 0.f);
	m_iDamage = active.m_iDamage;
	m_iCost = active.m_iDamage;
	m_fDelay = active.m_fDelay;
	m_tLT = active.m_tLT;
	m_tRB = active.m_tRB;

	m_vecHitEffectObj.clear();
	for (int i = 0; i < (int)active.m_vecHitEffectObj.size(); i++)
	{
		m_vecHitEffectObj.push_back(active.m_vecHitEffectObj[i]->Clone());
	}

	m_TargetList.clear();
}

CActiveSkill::~CActiveSkill()
{
	Safe_Release_VecList(m_vecHitEffectObj);
}

void CActiveSkill::Input(float fDeltaTime)
{
	CSkill::Input(fDeltaTime);
}

int CActiveSkill::Update(float fDeltaTime)
{
	CSkill::Update(fDeltaTime);

	// ������ ���� ������Ʈ ��Ͽ� �ֱ�
	if (m_eType == ST_ATTACK && m_bUsed)
	{
		if (m_pPlayer)
		{
			m_tPos = m_pPlayer->GetPos();
			m_eDir = m_pPlayer->GetDir();
			// otherplayer�� player ����
			if (GET_SINGLE(CSceneManager)->IsPlayer(m_pPlayer))
				((CPlayer*)m_pPlayer)->SetSkillDelay(m_fDelay);
		}

		RECTANGLE tWorldInfo;
		// left�� -1�̰� lt�� -������ �Է��ϹǷ� ���ش�.
		tWorldInfo.l = m_tPos.x - m_tLT.x * (int)m_eDir;
		tWorldInfo.t = m_tPos.y + m_tLT.y;
		tWorldInfo.r = m_tPos.x - m_tRB.x * (int)m_eDir; // ���⵵
		tWorldInfo.b = m_tPos.y + m_tRB.y;

		// ���� m_ePlayerDir�� MD_RIGHT��� tWorldInfo l, r���� �ݴ��̹Ƿ�
		// üũ �� �ٲ��ֱ�
		if (tWorldInfo.l > tWorldInfo.r)
		{
			tWorldInfo.l += tWorldInfo.r;
			tWorldInfo.r = tWorldInfo.l - tWorldInfo.r;
			tWorldInfo.l -= tWorldInfo.r;
		}
		// ��� ������ ���� �ʱ�ȭ
		m_TargetList.clear();
		// ���� �����Ҷ� scene �����͸��� scene�� ������ȣ�� �ϴ°� ������?
		// ��� �� �𸥴�.
		if (m_pPlayer)
		{
			CScene* pScene = m_pPlayer->GetScene();
			if (!pScene)
				return 0;
			pScene->GetObjList(OT_MONSTER, m_TargetList, tWorldInfo);
		}
	}

	return 0;
}

int CActiveSkill::LateUpdate(float fDeltaTime)
{
	CSkill::LateUpdate(fDeltaTime);

	return 0;
}

void CActiveSkill::Collision(float fDeltaTime)
{
	CSkill::Collision(fDeltaTime);

	if (m_eType == ST_ATTACK && m_bUsed)
	{
		list<CObj*>::iterator iter;
		list<CObj*>::iterator iterEnd = m_TargetList.end();

		for (iter = m_TargetList.begin(); iter != iterEnd; iter++)
		{
			ActiveSkill((*iter));
		}
	}

	if (m_bUsed)
		m_bUsed = false;
}

void CActiveSkill::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CSkill::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

void CActiveSkill::UseSkill()
{
	if (m_pPlayer && !GetEnable())
	{
		SetEnable(true);
		// ������ ��ų ��� �˸���
		GET_SINGLE(CServerManager)->SendUseSkill(m_pPlayer->GetScene()->GetNum(), m_iStepLevel, m_pPlayer->GetTag(), m_strTag);
		m_bUsed = true;

		if (m_pPlayer && m_pPlayer->GetObjType() == OT_PLAYER)
		{
			m_pPlayer->m_pAnimation->ChangeClip("Attack");
			m_pPlayer->SetObjState(OBJ_STATE_ATTACK);
			((CPlayer*)m_pPlayer)->SetAttack(true);
		}
	}
}
