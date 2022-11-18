#include "UIEquip.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"

CUIEquip::CUIEquip()
{
}

CUIEquip::CUIEquip(const CUIEquip & ui):
	CUI(ui)
{
}

CUIEquip::~CUIEquip()
{
}

POSITION CUIEquip::FindEquipPos(EQUIP_TYPE eType)
{
	map<EQUIP_TYPE, POSITION>::iterator iter = m_mapEquipPos.find(eType);

	if (iter == m_mapEquipPos.end())
		return POSITION();

	return iter->second;
}

bool CUIEquip::InEquipPos(EQUIP_TYPE eType, POSITION tPos)
{
	map<EQUIP_TYPE, POSITION>::iterator iter = m_mapEquipPos.find(eType);
	if (iter == m_mapEquipPos.end())
		return false;

	RECTANGLE tRect = RECTANGLE(m_tPos.x + iter->second.x, m_tPos.y + iter->second.y,
		m_tPos.x + iter->second.x + 40.f, m_tPos.y + iter->second.y + 39.f);

	return PointInRect(tRect, tPos);
}

bool CUIEquip::Init()
{
	if (!CUI::Init(UI_EQUIP))
		return false;

	// 텍스쳐, 버튼 등 초기화
	SetEnable(false);

	SetPos(300.f, 200.f);
	SetSize(232.f, 354.f);
	SetHeadSize(232.f, 19.f);

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);

	CTexture* pUITex = AddUITexture("EquipBackground", L"UI/Equip/Equip_Background.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(232.f, 354.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipBackground2", L"UI/Equip/Equip_Background2.png", POSITION(9.f, 44.f), tOrigin, tLT, POSITION(214.f, 256.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipTabbar", L"UI/Equip/Equip_Tabbar.png", POSITION(7.f, 41.f), tOrigin, tLT, POSITION(218.f, 4.f));
	SAFE_RELEASE(pUITex);

	pUITex = AddUITexture("EquipTabBackground", L"UI/Equip/Equip/Equip_Background.png", POSITION(55.f, 69.f), tOrigin, tLT, POSITION(127.f, 209.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipTabBackground2", L"UI/Equip/Equip/Equip_Background2.png", POSITION(10.f, 326.f), tOrigin, tLT, POSITION(212.f, 18.f));
	SAFE_RELEASE(pUITex);

	pUITex = AddUITexture("EquipSlot1", L"UI/Equip/Equip/Equip_Slot1.png", POSITION(96.f, 50.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	m_mapEquipPos.insert(make_pair(ET_HEAD, POSITION(96.f, 50.f)));
	pUITex = AddUITexture("EquipSlot2", L"UI/Equip/Equip/Equip_Slot2.png", POSITION(96.f, 91.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot3", L"UI/Equip/Equip/Equip_Slot3.png", POSITION(96.f, 132.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot4", L"UI/Equip/Equip/Equip_Slot4.png", POSITION(137.f, 132.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot5", L"UI/Equip/Equip/Equip_Slot5.png", POSITION(96.f, 173.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	m_mapEquipPos.insert(make_pair(ET_CLOTH, POSITION(96.f, 173.f)));
	pUITex = AddUITexture("EquipSlot6", L"UI/Equip/Equip/Equip_Slot6.png", POSITION(96.f, 214.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot7", L"UI/Equip/Equip/Equip_Slot7.png", POSITION(96.f, 255.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot8", L"UI/Equip/Equip/Equip_Slot8.png", POSITION(137.f, 214.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot9", L"UI/Equip/Equip/Equip_Slot9.png", POSITION(178.f, 214.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot10", L"UI/Equip/Equip/Equip_Slot10.png", POSITION(178.f, 173.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot11", L"UI/Equip/Equip/Equip_Slot11.png", POSITION(55.f, 173.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot12", L"UI/Equip/Equip/Equip_Slot12.png", POSITION(14.f, 173.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot13", L"UI/Equip/Equip/Equip_Slot13.png", POSITION(14.f, 132.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot14", L"UI/Equip/Equip/Equip_Slot14.png", POSITION(14.f, 91.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot15", L"UI/Equip/Equip/Equip_Slot15.png", POSITION(14.f, 50.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot16", L"UI/Equip/Equip/Equip_Slot16.png", POSITION(55.f, 132.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot17", L"UI/Equip/Equip/Equip_Slot17.png", POSITION(178.f, 132.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot18", L"UI/Equip/Equip/Equip_Slot18.png", POSITION(55.f, 214.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot19", L"UI/Equip/Equip/Equip_Slot19.png", POSITION(137.f, 173.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot20", L"UI/Equip/Equip/Equip_Slot20.png", POSITION(14.f, 214.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot21", L"UI/Equip/Equip/Equip_Slot21.png", POSITION(137.f, 255.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot22", L"UI/Equip/Equip/Equip_Slot22.png", POSITION(178.f, 255.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot23", L"UI/Equip/Equip/Equip_Slot23.png", POSITION(178.f, 91.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot24", L"UI/Equip/Equip/Equip_Slot24.png", POSITION(178.f, 50.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("EquipSlot25", L"UI/Equip/Equip/Equip_Slot25.png", POSITION(55.f, 91.f), tOrigin, tLT, POSITION(40.f, 39.f));
	SAFE_RELEASE(pUITex);

	// 0 : Equip ~ 5 : Cloth
	CTab* pTab = AddTab("EquipTab1", L"UI/Equip/EquipTab/", POSITION(9.f, 21.f), tOrigin, tLT, POSITION(42.f, 22.f));
	m_strLastTab = pTab->GetTag();
	SAFE_RELEASE(pTab);	

	CButton* pButton = AddButton("EquipClose", L"UI/Basic/Close/", POSITION(214.f, 6.f), tOrigin, tLT, POSITION(13.f, 13.f));
	pButton->SetIconFunction((CUI*)this, &CUI::CloseUI);
	SAFE_RELEASE(pButton);

	CKeyIcon* pIcon = AddKeyIcon("UIEquipIcon", L"UI/Key/Icon_Equip.png", "e");
	pIcon->SetDefaultKey("Extra7");
	pIcon->SetIconFunction((CUI*)this, &CUI::UseUIIcon);
	SAFE_RELEASE(pIcon);

	return true;
}

void CUIEquip::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);
}

int CUIEquip::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);

	map<EQUIP_TYPE, CItem*>::iterator iter;
	map<EQUIP_TYPE, CItem*>::iterator iterEnd = m_pPlayer->m_mapEquip.end();

	for (iter = m_pPlayer->m_mapEquip.begin(); iter != iterEnd; iter++)
	{
		POSITION tEquipPos = FindEquipPos(iter->first);
		// Equip ui의 위치 + Equip 칸 위치
		POSITION tPos = POSITION(m_tPos.x + tEquipPos.x, m_tPos.y + tEquipPos.y);
		iter->second->SetIconPos(tPos);

		CIcon* pIcon = (CIcon*)(iter->second);
		// 눌렀다면
		if (pIcon->IconUpdate(fDeltaTime) == 2 && !m_pSelectedIcon && !m_bHaveSelectedIcon)
		{
			m_pSelectedIcon = pIcon;
			m_pSelectedIcon->AddRef();
		}
	}

	// 아이템 오브젝트를 가지고 있는 상태에 장비창을 눌렀을때
	POSITION tMousePos = GET_SINGLE(CMouse)->GetPos();
	RECTANGLE tRect = RECTANGLE(m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y);
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();

	if (m_pSelectedIcon && m_bHaveSelectedIcon && bIsClicked)
	{
		if (m_pSelectedIcon->GetIconType() == IC_ITEM) // 아이템이고
		{
			if (PointInRect(tRect, tMousePos) &&
				(((CItem*)m_pSelectedIcon)->GetItemType() == IT_EQUIP)) //장비아이템인 경우만 처리
			{
				EQUIP_TYPE eType = ((CItem*)m_pSelectedIcon)->GetEquipType();

				// 올바른 위치인지 체크
				if (InEquipPos(eType, tMousePos)) // 맞다면
				{
					// 아이템 장착을 서버에게 알리기
					
					// 아이템 장착 시 아이템 맵에서 장비 맵으로 옮겨줘야함
					m_pPlayer->AddEquip((CItem*)m_pSelectedIcon);

					//printf("Equip!\n");
					SAFE_RELEASE(m_pSelectedIcon);
				}
			}
		}
	}

	return 0;
}

int CUIEquip::LateUpdate(float fDeltaTime)
{
	CUI::LateUpdate(fDeltaTime);
	return 0;
}

void CUIEquip::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);
}

void CUIEquip::Render(ID2D1GdiInteropRenderTarget * pGDIRenderTarget, ID2D1HwndRenderTarget * pRenderTarget, float fDeltaTime)
{
	CUI::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	map<EQUIP_TYPE, CItem*>::iterator iter;
	map<EQUIP_TYPE, CItem*>::iterator iterEnd = m_pPlayer->m_mapEquip.end();

	for (iter = m_pPlayer->m_mapEquip.begin(); iter != iterEnd; iter++)
	{
		CIcon* pIcon = (CIcon*)iter->second;
		pIcon->IconRender(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}
}

CUIEquip * CUIEquip::Clone()
{
	return new CUIEquip(*this);
}
