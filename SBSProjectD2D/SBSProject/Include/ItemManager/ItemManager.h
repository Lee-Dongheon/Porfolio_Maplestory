#pragma once
#include "../Game.h"

class CItemManager
{
	DECLARE_SINGLE(CItemManager)

private:
	map<string, ITEM_TYPE> m_mapItem;

public:
	void AddItem(const string& strTag, ITEM_TYPE eItemType);

public:
	ITEM_TYPE FindItemType(const string& strTag);

public:
	bool Init();

};

