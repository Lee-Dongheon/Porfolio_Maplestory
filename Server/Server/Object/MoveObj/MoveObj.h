#pragma once
#include "../Obj.h"

class CMoveObj :
	public CObj
{
protected:
	CMoveObj();
	CMoveObj(const CMoveObj& obj);
	virtual ~CMoveObj();

protected:
	float m_fSpeed;
	float m_fSpeedY;
	float m_fAngle; // 우측을 기준으로 반시계 +, 시계-
					// 위 방향 = default = 270
					// 텍스쳐 렌더링에 사용할 각도는 윗쪽 기준 시계 + 반시계 - 이므로
					// 텍스쳐 내부에서 수정해줄것.

	bool m_bIsDead;
	bool m_bMove;
	bool m_bIsAttack;
	bool m_bJumping;
	bool m_bFalling;
	bool m_bKnockBack;
	float m_fForce;
	float m_fForceOrigin;
	float m_fKnockBackForce;
	float m_fKnockBackForceOrigin;
	float m_fKnockBackTime;
	float m_fHitTime;

	float m_fStateTime;
	string m_strEnterFloor;

	float m_fMPRecoverTime;
	float m_fMPRecoverLimitTime;

	int m_iMaxHP;
	int m_iHP;
	int m_iMaxMP;
	int m_iMP;
	int m_iExp;
	int m_iLevel;

	bool m_bIsHit;

public:
	void SetForce(float fForce) { m_fForceOrigin = fForce; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetAngle(float fAngle) { m_fAngle = fAngle; }
	void SetSpeedY(float fSpeedY) { m_fSpeedY = fSpeedY; }
	void SetStateTime(float fStateTime) { m_fStateTime = fStateTime; }
	void SetKnockBackForce(float fKnockBackForce) { m_fKnockBackForceOrigin = fKnockBackForce; }

	void SetMaxHP(int iMaxHP) { m_iMaxHP = iMaxHP; }
	void SetHP(int iHP) { m_iHP = iHP; if (m_iHP > m_iMaxHP) { m_iHP = m_iMaxHP; } }
	void SetMaxMP(int iMaxMP) { m_iMaxMP = iMaxMP; }
	void SetMP(int iMP) { m_iMP = iMP; if (m_iMP > m_iMaxMP) { m_iMP = m_iMaxMP; } }
	void SetLevel(int iLevel) { m_iLevel = iLevel; }
	void SetExp(int iExp) { m_iExp = iExp; }

	void SetHit(bool bIsHit) { m_bIsHit = bIsHit; }
	void SetDead(bool bIsDead) { m_bIsDead = bIsDead; }

public:
	float GetSpeed() const { return m_fSpeed; }
	float GetAngle() const { return m_fAngle; }
	float GetStateTime() const { return m_fStateTime; }

	int GetMaxHP() const { return m_iMaxHP; }
	int GetHP() const { return m_iHP; }
	int GetMaxMP() const { return m_iMaxMP; }
	int GetMP() const { return m_iMP; }
	int GetLevel() const { return m_iLevel; }
	int GetExp() const { return m_iExp; }

	bool GetHit() const { return m_bIsHit; }
	bool GetDead() const { return m_bIsDead; }

public:
	void MoveX(float fDeltaTime, MOVE_DIR eDir);
	void MoveY(float fDeltaTime, MOVE_DIR eDir);
	void MoveAngle(float fDeltaTime);
	void Jump();
	void JumpEnd();
	void KnockBack();
	void KnockBackEnd();

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CMoveObj* Clone() = 0;
};

