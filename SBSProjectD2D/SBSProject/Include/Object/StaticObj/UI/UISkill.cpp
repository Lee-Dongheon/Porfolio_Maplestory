#include "UISkill.h"
#include "../../../Core/Core.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../../../Core/Input.h"
#include "../Icon/Skill/Skill.h"
#include "../../../Core/ServerManager.h"

CUISkill::CUISkill():
	m_iStepLevel(0),
	m_iSkillNum(0),
	m_iMaxSkillNum(12),
	m_iPointUpButtonNum(0)
{
}

CUISkill::CUISkill(const CUISkill& ui) :
	CUI(ui)
{
	
}

CUISkill::~CUISkill()
{
	Safe_Release_VecList(m_vecPointUpButton);
	Safe_Release_VecList(m_vecNoSkillBack);
	Safe_Release_VecList(m_vecSkillBack);
}
void CUISkill::AddPointUpButton(int iSkill)
{
	for (int i = 0; i < iSkill; i++)
	{	
		CButton* pButton = new CButton;

		if (!pButton)
			return;
		string strPointUp = "SkillPointUp" + to_string(i);
		POSITION tPos = POSITION(135.f + 143.f * (i % 2), 113.f + 40.f * (i / 2));

		pButton->LoadButton(strPointUp, L"UI/Skill/PointUp/", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(14.f, 12.f));

		pButton->SetIconFunction(this, &CUISkill::SkillPointUp);
		m_mapPos.insert(make_pair(strPointUp, tPos));
		m_vecPointUpButton.push_back(pButton);
	}
}
void CUISkill::AddSkillBack(int iSkill)
{
	// 첫 스킬 아이콘 위치 12, 95
	// 아이콘 간 간격 좌우 144, 상하 41
	for (int i = 0; i < iSkill; i++)
	{
		string strNoSkillBack = "NoSkillBack" + to_string(i);
		string strMainBack = "SkillBack" + to_string(i);

		POSITION tPos = POSITION(10.f + 143.f * (i % 2), 93.f + 40.f * (i / 2));

		CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTexture(strNoSkillBack, L"UI/Skill/NoSkillMain.png", POSITION(0.f, 0.f), POSITION(140.f, 35.f), POSITION(0.f, 0.f), POSITION(140.f, 35.f));
		if (!pTex)
			return;

		CTexture* pTex2 = GET_SINGLE(CResourcesManager)->LoadTexture(strMainBack, L"UI/Skill/SkillMain.png", POSITION(0.f, 0.f), POSITION(140.f, 35.f), POSITION(0.f, 0.f), POSITION(140.f, 35.f));
		if (!pTex2)
			return;

		m_vecNoSkillBack.push_back(pTex);
		SetPosition(strNoSkillBack, tPos);

		m_vecSkillBack.push_back(pTex2);
		SetPosition(strMainBack, tPos);
	}
}
void CUISkill::SkillPointUp()
{
	// 눌렀을때 해당 차수의 몇번째 버튼인지 찾아서
	// 해당 차수의 해당번째 스킬의 레벨을 올린다.
	CSkill* pSkill = m_pPlayer->m_vecSkill[m_iStepLevel][m_iPointUpButtonNum];
	pSkill->LevelUp();
	// 스킬 레벨 변화를 서버에 알림
	GET_SINGLE(CServerManager)->SendChangeSkill(pSkill->GetTag(), pSkill->GetLevel());

	int iLevel = m_pPlayer->m_vecSkill[m_iStepLevel][m_iPointUpButtonNum]->GetLevel();
	int iMaxLevel = m_pPlayer->m_vecSkill[m_iStepLevel][m_iPointUpButtonNum]->GetMaxLevel();
	if (iLevel == iMaxLevel)
		m_vecPointUpButton[m_iPointUpButtonNum]->SetButtonType(BT_DISABLE);
}

bool CUISkill::Init()
{
	if (!CUI::Init(UI_SKILL))
		return false;

	SetEnable(false);
	SetPos(200.f, 100.f);
	SetSize(318.f, 361.f);
	SetHeadSize(318.f, 22.f);

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);

	CTexture* pUITex = AddUITexture("SkillBackground", L"UI/Skill/Skill_background.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(318.f, 361.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("SkillBackground2", L"UI/Skill/Skill_background2.png", POSITION(6.f, 22.f), tOrigin, tLT, POSITION(306.f, 333.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("SkillBackground3", L"UI/Skill/Skill_background3.png", POSITION(8.f, 47.f), tOrigin, tLT, POSITION(304.f, 45.f));
	SAFE_RELEASE(pUITex);

	CButton* pButton = AddButton("Hyper", L"UI/Skill/Hyper/", POSITION(10.f, 335.f), tOrigin, tLT, POSITION(50.f, 16.f));
	SAFE_RELEASE(pButton);

	// 여기 나중에 같은 텍스쳐 공유하게 복사생성으로 해보자.
	// 최대 렌더링 가능 스킬 칸 12개
	AddSkillBack(12);
	AddPointUpButton(12);
	// 스킬칸 사이 경계선
	for (int i = 0; i < 10; i++)
	{
		POSITION tPos = POSITION(143.f * (i % 2), 40.f * (i / 2));
		string strLine = "SkillLine" + to_string(i);
		pUITex = AddUITexture(strLine, L"UI/Skill/SkillMain_Line.png", POSITION(10.f, 130.f) + tPos, tOrigin, tLT, POSITION(140.f, 1.f));
		SAFE_RELEASE(pUITex);
	}

	// 0 ~ 4차
	CTab* pTab = AddTab("SkillTab0", L"UI/Skill/SkillTab/0/", POSITION(10.f, 27.f), tOrigin, tLT, POSITION(25.f, 18.f));
	pTab->EnableTab();
	m_strLastTab = pTab->GetTag();
	SAFE_RELEASE(pTab);
	pTab = AddTab("SkillTab1", L"UI/Skill/SkillTab/1/", POSITION(36.f, 29.f), tOrigin, tLT, POSITION(25.f, 18.f));
	SAFE_RELEASE(pTab);
	pTab = AddTab("SkillTab2", L"UI/Skill/SkillTab/2/", POSITION(62.f, 29.f), tOrigin, tLT, POSITION(25.f, 18.f));
	SAFE_RELEASE(pTab);
	pTab = AddTab("SkillTab3", L"UI/Skill/SkillTab/3/", POSITION(88.f, 29.f), tOrigin, tLT, POSITION(25.f, 18.f));
	SAFE_RELEASE(pTab);
	pTab = AddTab("SkillTab4", L"UI/Skill/SkillTab/4/", POSITION(114.f, 29.f), tOrigin, tLT, POSITION(25.f, 18.f));
	SAFE_RELEASE(pTab);

	pButton = AddButton("SkillClose", L"UI/Basic/Close/", POSITION(294.f, 6.f), tOrigin, tLT, POSITION(13.f, 13.f));
	pButton->SetIconFunction((CUI*)this, &CUI::CloseUI);
	SAFE_RELEASE(pButton);

	CKeyIcon* pIcon = AddKeyIcon("UISkillIcon", L"UI/Key/Icon_Skill.png", "k");
	pIcon->SetDefaultKey("Extra4");
	pIcon->SetIconFunction((CUI*)this, &CUI::UseUIIcon);
	SAFE_RELEASE(pIcon);

	//SetVScroll("SkillVScroll", POSITION(296.f, 93.f), 235.f);

	return true;
}

void CUISkill::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);
	
	for (int i = 0; i < m_iSkillNum; i++)
	{
		m_vecPointUpButton[i]->Input(fDeltaTime);
	}	
}

int CUISkill::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);

	if (m_bTabChanged)
	{

		m_iStepLevel = m_strLastTab.at(m_strLastTab.size() - 1) - 48;
		m_iSkillNum = m_pPlayer->m_vecSkill[m_iStepLevel].size();

		for (int i = 0; i < m_iSkillNum; i++)
		{
			int iLevel = m_pPlayer->m_vecSkill[m_iStepLevel][i]->GetLevel();
			int iMaxLevel = m_pPlayer->m_vecSkill[m_iStepLevel][i]->GetMaxLevel();
			if (iLevel == 0)
				m_pPlayer->m_vecSkill[m_iStepLevel][i]->SetButtonType(BT_DISABLE);
			else
				m_pPlayer->m_vecSkill[m_iStepLevel][i]->SetButtonType(BT_NORMAL);

			if (iLevel == iMaxLevel)
				m_vecPointUpButton[i]->SetButtonType(BT_DISABLE);
			else
				m_vecPointUpButton[i]->SetButtonType(BT_NORMAL);
		}
	}

	// skill의 개수만큼 위치 갱신
	// skill과 버튼 둘 다 갱신
	for (int i = 0; i < m_iSkillNum; i++)
	{
		POSITION tPos = FindPosition(m_vecPointUpButton[i]->GetTag()) + m_tPos;
		m_vecPointUpButton[i]->SetIconPos(tPos);
		if (m_vecPointUpButton[i]->IconUpdate(fDeltaTime) == 1)
		{
			m_iPointUpButtonNum = i;
			m_vecPointUpButton[i]->UseIcon();
		}
		
		POSITION tPosSkill = POSITION(m_tPos.x + 10.f + 143.f * (i % 2) + 2.f, m_tPos.y + 93.f + 40.f *(i / 2) + 2.f);
		// skill icon의 위치 + skill칸 시작 위치 + 좌표 간격
		m_pPlayer->m_vecSkill[m_iStepLevel][i]->SetIconPos(tPosSkill);
		
		CIcon* pIcon = (CIcon*)m_pPlayer->m_vecSkill[m_iStepLevel][i];
		if (pIcon->IconUpdate(fDeltaTime) == 2 && !m_pSelectedIcon && !m_bHaveSelectedIcon
			&& m_pPlayer->m_vecSkill[m_iStepLevel][i]->GetType() != ST_PASSIVE)
		{
			m_pSelectedIcon = pIcon;
			m_pSelectedIcon->AddRef();
		}
	}

	return 0;
}

int CUISkill::LateUpdate(float fDeltaTime)
{
	CUI::LateUpdate(fDeltaTime);

	for (int i = 0; i < m_iSkillNum; i++)
	{
		m_vecPointUpButton[i]->LateUpdate(fDeltaTime);
	}
	
	return 0;
}

void CUISkill::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);
}

void CUISkill::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CUI::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	// 선택된 tab item_type으로 변환
	for (int i = 0; i < m_iSkillNum; i++)
	{
		POSITION tPos = FindPosition(m_vecSkillBack[i]->GetTag()) + m_tPos;
		m_vecSkillBack[i]->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
		m_pPlayer->m_vecSkill[m_iStepLevel][i]->IconRender(pGDIRenderTarget, pRenderTarget, fDeltaTime);
		
		HDC hDC = NULL;
		pGDIRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
		HFONT newFont, oldFont;
		GET_SINGLE(CResourcesManager)->SetFont(newFont, oldFont, hDC, 10);

		// 스킬 이름 95 x 10
		const char* pSkillName = m_pPlayer->m_vecSkill[m_iStepLevel][i]->GetTag().c_str();
		TextOutA(hDC, tPos.x + 41.f, tPos.y + 3.f, pSkillName, strlen(pSkillName));

		// 스킬 레벨 숫자 15 x 10
		int iLevel = m_pPlayer->m_vecSkill[m_iStepLevel][i]->GetLevel();
		char pSkillLevel[3] = {};
		sprintf(pSkillLevel, "%d", iLevel);
		TextOutA(hDC, tPos.x + 41.f, tPos.y + 20.f, pSkillLevel, strlen(pSkillLevel));
		GET_SINGLE(CResourcesManager)->ReleaseFont(newFont, oldFont, hDC);
		pGDIRenderTarget->ReleaseDC(NULL);
		
		m_vecPointUpButton[i]->IconRender(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}
	for (int i = m_iSkillNum; i < m_iMaxSkillNum; i++)
	{
		POSITION tPos = FindPosition(m_vecNoSkillBack[i]->GetTag()) + m_tPos;
		m_vecNoSkillBack[i]->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}
}

CUISkill* CUISkill::Clone()
{
	return new CUISkill(*this);
}