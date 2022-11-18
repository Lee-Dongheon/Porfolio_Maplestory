#pragma once
#include "Skill.h"

class CPassiveSkill :
	public CSkill
{
public:
	CPassiveSkill();
	CPassiveSkill(const CPassiveSkill& skill);
	virtual ~CPassiveSkill();

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CPassiveSkill* Clone() = 0;
};

