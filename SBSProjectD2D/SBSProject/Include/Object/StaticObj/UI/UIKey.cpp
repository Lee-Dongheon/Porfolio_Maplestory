#include "UIKey.h"
#include "../../../Core/Core.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../../../Core/Input.h"

CUIKey::CUIKey()
{
}

CUIKey::CUIKey(const CUIKey& ui) :
	CUI(ui)
{
}

CUIKey::~CUIKey()
{
}

POSITION CUIKey::FindKeyPos(const string & strTag)
{
	map<string, POSITION>::iterator iter = m_mapKeyBox.find(strTag);

	if (iter == m_mapKeyBox.end())
		return POSITION(0.f, 0.f);

	return iter->second;
}

string CUIKey::FindBoxTag(POSITION tPos)
{
	map<string, POSITION>::iterator iter;
	map<string, POSITION>::iterator iterEnd = m_mapKeyBox.end();

	for (iter = m_mapKeyBox.begin(); iter != iterEnd; iter++)
	{
		RECTANGLE tRect;
		tRect.l = m_tPos.x + iter->second.x;
		tRect.t = m_tPos.y + iter->second.y;
		tRect.r = tRect.l + 32.f;
		tRect.b = tRect.t + 32.f;
		if (PointInRect(tRect, tPos))
			return iter->first;
	}

	return "";
}

bool CUIKey::Init()
{
	if (!CUI::Init(UI_KEY))
		return false;

	SetEnable(false);
	SetPos(300.f, 100.f);
	SetSize(660.f, 511.f);
	SetHeadSize(660.f, 20.f);

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);

	CTexture* pUI = AddUITexture("KeyBackground", L"UI/Key/Key_Background.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(660.f, 511.f));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("KeyBackground2", L"UI/Key/Key_Background2.png", POSITION(10.f, 22.f), tOrigin, tLT, POSITION(640.f, 479.f));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("KeyBackground3", L"UI/Key/Key_Background3.png", POSITION(20.f, 67.f), tOrigin, tLT, POSITION(620.f, 231.f));
	SAFE_RELEASE(pUI);

	CButton* pButton = AddButton("KeyClose", L"UI/Basic/Close/", POSITION(644.f, 4.f), tOrigin, tLT, POSITION(13.f, 13.f));
	pButton->SetIconFunction((CUI*)this, &CUI::CloseUI);
	SAFE_RELEASE(pButton);

	KeyBoxInit();

	CKeyIcon* pIcon = AddKeyIcon("UIKeyIcon", L"UI/Key/Icon_Key.png", "\\");
	pIcon->SetDefaultKey("\\");
	pIcon->SetIconFunction((CUI*)this, &CUI::UseUIIcon);
	SAFE_RELEASE(pIcon);

	return true;
}

void CUIKey::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);
}

int CUIKey::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);
	POSITION tMousePos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();

	map<string, CIcon*>::iterator iter;
	map<string, CIcon*>::iterator iterEnd = m_mapIcon.end();

	for (iter = m_mapIcon.begin(); iter != iterEnd; iter++)
	{
		POSITION tPos = FindKeyPos(iter->first);

		if (tPos.x == 0.f && tPos.y == 0.f)
			continue;
		iter->second->SetKeyPos(tPos + m_tPos);

		CIcon* pIcon = (CIcon*)(iter->second);
		// 눌렀다면
		if (pIcon->KeyUpdate(fDeltaTime) && !m_pSelectedIcon && !m_bHaveSelectedIcon)
		{
			m_pSelectedIcon = pIcon;
			m_pSelectedIcon->AddRef();
		}
	}

	// 키세팅창이 아닌 다른 위치를 눌러도 함수가 실행된다.
	// 마우스의 위치부터 체크하자.
	// 여기서 Extra 부분(keyIcon의 default 부분)에 갖다 놓으면 취소해버려야 한다.
	// 따라서 tRect 설정을 키보드 부분으로 제한해주기(background3 만큼)
	RECTANGLE tRect;
	tRect.l = m_tPos.x + 20.f;
	tRect.t = m_tPos.y + 67.f;
	tRect.r = tRect.l + 620.f;
	tRect.b = tRect.t + 231.f;

	if (m_pSelectedIcon && m_bHaveSelectedIcon && bIsClicked)
	{ // 키세팅 창은 아이콘 타입에 제한이 없다.
		// 키세팅 창 안을 눌렀는지 체크, 안이라면 icon의 key값 변화
		if (PointInRect(tRect, tMousePos))
		{
			// 해당 위치의 칸 이름을 찾아 아이콘 넣어주기
			string strTag = FindBoxTag(tMousePos);
			AddIcon(strTag, m_pSelectedIcon);

			SAFE_RELEASE(m_pSelectedIcon);
		}		
	}

	return 0;
}

int CUIKey::LateUpdate(float fDeltaTime)
{
	CUI::LateUpdate(fDeltaTime);

	return 0;
}

void CUIKey::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);
}

void CUIKey::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CUI::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	map<string, POSITION>::iterator iter;
	map<string, POSITION>::iterator iterEnd = m_mapKeyBox.end();

	for (iter = m_mapKeyBox.begin(); iter != iterEnd; iter++)
	{
		CIcon* pIcon = FindIcon(iter->first);
		if (pIcon)
		{
			pIcon->KeyRender(pGDIRenderTarget, pRenderTarget, fDeltaTime);
			SAFE_RELEASE(pIcon);
		}
		CTexture* pKeyImage = FindKeyImage(iter->first);
		if (pKeyImage)
		{
			POSITION tPos = iter->second + m_tPos + POSITION(2.f, 3.f);
			pKeyImage->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
			SAFE_RELEASE(pKeyImage);
		}			
	}
}

CUIKey* CUIKey::Clone()
{
	return new CUIKey(*this);
}

void CUIKey::KeyBoxInit()
{
	// ESC ~ F12
	m_mapKeyBox.insert(make_pair("ESC", POSITION(29.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F1", POSITION(95.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F2", POSITION(128.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F3", POSITION(161.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F4", POSITION(194.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F5", POSITION(244.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F6", POSITION(277.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F7", POSITION(310.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F8", POSITION(343.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F9", POSITION(393.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F10", POSITION(426.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F11", POSITION(459.f, 85.f)));
	m_mapKeyBox.insert(make_pair("F12", POSITION(492.f, 85.f)));

	// SLK
	m_mapKeyBox.insert(make_pair("SLK", POSITION(566.f, 85.f)));

	// ` ~ =
	m_mapKeyBox.insert(make_pair("`", POSITION(29.f, 123.f)));
	m_mapKeyBox.insert(make_pair("1", POSITION(62.f, 123.f)));
	m_mapKeyBox.insert(make_pair("2", POSITION(95.f, 123.f)));
	m_mapKeyBox.insert(make_pair("3", POSITION(128.f, 123.f)));
	m_mapKeyBox.insert(make_pair("4", POSITION(161.f, 123.f)));
	m_mapKeyBox.insert(make_pair("5", POSITION(194.f, 123.f)));
	m_mapKeyBox.insert(make_pair("6", POSITION(227.f, 123.f)));
	m_mapKeyBox.insert(make_pair("7", POSITION(260.f, 123.f)));
	m_mapKeyBox.insert(make_pair("8", POSITION(293.f, 123.f)));
	m_mapKeyBox.insert(make_pair("9", POSITION(326.f, 123.f)));
	m_mapKeyBox.insert(make_pair("0", POSITION(359.f, 123.f)));
	m_mapKeyBox.insert(make_pair("-", POSITION(392.f, 123.f)));
	m_mapKeyBox.insert(make_pair("=", POSITION(425.f, 123.f)));

	// q ~ '\'
	m_mapKeyBox.insert(make_pair("q", POSITION(78.f, 156.f)));
	m_mapKeyBox.insert(make_pair("w", POSITION(111.f, 156.f)));
	m_mapKeyBox.insert(make_pair("e", POSITION(144.f, 156.f)));
	m_mapKeyBox.insert(make_pair("r", POSITION(177.f, 156.f)));
	m_mapKeyBox.insert(make_pair("t", POSITION(210.f, 156.f)));
	m_mapKeyBox.insert(make_pair("y", POSITION(243.f, 156.f)));
	m_mapKeyBox.insert(make_pair("u", POSITION(276.f, 156.f)));
	m_mapKeyBox.insert(make_pair("i", POSITION(309.f, 156.f)));
	m_mapKeyBox.insert(make_pair("o", POSITION(342.f, 156.f)));
	m_mapKeyBox.insert(make_pair("p", POSITION(375.f, 156.f)));
	m_mapKeyBox.insert(make_pair("[", POSITION(408.f, 156.f)));
	m_mapKeyBox.insert(make_pair("]", POSITION(441.f, 156.f)));
	m_mapKeyBox.insert(make_pair("\\", POSITION(483.f, 156.f)));

	// a ~ '
	m_mapKeyBox.insert(make_pair("a", POSITION(94.f, 189.f)));
	m_mapKeyBox.insert(make_pair("s", POSITION(127.f, 189.f)));
	m_mapKeyBox.insert(make_pair("d", POSITION(160.f, 189.f)));
	m_mapKeyBox.insert(make_pair("f", POSITION(193.f, 189.f)));
	m_mapKeyBox.insert(make_pair("g", POSITION(226.f, 189.f)));
	m_mapKeyBox.insert(make_pair("h", POSITION(259.f, 189.f)));
	m_mapKeyBox.insert(make_pair("j", POSITION(292.f, 189.f)));
	m_mapKeyBox.insert(make_pair("k", POSITION(325.f, 189.f)));
	m_mapKeyBox.insert(make_pair("l", POSITION(358.f, 189.f)));
	m_mapKeyBox.insert(make_pair(";", POSITION(391.f, 189.f)));
	m_mapKeyBox.insert(make_pair("\'", POSITION(424.f, 189.f)));
	
	// Shift ~ .
	m_mapKeyBox.insert(make_pair("Shift", POSITION(53.f, 222.f)));
	m_mapKeyBox.insert(make_pair("z", POSITION(110.f, 222.f)));
	m_mapKeyBox.insert(make_pair("x", POSITION(143.f, 222.f)));
	m_mapKeyBox.insert(make_pair("c", POSITION(176.f, 222.f)));
	m_mapKeyBox.insert(make_pair("v", POSITION(209.f, 222.f)));
	m_mapKeyBox.insert(make_pair("b", POSITION(242.f, 222.f)));
	m_mapKeyBox.insert(make_pair("n", POSITION(275.f, 222.f)));
	m_mapKeyBox.insert(make_pair("m", POSITION(308.f, 222.f)));
	m_mapKeyBox.insert(make_pair(",", POSITION(341.f, 222.f)));
	m_mapKeyBox.insert(make_pair(".", POSITION(374.f, 222.f)));

	// Ctrl, Alt, Space
	m_mapKeyBox.insert(make_pair("Ctrl", POSITION(37.f, 255.f)));
	m_mapKeyBox.insert(make_pair("Alt", POSITION(135.f, 255.f)));
	m_mapKeyBox.insert(make_pair("Space", POSITION(259.f, 255.f)));

	// Insert, Home, PgUp
	m_mapKeyBox.insert(make_pair("Ins", POSITION(533.f, 123.f)));
	m_mapKeyBox.insert(make_pair("Hm", POSITION(566.f, 123.f)));
	m_mapKeyBox.insert(make_pair("Pup", POSITION(599.f, 123.f)));

	// Del, End, PgDn
	m_mapKeyBox.insert(make_pair("Del", POSITION(533.f, 156.f)));
	m_mapKeyBox.insert(make_pair("End", POSITION(566.f, 156.f)));
	m_mapKeyBox.insert(make_pair("Pdn", POSITION(599.f, 156.f)));

	for (int i = 0; i < 68; i++)
	{
		string tempName = "Extra" + to_string(i);
		m_mapKeyBox.insert(make_pair(tempName, POSITION(26.f, 307) + POSITION(36.f * (i % 17), 36.f * (i / 17))));
	}
}
