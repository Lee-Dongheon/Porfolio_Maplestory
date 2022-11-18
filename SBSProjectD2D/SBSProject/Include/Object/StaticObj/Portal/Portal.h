#pragma once
#include "../StaticObj.h"

class CPortal :
	public CStaticObj
{
public:
	CPortal();
	CPortal(const CPortal& portal);
	~CPortal();

private:
	int m_iSceneNum;

public:
	void SetSceneNum(int iSceneNum) { m_iSceneNum = iSceneNum; }

public:
	int GetSceneNum() const { return m_iSceneNum; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CPortal* Clone();

public:
	void ChangeScene(CCollider* pSrc, CCollider* pDest, float fDeltaTime);
};

