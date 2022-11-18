#pragma once
#include "../StaticObj.h"

class CEditBox :
	public CStaticObj
{
public:
	CEditBox();
	CEditBox(const CEditBox& obj);
	virtual ~CEditBox();

private:
	HWND m_hEditBox; // edit box 윈도우 핸들
	int m_iId; // edit box id

	char* m_strText; // 텍스트를 받을 버퍼
	bool m_bEnter; // 엔터 입력

public:
	HWND GetHwnd() const { return m_hEditBox; }

public:
	HWND CreateEditBox(int id, RECTANGLE tRect, char* pBuffer, bool bIsPW = false);
	HWND CreateEditBox(int id, float l, float t, float r, float b, char* pBuffer, bool bIsPW = false);

public:
	void Reset();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CEditBox* Clone();
};

