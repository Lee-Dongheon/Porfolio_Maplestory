#pragma once
#include "../Monster.h"
class CMushmom :
	public CMonster
{
public:
	CMushmom();
	CMushmom(const CMushmom& mushmom);
	virtual ~CMushmom();

private:
	bool m_bIsAttack;
	class CEffectObj* m_pEffectObj;

public:
	virtual bool Init();
	//virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CMushmom* Clone();

//protected:
//	void Attack();

};

