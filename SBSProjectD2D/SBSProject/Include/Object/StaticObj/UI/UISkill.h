#pragma once
#include "UI.h"

class CUISkill :
    public CUI
{
private:
	friend class CObj;

protected:
	CUISkill();
	CUISkill(const CUISkill& ui);
	virtual ~CUISkill();

private:
	vector<CButton*> m_vecPointUpButton;
	vector<CTexture*> m_vecNoSkillBack;
	vector<CTexture*> m_vecSkillBack;

	int m_iStepLevel; // ���� ���õ� ���� ��
	int m_iSkillNum; // ���� ������ ��ų�� ����
	int m_iMaxSkillNum; // �� ��ų ����
	int m_iPointUpButtonNum; // ���� ��ư ��ġ

private:
	void AddPointUpButton(int iSkill);
	void AddSkillBack(int iSkill);

public:
	void SkillPointUp();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CUISkill* Clone();
};

