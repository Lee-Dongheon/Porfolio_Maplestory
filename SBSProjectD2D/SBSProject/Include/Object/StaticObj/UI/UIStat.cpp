#include "UIStat.h"
#include "../../../Core/Core.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../../../Core/Input.h"
#include "../../../Resources/ResourcesManager.h"

CUIStat::CUIStat()
{
}

CUIStat::CUIStat(const CUIStat & ui) :
	CUI(ui)
{
}

CUIStat::~CUIStat()
{
}

bool CUIStat::Init()
{
	if (!CUI::Init(UI_STAT))
		return false;

	SetEnable(false);
	SetPos(300.f, 100.f);
	SetSize(212.f, 373.f);
	SetHeadSize(212.f, 23.f);

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);

	CTexture* pUI = AddUITexture("StatBackground", L"UI/Stat/Stat_background.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(212.f, 373.f));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("StatBackground2", L"UI/Stat/Stat_background2.png", POSITION(6.f, 23.f), tOrigin, tLT, POSITION(200.f, 344.f));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("StatBackground3", L"UI/Stat/Stat_background3.png", POSITION(7.f, 212.f), tOrigin, tLT, POSITION(198.f, 46.f));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("StatDetailBackground", L"UI/Stat/Stat_detail_background.png", POSITION(213.f, 0.f), tOrigin, tLT, POSITION(212.f, 385.f));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("StatDetailBackground2", L"UI/Stat/Stat_detail_background2.png", POSITION(219.f, 7.f), tOrigin, tLT, POSITION(200.f, 372.f));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("StatDetailBackground3", L"UI/Stat/Stat_detail_background3.png", POSITION(224.f, 13.f), tOrigin, tLT, POSITION(190.f, 336.f));
	SAFE_RELEASE(pUI);

	CButton* pButton = AddButton("StatClose", L"UI/Basic/Close/", POSITION(190.f, 6.f), tOrigin, tLT, POSITION(13.f, 13.f));
	pButton->SetIconFunction((CUI*)this, &CUI::CloseUI);
	SAFE_RELEASE(pButton);

	CKeyIcon* pIcon = AddKeyIcon("UIStatIcon", L"UI/Key/Icon_Stat.png", "s");
	pIcon->SetDefaultKey("Extra3");
	pIcon->SetIconFunction((CUI*)this, &CUI::UseUIIcon);
	SAFE_RELEASE(pIcon);

	/*CIcon* pIcon = AddIcon("Icon_Stat", POSITION(0.f, 0.f), _SIZE(32.f, 32.f), L"UI/Key/Icon_Stat.png", "s");
	pIcon->SetUIObj(this);
	pIcon->SetIconFunction(this, &CUIStat::UseUIIcon);
	pIcon->SetDefaultKey("Extra3");*/

	return true;
}

void CUIStat::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);
}

int CUIStat::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);

	return 0;
}

int CUIStat::LateUpdate(float fDeltaTime)
{
	CUI::LateUpdate(fDeltaTime);

	return 0;
}

void CUIStat::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);
}

void CUIStat::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CUI::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	//IDWriteTextFormat* pStatFormat = GET_SINGLE(CResourcesManager)->GetUIStatTextFormat();
	//ID2D1SolidColorBrush* pStatBrush = GET_SINGLE(CResourcesManager)->GetUIStatBrush();

	D2D1_RECT_F tRect = RectF(76 + m_tPos.x, 33 + m_tPos.y, 202 + m_tPos.x, 45 + m_tPos.y);
	//pRenderTarget->DrawTextW(m_pPlayer->GetPlayerName(), lstrlen(m_pPlayer->GetPlayerName()), pStatFormat, &tRect, pStatBrush);
	tRect.top += 18.f;
	tRect.bottom += 18.f;
	//pRenderTarget->DrawTextW(m_pPlayer->GetPlayerJob(), lstrlen(m_pPlayer->GetPlayerJob()), pStatFormat, &tRect, pStatBrush);
}

CUIStat * CUIStat::Clone()
{
	return new CUIStat(*this);
}