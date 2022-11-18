#pragma once
#include "../MoveObj.h"

class CPlayer :
	public CMoveObj
{
private:
	friend class CObj;
	friend class CDBManager;
	friend class CClientManager;

protected:
	CPlayer();
	CPlayer(const CPlayer& player);
	~CPlayer();

private:
	bool m_bDoubleJump;
	bool m_bSceneChange;
	bool m_bOnRope;
	bool m_bOnLadder;
	bool m_bInertia;
	bool m_bIsGet;
	bool m_bProne;
	bool m_bProneAttack;
	bool m_bJumpBelow;
	string m_strLastLeaveFloor;

	int* m_pMaxExp;

	int m_iMeso;

	float m_fSkillDelay;
	float m_fSkillTime;

	float m_fHitTime;
	bool m_bInvincible; // 무적 플래그
	RECTANGLE m_tAttackRect;
	MOVE_DIR m_eJumpDir;

	bool m_bStatusChange;
	// item
	list<class CItem*> m_ItemList;
	// skill
	list<class CSkill*> m_SkillList;
	list<class CItem*> m_EquipList;

public:
	void SetAttack(bool bIsAttack) { m_bIsAttack = bIsAttack; }
	void SetAttackRect(RECTANGLE tAttackRect) { m_tAttackRect = tAttackRect; }
	void SetAttackRect(float l, float t, float r, float b);
	void SetSceneChange(bool bSceneChange) { m_bSceneChange = bSceneChange; }
	void SetOnRope(bool bOnRope) { m_bOnRope = bOnRope; }
	void SetStatusChange(bool bStatusChange) { m_bStatusChange = bStatusChange; }
	void SetSkillDelay(float fSkillDelay) { m_fSkillDelay = fSkillDelay; }
	void SetMeso(int iMeso) { m_iMeso = iMeso; }

public:
	bool GetAttack() const { return m_bIsAttack; }
	bool GetSceneChange() const { return m_bSceneChange; }
	int GetMaxExp();
	bool GetStatusChange() const { return m_bStatusChange; }
	float GetSkillDelay() const { return m_fSkillDelay; }
	int GetMeso() const { return m_iMeso; }

public:
	class CItem* FindItem(const string& strTag);
	class CSkill* FindSkill(const string& strTag);

public:
	CItem* CreateItem(const string& strTag, ITEM_TYPE eItemType, POSITION tItemPos, int iNum);
	CSkill* CreateSkill(const string& strTag, int iStepLevel, int iSkillLevel);
	CItem* CreateEquip(const string& strTag, EQUIP_TYPE eEquipType);

public:
	void AddItem(const string& strTag, ITEM_TYPE eItemType, POSITION tItemPos, int iNum);

public:
	void ChangeItem(const string& strTag, ITEM_TYPE eItemType, POSITION tItemPos, int iNum, bool bErased);
	void ChangeSkill(const string& strTag, int iSkillLevel);

public:
	void EraseItem(const string& strTag);
	void EraseEquip(EQUIP_TYPE eType);

public:
	void ExpInit();
	void AddExp(int iExp);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CPlayer* Clone();

private:
	void DoubleJump(int level);
	void LevelUp();
};