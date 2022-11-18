#include "DBManager.h"
#include "../Object/Obj.h"
#include "../Object/MoveObj/Player/Player.h"
#include "../Scene/Scene.h"

#include "../Object/StaticObj/Item/Item.h"
#include "../Object/StaticObj/Skill/Skill.h"
#include "CriticalSectionObj.h"

DEF_SINGLE(CDBManager);

CDBManager::CDBManager():
	m_pCritic(NULL)
{
}

CDBManager::~CDBManager()
{
	// mysql 닫기
	m_pCritic->Enter();
	mysql_close(m_mysql);
	m_pCritic->Leave();

	SAFE_DELETE(m_pCritic);
}

bool CDBManager::Init()
{
	// mysql 초기화
	m_mysql = mysql_init(NULL);
	// 로그인
	if (!mysql_real_connect(m_mysql, NULL, "root", "0000", NULL, 3306, NULL, 0))
	{
		printf("MySQL connect failed!");
		return false;
	}

	// 임계영역 초기화
	m_pCritic = new CCriticalSectionObj;

	return true;
}

bool CDBManager::CreateAccount(const char * strID, const char * strPW)
{
	m_pCritic->Enter();

	char strTemp[1024] = {};
	// 초기화

	sprintf(strTemp, "SELECT * FROM user_db.user_account WHERE user_id = \"%s\"", strID);
	// SELECT * FROM user_db.user_account WHERE user_id = "id"
	mysql_query(m_mysql, strTemp);

	// 결과 저장
	m_mysqlRes = mysql_store_result(m_mysql);

	// 있었다면 fasle 리턴
	if (mysql_fetch_row(m_mysqlRes) != NULL)
	{
		printf("ID already exist!\n");
		mysql_free_result(m_mysqlRes);
		m_pCritic->Leave();

		return false;
	}
	mysql_free_result(m_mysqlRes);

	// 없었다면 새로 추가
	sprintf(strTemp, "INSERT INTO user_db.user_account VALUE(0, \"%s\", \"%s\")", strID, strPW);
	// INSERT INTO user_db.user_account VALUE(0, "id", "pw")
	mysql_query(m_mysql, strTemp);

	// 아이디로 새로운 db 만들기
	sprintf(strTemp, "CREATE DATABASE %s", strID);
	// CREATE DATABASE ID;

	mysql_query(m_mysql, strTemp);

	sprintf(strTemp, "USE %s", strID);
	// USE ID;
	mysql_query(m_mysql, strTemp);

	// status table
	mysql_query(m_mysql, "CREATE TABLE status(level int not null, exp int not null, maxhp int not null, hp int not null, maxmp int not null, mp int not null)");
	// 기본값
	// level 1, exp 0, maxhp = hp 50, maxmp = mp 10
	mysql_query(m_mysql, "INSERT INTO status VALUE(1, 0, 50, 50, 10, 10)");

	// skill table
	mysql_query(m_mysql, "CREATE TABLE skill(skill_step int, skill_name varchar(30) not null, skill_point int)");
	// 기본값 - 아델 스킬만
	mysql_query(m_mysql, "INSERT INTO skill VALUE(1, \"BoltIcon\", 0)");
	mysql_query(m_mysql, "INSERT INTO skill VALUE(1, \"PlainIcon\", 0)");
	mysql_query(m_mysql, "INSERT INTO skill VALUE(2, \"ShardIcon\", 0)");
	mysql_query(m_mysql, "INSERT INTO skill VALUE(3, \"ExpertIcon\", 0)");
	mysql_query(m_mysql, "INSERT INTO skill VALUE(4, \"DivideIcon\", 0)");

	// item table
	mysql_query(m_mysql, "CREATE TABLE item(item_name varchar(30) not null, item_type int not null, item_count int not null, x int not null, y int not null)");
	// 기본값 소비 아이템 = IT_USABLE = 1
	mysql_query(m_mysql, "INSERT INTO item VALUE(\"RedPotion\", 1, 1, 0, 0)");
	mysql_query(m_mysql, "INSERT INTO item VALUE(\"BluePotion\", 1, 1, 1, 0)");
	mysql_query(m_mysql, "INSERT INTO item VALUE(\"WhitePotion\", 1, 1, 2, 0)");
	mysql_query(m_mysql, "INSERT INTO item VALUE(\"ManaElixer\", 1, 1, 3, 0)");
	mysql_query(m_mysql, "INSERT INTO item VALUE(\"PowerElixer\", 1, 1, 0, 1)");
	mysql_query(m_mysql, "INSERT INTO item VALUE(\"TwilightDew\", 1, 1, 1, 1)");
	mysql_query(m_mysql, "INSERT INTO item VALUE(\"ReindeerMilk\", 1, 1, 2, 1)");
	mysql_query(m_mysql, "INSERT INTO item VALUE(\"MeltCheese\", 1, 1, 3, 1)");
	mysql_query(m_mysql, "INSERT INTO item VALUE(\"BrownGot\", 0, 1, 0, 0)");

	// equip table
	mysql_query(m_mysql, "CREATE TABLE equip(equip_name varchar(30) not null, equip_type int not null)");
	// 기본값 없음

	// position table
	mysql_query(m_mysql, "CREATE TABLE pos(scene_num int not null, x int not null, y int not null)");
	// 기본값
	mysql_query(m_mysql, "INSERT INTO pos VALUE(0, 400, 606)");

	m_pCritic->Leave();

	return true;
}

bool CDBManager::DeleteAccount(const char * strID)
{
	m_pCritic->Enter();

	// 계정에 있는 캐릭터명을 순서대로 delete character해주기
	char strTemp[1024] = {};

	// 아이디로 된 db 지우기
	sprintf(strTemp, "DROP DATABASE %s", strID);
	// DROP DATABASE ID
	mysql_query(m_mysql, strTemp);

	// user_account에서 id 지우기
	sprintf(strTemp, "DELETE FROM user_db.user_account WHERE user_id = \"%s\"", strID);
	// DELETE FROM user_db.user_account WHERE user_id = "ID"
	mysql_query(m_mysql, strTemp);

	// 계정 목록 auto_increment 재조정하기
	mysql_query(m_mysql, "ALTER TABLE user_db.user_account AUTO_INCREMENT = 1");
	mysql_query(m_mysql, "SET @COUNT = 0");
	mysql_query(m_mysql, "UPDATE user_db.user_account SET user_key = @COUNT := @COUNT + 1");

	m_pCritic->Leave();

	return true;
}

int CDBManager::LogIn(const char * strID, const char * strPW)
{
	m_pCritic->Enter();

	char strTemp[1024] = {};
	// 초기화
	sprintf(strTemp, "SELECT user_pw FROM user_db.user_account WHERE user_id = \"%s\"", strID);
	// SELECT user_pw FROM user_db.user_account WHERE user_id = "id"
	mysql_query(m_mysql, strTemp);

	// 결과 저장
	m_mysqlRes = mysql_store_result(m_mysql);

	// 없다면 0리턴
	if ((m_mysqlRow = mysql_fetch_row(m_mysqlRes)) == NULL)
	{
		printf("ID doesn't exist!\n");
		mysql_free_result(m_mysqlRes);
		m_pCritic->Leave();

		return 0;
	}
	else // 있다면 비밀번호 대조 후 비밀번호가 틀리면 1, 맞으면 2 리턴
	{
		int returnval = 1;
		if (strcmp(m_mysqlRow[0], strPW) == 0)
			returnval = 2;

		mysql_free_result(m_mysqlRes);
		m_pCritic->Leave();

		return returnval;
	}

	m_pCritic->Leave();
}

bool CDBManager::CharacterInit(CObj * pObj)
{
	m_pCritic->Enter();

	char strTemp[1024] = {};
	// 초기화
	sprintf(strTemp, "USE %s", pObj->GetTag().c_str());
	
	// USE ID;
	// ID database 사용
	mysql_query(m_mysql, strTemp);

	// status 초기화
	mysql_query(m_mysql, "SELECT * FROM status");

	m_mysqlRes = mysql_store_result(m_mysql);
	if (m_mysqlRes)
	{
		if ((m_mysqlRow = mysql_fetch_row(m_mysqlRes)) == NULL)
		{
			//printf("Status query Null!\n");
		}
		else
		{
			pObj->SetCriticEnter();
			CPlayer* pPlayer = (CPlayer*)pObj;
			pPlayer->SetLevel(atoi(m_mysqlRow[0])); // level
			pPlayer->SetExp(atoi(m_mysqlRow[1])); // exp
			pPlayer->SetMaxHP(atoi(m_mysqlRow[2])); // maxhp
			pPlayer->SetHP(atoi(m_mysqlRow[3])); // hp
			pPlayer->SetMaxMP(atoi(m_mysqlRow[4])); // mp
			pPlayer->SetMP(atoi(m_mysqlRow[5])); // mp
			pObj->SetCriticLeave();
		}
		mysql_free_result(m_mysqlRes);
	}	

	// skill 초기화
	mysql_query(m_mysql, "SELECT * FROM skill");

	m_mysqlRes = mysql_store_result(m_mysql);
	if (m_mysqlRes)
	{
		if ((m_mysqlRow = mysql_fetch_row(m_mysqlRes)) == NULL)
		{
			//printf("Skill query Null!\n");
		}
		else
		{
			do
			{
				CPlayer* pPlayer = (CPlayer*)pObj;
				pPlayer->CreateSkill(string(m_mysqlRow[1]), atoi(m_mysqlRow[0]), atoi(m_mysqlRow[2]));
			} while (m_mysqlRow = mysql_fetch_row(m_mysqlRes));
		}
		mysql_free_result(m_mysqlRes);
	}	

	// item 초기화
	mysql_query(m_mysql, "SELECT * FROM item");

	m_mysqlRes = mysql_store_result(m_mysql);

	if (m_mysqlRes)
	{
		if ((m_mysqlRow = mysql_fetch_row(m_mysqlRes)) == NULL)
		{
			//printf("Item query Null!\n");
		}
		else
		{
			do
			{
				CPlayer* pPlayer = (CPlayer*)pObj;
				CItem* pItem = pPlayer->CreateItem(string(m_mysqlRow[0]), (ITEM_TYPE)(atoi(m_mysqlRow[1])),
					POSITION(atoi(m_mysqlRow[3]), atoi(m_mysqlRow[4])), atoi(m_mysqlRow[2]));
				SAFE_RELEASE(pItem);
			} while (m_mysqlRow = mysql_fetch_row(m_mysqlRes));
		}
		mysql_free_result(m_mysqlRes);
	}	

	// equip 초기화
	mysql_query(m_mysql, "SELECT * FROM equip");

	m_mysqlRes = mysql_store_result(m_mysql);

	if (m_mysqlRes)
	{
		if ((m_mysqlRow = mysql_fetch_row(m_mysqlRes)) == NULL)
		{
			//printf("Equip query Null!\n");
		}
		else
		{
			do
			{
				CPlayer* pPlayer = (CPlayer*)pObj;
				CItem* pItem = pPlayer->CreateEquip(string(m_mysqlRow[0]), (EQUIP_TYPE)(atoi(m_mysqlRow[1])));
				SAFE_RELEASE(pItem);
			} while (m_mysqlRow = mysql_fetch_row(m_mysqlRes));
		}
		mysql_free_result(m_mysqlRes);
	}
	
	// 위치 초기화
	mysql_query(m_mysql, "SELECT * FROM pos");

	m_mysqlRes = mysql_store_result(m_mysql);

	if (m_mysqlRes)
	{
		if ((m_mysqlRow = mysql_fetch_row(m_mysqlRes)) == NULL)
		{
			//printf("Pos query Null!\n");
		}
		else
		{
			pObj->SetCriticEnter();
			CPlayer* pPlayer = (CPlayer*)pObj;
			pPlayer->SetPos(POSITION((float)atoi(m_mysqlRow[1]), (float)atoi(m_mysqlRow[2]))); // (x, y)
			pPlayer->SetScene(atoi(m_mysqlRow[0])); // scene
			pObj->SetCriticLeave();
		}
		mysql_free_result(m_mysqlRes);
	}

	m_pCritic->Leave();

	return true;
}

bool CDBManager::SaveCharacter(CObj * pObj)
{
	m_pCritic->Enter();

	char strTemp[1024] = {};
	// 초기화
	sprintf(strTemp, "USE %s", pObj->GetTag().c_str());
	// USE ID;
	// ID database 사용
	mysql_query(m_mysql, strTemp);

	// status 변경
	CPlayer* pPlayer = (CPlayer*)pObj;
	int iLevel = pPlayer->GetLevel();
	int iExp = pPlayer->GetExp();
	int iMaxHp = pPlayer->GetMaxHP();
	int iHp = pPlayer->GetHP();
	int iMaxMp = pPlayer->GetMaxMP();
	int iMp = pPlayer->GetMP();

	sprintf(strTemp, "UPDATE status SET level = \"%d\", exp = \"%d\", maxhp = \"%d\", hp = \"%d\", maxmp = \"%d\", mp = \"%d\"",
		iLevel, iExp, iMaxHp, iHp, iMaxMp, iMp);
	// UPDATE status SET level = "iLevel", exp = "iExp", maxhp = "iMaxHp", hp = "iHp", maxmp = "iMaxMp", mp = "iMp";

	mysql_query(m_mysql, strTemp);

	// skill 변경
	list<CSkill*>::iterator iter;
	list<CSkill*>::iterator iterEnd = pPlayer->m_SkillList.end();

	for (iter = pPlayer->m_SkillList.begin(); iter != iterEnd; iter++)
	{
		sprintf(strTemp, "UPDATE skill SET skill_point = \"%d\" where skill_name = \"%s\"", (*iter)->GetLevel(), (*iter)->GetTag().c_str());
		// UPDATE skill SET skill_point = "skillPoint" where skill_name = "skillName";
		mysql_query(m_mysql, strTemp);
	}

	// item 변경
	list<CItem*>::iterator iterItem;
	list<CItem*>::iterator iterItemEnd = pPlayer->m_ItemList.end();

	for (iterItem = pPlayer->m_ItemList.begin(); iterItem != iterItemEnd; iterItem++)
	{
		sprintf(strTemp, "UPDATE item SET item_count = \"%d\", x = \"%d\", y = \"%d\" where item_name = \"%s\"",
			(*iterItem)->GetNum(), int((*iterItem)->GetCoord().x), int((*iterItem)->GetCoord().y), (*iterItem)->GetTag().c_str());
		// UPDATE item SET item_count = "iNum", x = "x", y = "y" where item_name = "itemName";
		mysql_query(m_mysql, strTemp);
	}

	// pos 변경
	int iSceneNum = pPlayer->GetScene()->GetNum();
	int x = (int)(pPlayer->GetPos().x);
	int y = (int)(pPlayer->GetPos().y);

	sprintf(strTemp, "UPDATE pos SET scene_num = \"%d\", x = \"%d\", y = \"%d\"",
		iSceneNum, x, y);
	// UPDATE pos SET scene_num = "iSceneNum", x = "x", y = "y";

	mysql_query(m_mysql, strTemp);

	m_pCritic->Leave();

	return true;
}

bool CDBManager::AddItem(const string & strPlayerName, CItem * pItem)
{
	m_pCritic->Enter();

	char strTemp[1024] = {};
	// 초기화

	sprintf(strTemp, "USE %s", strPlayerName.c_str());
	// USE ID;
	// ID database 사용
	mysql_query(m_mysql, strTemp);

	// 아이템 정보대로 추가
	sprintf(strTemp, "INSERT INTO item VALUE(\"%s\", %d, %d, %d, %d)",
		pItem->GetTag().c_str(), (int)pItem->GetItemType(), pItem->GetNum(), int(pItem->GetCoord().x), int(pItem->GetCoord().y));

	mysql_query(m_mysql, strTemp);

	m_pCritic->Leave();

	return true;
}

bool CDBManager::EraseItem(const string & strPlayerName, CItem * pItem)
{
	m_pCritic->Enter();

	char strTemp[1024] = {};

	sprintf(strTemp, "USE %s", strPlayerName.c_str());
	// USE ID;
	// ID database 사용
	mysql_query(m_mysql, strTemp);

	// 아이템 삭제
	sprintf(strTemp, "DELETE FROM item where item_name = \"%s\"", pItem->GetTag().c_str());

	mysql_query(m_mysql, strTemp);

	m_pCritic->Leave();

	return true;
}

bool CDBManager::AddEquip(const string & strPlayerName, CItem * pItem)
{
	m_pCritic->Enter();

	char strTemp[1024] = {};

	sprintf(strTemp, "USE %s", strPlayerName.c_str());
	// USE ID;
	// ID database 사용
	mysql_query(m_mysql, strTemp);

	// 아이템 정보대로 추가
	sprintf(strTemp, "INSERT INTO equip VALUE(\"%s\", %d)",
		pItem->GetTag().c_str(), (int)pItem->GetEquipType());

	mysql_query(m_mysql, strTemp);

	m_pCritic->Leave();

	return true;
}

bool CDBManager::EraseEquip(const string & strPlayerName, CItem * pItem)
{
	m_pCritic->Enter();

	char strTemp[1024] = {};

	sprintf(strTemp, "USE %s", strPlayerName.c_str());
	// USE ID;
	// ID database 사용
	mysql_query(m_mysql, strTemp);

	// 아이템 삭제
	sprintf(strTemp, "DELETE FROM equip where equip_name = \"%s\"", pItem->GetTag().c_str());

	mysql_query(m_mysql, strTemp);

	m_pCritic->Leave();

	return true;
}
