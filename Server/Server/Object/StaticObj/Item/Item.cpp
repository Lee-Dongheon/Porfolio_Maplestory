#include "Item.h"

CItem::CItem():
	m_iNum(0),
	m_eItemType(IT_DEFAULT),
	m_fThrowTime(0.f),
	m_tCoord(POSITION(0.f, 0.f)),
	m_eEquipType(ET_DEFAULT)
{
}

CItem::CItem(const CItem & obj)
{
	m_iNum = obj.m_iNum;
	m_eItemType = obj.m_eItemType;
	m_fThrowTime = 0.f;
	m_tCoord = obj.m_tCoord;
}

CItem::~CItem()
{
}

bool CItem::Init()
{
	SetObjType(OT_ITEM);

	return true;
}

void CItem::Input(float fDeltaTime)
{
}

int CItem::Update(float fDeltaTime)
{
	m_fThrowTime += fDeltaTime;

	// 버린 지 15초가 지나면 제거
	if (m_fThrowTime >= 15.f)
	{
		m_fThrowTime -= 15.f;
		Die();		
	}

	return 0;
}

int CItem::LateUpdate(float fDeltaTime)
{
	return 0;
}

//void CItem::Collision(float fDeltaTime)
//{
//}

CItem * CItem::Clone()
{
	return new CItem(*this);
}
