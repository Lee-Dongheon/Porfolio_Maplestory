#include "Icon.h"
#include "../../../Resources/ResourcesManager.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../UI/UI.h"

CIcon::CIcon() :
	m_eIconType(IC_DEFAULT),
	m_bSelected(false),
	m_eButtonType(BT_NORMAL),
	m_pIconTexture(NULL),
	m_tIconPos(POSITION(0.f, 0.f)),
	m_tIconSize(_SIZE(0.f, 0.f)),
	m_tIconPivot(POSITION(0.f, 0.f))
{
	for (int i = 0; i < (int)BT_END; i++)
	{
		m_pIcon[i] = NULL;
	}
}

CIcon::CIcon(const CIcon& icon) :
	CStaticObj(icon)
{
	m_tIconSize = icon.m_tIconSize;
	m_tIconPivot = icon.m_tIconPivot;
	m_eButtonType = BT_NORMAL;
	m_eIconType = icon.m_eIconType;

	m_iRef = 1;

	m_IconFunction = icon.m_IconFunction;

	m_pIconTexture = NULL;
	if (icon.m_pIconTexture)
	{
		m_pIconTexture = icon.m_pIconTexture;
		m_pIconTexture->AddRef();
	}

	for (int i = 0; i < (int)BT_END; i++)
	{
		m_pIcon[i] = NULL;
		if (icon.m_pIcon[i])
		{
			m_pIcon[i] = icon.m_pIcon[i];
			m_pIcon[i]->AddRef();
		}
	}
}

CIcon::~CIcon()
{
	for (int i = 0; i < (int)BT_END; i++)
	{
		SAFE_RELEASE(m_pIcon[i]);
	}
	SAFE_RELEASE(m_pIconTexture);
}

bool CIcon::Init()
{
	SetPivot(0.f, 0.f);

	return true;
}

void CIcon::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

int CIcon::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);
	return 0;
}

int CIcon::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void CIcon::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);
}

void CIcon::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CStaticObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

int CIcon::IconUpdate(float fDeltaTime)
{
	POSITION tPos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();
	bool bMouseUp = GET_SINGLE(CMouse)->GetMouseUp();

	if (m_eButtonType != BT_DISABLE)
	{
		RECTANGLE tRect;
		tRect.l = m_tIconPos.x - m_tIconSize.x * m_tIconPivot.x;
		tRect.t = m_tIconPos.y - m_tIconSize.y * m_tIconPivot.y;
		tRect.r = tRect.l + m_tIconSize.x;
		tRect.b = tRect.t + m_tIconSize.y;
		bool bMouseOn = PointInRect(tRect, tPos);

		if (!bMouseOn)
		{
			m_eButtonType = BT_NORMAL;
		}
		else
		{
			if (bIsClicked)
			{
				m_eButtonType = BT_PRESSED;
				m_bSelected = true;
				return 2;
			}
			else
			{
				m_eButtonType = BT_MOUSEON;
			}
		}
		if (bMouseUp && m_bSelected)
		{
			m_bSelected = false;
			return 1;
		}
	}

	return 0;
}

void CIcon::IconRender(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	if (m_pIcon[m_eButtonType])
		m_pIcon[m_eButtonType]->Render(m_tIconPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
	if (m_pIconTexture)
		m_pIconTexture->Render(m_tIconPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

int CIcon::KeyUpdate(float fDeltaTime)
{
	POSITION tPos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();

	RECTANGLE tRect;
	tRect.l = m_tKeyPos.x - m_tIconSize.x * m_tIconPivot.x;
	tRect.t = m_tKeyPos.y - m_tIconSize.y * m_tIconPivot.y;
	tRect.r = tRect.l + m_tIconSize.x;
	tRect.b = tRect.t + m_tIconSize.y;
	if (PointInRect(tRect, tPos) && bIsClicked)
		return 1;

	return 0;
}

void CIcon::KeyRender(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	if (m_pIcon[BT_NORMAL])
		m_pIcon[BT_NORMAL]->Render(m_tKeyPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
	if (m_pIconTexture)
		m_pIconTexture->Render(m_tKeyPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

int CIcon::QuickUpdate(float fDeltaTime)
{
	POSITION tPos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();

	RECTANGLE tRect;
	tRect.l = m_tQuickPos.x - m_tIconSize.x * m_tIconPivot.x;
	tRect.t = m_tQuickPos.y - m_tIconSize.y * m_tIconPivot.y;
	tRect.r = tRect.l + m_tIconSize.x;
	tRect.b = tRect.t + m_tIconSize.y;
	if (PointInRect(tRect, tPos) && bIsClicked)
		return 1;

	return 0;
}

void CIcon::QuickRender(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	if (m_pIcon[BT_NORMAL])
		m_pIcon[BT_NORMAL]->Render(m_tQuickPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
	if (m_pIconTexture)
		m_pIconTexture->Render(m_tQuickPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

void CIcon::MouseRender(POSITION tPos, ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime, float fOpacity)
{
	if (m_pIcon[BT_NORMAL])
		m_pIcon[BT_NORMAL]->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime, fOpacity);
	if (m_pIconTexture)
		m_pIconTexture->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime, fOpacity);
}

CIcon* CIcon::Clone()
{
	return new CIcon(*this);
}

void CIcon::LoadButton(const string& strTag, const wchar_t* pFileName,
	POSITION& tOrigin, POSITION& tLT, POSITION& tRB)
{
	SetTag(strTag);
	_SIZE tSize = tRB - tLT;
	SetIconSize(tSize);

	wchar_t* tempFileName = new wchar_t[wcslen(pFileName) + 21];

	memset(tempFileName, 0, sizeof(tempFileName));
	wchar_t temp[20] = L"Normal/0.png";
	wcscpy(tempFileName, pFileName);
	wcscat(tempFileName, temp);
	string tempTag = strTag + "_0";
	m_pIcon[BT_NORMAL] = GET_SINGLE(CResourcesManager)->LoadTexture(tempTag, tempFileName,
		tOrigin, tSize, tLT, tRB);

	memset(tempFileName, 0, sizeof(tempFileName));
	wchar_t temp2[20] = L"Pressed/0.png";
	wcscpy(tempFileName, pFileName);
	wcscat(tempFileName, temp2);
	string tempTag2 = strTag + "_1";
	m_pIcon[BT_PRESSED] = GET_SINGLE(CResourcesManager)->LoadTexture(tempTag2, tempFileName,
		tOrigin, tSize, tLT, tRB);

	memset(tempFileName, 0, sizeof(tempFileName));
	wchar_t temp3[20] = L"Disable/0.png";
	wcscpy(tempFileName, pFileName);
	wcscat(tempFileName, temp3);
	string tempTag3 = strTag + "_2";
	m_pIcon[BT_DISABLE] = GET_SINGLE(CResourcesManager)->LoadTexture(tempTag3, tempFileName,
		tOrigin, tSize, tLT, tRB);

	memset(tempFileName, 0, sizeof(tempFileName));
	wchar_t temp4[20] = L"MouseOn/0.png";
	wcscpy(tempFileName, pFileName);
	wcscat(tempFileName, temp4);
	string tempTag4 = strTag + "_3";
	m_pIcon[BT_MOUSEON] = GET_SINGLE(CResourcesManager)->LoadTexture(tempTag4, tempFileName,
		tOrigin, tSize, tLT, tRB);

	delete[] tempFileName;
}

void CIcon::LoadTextureIcon(const string & strTag, const wchar_t * pFileName, POSITION & tOrigin, POSITION & tLT, POSITION & tRB)
{
	SetTag(strTag);
	SAFE_RELEASE(m_pIconTexture);
	_SIZE tSize = tRB - tLT;
	SetIconSize(tSize);
	m_pIconTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strTag, pFileName, tOrigin, tSize, tLT, tRB);
}

void CIcon::LoadItemIcon(const string & strTag)
{
	if (strTag == "RedPotion")
	{
		SetTexture("RedPotionRaw", L"Item/RedPotion/Icon_RedPotionRaw.png", POSITION(14.f, 27.f), POSITION(-13.f, -27.f), POSITION(14.f, 0.f));
		LoadTextureIcon("RedPotion", L"Item/RedPotion/Icon_RedPotion.png", POSITION(-3.f, -2.f), POSITION(3.f, 2.f), POSITION(30.f, 32.f));
	}
	else if (strTag == "BluePotion")
	{
		SetTexture("BluePotionRaw", L"Item/BluePotion/Icon_BluePotionRaw.png", POSITION(14.f, 27.f), POSITION(-13.f, -27.f), POSITION(14.f, 0.f));
		LoadTextureIcon("BluePotion", L"Item/BluePotion/Icon_BluePotion.png", POSITION(-3.f, -2.f), POSITION(3.f, 2.f), POSITION(30.f, 32.f));
	}
	else if (strTag == "WhitePotion")
	{
		SetTexture("WhitePotionRaw", L"Item/WhitePotion/Icon_WhitePotionRaw.png", POSITION(14.f, 27.f), POSITION(-13.f, -27.f), POSITION(14.f, 0.f));
		LoadTextureIcon("WhitePotion", L"Item/WhitePotion/Icon_WhitePotion.png", POSITION(-3.f, -2.f), POSITION(3.f, 2.f), POSITION(30.f, 32.f));
	}
	else if (strTag == "ManaElixer")
	{
		SetTexture("ManaElixerRaw", L"Item/ManaElixer/Icon_ManaElixerRaw.png", POSITION(16.f, 31.f), POSITION(-16.f, -31.f), POSITION(16.f, 0.f));
		LoadTextureIcon("ManaElixer", L"Item/ManaElixer/Icon_ManaElixer.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(32.f, 32.f));
	}
	else if (strTag == "PowerElixer")
	{
		SetTexture("PowerElixerRaw", L"Item/PowerElixer/Icon_PowerElixerRaw.png", POSITION(16.f, 31.f), POSITION(-16.f, -31.f), POSITION(16.f, 0.f));
		LoadTextureIcon("PowerElixer", L"Item/PowerElixer/Icon_PowerElixer.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(32.f, 32.f));
	}
	else if (strTag == "TwilightDew")
	{
		SetTexture("TwilightDewRaw", L"Item/TwilightDew/Icon_TwilightDewRaw.png", POSITION(16.f, 33.f), POSITION(-15.f, -33.f), POSITION(16.f, 0.f));
		LoadTextureIcon("TwilightDew", L"Item/TwilightDew/Icon_TwilightDew.png", POSITION(0.f, 1.f), POSITION(0.f, -1.f), POSITION(31.f, 32.f));
	}
	else if (strTag == "ReindeerMilk")
	{
		SetTexture("ReindeerMilkRaw", L"Item/ReindeerMilk/Icon_ReindeerMilkRaw.png", POSITION(14.f, 28.f), POSITION(-14.f, -28.f), POSITION(14.f, 0.f));
		LoadTextureIcon("ReindeerMilk", L"Item/ReindeerMilk/Icon_ReindeerMilk.png", POSITION(-2.f, -4.f), POSITION(2.f, 4.f), POSITION(30.f, 32.f));
	}
	else if (strTag == "MeltCheese")
	{
		SetTexture("MeltCheeseRaw", L"Item/MeltCheese/Icon_MeltCheeseRaw.png", POSITION(13.f, 21.f), POSITION(-12.f, -21.f), POSITION(13.f, 0.f));
		LoadTextureIcon("MeltCheese", L"Item/MeltCheese/Icon_MeltCheese.png", POSITION(-4.f, -9.f), POSITION(4.f, 9.f), POSITION(31.f, 32.f));
	}
	else if (strTag == "OrangeHead")
	{
		SetTexture("OrangeHeadRaw", L"Item/OrangeMushroom/Icon_OrangeHeadRaw.png", POSITION(15.f, 26.f), POSITION(-15.f, -26.f), POSITION(15.f, 0.f));
		LoadTextureIcon("OrangeHead", L"Item/OrangeMushroom/Icon_OrangeHead.png", POSITION(-1.f, -4.f), POSITION(1.f, 4.f), POSITION(31.f, 32.f));
	}
	else if (strTag == "MushmomPoja")
	{
		SetTexture("MushmomPojaRaw", L"Item/Mushmom/Icon_MushmomPojaRaw.png", POSITION(16.f, 31.f), POSITION(-16.f, -31.f), POSITION(16.f, 0.f));
		LoadTextureIcon("MushmomPoja", L"Item/Mushmom/Icon_MushmomPoja.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(32.f, 32.f));
	}
	else if (strTag == "GreenHead")
	{
		SetTexture("GreenHeadRaw", L"Item/GreenMushroom/Icon_GreenHeadRaw.png", POSITION(15.f, 25.f), POSITION(-15.f, -25.f), POSITION(15.f, 0.f));
		LoadTextureIcon("GreenHead", L"Item/GreenMushroom/Icon_GreenHead.png", POSITION(-1.f, -3.f), POSITION(1.f, 3.f), POSITION(31.f, 32.f));
	}
	else if (strTag == "BlueGown")
	{
		SetTexture("BlueGownRaw", L"Equip/Cloth/Icon/Icon_BlueGownRaw.png", POSITION(15.f, 25.f), POSITION(-15.f, -25.f), POSITION(15.f, 0.f));
		LoadTextureIcon("BlueGown", L"Equip/Cloth/Icon/Icon_BlueGown.png", POSITION(-2.f, -3.f), POSITION(2.f, 3.f), POSITION(32.f, 32.f));
	}
	else if (strTag == "BrownGot")
	{
		SetTexture("BrownGotRaw", L"Equip/Head/Icon/Icon_BrownGotRaw.png", POSITION(16.f, 25.f), POSITION(-16.f, -25.f), POSITION(16.f, 0.f));
		LoadTextureIcon("BrownGot", L"Equip/Head/Icon/Icon_BrownGot.png", POSITION(0.f, -6.f), POSITION(0.f, 6.f), POSITION(32.f, 32.f));
	}
}
