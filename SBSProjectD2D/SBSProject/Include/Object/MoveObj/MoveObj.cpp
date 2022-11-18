#include "MoveObj.h"
#include "../../Scene/Scene.h"
#include "../../Collider/ColliderRect.h"
#include "../../Core/Camera.h"
#include "../../ItemManager/ItemManager.h"

CMoveObj::CMoveObj() :
	m_fSpeed(100.f),
	m_fSpeedY(50.f),
	m_fAngle(270.f),
	m_bMove(false),
	m_bIsAttack(false),
	m_bJumping(false),
	m_bFalling(false),
	m_bKnockBack(false),
	m_bIsHit(false),
	m_fForceOrigin(0.f),
	m_fForce(0.f),
	m_iMaxHP(1),
	m_iMaxMP(1),
	m_iHP(1),
	m_iMP(1),
	m_iExp(0),
	m_iLevel(1),
	m_fKnockBackForce(0.f),
	m_fKnockBackForceOrigin(0.f),
	m_fKnockBackTime(0.f),
	m_fStateTime(0.f),
	m_strEnterFloor(""),
	m_fMPRecoverTime(0.f),
	m_fMPRecoverLimitTime(0.f),
	m_bIsDead(false)
{
}

CMoveObj::CMoveObj(const CMoveObj& obj) :
	CObj(obj)
{
	m_fSpeed = obj.m_fSpeed;

	m_fForceOrigin = obj.m_fForceOrigin;
	m_fForce = 0.f;
	m_fKnockBackForceOrigin = obj.m_fKnockBackForceOrigin;
	m_fKnockBackForce = 0.f;
	m_fKnockBackTime = 0.f;

	m_fMPRecoverTime = obj.m_fMPRecoverTime;
	m_fMPRecoverLimitTime = obj.m_fMPRecoverLimitTime;

	m_fStateTime = 0.f;

	m_bIsDead = false;
	m_bMove = false;
	m_bJumping = false;
	m_bKnockBack = false;

	m_iMaxHP = obj.m_iMaxHP;
	m_iMaxMP = obj.m_iMaxMP;
	m_iHP = obj.m_iHP;
	m_iMP = obj.m_iMP;
	m_iExp = obj.m_iExp;
	m_iLevel = obj.m_iLevel;
}

CMoveObj::~CMoveObj()
{
}

void CMoveObj::MoveX(float fDeltaTime, MOVE_DIR eDir)
{
	m_tPos.x += m_fSpeed * fDeltaTime * eDir;

	m_bMove = true;
}

void CMoveObj::MoveY(float fDeltaTime, MOVE_DIR eDir)
{
	m_tPos.y += m_fSpeedY * fDeltaTime * eDir;

	m_bMove = true;
}

void CMoveObj::MoveAngle(float fDeltaTime)
{
	// 각도가 위를 기준으로 시계 +, 반시계 - 이므로
	// 삼각함수 잘 생각하기

	m_tPos.x += m_fSpeed * fDeltaTime * cos(m_fAngle * PI / 180.f);
	m_tPos.y += m_fSpeed * fDeltaTime * sin(m_fAngle * PI / 180.f);

	m_bMove = true;
}

void CMoveObj::Jump()
{
	if (!m_bJumping)
	{
		m_bJumping = true;
		m_fForce = m_fForceOrigin;
		SetPhysics(true);
	}
}

void CMoveObj::JumpEnd()
{
	m_bJumping = false;
	m_bFalling = false;
	m_fForce = 0.f;
}

void CMoveObj::KnockBack()
{
	if (!m_bKnockBack)
	{
		m_bKnockBack = true;
		m_fKnockBackForce = m_fKnockBackForceOrigin;
	}
}

void CMoveObj::KnockBackEnd()
{
	m_bKnockBack = false;
	m_fKnockBackForce = 0.f;
	m_fKnockBackTime = 0.f;
}

void CMoveObj::Input(float fDeltaTime)
{
	CObj::Input(fDeltaTime);

	//input 처리는 usable 아이템만 받을 것

	/*map<POSITION, CItem*>::iterator iter;
	map<POSITION, CItem*>::iterator iterEnd = m_mapItem[IT_USABLE].end();

	for (iter = m_mapItem[IT_USABLE].begin(); iter != iterEnd;)
	{
		if (!iter->second->GetEnable())
		{
			iter++;
			continue;
		}

		iter->second->Input(fDeltaTime);

		if (!iter->second->GetLife())
		{
			SAFE_RELEASE(iter->second);
			iter = m_mapItem[IT_USABLE].erase(iter);
			iterEnd = m_mapItem[IT_USABLE].end();
		}
		else
			iter++;
	}*/
}

int CMoveObj::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);

	/*if (m_fMPRecoverLimitTime > 0.f)
	{
		m_fMPRecoverTime += fDeltaTime;
		if (m_fMPRecoverTime >= m_fMPRecoverLimitTime)
		{
			m_fMPRecoverTime -= m_fMPRecoverLimitTime;
			AddMP(50);
		}
	}*/

	if (m_bIsPhysics)
	{
		m_fGravityTime += fDeltaTime;

		m_fForce -= (GRAVITY * m_fGravityTime * m_fGravityTime);
		if (m_fForce < FORCE_MIN)
			m_fForce = FORCE_MIN;
		if (m_fForce < 0)
			m_bFalling = true;
	}
	m_tPos.y -= m_fForce * fDeltaTime;

	if (m_bKnockBack)
	{
		m_fKnockBackTime += fDeltaTime;

		m_fKnockBackForce -= (FRICTION * m_fKnockBackTime * m_fKnockBackTime);
		if (m_fKnockBackForce <= 0)
			KnockBackEnd();
		m_tPos.x += m_eDir * m_fKnockBackForce * fDeltaTime;
	}

	if (m_pAnimation)
	{
		if (m_pAnimation->GetMotionEnd() && m_bIsAttack)
			m_bIsAttack = false;
	}

	return 0;
}

int CMoveObj::LateUpdate(float fDeltaTime)
{
	CObj::LateUpdate(fDeltaTime);

	//RESOLUTION tRS = m_pScene->GetSceneRS();
	//// 현재 위치한 scene 크기 밖으로 안나가게 위치보정
	//// y값 아래쪽으로는 제대로 코딩했다는 가정하에 나갈 일이 없으므로 
	//// 좌우/상단만
	//
	//if (m_tPos.x - m_tSize.x * m_tPivot.x < 0.f)
	//	m_tPos.x = m_tSize.x + m_tPivot.x;
	//else if (m_tPos.x + m_tSize.x * (1 - m_tPivot.x) > (float)tRS.iW)
	//	m_tPos.x = (float)tRS.iW - m_tSize.x * (1 - m_tPivot.x);

	//if (m_tPos.y - m_tSize.y *m_tPivot.y < 0.f)
	//	m_tPos.y = m_tSize.y *m_tPivot.y;
	//if (m_tPos.y > m_pScene->GetYPosMax())
	//{
	//	m_tPos.y = m_pScene->GetYPosMax();
	//	JumpEnd();
	//	ClearGravity();
	//	SetPhysics(false);
	//}

	//if (m_fAngle > 360.f)
	//	m_fAngle -= 360.f;
	//if (m_fAngle < 0.f)
	//	m_fAngle += 360.f;

	return 0;
}

void CMoveObj::Collision(float fDeltaTime)
{
	CObj::Collision(fDeltaTime);
}

void CMoveObj::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

int CMoveObj::Update(PSC_ObjectPacket pPacket)
{
	CObj::Update(pPacket);

	SetHP(pPacket->hp);
	SetMP(pPacket->mp);

	return 0;
}
