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
	POSITION m_tCoordinate; // ������ ������ ������ ��ǥ, ��ũ�ѿ� ���� ��ȭ
	ITEM_TYPE m_eItemTabType; // ���� ���õ� ��

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CUIItem* Clone();
};



