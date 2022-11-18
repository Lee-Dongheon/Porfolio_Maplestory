#pragma once
#include "../Icon.h"

class CPlayer;

class CSkill :
	public CIcon
{
private:
	friend class CObj;

protected:
	CSkill();
	CSkill(const CSkill& skill);
	virtual ~CSkill();

protected:
	CObj* m_pPlayer; // 스킬을 가진 플레이어
	int m_iStepLevel; // 스킬 차수
	int m_iMaxLevel; // 마스터레벨
	int m_iLevel; // 현재레벨
	SKILL_TYPE m_eType; // 스킬 타입
	wchar_t* m_pSkillName; // 스킬 이름

public:
	void SetPlayer(CObj* pObj)				{ m_pPlayer = pObj; }
	void SetStepLevel(int iStepLevel)		{ m_iStepLevel = iStepLevel; }
	void SetMaxLevel(int iMaxLevel)			{ m_iMaxLevel = iMaxLevel; }
	void SetLevel(int iLevel)				{ m_iLevel = iLevel; }
	void SetType(SKILL_TYPE eType)			{ m_eType = eType; }
	void SetSkillName(wchar_t* pSkillName);

	void LevelUp();
	void AddLevel(int i);

public:
	CObj* GetPlayer() const			{ return m_pPlayer; }
	int GetStepLevel() const		{ return m_iStepLevel; }
	int GetMaxLevel() const			{ return m_iMaxLevel; }
	int GetLevel() const			{ return m_iLevel; }
	SKILL_TYPE GetType() const		{ return m_eType; }
	wchar_t* GetSkillName() const	{ return m_pSkillName; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CSkill* Clone() = 0;
};

