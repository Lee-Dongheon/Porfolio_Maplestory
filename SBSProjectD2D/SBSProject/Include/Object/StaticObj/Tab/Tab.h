#pragma once
#include "../StaticObj.h"

class CTab :
	public CStaticObj
{
public:
	CTab();
	CTab(const CTab& obj);
	virtual ~CTab();

protected:
	class CUI* m_pUI; // tab을 가진 ui
	TAB_TYPE m_eTabType; // 버튼 상태
	CTexture* m_pTab[TT_END];

public:
	void EnableTab() { m_eTabType = TT_ENABLE; m_tSize.y += 2.f; }
	void DisableTab() { m_eTabType = TT_DISABLE; m_tSize.y -= 2.f; }

public:
	TAB_TYPE GetTabType() const { return m_eTabType; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CTab* Clone();

public:
	void LoadTab(const string& strTag, const wchar_t* pFileName, POSITION& arrOrigin, POSITION& arrLT, POSITION& arrRB);
};

