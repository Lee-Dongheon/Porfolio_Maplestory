#pragma once
#include "Skill.h"
#include "../../../../Core/Animation.h"
#include "../../Effect/EffectObj.h"

class CActiveSkill :
	public CSkill
{
public:
	CActiveSkill();
	CActiveSkill(const CActiveSkill& active);
	virtual ~CActiveSkill();

protected:
	bool m_bUsed; // 스킬 사용 플래그
	POSITION m_tLT; // 스킬 lt 적용 범위(left기준)
	POSITION m_tRB; // 스킬 rb 적용 범위(left기준)
	int m_iDamage; // 스킬 데미지
	int m_iCost; // 스킬 코스트(마나 고정)
	float m_fDelay; // 스킬 딜레이

	vector<CEffectObj*> m_vecHitEffectObj;

	list<CObj*> m_TargetList; // 스킬 효과를 받을 오브젝트

public:
	void SetLT(float l, float t) { m_tLT.x = l; m_tLT.y = t; }
	void SetLT(POSITION& tLT) { m_tLT = tLT; }
	void SetRB(float r, float b) { m_tRB.x = r; m_tRB.y = b; }
	void SetRB(POSITION& tRB) { m_tRB = tRB; }

	void SetDamage(int iDamage) { m_iDamage = iDamage; }
	void SetCost(int iCost) { m_iCost = iCost; }
	void SetUsed(bool bUsed) { m_bUsed = bUsed; }

public:
	int GetCost() const { return m_iCost; }
	int GetDamage() const { return m_iDamage; }
	float GetDelay() const { return m_fDelay; }
	bool GetUsed() const { return m_bUsed; }

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CActiveSkill* Clone() = 0;

public:
	virtual void ActiveSkill(CObj* pObj) = 0;
	virtual void UseSkill();
};

