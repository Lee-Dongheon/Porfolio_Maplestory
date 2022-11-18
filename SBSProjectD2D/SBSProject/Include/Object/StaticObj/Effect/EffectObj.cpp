#include "EffectObj.h"
#include "../../MoveObj/MoveObj.h"
#include "../../../Core/Camera.h"

CEffectObj::CEffectObj():
	m_pObj(NULL),
	m_fDelay(0.f),
	m_fMaxDelay(0.f)
{
}

CEffectObj::CEffectObj(const CEffectObj & effect):
	CStaticObj(effect)
{
	m_iRef = 1;
	m_pObj = effect.m_pObj;
	m_fDelay = 0.f;
	m_fMaxDelay = m_fMaxDelay;
	m_bEnable = effect.m_bEnable;
}

CEffectObj::~CEffectObj()
{
}

void CEffectObj::AddDelay(float fDelay)
{
	m_fDelay += fDelay;
	if (m_fDelay > m_fMaxDelay)
	{
		m_fDelay = 0.f;
		SetEnable(true);
	}
}

bool CEffectObj::Init()
{
	SetEnable(false);

	return true;
}

int CEffectObj::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);

	if (m_pObj)
		SetPos(m_pObj->GetPos());

	return 0;
}

void CEffectObj::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CStaticObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CEffectObj * CEffectObj::Clone()
{
	return new CEffectObj(*this);
}
