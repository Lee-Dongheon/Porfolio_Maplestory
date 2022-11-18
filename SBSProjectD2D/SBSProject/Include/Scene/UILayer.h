#pragma once
#include "Layer.h"

class CUILayer :
	public CLayer
{
private:
	friend class CScene;
	friend class CSceneManager;

protected:
	CUILayer();

public:
	virtual ~CUILayer();

protected:
	CObj* m_pPlayer;
	string m_strSelectedUI;

protected:
	void ReleasePlayer();
	void SetPlayer(CObj* pObj);

public:
	bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	

public:
	static bool UISort(CObj * pL1, CObj * pL2);
};

