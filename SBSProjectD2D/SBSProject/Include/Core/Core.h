#pragma once

#include "../Game.h"

class CCore
{
	DECLARE_SINGLE(CCore)

private:
	static bool m_bLoop;

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HDC m_hDC;
	RESOLUTION m_tRS;

	float m_fFPS;

public:
	static bool m_bIsActivated;

public:
	static void SetActivate(bool bActivate) { m_bIsActivated = bActivate; }

public:
	static bool GetActivate() { return m_bIsActivated; }

public:
	RESOLUTION GetResolution() const { return m_tRS; }

public:
	void ChangeWindowRes(int iWidth, int iHeight);

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void Render(float fDeltaTime);

private:
	ATOM MyRegisterClass();
	BOOL Create();

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	void Exit();

public:
	HWND CreateEditBox(int id, float l, float t, float r, float b, bool bIsPW = false);
	HWND CreateEditBox(int id, RECTANGLE tRect, bool bIsPW = false);
};

