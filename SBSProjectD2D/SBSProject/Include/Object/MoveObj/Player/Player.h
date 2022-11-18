#pragma once
#include "../MoveObj.h"
#include "../../../Scene/Layer.h"
#include "../../StaticObj/Icon/Skill/Skill.h"
#include "../../StaticObj/Icon/Item/Item.h"

class CPlayer :
	public CMoveObj
{
private:
	friend class CObj;
	friend class CScene;
	friend class CSceneManager;

	friend class CUI;
	friend class CUIMain;
	friend class CUIStat;
	friend class CUISkill;
	friend class CUIKey;
	friend class CUIItem;
	friend class CUIShop;
	friend class CUIEquip;

private:
	CPlayer();
	CPlayer(const CPlayer& player);
	~CPlayer();

private:
	bool m_bDoubleJump;
	bool m_bSceneChange;
	bool m_bOnRope;
	bool m_bOnLadder;
	bool m_bInertia;
	bool m_bProne;
	bool m_bProneAttack;
	bool m_bJumpBelow;
	string m_strLastLeaveFloor;

	class CEffectObj* m_pLevelUpEffectObj;

	int* m_pMaxExp;

	int m_iMeso;

	float m_fSkillDelay;
	float m_fSkillTime;

	float m_fHitTime;
	bool m_bInvincible; // 무적 플래그
	RECTANGLE m_tAttackRect;
	MOVE_DIR m_eJumpDir;

	// item_type 만큼의 item map
	// map의 key값은 ui에서 위치할 좌표
	map<POSITION, CItem*> m_mapItem[IT_END];
	vector<CSkill*> m_vecSkill[5];
	map<EQUIP_TYPE, CItem*> m_mapEquip; // 장착한 위치, 장착한 아이템 맵

	bool m_bChatMode; // 채팅 모드

public:
	void SetAttack(bool bIsAttack)			{ m_bIsAttack = bIsAttack; }
	void SetAttackRect(RECTANGLE tAttackRect) { m_tAttackRect = tAttackRect; }
	void SetAttackRect(float l, float t, float r, float b);
	void SetSceneChange(bool bSceneChange) { m_bSceneChange = bSceneChange; }
	void SetOnRope(bool bOnRope) { m_bOnRope = bOnRope; }
	void SetSkillDelay(float fSkillDelay) { m_fSkillDelay = fSkillDelay; }
	void SetAlertMotion();

public:
	bool GetAttack() const		{ return m_bIsAttack; }
	bool GetSceneChange() const { return m_bSceneChange; }
	int GetMaxExp();
	float GetSkillDelay() const { return m_fSkillDelay; }

public:
	bool AddItem(CItem* pItem, POSITION tPos = POSITION(-1.f, -1.f)); // 이미 있는 아이템 true, 새 아이템 false 리턴
	void ChangeItemPos(POSITION tUIPos, POSITION tPos, CItem* pItem);
	void EraseItem(CItem* pItem);

public:
	void AddEquip(CItem* pItem);
	void EraseEquip(CItem* pItem);

public:
	CItem* FindItem(const string& strTag);
	CItem* FindItem(POSITION tPos, ITEM_TYPE eType);
	POSITION FindItemCoord(ITEM_TYPE eType);
	CSkill* FindSkill(int iStepLevel, const string& strTag);
	CItem* FindEquip(EQUIP_TYPE eType);

public:
	void ExpInit();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual int Update(PSC_ObjectPacket pPacket);
	virtual CPlayer* Clone();

public:
	virtual void Attack();
	void JumpBelow();

public:
	void DoubleJump(int level);
	void LevelUp();

public:
	void FootEnter(CCollider* pSrc, CCollider* pDest, float fDeltaTime);
	void FootStay(CCollider* pSrc, CCollider* pDest, float fDeltaTime);
	void FootLeave(CCollider* pSrc, CCollider* pDest, float fDeltaTime);

	template<typename T>
	T* AddSkill(const string& strTag)
	{
		T* pSkill = new T;

		pSkill->SetTag(strTag);

		if (!pSkill->Init())
		{
			SAFE_RELEASE(pSkill);
			return NULL;
		}

		pSkill->SetPlayer(this);
		pSkill->AddRef();
		m_vecSkill[pSkill->GetStepLevel()].push_back(pSkill);

		return pSkill;
	}

public:
	template<typename T>
	T* CreateItem(const string& strTag, ITEM_TYPE eType = IT_ETC, POSITION tPos = POSITION(-1.f, -1.f))
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
		AddObj(pItem);
		// 요 안에서 addref 해서 결국 ref가 2가 되었음.
		AddItem(pItem, tPos);

		return pItem;
	}

public:
	template<typename T>
	T* CreateEquip(const string& strTag, EQUIP_TYPE eType)
	{
		T* pItem = new T;

		pItem->SetObj(this);
		pItem->SetTag(strTag);
		pItem->SetItemType(IT_EQUIP);
		pItem->SetEquipType(eType);

		if (!pItem->Init())
		{
			SAFE_RELEASE(pItem);
			return NULL;
		}
		AddObj(pItem);

		m_mapEquip.insert(make_pair(pItem->GetEquipType(), pItem));
		pItem->AddRef();

		return pItem;
	}
};