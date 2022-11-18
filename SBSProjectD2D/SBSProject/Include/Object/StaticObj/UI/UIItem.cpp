#include "UIItem.h"
#include "../../../Core/Core.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../../../Core/Input.h"
#include "../Icon/Item/Item.h"
#include "../../../Core/ServerManager.h"

CUIItem::CUIItem() :
	m_tCoordinate(POSITION(0.f, 0.f))
{
}

CUIItem::CUIItem(const CUIItem& ui) :
	CUI(ui)
{
	
}

CUIItem::~CUIItem()
{

}

bool CUIItem::Init()
{
	if (!CUI::Init(UI_ITEM))
		return false;

	SetEnable(false);

	SetPos(100.f, 100.f);
	SetSize(197.f, 380.f);
	SetHeadSize(197.f, 22.f);

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);

	CTexture* pUITex = AddUITexture("ItemBackground", L"UI/Item/Item_Background1.png", POSITION(0.f, 0.f), tOrigin, tLT, POSITION(197.f, 380.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("ItemBackground2", L"UI/Item/Item_Background2.png", POSITION(6.f, 24.f), tOrigin, tLT, POSITION(185.f, 351.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("ItemBackground3", L"UI/Item/Item_Background3.png", POSITION(7.f, 45.f), tOrigin, tLT, POSITION(183.f, 296.f));
	SAFE_RELEASE(pUITex);

	// 0 : Equip ~ 5 : Cloth
	CTab* pTab = AddTab("ItemTab0", L"UI/Item/Equip/", POSITION(9.f, 26.f), tOrigin, tLT, POSITION(29.f, 17.f));
	pTab->EnableTab();
	m_strLastTab = pTab->GetTag();
	m_eItemTabType = IT_EQUIP;
	SAFE_RELEASE(pTab);
	pTab = AddTab("ItemTab1", L"UI/Item/Usable/", POSITION(39.f, 28.f), tOrigin, tLT, POSITION(29.f, 17.f));
	SAFE_RELEASE(pTab);
	pTab = AddTab("ItemTab2", L"UI/Item/Etc/", POSITION(69.f, 28.f), tOrigin, tLT, POSITION(29.f, 17.f));
	SAFE_RELEASE(pTab);
	pTab = AddTab("ItemTab3", L"UI/Item/Install/", POSITION(99.f, 28.f), tOrigin, tLT, POSITION(29.f, 17.f));
	SAFE_RELEASE(pTab);
	pTab = AddTab("ItemTab4", L"UI/Item/Cash/", POSITION(129.f, 28.f), tOrigin, tLT, POSITION(29.f, 17.f));
	SAFE_RELEASE(pTab);
	pTab = AddTab("ItemTab5", L"UI/Item/Cloth/", POSITION(159.f, 28.f), tOrigin, tLT, POSITION(29.f, 17.f));
	SAFE_RELEASE(pTab);
	
	CButton* pButton = AddButton("Meso", L"UI/Item/Meso/", POSITION(9.f, 307.f), tOrigin, tLT, POSITION(40.f, 16.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Mpoint", L"UI/Item/Mpoint/", POSITION(9.f, 325.f), tOrigin, tLT, POSITION(82.f, 16.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Disassemble", L"UI/Item/Disassemble/", POSITION(9.f, 343.f), tOrigin, tLT, POSITION(28.f, 28.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Extract", L"UI/Item/Extract/", POSITION(39.f, 343.f), tOrigin, tLT, POSITION(28.f, 28.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Appraise", L"UI/Item/Appraise/", POSITION(69.f, 343.f), tOrigin, tLT, POSITION(28.f, 28.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Protect", L"UI/Item/Protect/", POSITION(99.f, 343.f), tOrigin, tLT, POSITION(28.f, 28.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Toad", L"UI/Item/Toad/", POSITION(129.f, 343.f), tOrigin, tLT, POSITION(28.f, 28.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Upgrade", L"UI/Item/Upgrade/", POSITION(159.f, 343.f), tOrigin, tLT, POSITION(28.f, 28.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Sort", L"UI/Item/Sort/", POSITION(154.f, 307.f), tOrigin, tLT, POSITION(16.f, 16.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Full", L"UI/Item/Full/", POSITION(172.f, 307.f), tOrigin, tLT, POSITION(16.f, 16.f));
	SAFE_RELEASE(pButton);

	pButton = AddButton("ItemClose", L"UI/Basic/Close/", POSITION(179.f, 7.f), tOrigin, tLT, POSITION(13.f, 13.f));
	pButton->SetIconFunction((CUI*)this, &CUI::CloseUI);
	SAFE_RELEASE(pButton);

	CKeyIcon* pIcon = AddKeyIcon("UIItemIcon", L"UI/Key/Icon_Item.png", "i");
	pIcon->SetDefaultKey("Extra5");
	pIcon->SetIconFunction((CUI*)this, &CUI::UseUIIcon);
	SAFE_RELEASE(pIcon);

	//SetVScroll("ItemVScroll", POSITION(176.f, 54.f), 243.f);

	////list<CIcon*>::iterator iterIcon;
	////list<CIcon*>::iterator iterIconEnd = m_ItemIconList.end();

	////int count = 0;
	////for (iterIcon = m_ItemIconList.begin(); iterIcon != iterIconEnd; iterIcon++)
	////{
	////	if (((CItem*)(*iterIcon)->GetObj())->GetItemType() == (ITEM_TYPE)m_iSelectedTab)
	////	{
	////		(*iterIcon)->AddRef();
	////		m_SelectedItemIconList.push_back(*iterIcon);
	////		POSITION tPos = m_tPos + POSITION(12.f, 95.f) + POSITION(144.f * (count % 2), 41.f * (count / 2)); // count ��° �������� ��ġ
	////		(*iterIcon)->SetPos(tPos);
	////		count++;
	////	}
	////}

	return true;
}

void CUIItem::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);
}

int CUIItem::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);

	// ���� ���õ� tab item_type���� ��ȯ
	// tab ������ ��ȣ�� �� item_type�̴�
	if(m_bTabChanged)
		m_eItemTabType = (ITEM_TYPE)(m_strLastTab.at(m_strLastTab.size() - 1) - 48);

	// Item Position�� ������Ʈ ���ֱ�
	map<POSITION, CItem*>::iterator iter;
	map<POSITION, CItem*>::iterator iterEnd = m_pPlayer->m_mapItem[m_eItemTabType].end();

	for (iter = m_pPlayer->m_mapItem[m_eItemTabType].begin(); iter != iterEnd; iter++)
	{
		POSITION tCoordinate = iter->first;
		// item ui�� ��ġ + item ĭ ���� ��ġ + ��ǥ x ��ǥ ����
		POSITION tPos = POSITION(m_tPos.x + 13.f + tCoordinate.x * 42.f, m_tPos.y + 54.f + tCoordinate.y * 42.f);
		iter->second->SetIconPos(tPos);

		CIcon* pIcon = (CIcon*)(iter->second);
		// �����ٸ�
		if (pIcon->IconUpdate(fDeltaTime) == 2 && !m_pSelectedIcon && !m_bHaveSelectedIcon)
		{
			m_pSelectedIcon = pIcon;
			m_pSelectedIcon->AddRef();
		}
	}

	// ������â�� �ƴ� �ٸ� ��ġ�� ������ �Լ��� ����ȴ�.
	// ���콺�� ��ġ���� üũ����.
	POSITION tMousePos = GET_SINGLE(CMouse)->GetPos();
	RECTANGLE tRect = RECTANGLE(m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y);
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();

	if (m_pSelectedIcon && m_bHaveSelectedIcon && bIsClicked)
	{
		if (m_pSelectedIcon->GetIconType() == IC_ITEM) // �����ۿ��� ������ �����ܸ� ó��
		{
			// ������ â ���� �������� üũ, ���̶�� position ��ȭ
			if (PointInRect(tRect, tMousePos))
			{
				// ��ǥ ã��
				POSITION tCoord;
				tCoord.x = floorf((tMousePos.x - m_tPos.x - 13.f) / 42.f);
				tCoord.y = floorf((tMousePos.y - m_tPos.y - 54.f) / 42.f);

				// �������� ����� ���
				if (((CItem*)m_pSelectedIcon)->GetItemType() == IT_EQUIP)
				{
					EQUIP_TYPE eType = ((CItem*)m_pSelectedIcon)->GetEquipType();
					// �ش� ����� ���� Ȯ��
					CItem* pEquip = m_pPlayer->FindEquip(eType);
					if (pEquip) // �ش� ������ ��� ����Ǿ� �ְ�
					{
						if (m_pSelectedIcon == pEquip) // �װ� ����ִ� �������̶��
						{
							CItem* pBeforeItem = m_pPlayer->FindItem(tCoord, IT_EQUIP);
							if (pBeforeItem) // ���� �ϰ�
							{
								// 1) ������â �Ű��� ��ġ�� �ִ� ���� ���� ������� ���� �ڸ���ü
								if (pEquip->GetEquipType() == pBeforeItem->GetEquipType())
								{
									m_pPlayer->EraseEquip(pEquip); // ��񿡼� �����
									m_pPlayer->AddEquip(pBeforeItem); // ���� ��� �����ϰ�
									// ���� ������ ������â���� before item�� ��������
									m_pPlayer->AddItem(pEquip, tCoord); // ������ â�� �߰�
									// ������ �˸�
									GET_SINGLE(CServerManager)->SendGetItem(pEquip->GetTag(), pEquip->GetItemType(), POSITION(-1.f, -1.f), tCoord);
								}
								// 2) ������â �Ű��� ��ġ�� �ִ� ���� �ٸ� ������� �׳� ���
								SAFE_RELEASE(pBeforeItem);
							}
							else // �������� ������ �׳� ������ �߰����ֱ�
							{
								m_pPlayer->EraseEquip(pEquip); // ��񿡼� �����
								m_pPlayer->AddItem(pEquip, tCoord); // ������ â�� �߰�
								// ������ �˸�
								GET_SINGLE(CServerManager)->SendGetItem(pEquip->GetTag(), pEquip->GetItemType(), POSITION(-1.f, -1.f), tCoord);
							}
						}
						else // ��� �ִ°Ͱ� �޶��ٸ� �׳� �ű��!
							m_pPlayer->ChangeItemPos(m_tPos, tCoord, (CItem*)m_pSelectedIcon);

						SAFE_RELEASE(pEquip);
					}
					else // ��� ����Ǿ� ���� ������
					{
						m_pPlayer->ChangeItemPos(m_tPos, tCoord, (CItem*)m_pSelectedIcon);
					}
				}
				else // ��� �ƴ� ��� �׳� �ű��
				{
					m_pPlayer->ChangeItemPos(m_tPos, tCoord, (CItem*)m_pSelectedIcon);
				}
				
				SAFE_RELEASE(m_pSelectedIcon);
			}
			else // ������ â ���̶��
			{
				// ���â�� �������� �ƴ϶�� ������
				CUI* pUI = FindUI(UI_EQUIP);

				if (pUI)
				{
					// Ȱ��ȭ �Ǿ��ְ�, â�� ũ�� ���� ���� ���콺 ��ġ�� �ִٸ� ����
					// ������ �ؼ� Equipâ���� �˻� ��ȸ �ο�
					RECTANGLE tRect(pUI->GetPos().x, pUI->GetPos().y,
						pUI->GetPos().x + pUI->GetSize().x, pUI->GetPos().y + pUI->GetSize().y);
					
					if (pUI->GetEnable() && PointInRect(tRect, tMousePos))
					{
						return 0;
					}
				}

				// �� �� ������ �������� Ŭ���̾�Ʈ���� �����ܰ� ���ÿ�
				// ������ �˷��� scene�� item�� ������־�� ��
				CItem* pItem = (CItem*)m_pSelectedIcon;
				// �÷��̾� ��ġ�� �����
				pItem->SetPos(m_pPlayer->m_tPos);
				// ������ �˸���
				GET_SINGLE(CServerManager)->SendThrowItem(pItem->GetTag(), pItem->GetItemType(), pItem->GetNum(), m_pPlayer->m_tPos);
				// �÷��̾� ������ ��Ͽ����� �����ֱ�
				m_pPlayer->EraseItem(pItem);
				// ������ �����ܵ� �����ֱ�
				SAFE_RELEASE(m_pSelectedIcon);
			}
		}
	}

	return 0;
}

int CUIItem::LateUpdate(float fDeltaTime)
{
	CUI::LateUpdate(fDeltaTime);

	return 0;
}

void CUIItem::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);
}

void CUIItem::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CUI::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	// item render
	map<POSITION, CItem*>::iterator iter;
	map<POSITION, CItem*>::iterator iterEnd = m_pPlayer->m_mapItem[m_eItemTabType].end();

	for (iter = m_pPlayer->m_mapItem[m_eItemTabType].begin(); iter != iterEnd; iter++)
	{
		CIcon* pIcon = (CIcon*)iter->second;
		pIcon->IconRender(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}

	// ���������� ���� ǥ�� X
	if (m_eItemTabType == IT_EQUIP)
		return;

	HDC hDC;
	RECT tRect;
	tRect.left = m_tPos.x;
	tRect.top = m_tPos.y;
	tRect.right = m_tPos.x + m_tSize.x;
	tRect.bottom = m_tPos.y + m_tSize.y;

	pGDIRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);

	HFONT newFont, oldFont;
	GET_SINGLE(CResourcesManager)->SetFont(newFont, oldFont, hDC, 10);
	SetBkMode(hDC, TRANSPARENT);

	for (iter = m_pPlayer->m_mapItem[m_eItemTabType].begin(); iter != iterEnd; iter++)
	{
		char temp[5];
		memset(temp, '\0', 5);
		sprintf(temp, "%d", iter->second->GetNum());
		TextOutA(hDC, iter->second->GetIconPos().x, iter->second->GetIconPos().y + 23, temp, strlen(temp));
	}

	GET_SINGLE(CResourcesManager)->ReleaseFont(newFont, oldFont, hDC);
	pGDIRenderTarget->ReleaseDC(&tRect);
}

CUIItem* CUIItem::Clone()
{
	return new CUIItem(*this);
}