#include "UILayer.h"
#include "../Object/Obj.h"
#include "../Object/StaticObj/UI/UI.h"
#include "../Object/StaticObj/UI/UIMain.h"
#include "../Core/Mouse.h"
#include "../Math.h"
#include "../Object/StaticObj/Icon/Icon.h"

CUILayer::CUILayer():
	m_strSelectedUI(""),
	m_pPlayer(NULL)
{
}

CUILayer::~CUILayer()
{
	SAFE_RELEASE(m_pPlayer);
}

void CUILayer::ReleasePlayer()
{
	if (m_pPlayer)
	{
		// ui에서도 연결 해제
		list<CObj*>::iterator iter;
		list<CObj*>::iterator iterEnd = m_ObjList.end();

		for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
		{
			((CUI*)(*iter))->ReleasePlayer();
		}

		SAFE_RELEASE(m_pPlayer);
	}
}

void CUILayer::SetPlayer(CObj * pObj)
{
	m_pPlayer = pObj;
	m_pPlayer->AddRef();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		((CUI*)(*iter))->SetPlayer(m_pPlayer);
	}

	// ui 연결해주기
}

bool CUILayer::Init()
{
	CObj* pUI = CObj::FindObject("UIMain");
	if (!pUI)
		return false;
	AddObject(pUI);
	SAFE_RELEASE(pUI);

	pUI = CObj::FindObject("UIStat");
	if (!pUI)
		return false;
	AddObject(pUI);
	SAFE_RELEASE(pUI);

	pUI = CObj::FindObject("UISkill");
	if (!pUI)
		return false;
	AddObject(pUI);
	SAFE_RELEASE(pUI);

	pUI = CObj::FindObject("UIKey");
	if (!pUI)
		return false;
	AddObject(pUI);
	SAFE_RELEASE(pUI);

	pUI = CObj::FindObject("UIItem");
	if (!pUI)
		return false;
	AddObject(pUI);
	SAFE_RELEASE(pUI);

	pUI = CObj::FindObject("UIMinimap");
	if (!pUI)
		return false;
	AddObject(pUI);
	SAFE_RELEASE(pUI);

	pUI = CObj::FindObject("UIEquip");
	if (!pUI)
		return false;
	AddObject(pUI);
	SAFE_RELEASE(pUI);

	return true;
}

void CUILayer::Input(float fDeltaTime)
{
	POSITION tPos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Input(fDeltaTime);

		// UI 정렬을 위한 선택된 ui 찾기
		// 마우스 위치에 겹쳐있어도 가장 마지막 ui가 선택될 것
		if (bIsClicked)
		{
			POSITION tUIPos = (*iter)->GetPos();
			_SIZE tUISize = (*iter)->GetSize();
			RECTANGLE tUIRect = RECTANGLE(tUIPos.x, tUIPos.y, tUIPos.x + tUISize.x, tUIPos.y + tUISize.y);

			if (PointInRect(tUIRect, tPos) && ((CUI*)(*iter))->GetUIType() != UI_MAIN)
			{
				m_strSelectedUI = (*iter)->GetTag();
			}
		}

		if (!(*iter)->GetLife())
		{
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}
}

int CUILayer::Update(float fDeltaTime)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		if (m_strSelectedUI != "")
		{
			if (m_strSelectedUI == (*iter)->GetTag())
				((CUI*)(*iter))->SetZOrder(INT_MAX);
			else
				((CUI*)(*iter))->SetZOrder(1);
		}		

		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}

	// 정렬
	if (m_ObjList.size() >= 2)
		m_ObjList.sort(CUILayer::UISort);

	m_strSelectedUI = "";

	return 0;
}

int CUILayer::LateUpdate(float fDeltaTime)
{
	CLayer::LateUpdate(fDeltaTime);

	return 0;
}

void CUILayer::Collision(float fDeltaTime)
{
}

void CUILayer::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CLayer::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	if (CUI::m_pSelectedIcon)
	{
		POSITION tPos = GET_SINGLE(CMouse)->GetPos() - POSITION(16.f, 16.f);
		CUI::m_pSelectedIcon->MouseRender(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}
}

bool CUILayer::UISort(CObj * pUI1, CObj * pUI2)
{
	return ((CUI*)pUI1)->GetZOrder() < ((CUI*)pUI2)->GetZOrder(); // <= 사용 절대금지! ZOrder가 가장 큰 레이어가 앞으로 오게됨}
}