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

	int m_iStepLevel; // 현재 선택된 차수 탭
	int m_iSkillNum; // 현재 차수의 스킬의 개수
	int m_iMaxSkillNum; // 총 스킬 개수
	int m_iPointUpButtonNum; // 눌린 버튼 위치

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

