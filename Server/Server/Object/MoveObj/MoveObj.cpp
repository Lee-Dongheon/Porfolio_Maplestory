#include "MoveObj.h"
#include "../../Scene/Scene.h"
#include "../../Collider/ColliderRect.h"

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
	m_fHitTime(1.f),
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
	m_fHitTime = 1.f;

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
		SetObjState(OBJ_STATE_JUMP);
	}
}

void CMoveObj::JumpEnd()
{
	m_bJumping = false;
	m_bFalling = false;
	m_fForce = 0.f;
	SetObjState(OBJ_STATE_IDLE);
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
	SetObjState(OBJ_STATE_IDLE);
}

void CMoveObj::Input(float fDeltaTime)
{
	CObj::Input(fDeltaTime);
}

int CMoveObj::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);

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
		/*if (m_fKnockBackForce <= 0)
			KnockBackEnd();*/
		if (m_fKnockBackTime >= m_fHitTime)
			KnockBackEnd();
		m_tPos.x += m_eDir * m_fKnockBackForce * fDeltaTime;
	}

	return 0;
}

int CMoveObj::LateUpdate(float fDeltaTime)
{
	CObj::LateUpdate(fDeltaTime);

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

	return 0;
}

//void CMoveObj::Collision(float fDeltaTime)
//{
//	CObj::Collision(fDeltaTime);
//}