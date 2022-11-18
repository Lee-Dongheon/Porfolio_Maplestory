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
	CObj* m_pPlayer; // ��ų�� ���� �÷��̾�
	int m_iStepLevel; // ��ų ����
	int m_iMaxLevel; // �����ͷ���
	int m_iLevel; // ���緹��
	SKILL_TYPE m_eType; // ��ų Ÿ��
	wchar_t* m_pSkillName; // ��ų �̸�

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

