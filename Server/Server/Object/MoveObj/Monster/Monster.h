#pragma once
#include "../MoveObj.h"

class CCollider;

class CMonster :
	public CMoveObj
{
protected:
	friend class CObj;
	friend class CScene;
	friend class CStage;

protected:
	CMonster();
	CMonster(const CMonster& monster);
	virtual ~CMonster();

protected:
	float m_fHitTime;
	float m_fHitLimitTime;
	float m_fJumpTime;
	float m_fJumpLimitTime;
	float m_fRandomMoveTime;
	float m_fRandomMoveLimitTime;

	CObj* m_pAggressiveObj; // 공격 대상
	float m_fAggressiveTime;
	float m_fAggressiveMoveTime; // 추적 갱신 시간
	float m_fAggressiveLimitTime;

	bool m_bIsNoJump;
	POSITION m_tOriginPos;

	list<class CItem*> m_ItemList; // 몬스터가 가지고 있는 아이템 리스트

	MONSTER_TYPE m_eMonsterType; // 일반, 보스몬스터 구분

public:
	void SetMonsterType(MONSTER_TYPE eMonsterType) { m_eMonsterType = eMonsterType; }
	void SetAggressiveObj(CObj* pObj);
	void SetOriginPos(POSITION tOriginPos) { m_tOriginPos = tOriginPos; }
	void SetOriginPos(float x, float y) { m_tOriginPos.x = x; m_tOriginPos.y = y; }

public:
	MONSTER_TYPE GetMonsterType() const { return m_eMonsterType; }
	CObj* GetAggressiveObj() const { return m_pAggressiveObj; }
	POSITION GetOriginPos() const { return m_tOriginPos; }

public:
	bool GetDamage(int iDamage);

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CMonster* Clone() = 0;

public:
	void FootEnter(CCollider* pSrc, CCollider* pDest, float fDeltaTime);
	void FootStay(CCollider* pSrc, CCollider* pDest, float fDeltaTime);
	void FootLeave(CCollider* pSrc, CCollider* pDest, float fDeltaTime);
};

