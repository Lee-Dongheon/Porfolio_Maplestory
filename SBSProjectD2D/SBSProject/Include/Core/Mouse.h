#pragma once
#include "../Game.h"

class CMouse
{
	DECLARE_SINGLE(CMouse)

private:
	POINT m_tPos;
	bool m_bIsClicked;
	bool m_bMouseUp;
	POSITION m_tMoveDist;
	//HWND m_hEditBox; // 마우스로 최근에 선택된 에디트박스

public:
	void SetPos(POINT tPos) { m_tPos = tPos; }
	void SetPos(int x, int y) { m_tPos.x = x; m_tPos.y = y; }
	void SetClicked(bool bIsClicked) { m_bIsClicked = bIsClicked; }
	void SetMouseUp(bool bMouseUp) { m_bMouseUp = bMouseUp; }
	/*void SetEditBox(HWND& hEditBox) { m_hEditBox = hEditBox; }*/

public:
	POSITION GetPos() const { return POSITION(m_tPos); }
	bool GetClicked() const { return m_bIsClicked; }
	bool GetMouseUp() const { return m_bMouseUp; }
	POSITION GetMoveDist() const { return m_tMoveDist; }
	/*HWND GetEditBox() const { return m_hEditBox; }*/

public:
	bool Init();
	void Input(float fDeltaTime);
	int Update(HWND hWnd, float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Render(HDC hDC, float fDeltaTime);

};

