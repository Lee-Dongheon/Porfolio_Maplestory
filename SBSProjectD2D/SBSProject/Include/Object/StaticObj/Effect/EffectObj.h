#pragma once
#include "../StaticObj.h"
class CEffectObj :
	public CStaticObj
{
private:
	friend class CMonster;

public:
	CEffectObj();
	CEffectObj(const CEffectObj& effect);
	~CEffectObj();

private:
	CObj* m_pObj;
	float m_fDelay;
	float m_fMaxDelay;

public:
	void SetObj(CObj* pObj) { m_pObj = pObj; }
	void SetMaxDelay(float fMaxDelay) { m_fMaxDelay = fMaxDelay; }
	void AddDelay(float fDelay);

public:
	CObj* GetObj() const { return m_pObj; }

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CEffectObj* Clone();
};

