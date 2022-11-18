#pragma once
#include "../Core/Ref.h"

class CTexture :
	public CRef
{
private:
	friend class CResourcesManager;

public:
	CTexture();
	~CTexture();

private:
	ID2D1Bitmap* m_pD2DBitmap;
	_SIZE m_tSize;
	POSITION m_tOrigin;
	POSITION m_tLT;
	POSITION m_tRB;

public:
	void SetSize(float x, float y)		{ m_tSize = _SIZE(x, y); }
	void SetSize(_SIZE tSize)			{ m_tSize = tSize; }
	void SetOrigin(float x, float y)	{ m_tOrigin = POSITION(x, y); }
	void SetOrigin(POSITION tOrigin)	{ m_tOrigin = tOrigin; }
	void SetLT(float x, float y)		{ m_tLT = POSITION(x, y); }
	void SetLT(POSITION tLT)			{ m_tLT = tLT; }
	void SetRB(float x, float y)		{ m_tRB = POSITION(x, y); }
	void SetRB(POSITION tRB)			{ m_tRB = tRB; }

public:
	ID2D1Bitmap* GetBitmap() const { return m_pD2DBitmap; }
	_SIZE GetSize() const { return m_tSize; }
	POSITION GetOrigin() const { return m_tOrigin; }
	POSITION GetLT() const { return m_tLT; }
	POSITION GetRB() const { return m_tRB; }

public:
	void Render(POSITION& tPos, ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime, float opacity = 1.f, bool bCam = false, MOVE_DIR eDir = MD_LEFT, float fAngle = 270.f, float fWidth = 1.f, float fHeight = 1.f);
};

