#pragma once
#include "../Monster.h"

class CGreenMushroom :
	public CMonster
{
public:
	CGreenMushroom();
	CGreenMushroom(const CGreenMushroom& mushroom);
	virtual ~CGreenMushroom();

public:
	virtual bool Init();
	//virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CGreenMushroom* Clone();
};
