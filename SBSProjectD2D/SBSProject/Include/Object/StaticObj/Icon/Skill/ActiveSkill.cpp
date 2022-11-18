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

	// 영향을 받을 오브젝트 목록에 넣기
	if (m_eType == ST_ATTACK && m_bUsed)
	{
		if (m_pPlayer)
		{
			m_tPos = m_pPlayer->GetPos();
			m_eDir = m_pPlayer->GetDir();
			// otherplayer와 player 구분
			if (GET_SINGLE(CSceneManager)->IsPlayer(m_pPlayer))
				((CPlayer*)m_pPlayer)->SetSkillDelay(m_fDelay);
		}

		RECTANGLE tWorldInfo;
		// left는 -1이고 lt는 -값으로 입력하므로 빼준다.
		tWorldInfo.l = m_tPos.x - m_tLT.x * (int)m_eDir;
		tWorldInfo.t = m_tPos.y + m_tLT.y;
		tWorldInfo.r = m_tPos.x - m_tRB.x * (int)m_eDir; // 여기도
		tWorldInfo.b = m_tPos.y + m_tRB.y;

		// 만약 m_ePlayerDir이 MD_RIGHT라면 tWorldInfo l, r값이 반대이므로
		// 체크 후 바꿔주기
		if (tWorldInfo.l > tWorldInfo.r)
		{
			tWorldInfo.l += tWorldInfo.r;
			tWorldInfo.r = tWorldInfo.l - tWorldInfo.r;
			tWorldInfo.l -= tWorldInfo.r;
		}
		// 목록 갱신을 위해 초기화
		m_TargetList.clear();
		// 서버 구현할땐 scene 포인터말고 scene의 고유번호로 하는게 좋을듯?
		// 사실 잘 모른다.
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
		// 서버에 스킬 사용 알리기
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
