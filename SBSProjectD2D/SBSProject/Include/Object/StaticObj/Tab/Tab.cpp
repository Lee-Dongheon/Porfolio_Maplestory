#include "Tab.h"
#include "../../../Resources/Texture.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../../../Resources/ResourcesManager.h"

CTab::CTab():
	m_eTabType(TT_DISABLE)
{
}

CTab::CTab(const CTab & obj):
	CStaticObj(obj)
{
	m_eTabType = TT_DISABLE;
}

CTab::~CTab()
{
	for (int i = 0; i < (int)TT_END; i++)
	{
		SAFE_RELEASE(m_pTab[i]);
	}
}

bool CTab::Init()
{
	return true;
}

void CTab::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

int CTab::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);

	POSITION tPos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();

	if (m_eTabType == TT_DISABLE)
	{
		RECTANGLE tRect = RECTANGLE(m_tPos.x, m_tPos.y,
			m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y);
		if (PointInRect(tRect, tPos) && bIsClicked)
		{
			EnableTab();
			return 1;
		}
	}

	return 0;
}

int CTab::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);

	return 0;
}

void CTab::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);
}

void CTab::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	if (m_pTab)
		m_pTab[m_eTabType]->Render(m_tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CTab * CTab::Clone()
{
	return new CTab(*this);
}

void CTab::LoadTab(const string & strTag, const wchar_t * pFileName, POSITION & tOrigin, POSITION & tLT, POSITION & tRB)
{
	SetTag(strTag);
	_SIZE tSize = tRB - tLT;
	SetSize(tSize);

	// Enable tab이 y크기가 2만큼 크다.
	POSITION tRB1 = tRB + POSITION(0.f, 2.f);
	_SIZE tSize1 = tRB1 - tLT;

	wchar_t* tempFileName = new wchar_t[wcslen(pFileName) + 21];

	memset(tempFileName, 0, sizeof(tempFileName));
	wchar_t temp[20] = L"Enable/0.png";
	wcscpy(tempFileName, pFileName);
	wcscat(tempFileName, temp);
	string tempTag = strTag + "_0";
	m_pTab[TT_ENABLE] = GET_SINGLE(CResourcesManager)->LoadTexture(tempTag, tempFileName,
		tOrigin, tSize1, tLT, tRB1);

	memset(tempFileName, 0, sizeof(tempFileName));
	wchar_t temp2[20] = L"Disable/0.png";
	wcscpy(tempFileName, pFileName);
	wcscat(tempFileName, temp2);
	string tempTag2 = strTag + "_1";
	m_pTab[TT_DISABLE] = GET_SINGLE(CResourcesManager)->LoadTexture(tempTag2, tempFileName,
		tOrigin, tSize, tLT, tRB);

	delete[] tempFileName;
}
