#pragma once
#include "../StaticObj.h"

class CShopNPC :
	public CStaticObj
{
public:
	CShopNPC();
	CShopNPC(const CShopNPC& obj);
	virtual ~CShopNPC();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CShopNPC* Clone();
};