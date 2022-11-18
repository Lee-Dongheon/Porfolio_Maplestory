#pragma once
#include "../Monster.h"

class CMushroom :
	public CMonster
{
public:
	CMushroom();
	CMushroom(const CMushroom& monster);
	virtual ~CMushroom();

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CMushroom* Clone();
};

