#pragma once
#include "UI.h"

class CUIItem :
	public CUI
{
private:
	friend class CObj;

protected:
	CUIItem();
	CUIItem(const CUIItem& ui);
	virtual ~CUIItem();

public:
	POSITION m_tCoordinate; // 아이템 렌더를 시작할 좌표, 스크롤에 의해 변화
	ITEM_TYPE m_eItemTabType; // 현재 선택된 탭

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CUIItem* Clone();
};



