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
	POSITION m_tLT; // 탐지범위 LT
	POSITION m_tRB; // 탐지범위 RB

	list<CObj*> m_TargetList; // 범위 안에 있는 objlist
	class CEffectObj* m_pEffectObj;
	class CShard* m_pShard; // 스킬
	CObj* m_pTarget; // 목표로하는 obj
	
	float m_fExistTime;
	float m_fExistLimitTime; // 타겟이 없을때 최대 존재할 수 있는 시간

	float m_fDelayTime;
	float m_fDelayLimitTime; // 생성 이후 추적까지 delay 될 시간, 초기값 -1

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

