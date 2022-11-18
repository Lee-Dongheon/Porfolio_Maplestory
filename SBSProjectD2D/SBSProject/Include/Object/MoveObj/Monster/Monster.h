#pragma once
#include "../MoveObj.h"
#include "../../StaticObj/Icon/Item/Item.h"

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
	float m_fRandomMoveTime;
	float m_fRandomMoveLimitTime;

	POSITION m_tOriginPos;

	list<class CItem*> m_ItemList; // 몬스터가 가지고 있는 아이템 리스트

	MONSTER_TYPE m_eMonsterType; // 일반, 보스몬스터 구분
	list<class CEffectObj*> m_HitEffectObjList; // 피격이펙트, 두개 이상 있을 수 있어서 리스트로 구현

public:
	void SetMonsterType(MONSTER_TYPE eMonsterType) { m_eMonsterType = eMonsterType; }
	void SetOriginPos(POSITION tOriginPos) { m_tOriginPos = tOriginPos; }
	void SetOriginPos(float x, float y) { m_tOriginPos.x = x; m_tOriginPos.y = y; }
	void AddHitEffect(CEffectObj* pHitEffect) { m_HitEffectObjList.push_back(pHitEffect); }

public:
	MONSTER_TYPE GetMonsterType() const { return m_eMonsterType; }
	POSITION GetOriginPos() const { return m_tOriginPos; }

public:
	void GetDamage(int iDamage, bool bIsCri, int i);

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual int Update(PSC_ObjectPacket pPacket);
	virtual CMonster* Clone() = 0;;

public:
	template<typename T>
	T* CreateItem(const string& strTag, ITEM_TYPE eType = IT_ETC)
	{
		T* pItem = new T;

		pItem->SetObj(this);
		pItem->SetTag(strTag);
		pItem->SetItemType(eType);

		if (!pItem->Init())
		{
			SAFE_RELEASE(pItem);
			return NULL;
		}

		m_ItemList.push_back(pItem);
		pItem->AddRef();

		return pItem;
	}
};

