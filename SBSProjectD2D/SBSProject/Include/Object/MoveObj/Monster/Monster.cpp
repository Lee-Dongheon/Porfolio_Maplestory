#include "Monster.h"
#include "../Player/Player.h"
#include "../../../Core/Core.h"
#include "../../../Core/Camera.h"
#include "../../../Collider/ColliderRect.h"
#include "../../StaticObj/Stage/Stage.h"
#include "../../../Scene/Scene.h"
#include "../../../Scene/Layer.h"
#include "../../StaticObj/Effect/EffectObj.h"
#include "../../../DamageManager/DamageManager.h"
#include "../../../Core/ServerManager.h"

CMonster::CMonster():
	m_fHitTime(0.f),
	m_fHitLimitTime(0.5f),
	m_fRandomMoveTime(0.f),
	m_fRandomMoveLimitTime(0.2f)
{
}

CMonster::CMonster(const CMonster & monster):
	CMoveObj(monster)
{
	m_fHitTime = 0;
	m_fHitLimitTime = monster.m_fHitLimitTime;
	m_fRandomMoveTime = 0;
	m_fRandomMoveLimitTime = monster.m_fRandomMoveLimitTime;

	m_bIsHit = false;

	m_HitEffectObjList.clear();
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
	Safe_Release_VecList(m_ItemList);
	Safe_Release_VecList(m_HitEffectObjList);
}

void CMonster::GetDamage(int iDamage, bool bIsCri, int i)
{
	// 서버에 데미지 전달
	GET_SINGLE(CServerManager)->SendDamage(m_pScene->GetNum(), m_tOriginPos, iDamage);
	m_iHP -= iDamage;
	m_bIsHit = true;
	// 데미지스킨은 클라이언트 연산
	GET_SINGLE(CDamageManager)->CreateDamage(iDamage, m_tPos, bIsCri, i);
}

bool CMonster::Init()
{
	SetPivot(0.5f, 1.0f);
	SetObjType(OT_MONSTER);
	SetPhysics(false);

	return true;
}

int CMonster::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);

	SetCriticEnter();

	if (m_pAnimation)
	{
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
			default:
				break;
			}
		}
		/*bool bChange = m_pAnimation->Update(fDeltaTime);
		if (bChange)
		{
			PANIMATIONCLIP pClip = m_pAnimation->GetCurrentClip();
			SetTexture(pClip->vecTexture[pClip->iFrame]);
		}*/
	}

	SetCriticLeave();

	list<CEffectObj*>::iterator iter;
	list<CEffectObj*>::iterator iterEnd = m_HitEffectObjList.end();

	for (iter = m_HitEffectObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			(*iter)->AddDelay(fDeltaTime);
			iter++;
			continue;
		}

		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_HitEffectObjList.erase(iter);
			iterEnd = m_HitEffectObjList.end();
		}
		else
			iter++;
	}

	return 0;
}

int CMonster::LateUpdate(float fDeltaTime)
{
	CMoveObj::LateUpdate(fDeltaTime);

	return 0;
}

void CMonster::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);
}

void CMonster::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CMoveObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	// 오브젝트 하단에 이름 표시
	HDC hDC;
	pGDIRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
	SetBkColor(hDC, RGB(128, 128, 128));
	//SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));

	POSITION tPos = m_tPos - GET_SINGLE(CCamera)->GetPos();
	TextOutA(hDC, tPos.x - m_tSize.x / 2, tPos.y, m_strTag.c_str(), m_strTag.length());

	pGDIRenderTarget->ReleaseDC(NULL);

	list<CEffectObj*>::iterator iter;
	list<CEffectObj*>::iterator iterEnd = m_HitEffectObjList.end();

	for (iter = m_HitEffectObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			(*iter)->AddDelay(fDeltaTime);
			iter++;
			continue;
		}

		(*iter)->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_HitEffectObjList.erase(iter);
			iterEnd = m_HitEffectObjList.end();
		}
		else
			iter++;
	}
}

int CMonster::Update(PSC_ObjectPacket pPacket)
{
	CMoveObj::Update(pPacket);

	if (!pPacket->enable)
		Safe_Release_VecList(m_HitEffectObjList);

	return 0;
}
