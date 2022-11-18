#pragma once
#include "../../../../MoveObj/MoveObj.h"

class CShardBullet :
	public CMoveObj
{
private:
	friend class CShard;

public:
	CShardBullet();
	CShardBullet(const CShardBullet& bullet);
	~CShardBullet();

private:
	POSITION m_tLT; // Ž������ LT
	POSITION m_tRB; // Ž������ RB

	list<CObj*> m_TargetList; // ���� �ȿ� �ִ� objlist
	class CEffectObj* m_pEffectObj;
	class CShard* m_pShard; // ��ų
	CObj* m_pTarget; // ��ǥ���ϴ� obj
	
	float m_fExistTime;
	float m_fExistLimitTime; // Ÿ���� ������ �ִ� ������ �� �ִ� �ð�

	float m_fDelayTime;
	float m_fDelayLimitTime; // ���� ���� �������� delay �� �ð�, �ʱⰪ -1

public:
	void SetLT(float x, float y) { m_tLT.x = x; m_tLT.y = y; }
	void SetLT(POSITION tLT) { m_tLT = tLT; }
	void SetRB(float x, float y) { m_tRB.x = x; m_tRB.y = y; }
	void SetRB(POSITION tRB) { m_tRB = tRB; }
	void SetShard(CShard* pShard);
	//void SetPlayer(CPlayer* pPlayer);
	void SetMaxDelay(float fMaxDelay) { m_fDelayLimitTime = fMaxDelay; }

public:
	void SetTarget();

public:
	float GetDelayLimitTime() const { return m_fDelayLimitTime; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CShardBullet* Clone();
};

