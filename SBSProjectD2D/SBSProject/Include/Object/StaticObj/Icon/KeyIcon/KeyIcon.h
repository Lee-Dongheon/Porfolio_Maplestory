#pragma once
#include "../Icon.h"
class CKeyIcon :
	public CIcon
{
public:
	CKeyIcon();
	CKeyIcon(const CKeyIcon& key);
	~CKeyIcon();

private:
	string m_strDefaultKey; // �������� default�� �־�� �� �ڸ�

public:
	void SetDefaultKey(const string& strDefaultKey) { m_strDefaultKey = strDefaultKey; }

public:
	string GetDefaultKey() const { return m_strDefaultKey; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CKeyIcon* Clone();
};

