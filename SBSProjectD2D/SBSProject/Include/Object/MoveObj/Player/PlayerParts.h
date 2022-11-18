#pragma once
#include "../../Obj.h"

class CPlayerParts :
	public CObj
{
public:
	CPlayerParts();
	CPlayerParts(const CPlayerParts& parts);
	~CPlayerParts();

private:
	CObj* m_pPlayer; // ÆÄÃ÷ ÁÖÀÎ
	OBJECT_STATE m_ePlayerObjState;

public:
	void InitParts(PART_TYPE eType);

public:
	void SetPlayer(CObj* pObj) { m_pPlayer = pObj; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);

	virtual CPlayerParts* Clone();
};

