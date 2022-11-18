#pragma once

#include "../server.h"

class CDBManager
{
	DECLARE_SINGLE(CDBManager)

private:
	MYSQL* m_mysql;
	MYSQL_RES* m_mysqlRes;
	MYSQL_ROW m_mysqlRow;
	class CCriticalSectionObj* m_pCritic;

public:
	bool Init();
	bool CreateAccount(const char* strID, const char* strPW);
	bool DeleteAccount(const char* strID);
	int LogIn(const char* strID, const char* strPW);
	bool CharacterInit(class CObj* pObj);
	bool SaveCharacter(class CObj* pObj);
	bool AddItem(const string& strPlayerName, class CItem* pItem);
	bool EraseItem(const string& strPlayerName, class CItem* pItem);
	bool AddEquip(const string& strPlayerName, class CItem* pItem);
	bool EraseEquip(const string& strPlayerName, class CItem* pItem);
};

