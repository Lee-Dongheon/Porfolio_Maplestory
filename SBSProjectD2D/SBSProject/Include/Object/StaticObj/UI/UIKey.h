#pragma once
#include "UI.h"
class CUIKey :
	public CUI
{
private:
	friend class CObj;

protected:
	CUIKey();
	CUIKey(const CUIKey& ui);
	virtual ~CUIKey();

private:
	map<string, POSITION> m_mapKeyBox; // 키 정보창에서의 위치
	// 키아이콘의 경우 존재는 하지만 지정되지 않을 수 있다
	// > "Extra"공간에 저장 (키 정보창에 올려져있지 않은것)

private:
	POSITION FindKeyPos(const string& strTag);
	string FindBoxTag(POSITION tPos);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CUIKey* Clone();

public:
	void KeyBoxInit();
};

