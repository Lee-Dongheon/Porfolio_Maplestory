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

	CObj* m_pAggressiveObj; // ���� ���
	float m_fAggressiveTime;
	float m_fAggressiveMoveTime; // ���� ���� �ð�
	float m_fAggressiveLimitTime;

	bool m_bIsNoJump;
	POSITION m_tOriginPos;

	list<class CItem*> m_ItemList; // ���Ͱ� ������ �ִ� ������ ����Ʈ

	MONSTER_TYPE m_eMonsterType; // �Ϲ�, �������� ����

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

