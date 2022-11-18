#pragma once
#include "UI.h"
class CUIEquip :
	public CUI
{
private:
	friend class CObj;

protected:
	CUIEquip();
	CUIEquip(const CUIEquip& ui);
	virtual ~CUIEquip();

private:
	map<EQUIP_TYPE, POSITION> m_mapEquipPos;

private:
	POSITION FindEquipPos(EQUIP_TYPE eType);
	bool InEquipPos(EQUIP_TYPE eType, POSITION tPos);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);

	virtual CUIEquip* Clone();
};

