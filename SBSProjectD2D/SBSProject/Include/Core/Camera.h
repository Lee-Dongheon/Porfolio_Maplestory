#pragma once
#include "../Game.h"

class CObj;

class CCamera
{
	DECLARE_SINGLE(CCamera)

private:
	POSITION m_tPos; // Ä«¸Þ¶óÀÇ ÁÂ»ó´Ü ÁÂÇ¥
	RESOLUTION m_tClientRS;
	RESOLUTION m_tWorldRS;
	POSITION m_tPivot;
	CObj* m_pTarget;

public:
	POSITION GetPos() const { return m_tPos; }

public:
	void SetTarget(CObj* pObj);
	void SetPivot(const POSITION& tPivot) { m_tPivot = tPivot; }
	void SetPivot(float x, float y) { m_tPivot = POSITION(x, y); }
	void SetPos(const POSITION& tPos) { m_tPos = tPos; }
	void SetPos(float x, float y) { m_tPos = POSITION(x, y); }
	void SetClientResolution(const RESOLUTION& tRS) { m_tClientRS = tRS; }
	void SetClientResolution(int x, int y) { m_tClientRS = RESOLUTION(x, y); }
	void SetWorldResolution(const RESOLUTION& tRS) { m_tWorldRS = tRS; }
	void SetWorldResolution(int x, int y) { m_tWorldRS = RESOLUTION(x, y); }

public:
	bool Init(const POSITION& tPos, const RESOLUTION& tRS, const RESOLUTION& tWorldRS);
	void Input(float fDeltaTime);
	void Update(float fDeltaTime);
};

