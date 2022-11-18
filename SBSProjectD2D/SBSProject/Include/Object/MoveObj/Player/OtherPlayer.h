#pragma once
#include "../MoveObj.h"
#include "../../../Scene/Layer.h"
#include "../../StaticObj/Icon/Skill/Skill.h"
#include "../../StaticObj/Icon/Item/Item.h"

class COtherPlayer :
	public CMoveObj
{
private:
	friend class CObj;
	friend class CScene;

private:
	COtherPlayer();
	COtherPlayer(const COtherPlayer& player);
	~COtherPlayer();

private:
	bool m_bDoubleJump;
	bool m_bSceneChange;
	bool m_bOnRope;
	bool m_bOnLadder;
	bool m_bInertia;
	bool m_bJumpBelow;

	class CEffectObj* m_pLevelUpEffectObj;

	float m_fSkillDelay;
	float m_fSkillTime;

	float m_fHitTime;
	bool m_bInvincible; // 무적 플래그
	RECTANGLE m_tAttackRect;
	MOVE_DIR m_eJumpDir;

	bool m_bStatusChange;
	wchar_t* m_strName;
	wchar_t* m_strJob;

	// item_type 만큼의 item map
	// map의 key값은 ui에서 위치할 좌표
	vector<CSkill*> m_vecSkill[5];
	map<EQUIP_TYPE, CItem*> m_mapEquip;

public:
	void SetAttack(bool bIsAttack) { m_bIsAttack = bIsAttack; }
	void SetSceneChange(bool bSceneChange) { m_bSceneChange = bSceneChange; }
	void SetOnRope(bool bOnRope) { m_bOnRope = bOnRope; }

public:
	void ActivateSkill(int iSkillStep, string strSkillName);

public:
	void AddEquip(const string& strTag,	EQUIP_TYPE eType);
	void EraseEquip(EQUIP_TYPE eType);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);

	virtual COtherPlayer* Clone();

public:
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
};