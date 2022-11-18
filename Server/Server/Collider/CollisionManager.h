#pragma once

#include "../server.h"

class CObj;

class CCollisionManager
{
	DECLARE_SINGLE(CCollisionManager)

private:
	list<CObj*> m_CollisionList;

public:
	void AddObject(CObj* pObj);
	void Collision(float fDeltaTime);
	bool Collision(CObj* pSrc, CObj* pDest, float fDeltaTime);
};

