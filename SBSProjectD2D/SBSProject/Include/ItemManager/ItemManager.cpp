#include "ItemManager.h"

DEF_SINGLE(CItemManager);

CItemManager::CItemManager()
{
}

CItemManager::~CItemManager()
{
}

void CItemManager::AddItem(const string & strTag, ITEM_TYPE eItemType)
{
	ITEM_TYPE eType = FindItemType(strTag);
	if (eType == IT_DEFAULT)
		m_mapItem.insert(make_pair(strTag, eItemType));
}

ITEM_TYPE CItemManager::FindItemType(const string & strTag)
{
	map<string, ITEM_TYPE>::iterator iter = m_mapItem.find(strTag);

	if (iter == m_mapItem.end())
		return IT_DEFAULT;

	return iter->second;
}

bool CItemManager::Init()
{
	m_mapItem.insert(make_pair("RedPotion",IT_USABLE));
	m_mapItem.insert(make_pair("BluePotion", IT_USABLE));
	m_mapItem.insert(make_pair("YellowPotion", IT_USABLE));
	m_mapItem.insert(make_pair("WhitePotion", IT_USABLE));
	m_mapItem.insert(make_pair("ManaElixer", IT_USABLE));
	m_mapItem.insert(make_pair("PowerElixer", IT_USABLE));
	m_mapItem.insert(make_pair("TwilightDew", IT_USABLE));
	m_mapItem.insert(make_pair("ReindeerMilk", IT_USABLE));
	m_mapItem.insert(make_pair("MeltCheese", IT_USABLE));
	m_mapItem.insert(make_pair("OrangeHead", IT_ETC));
	m_mapItem.insert(make_pair("GreenHead", IT_ETC));
	m_mapItem.insert(make_pair("MushmomPoja", IT_ETC));
	m_mapItem.insert(make_pair("BlueGown", IT_EQUIP));
	m_mapItem.insert(make_pair("BrownGot", IT_EQUIP));

	return true;
}
