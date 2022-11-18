#include "ShopNPC.h"

CShopNPC::CShopNPC()
{
}

CShopNPC::CShopNPC(const CShopNPC & obj) :
	CStaticObj(obj)
{
}

CShopNPC::~CShopNPC()
{
}

bool CShopNPC::Init()
{
	SetSize(44.f, 71.f);
	SetPivot(0.5f, 1.f);

	return true;
}

void CShopNPC::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

int CShopNPC::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);

	return 0;
}

int CShopNPC::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);
	return 0;
}

//void CShopNPC::Collision(float fDeltaTime)
//{
//	CStaticObj::Collision(fDeltaTime);
//}

CShopNPC * CShopNPC::Clone()
{
	return new CShopNPC(*this);
}
