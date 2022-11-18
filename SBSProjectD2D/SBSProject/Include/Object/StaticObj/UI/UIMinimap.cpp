#include "UIMinimap.h"
#include "../Stage/Stage.h"
#include "../../../Core/Mouse.h"

CUIMinimap::CUIMinimap() :
	m_pMinimapTexture(NULL),
	m_tMinimapSize(0.f, 0.f),
	m_tMapSize(1.f, 1.f),
	m_eState(MAP_MIN),
	m_eBeforeState(MAP_MAX),
	m_bMinimapChanged(false),
	m_fNSLength(1.f),
	m_fWELength(1.f)
{
}

CUIMinimap::CUIMinimap(const CUIMinimap &minimap) :
	CUI(minimap)
{
	m_pMinimapTexture = NULL;
	if (minimap.m_pMinimapTexture)
	{
		m_pMinimapTexture = minimap.m_pMinimapTexture;
		m_pMinimapTexture->AddRef();
	}

	for (int i = 0; i < (int)MAP_END; i++)
	{
		m_mapMinimapTexture[i].clear();

		map<string, CTexture*>::const_iterator iter;
		map<string, CTexture*>::const_iterator iterEnd = minimap.m_mapMinimapTexture[i].end();

		for (iter = minimap.m_mapMinimapTexture[i].begin(); iter != iterEnd; iter++)
		{
			m_mapMinimapTexture[i].insert(make_pair(iter->first, iter->second));
			iter->second->AddRef();
		}
	}

	m_eState = MAP_MIN;
	m_eBeforeState = MAP_MAX;
}

CUIMinimap::~CUIMinimap()
{
	SAFE_RELEASE(m_pMinimapTexture);

	for (int i = 0; i < (int)MAP_END; i++)
		Safe_Release_Map(m_mapMinimapTexture[i]);
}

void CUIMinimap::SetPlayer(CObj * pPlayer)
{
	CUI::SetPlayer(pPlayer);

	CObj* pObj = pPlayer->GetLayer()->FindStage();
	if (pObj)
	{
		SetMinimapTexture(((CStage*)pObj)->GetMinimapTexture());
		SetMinimapSize(((CStage*)pObj)->GetMinimapSize());
		SetMapSize(((CStage*)pObj)->GetSize());

		// Init 이후이므로 텍스쳐가 이미 존재.
		// 미니맵 크기에 맞게 텍스쳐 위치 조정

		//POSITION(10.f, 19.f) + m_tMinimapSize - POSITION(53.f, 19.f);
		SetPosition("MinimapSmall_NE", POSITION(max(10.f + m_tMinimapSize.x - 53.f, 65.f), 0.f));
		POSITION tNEPos = FindPosition("MinimapSmall_NE");
		m_fNSLength = tNEPos.x - 64.f;
		SetPosition("MinimapSmall_E", POSITION(max(10.f + m_tMinimapSize.x + 2.f, 120.f), 27.f));
		SetPosition("MinimapSmall_SE", POSITION(max(10.f + m_tMinimapSize.x - 53.f, 65.f), max(m_tMinimapSize.y, 28.f)));
		SetPosition("MinimapSmall_S", POSITION(64.f, max(m_tMinimapSize.y + 18.f, 28.f)));
		SetPosition("MinimapSmall_SW", POSITION(0.f, max(m_tMinimapSize.y, 28.f)));
		POSITION tWEPos = FindPosition("MinimapSmall_SW");
		m_fWELength = tWEPos.y - 27.f;

		//POSITION(10.f, 59.f) + m_tMinimapSize - POSITION(53.f, 19.f);
		SetPosition("MinimapMax_NE", POSITION(max(10.f + m_tMinimapSize.x - 53.f, 65.f), 0.f));
		SetPosition("MinimapMax_E", POSITION(max(10.f + m_tMinimapSize.x + 2.f, 120.f), 67.f));
		SetPosition("MinimapMax_SE", POSITION(max(10.f + m_tMinimapSize.x - 53.f, 65.f), max(m_tMinimapSize.y + 40.f, 68.f)));
		SetPosition("MinimapMax_S", POSITION(64.f, max(m_tMinimapSize.y + 58.f, 68.f)));
		SetPosition("MinimapMax_SW", POSITION(0.f, max(m_tMinimapSize.y + 40.f, 68.f)));

		SAFE_RELEASE(pObj);
	}
}

void CUIMinimap::SetMinimapTexture(CTexture * pMinimap)
{
	SAFE_RELEASE(m_pMinimapTexture);
	if (pMinimap)
	{
		m_pMinimapTexture = pMinimap;
		m_pMinimapTexture->AddRef();
	}
}

void CUIMinimap::UseMinimapIcon()
{
	m_eBeforeState = m_eState;
	m_eState = (MAP_STATE)((int)m_eState + 1);
	if (m_eState == MAP_END)
		m_eState = MAP_MIN;
	m_bMinimapChanged = true;
}

void CUIMinimap::UseIncrease()
{
	m_eBeforeState = m_eState;
	m_eState = (MAP_STATE)((int)m_eState + 1);
	m_bMinimapChanged = true;
}

void CUIMinimap::UseDecrease()
{
	m_eBeforeState = m_eState;
	m_eState = (MAP_STATE)((int)m_eState - 1);
	m_bMinimapChanged = true;
}

bool CUIMinimap::Init()
{
	if (!CUI::Init(UI_MINIMAP))
		return false;

	SetPos(0.f, 0.f);
	SetSize(128.f, 20.f);
	SetHeadSize(128.f, 20.f);

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);

	CTexture* pUI = AddUITexture("MinimapMin_C", L"UI/Minimap/MiniMapMin_C.png", POSITION(64.f, 0.f), tOrigin, tLT, POSITION(1.f, 20.f));
	m_mapMinimapTexture[MAP_MIN].insert(make_pair("C", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapMin_E", L"UI/Minimap/MiniMapMin_E.png", POSITION(65.f, 0.f), tOrigin, tLT, POSITION(64.f, 20.f));
	m_mapMinimapTexture[MAP_MIN].insert(make_pair("E", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapMin_W", L"UI/Minimap/MiniMapMin_W.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(64.f, 20.f));
	m_mapMinimapTexture[MAP_MIN].insert(make_pair("W", pUI));
	SAFE_RELEASE(pUI);

	//pUI = AddUITexture("MinimapSmall_C", L"UI/Minimap/MiniMap_C.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(64.f, 27.f));
	//m_mapMinimapTexture[MAP_SMALL].insert(make_pair("C", pUI));
	//SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapSmall_E", L"UI/Minimap/MiniMap_E.png", POSITION(120.f, 27.f), tOrigin, tLT, POSITION(9.f, 1.f));
	m_mapMinimapTexture[MAP_SMALL].insert(make_pair("E", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapSmall_N", L"UI/Minimap/MiniMap_N.png", POSITION(64.f, 0.f), tOrigin, tLT, POSITION(1.f, 21.f));
	m_mapMinimapTexture[MAP_SMALL].insert(make_pair("N", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapSmall_NE", L"UI/Minimap/MiniMap_NE.png", POSITION(65.f, 0.f), tOrigin, tLT, POSITION(64.f, 27.f));
	m_mapMinimapTexture[MAP_SMALL].insert(make_pair("NE", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapSmall_NW", L"UI/Minimap/MiniMap_NW.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(64.f, 27.f));
	m_mapMinimapTexture[MAP_SMALL].insert(make_pair("NW", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapSmall_S", L"UI/Minimap/MiniMap_S.png", POSITION(64.f, 46.f), tOrigin, tLT, POSITION(1.f, 9.f));
	m_mapMinimapTexture[MAP_SMALL].insert(make_pair("S", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapSmall_SE", L"UI/Minimap/MiniMap_SE.png", POSITION(65.f, 28.f), tOrigin, tLT, POSITION(64.f, 27.f));
	m_mapMinimapTexture[MAP_SMALL].insert(make_pair("SE", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapSmall_SW", L"UI/Minimap/MiniMap_SW.png", POSITION(0.f, 28.f), tOrigin, tLT, POSITION(64.f, 27.f));
	m_mapMinimapTexture[MAP_SMALL].insert(make_pair("SW", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapSmall_W", L"UI/Minimap/MiniMap_W.png", POSITION(0.f, 27.f), tOrigin, tLT, POSITION(9.f, 1.f));
	m_mapMinimapTexture[MAP_SMALL].insert(make_pair("W", pUI));
	SAFE_RELEASE(pUI);

	pUI = AddUITexture("MinimapMax_C", L"UI/Minimap/MaxMap_C.png", POSITION(64.f, 67.f), tOrigin, tLT, POSITION(1.f, 1.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("C", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapMax_E", L"UI/Minimap/MaxMap_E.png", POSITION(120.f, 67.f), tOrigin, tLT, POSITION(9.f, 1.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("E", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapMax_N", L"UI/Minimap/MaxMap_N.png", POSITION(64.f, 0.f), tOrigin, tLT, POSITION(1.f, 61.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("N", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapMax_NE", L"UI/Minimap/MaxMap_NE.png", POSITION(65.f, 0.f), tOrigin, tLT, POSITION(64.f, 67.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("NE", pUI));
	SAFE_RELEASE(pUI);
	/*pUI = AddUITexture("MinimapMax_NW", L"UI/Minimap/MaxMap_NW.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(64.f, 67.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("NW", pUI));
	SAFE_RELEASE(pUI);*/
	pUI = AddUITexture("MinimapMax_NW2", L"UI/Minimap/MaxMap_NW2.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(64.f, 67.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("NW2", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapMax_S", L"UI/Minimap/MaxMap_S.png", POSITION(64.f, 86.f), tOrigin, tLT, POSITION(1.f, 9.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("S", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapMax_SE", L"UI/Minimap/MaxMap_SE.png", POSITION(65.f, 68.f), tOrigin, tLT, POSITION(64.f, 27.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("SE", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapMax_SW", L"UI/Minimap/MaxMap_SW.png", POSITION(0.f, 68.f), tOrigin, tLT, POSITION(64.f, 27.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("SW", pUI));
	SAFE_RELEASE(pUI);
	pUI = AddUITexture("MinimapMax_W", L"UI/Minimap/MaxMap_W.png", POSITION(0.f, 67.f), tOrigin, tLT, POSITION(9.f, 1.f));
	m_mapMinimapTexture[MAP_MAX].insert(make_pair("W", pUI));
	SAFE_RELEASE(pUI);

	CButton* pButton = AddButton("MinimapDecreaseButton", L"UI/Minimap/Decrease/", POSITION(54.f, 3.f), tOrigin, tLT, POSITION(13.f, 12.f));
	pButton->SetIconFunction(this, &CUIMinimap::UseDecrease);
	pButton->SetButtonType(BT_DISABLE);
	SAFE_RELEASE(pButton);
	pButton = AddButton("MinimapIncreaseButton", L"UI/Minimap/Increase/", POSITION(68.f, 3.f), tOrigin, tLT, POSITION(13.f, 12.f));
	pButton->SetIconFunction(this, &CUIMinimap::UseIncrease);
	SAFE_RELEASE(pButton);
	pButton = AddButton("MinimapWorldButton", L"UI/Minimap/World/", POSITION(82.f, 3.f), tOrigin, tLT, POSITION(40.f, 12.f));
	SAFE_RELEASE(pButton);

	CKeyIcon* pIcon = AddKeyIcon("UIMinimapIcon", L"UI/Key/Icon_Minimap.png", "m");
	pIcon->SetDefaultKey("7");
	pIcon->SetIconFunction(this, &CUIMinimap::UseMinimapIcon);
	SAFE_RELEASE(pIcon);

	return true;
}

void CUIMinimap::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);
}

int CUIMinimap::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);

	if (m_bMinimapChanged)
	{
		m_bMinimapChanged = false;
		// 미니맵 크기와 head 사이즈,
		// 버튼의 위치, 활성화 상태 변경
		CButton* pButton;
		POSITION tPos = POSITION(0.f, 0.f);

		switch (m_eState)
		{
		case MAP_MIN:
			SetSize(64.f + 64.f + 1.f, 20.f);
			SetHeadSize(m_tSize.x, 20.f);
			tPos = FindPosition("MinimapMin_E");
			pButton = FindButton("MinimapDecreaseButton");
			if (pButton)
			{
				pButton->SetButtonType(BT_DISABLE);
				pButton->SetPos(tPos + POSITION(3.f, 0.f));
				SAFE_RELEASE(pButton);
			}
			pButton = FindButton("MinimapIncreaseButton");
			if (pButton)
			{
				pButton->SetButtonType(BT_NORMAL);
				SAFE_RELEASE(pButton);
			}
			break;
		case MAP_SMALL:
			SetSize(POSITION(max(21.f + m_tMinimapSize.x, 129.f), max(27.f + m_tMinimapSize.y, 55.f)));
			SetHeadSize(m_tSize.x, 20.f);
			tPos = FindPosition("MinimapSmall_NE");
			pButton = FindButton("MinimapDecreaseButton");
			if (pButton)
			{
				pButton->SetButtonType(BT_NORMAL);
				SAFE_RELEASE(pButton);
			}
			pButton = FindButton("MinimapIncreaseButton");
			if (pButton)
			{
				pButton->SetButtonType(BT_NORMAL);
				SAFE_RELEASE(pButton);
			}
			break;
		case MAP_MAX:
			SetSize(POSITION(max(21.f + m_tMinimapSize.x, 129.f), max(67.f + m_tMinimapSize.y, 95.f)));
			SetHeadSize(m_tSize.x, 60.f);
			tPos = FindPosition("MinimapMax_NE");
			pButton = FindButton("MinimapDecreaseButton");
			if (pButton)
			{
				pButton->SetButtonType(BT_NORMAL);
				SAFE_RELEASE(pButton);
			}
			pButton = FindButton("MinimapIncreaseButton");
			if (pButton)
			{
				pButton->SetButtonType(BT_DISABLE);
				SAFE_RELEASE(pButton);
			}
			break;
		default:
			break;
		}

		pButton = FindButton("MinimapIncreaseButton");
		if (pButton)
		{
			pButton->SetPos(tPos - POSITION(9.f, 0.f));
			SAFE_RELEASE(pButton);
		}
		pButton = FindButton("MinimapDecreaseButton");
		if (pButton)
		{
			pButton->SetPos(tPos + POSITION(3.f, 0.f));
			SAFE_RELEASE(pButton);
		}
		pButton = FindButton("MinimapWorldButton");
		if (pButton)
		{
			pButton->SetPos(tPos + POSITION(17.f, 0.f));
			SAFE_RELEASE(pButton);
		}
	}

	m_tPlayerMinimapPos = m_tMinimapSize * (m_pPlayer->GetPos() + POSITION(0.f, -33.f)) / m_tMapSize;

	return 0;
}

int CUIMinimap::LateUpdate(float fDeltaTime)
{
	CUI::LateUpdate(fDeltaTime);

	return 0;
}

void CUIMinimap::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);
}

void CUIMinimap::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	//CUI::Render(hDC, pGDIRenderTarget, pRenderTarget, fDeltaTime);

	// 다른 ui와 다르게 미니맵의 사이즈에 따라 텍스쳐/버튼의 위치가 바뀌므로
	// 별도의 Render함수를 만들어준다.

	map<string, CTexture*>::iterator iter;
	map<string, CTexture*>::iterator iterEnd = m_mapMinimapTexture[m_eState].end();

	for (iter = m_mapMinimapTexture[m_eState].begin(); iter != iterEnd; iter++)
	{
		if (!iter->second->GetEnable())
			continue;

		POSITION tPos = FindPosition(iter->second->GetTag()) + m_tPos;
		if (iter->second->GetTag().find("SE") != string::npos || iter->second->GetTag().find("SW") != string::npos ||
			iter->second->GetTag().find("NE") != string::npos || iter->second->GetTag().find("NW") != string::npos)
			iter->second->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
		else
		{
			if (iter->second->GetTag().find("MinimapMin") == string::npos)
			{
				if (iter->second->GetTag().find("W") != string::npos || iter->second->GetTag().find("E") != string::npos)
					iter->second->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime, 1.f, false, MD_LEFT, 270.f, 1.f, m_fWELength);
				else if (iter->second->GetTag().find("N") != string::npos || iter->second->GetTag().find("S") != string::npos)
					iter->second->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime, 1.f, false, MD_LEFT, 270.f, m_fNSLength, 1.f);
				else
					iter->second->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
			}
			else
				iter->second->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
		}
	}

	list<CButton*>::iterator iterButton;
	list<CButton*>::iterator iterButtonEnd = m_ButtonList.end();

	for (iterButton = m_ButtonList.begin(); iterButton != iterButtonEnd; iterButton++)
	{
		if (!(*iterButton)->GetEnable())
			continue;

		(*iterButton)->IconRender(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}

	if (m_pMinimapTexture)
	{
		ID2D1SolidColorBrush* pBrush;
		pRenderTarget->CreateSolidColorBrush(ColorF(ColorF::Red), &pBrush);
		if (m_eState == MAP_SMALL)
		{
			m_pMinimapTexture->Render(m_tPos + POSITION(max(m_tSize.x / 2.f - m_tMinimapSize.x / 2.f, 10.f), 19.f), pGDIRenderTarget, pRenderTarget, fDeltaTime);
			D2D1_POINT_2F tPoint;
			tPoint.x = m_tPos.x + max(m_tSize.x / 2.f - m_tMinimapSize.x / 2.f, 10.f) + m_tPlayerMinimapPos.x;
			tPoint.y = m_tPos.y + 19.f + m_tPlayerMinimapPos.y;
			D2D1_ELLIPSE tPlayerEllipse = Ellipse(tPoint, 2.f, 4.f);
			pRenderTarget->FillEllipse(tPlayerEllipse, pBrush);
		}
		if (m_eState == MAP_MAX)
		{
			m_pMinimapTexture->Render(m_tPos + POSITION(max(m_tSize.x / 2.f - m_tMinimapSize.x / 2.f, 10.f), 59.f), pGDIRenderTarget, pRenderTarget, fDeltaTime);
			D2D1_POINT_2F tPoint;
			tPoint.x = m_tPos.x + max(m_tSize.x / 2.f - m_tMinimapSize.x / 2.f, 10.f) + m_tPlayerMinimapPos.x;
			tPoint.y = m_tPos.y + 59.f + m_tPlayerMinimapPos.y;
			D2D1_ELLIPSE tPlayerEllipse = Ellipse(tPoint, 2.f, 4.f);
			pRenderTarget->FillEllipse(tPlayerEllipse, pBrush);
		}
		pBrush->Release();
	}
}

CUIMinimap * CUIMinimap::Clone()
{
	return new CUIMinimap(*this);
}
