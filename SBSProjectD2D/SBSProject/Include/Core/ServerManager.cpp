#include "ServerManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include <process.h>
#include "../Object/StaticObj/UI/TitleUI.h"

#include "../Object/Obj.h"
#include "../Object/MoveObj/Player/OtherPlayer.h"
#include "../Object/MoveObj/Player/Player.h"
#include "../Object/StaticObj/UI/UIMain.h"
#include "Core.h"

DEF_SINGLE(CServerManager);

//// worker thread 호출함수
//unsigned int WINAPI CallWorkerThread(void *p)
//{
//	GET_SINGLE(CServerManager)->WorkerThread();
//	return 0;
//}

unsigned int WINAPI CallWorkerThread(void *p)
{
	GET_SINGLE(CServerManager)->Run();
	return 0;
}

CServerManager::CServerManager() :
	m_iServerPort(9190),
	//m_strServerAddr("192.168.25.5") // 공유기 아이피로 접속하는 주소
	// m_strServerAddr("121.125.57.116") // 외부주소
	m_strServerAddr("127.0.0.1"), // 같은 컴퓨터 내 주소
	m_hThread(NULL),
	m_iLoginSceneNum(0)
	/*m_bWorkerThread(true),
	m_hIocp(NULL),
	m_pWorkerThreadHandle(NULL)*/
{
}

CServerManager::~CServerManager()
{
	DisconnectServer();
	// join worker thread
	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, 3000);
		CloseHandle(m_hThread);
		//delete m_hThread;
	}
	/*for (int i = 0; i < m_iThreads; i++)
	{
		WaitForSingleObject(m_pWorkerThreadHandle[i], 3000);
	}
	SAFE_DELETE_ARR(m_pWorkerThreadHandle);*/
	closesocket(m_hServSock);
	WSACleanup();
}

bool CServerManager::Init()
{
	// 소켓 라이브러리 초기화(version 2.2)
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
	{
		//printf("WSAStartup failed!\n");
		return false;
	}


	m_hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	//m_hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 소켓 생성, tcp/ip 통신
															 // 소켓 생성 실패 예외처리
	if (m_hServSock == INVALID_SOCKET)
	{
		//printf("Socket init error!\n");
		return false;
	}

	memset(&m_ServAddr, 0, sizeof(m_ServAddr)); // 서버 주소 구조체 메모리 초기화
	m_ServAddr.sin_family = AF_INET;
	m_ServAddr.sin_addr.s_addr = inet_addr(m_strServerAddr); // 서버 ip 변환하여 넣어주기
	m_ServAddr.sin_port = htons(m_iServerPort); // 서버 포트 변환하여 넣어주기

	// 생성된 소켓을 바탕으로 서버에 connect 시도
	if (::connect(m_hServSock, (SOCKADDR*)&m_ServAddr, sizeof(m_ServAddr)) == SOCKET_ERROR)
	{
		//printf("Connect error!\n");
		return false;
	}

	unsigned int threadID;
	//m_hThread = new HANDLE;
	m_hThread = (HANDLE*)_beginthreadex(NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &threadID);

	if (!m_hThread)
	{
		//printf("Run thread error!\n");
		return false;
	}

	ResumeThread(m_hThread);

	return true;
}
void CServerManager::Run()
{
	char* buffer = new char[PACKET_LENGTH];

	while (true)
	{
		memset(buffer, '\0', PACKET_LENGTH);

		int nRecvLen = recv(m_hServSock, buffer, PACKET_LENGTH, 0);
		if (nRecvLen > 0)
		{
			PACKET_TYPE ePacketType = (PPACKET_BASE(buffer))->type;

			switch (ePacketType)
			{
			case PT_CREATEACCOUNT:
				RecvCreateAccount(buffer);
				break;
			case PT_LOGIN:
				RecvLogIn(buffer);
				break;
			case PT_STATUS:
				RecvStatus(buffer);
				break;
			case PT_SKILLINFO:
				RecvSkillInfo(buffer);
				break;
			case PT_ITEMINFO:
				RecvItemInfo(buffer);
				break;
			case PT_EQUIPINFO:
				RecvEquipInfo(buffer);
				break;
			case PT_POSITION:
				RecvPosition(buffer);
				break;
			case PT_MESSAGE:
				RecvMessagePacket(buffer);
				break;
			case PT_OBJECT:
				RecvObjectPacket(buffer);
				break;
			case PT_ENTER:
				RecvEnterPacket(buffer);
				break;
			case PT_LEAVE:
				RecvLeavePacket(buffer);
				break;
			case PT_SKILL:
				RecvUseSkillPacket(buffer);
				break;
			case PT_GETITEM:
				RecvGetItem(buffer);
				break;
			case PT_LEVELUP:
				RecvLevelUp(buffer);
				break;
			case PT_ADDEQUIP:
				RecvAddEquip(buffer);
				break;
			case PT_ERASEEQUIP:
				RecvEraseEquip(buffer);
				break;
			}
		}
	}

	delete[] buffer;
}

void CServerManager::RecvCreateAccount(char* buffer)
{
	PSC_CreateAccountPacket pPacket = (PSC_CreateAccountPacket)buffer;
	if (pPacket->accept)
	{
		//printf("Create Account Success!\n");
		CTitleUI::SetCreateAccount(true);
	}
	//else
		//printf("Create Account failed!\n");
}

void CServerManager::RecvLogIn(char* buffer)
{
	PSC_LoginPacket pPacket = (PSC_LoginPacket)buffer;
	if (pPacket->accept)
	{
		//printf("Login accepted!\n");
		CTitleUI::SetLoginNum(2);
	}
	else
	{
		if (pPacket->idExist)
		{
			//printf("Wrong password!\n");
			CTitleUI::SetLoginNum(1);
		}
		else
		{
			//printf("Wrong ID!\n");
			CTitleUI::SetLoginNum(0);
		}
	}
}

void CServerManager::RecvStatus(char* buffer)
{
	PSC_StatusPacket pPacket = (PSC_StatusPacket)buffer;
	CPlayer* pPlayer = GET_SINGLE(CSceneManager)->m_pPlayer;
	if (!pPlayer)
	{
		//printf("RecvStatus error(Cannot find player)\n");
		return;
	}

	pPlayer->SetCriticEnter();

	pPlayer->SetLevel(pPacket->level);
	pPlayer->SetExp(pPacket->exp);
	pPlayer->SetHP(pPacket->hp);
	pPlayer->SetMP(pPacket->mp);

	pPlayer->SetCriticLeave();

	//printf("Status init Success!\n");
}

void CServerManager::RecvSkillInfo(char * buffer)
{
	PSC_SkillInfoPacket pPacket = (PSC_SkillInfoPacket)buffer;
	CPlayer* pPlayer = GET_SINGLE(CSceneManager)->m_pPlayer;
	if (!pPlayer)
	{
		//printf("RecvSkillInfo error(Cannot find player)\n");
		return;
	}

	pPlayer->SetCriticEnter();

	CSkill* pSkill = pPlayer->FindSkill(pPacket->skillStepLevel, string(pPacket->skillName));
	if (!pSkill)
	{
		//printf("RecvSkillInfo error(Cannot find skill)\n");
		return;
	}
	pSkill->SetLevel(pPacket->skillLevel);
	SAFE_RELEASE(pSkill);

	pPlayer->SetCriticLeave();

	//printf("Skill init Success!\n");
}

void CServerManager::RecvItemInfo(char * buffer)
{
	PSC_ItemInfoPacket pPacket = (PSC_ItemInfoPacket)buffer;
	CPlayer* pPlayer = GET_SINGLE(CSceneManager)->m_pPlayer;
	if (!pPlayer)
	{
		//printf("RecvItemInfo error(Cannot find player)\n");
		return;
	}

	pPlayer->SetCriticEnter();

	CItem* pItem = pPlayer->CreateItem<CItem>(string(pPacket->itemName), ITEM_TYPE(pPacket->eItemType), POSITION(pPacket->x, pPacket->y));
	pItem->ItemInit();
	SAFE_RELEASE(pItem);

	pPlayer->SetCriticLeave();

	//printf("Item init Success!\n");
}

void CServerManager::RecvEquipInfo(char * buffer)
{
	PSC_EquipInfoPacket pPacket = (PSC_EquipInfoPacket)buffer;
	CPlayer* pPlayer = GET_SINGLE(CSceneManager)->m_pPlayer;
	if (!pPlayer)
	{
		//printf("RecvEquipInfo error(Cannot find player)\n");
		return;
	}

	pPlayer->SetCriticEnter();

	CItem* pItem = pPlayer->CreateEquip<CItem>(string(pPacket->equipName), EQUIP_TYPE(pPacket->eEquipType));
	pItem->ItemInit();
	SAFE_RELEASE(pItem);

	pPlayer->SetCriticLeave();

	//printf("Equip init Success!\n");
}

void CServerManager::RecvPosition(char* buffer)
{
	PSC_PositionPacket pPacket = (PSC_PositionPacket)buffer;
	CPlayer* pPlayer = GET_SINGLE(CSceneManager)->m_pPlayer;
	if (!pPlayer)
	{
		//printf("RecvPosition error(Cannot find player)\n");
		return;
	}

	m_iLoginSceneNum = pPacket->sceneNum;
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(pPacket->sceneNum);
	if (!pScene)
	{
		//printf("RecvPosition error(Cannot find scene)\n");
		return;
	}

	pPlayer->SetCriticEnter();

	pPlayer->SetPos(pPacket->tPos);

	pPlayer->SetCriticLeave();

	//printf("Position init Success!\n");

	CTitleUI::SetLogin(true);
	//GET_SINGLE(CSceneManager)->SetSceneToChange(pScene->GetNum());
}

void CServerManager::RecvMessagePacket(char* buffer)
{
	PPACKETMESSAGE pPacket = (PPACKETMESSAGE)buffer;

	CUIMain* pUIMain = (CUIMain*)CObj::FindObject("UIMain");
	if (!pUIMain)
	{
		//printf("RecvMessage error(cannot find UI)\n");
		return;
	}

	pUIMain->RecvMessage(pPacket);
	SAFE_RELEASE(pUIMain);
}

void CServerManager::RecvObjectPacket(char* buffer)
{
	PSC_ObjectPacket pPacket = (PSC_ObjectPacket)buffer;
	CObj* pObj = NULL;

	// 우선 현재 플레이어가 있는 scene과 패킷의 scene이 동일한지 확인
	if (pPacket->sceneNum != GET_SINGLE(CSceneManager)->m_pPlayer->GetScene()->GetNum())
		return;

	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(pPacket->sceneNum);
	if (!pScene)
		return;
	CLayer* pLayer = pScene->FindLayer("Object");
	if (!pLayer)
		return;

	// 아이템의 경우 이름과 위치로 판별
	if (pPacket->objType == OT_ITEM)
		pObj = pLayer->FindItem(pPacket->name, pPacket->tPos);
	// 몬스터인 경우 scene의 origin값으로 판별
	else if (pPacket->objType == OT_MONSTER)
		pObj = pLayer->FindMonster(pPacket->tOrigin);
	else // 아이템, 몬스터가 아닐경우 이름으로 판별
		pObj = CObj::FindObject(pPacket->name);

	// 현재 오브젝트가 있으면 정보 수정
	if (pObj)
	{
		// 만약 life가 false인 경우
		if (!pPacket->life)
			pObj->Die();
		else
			pObj->Update(pPacket);

		SAFE_RELEASE(pObj);
	}
	else if (pPacket->isSelf) // 자기 자신인 경우
	{
		GET_SINGLE(CSceneManager)->m_pPlayer->Update(pPacket);
	}
	else // 만약 없는 오브젝트라면 만들기
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(pPacket->sceneNum);
		if (!pScene)
		{
			//printf("RecvObject error(cannot find scene)!\n");
			return;
		}
		CLayer* pLayer = pScene->FindLayer("Object");
		if (!pLayer)
		{
			//printf("RecvObject error(cannot find layer)!\n");
			return;
		}
		// 플레이어일 시
		if (pPacket->objType == OT_PLAYER)
		{	
			string tempName(pPacket->name);
			pObj = CObj::CreateObj<COtherPlayer>(tempName, pLayer);
			// 그리고 수정
			pObj->Update(pPacket);

			// 장착한 장비도 업데이트 해주기
			COtherPlayer* pPlayer = (COtherPlayer*)pObj;

			for (int i = 0; i < (int)ET_END; i++)
			{
				// 장비가 있다면 업데이트
				if (strcmp(pPacket->equipName[i], "") == 0)
					continue;

				pPlayer->AddEquip(pPacket->equipName[i], (EQUIP_TYPE)i);
			}

			SAFE_RELEASE(pObj);
		}
		else if (pPacket->objType == OT_ITEM) // 아이템일시
		{
			string tempName(pPacket->name);
			pObj = CObj::CreateObj<CItem>(tempName, pLayer);
			// 그리고 수정
			pObj->SetPos(pPacket->tPos);
			((CItem*)pObj)->SetNum(pPacket->iNum);
			((CItem*)pObj)->ItemInit();
			SAFE_RELEASE(pObj);
		}
	}
}

void CServerManager::RecvEnterPacket(char * buffer)
{
	PSC_CharacterEnter pPacket = (PSC_CharacterEnter)buffer;
	CScene* pScene = GET_SINGLE(CSceneManager)->m_pPlayer->GetScene();
	if (!pScene)
	{
		//printf("RecvEnter error(Scene error)!\n");
		return;
	}
	CLayer* pLayer = pScene->FindLayer("Object");
	if (!pLayer)
	{
		//printf("RecvEnter error(Layer error)!\n");
		return;
	}

	// 현재 scene의 obj layer에 otherplayer 오브젝트 추가
	COtherPlayer* pOtherPlayer = CObj::CreateObj<COtherPlayer>(pPacket->name, pLayer);
	
	// 장착한 장비 업데이트 해주기
	for (int i = 0; i < (int)ET_END; i++)
	{
		// 장비가 있다면 업데이트
		if (strcmp(pPacket->equipName[i], "") == 0)
			continue;

		pOtherPlayer->AddEquip(pPacket->equipName[i], (EQUIP_TYPE)i);
	}

	SAFE_RELEASE(pOtherPlayer);
}

void CServerManager::RecvLeavePacket(char * buffer)
{
	PSC_CharacterLeave pPacket = (PSC_CharacterLeave)buffer;
	CScene* pScene = GET_SINGLE(CSceneManager)->m_pPlayer->GetScene();
	if (!pScene)
	{
		//printf("RecvLeave error(Scene error)!\n");
		return;
	}
	CLayer* pLayer = pScene->FindLayer("Object");
	if (!pLayer)
	{
		//printf("RecvLeave error(Layer error)!\n");
		return;
	}

	// 찾아서 지우기
	CObj* pOtherPlayer = CObj::FindObject(pPacket->name);
	if (!pOtherPlayer)
	{
		//printf("RecvLeave error(Find error)!\n");
		return;
	}
	pOtherPlayer->Die();
	SAFE_RELEASE(pOtherPlayer);
}

void CServerManager::RecvUseSkillPacket(char * buffer)
{
	// 해당 이름의 otherplayer를 찾아
	// 해당 스킬을 enable
	PSC_UseSkill pPacket = (PSC_UseSkill)buffer;
	CScene* pScene = GET_SINGLE(CSceneManager)->m_pPlayer->GetScene();
	if (!pScene)
	{
		//printf("RecvUseSkill error(Scene error)!\n");
		return;
	}
	CLayer* pLayer = pScene->FindLayer("Object");
	if (!pLayer)
	{
		//printf("RecvUseSkill error(Layer error)!\n");
		return;
	}
	CObj* pOtherPlayer = CObj::FindObject(pPacket->name);
	if (!pOtherPlayer)
	{
		//printf("RecvUseSkill error(Find error)!\n");
		return;
	}
	((COtherPlayer*)pOtherPlayer)->ActivateSkill(pPacket->skillstep, string(pPacket->skillname));
	SAFE_RELEASE(pOtherPlayer);
}

void CServerManager::RecvGetItem(char * buffer)
{
	PSC_GetItem pPacket = (PCS_GetItem)buffer;
	
	CScene* pScene = GET_SINGLE(CSceneManager)->m_pPlayer->GetScene();
	if (!pScene)
	{
		//printf("RecvGetItem error(Scene error)!\n");
		return;
	}
	CLayer* pLayer = pScene->FindLayer("Object");
	if (!pLayer)
	{
		//printf("RecvGetItem error(Layer error)!\n");
		return;
	}
	CItem* pItem = (CItem*)pLayer->FindItem(string(pPacket->name), pPacket->tPos);
	if (pItem)
	{
		GET_SINGLE(CSceneManager)->m_pPlayer->AddItem(pItem, pPacket->tCoord);
		pLayer->EraseObject(pItem);
		SAFE_RELEASE(pItem);
	}
	else
	{
		if (pPacket->tPos != POSITION(-1.f, -1.f))
		{
			//printf("RecvGetItem error(cannot find item)\n");
			return;
		}		
	}
}

void CServerManager::RecvLevelUp(char * buffer)
{
	PSC_LevelUp pPacket = (PSC_LevelUp)buffer;
	if (!pPacket->levelup)
	{
		//printf("RecvLevelUp error(wrong data)\n");
		return;
	}
	GET_SINGLE(CSceneManager)->m_pPlayer->LevelUp();
}

void CServerManager::RecvAddEquip(char * buffer)
{
	PSC_AddEquip pPacket = (PSC_AddEquip)buffer;

	CObj* pObj = CObj::FindObject(pPacket->name);
	if (!pObj)
	{
		//printf("RecvAddEquip error(cannot find otherplayer)\n");
		return;
	}

	((COtherPlayer*)pObj)->AddEquip(pPacket->equipName, pPacket->eEquipType);
	SAFE_RELEASE(pObj);
}

void CServerManager::RecvEraseEquip(char * buffer)
{
	PSC_EraseEquip pPacket = (PSC_EraseEquip)buffer;

	CObj* pObj = CObj::FindObject(pPacket->name);
	if (!pObj)
	{
		//printf("RecvEraseEquip error(cannot find otherplayer)\n");
		return;
	}

	((COtherPlayer*)pObj)->EraseEquip(pPacket->eEquipType);
	SAFE_RELEASE(pObj);
}

// Send

bool CServerManager::SendCreateAccount(char * id, char * pw)
{
	DWORD flags = 0;

	// packet 초기화
	PCS_CreateAccountPacket pPacket = new CS_CreateAccountPacket;
	pPacket->type = PT_CREATEACCOUNT;
	strcpy(pPacket->id, id);
	strcpy(pPacket->pw, pw);
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send create account error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendLogIn(char * id, char * pw)
{
	// packet 초기화
	PCS_LoginPacket pPacket = new CS_LoginPacket;
	pPacket->type = PT_LOGIN;
	strcpy(pPacket->id, id);
	strcpy(pPacket->pw, pw);
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send login error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendMoveCharacter(CObj* pObj)
{
	PCS_MovePacket pPacket = new CS_MovePacket;
	pPacket->type = PT_MOVE;
	pPacket->tPos = pObj->GetPos();
	pPacket->eDir = pObj->GetDir();
	pPacket->objState = pObj->GetObjState();
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send move error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendMessagePacket(const char* name, const  char * message)
{
	PPACKETMESSAGE pPacket = new PACKETMESSAGE;
	memset(pPacket->name, '\0', MAX_CHAR_LEN);
	memcpy(pPacket->name, name, strlen(name));
	memset(pPacket->message, '\0', MAX_MESSAGE_LEN);
	memcpy(pPacket->message, message, strlen(message));
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("SendMessage error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendPortal(int iSceneFrom, int iSceneTo)
{
	PCS_Portal pPacket = new CS_Portal;
	pPacket->type = PT_PORTAL;
	pPacket->sceneNumFrom = iSceneFrom;
	pPacket->sceneNumTo = iSceneTo;
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send portal error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendUseSkill(int iSceneNum, int iSkillStep, const string& strName, const string& strSkillName)
{
	PCS_UseSkill pPacket = new CS_UseSkill;
	pPacket->type = PT_SKILL;
	pPacket->sceneNum = iSceneNum;
	pPacket->skillstep = iSkillStep;
	strcpy(pPacket->name, strName.c_str());
	strcpy(pPacket->skillname, strSkillName.c_str());
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send UseSkill error!\n");
		return false;
	}
	delete pPacket;
	
	return true;
}

bool CServerManager::SendChangeSkill(const string & strTag, int iLevel)
{
	PCS_ChangeSkill pPacket = new CS_ChangeSkill;
	strcpy(pPacket->name, strTag.c_str());
	pPacket->skillLevel = iLevel;
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send ChangeSkill error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendChangeItem(const string & strTag, ITEM_TYPE eType, int iNum, POSITION tPos, bool bErased)
{
	PCS_ChangeItem pPacket = new CS_ChangeItem;
	strcpy(pPacket->name, strTag.c_str());
	pPacket->eType = eType;
	pPacket->iNum = iNum;
	pPacket->tPos = tPos;
	pPacket->erased = bErased;
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send ChangeItem error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendThrowItem(const string & strTag, ITEM_TYPE eType, int iNum, POSITION tPos)
{
	PCS_ThrowItem pPacket = new CS_ThrowItem;
	strcpy(pPacket->name, strTag.c_str());
	pPacket->eType = eType;
	pPacket->iNum = iNum;
	pPacket->tPos = tPos;
	pPacket->size = sizeof(*pPacket);	

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send ThrowItem error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendGetItem(const string & strTag, ITEM_TYPE eType, POSITION tPos, POSITION tCoord)
{
	PCS_GetItem pPacket = new CS_GetItem;
	strcpy(pPacket->name, strTag.c_str());
	pPacket->eType = eType;
	pPacket->tPos = tPos;
	pPacket->tCoord = tCoord;
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send GetItem error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendDamage(int iSceneNum, POSITION tOrigin, int iDamage)
{
	PCS_Damage pPacket = new CS_Damage;
	pPacket->sceneNum = iSceneNum;
	pPacket->tOrigin = tOrigin;
	pPacket->iDamage = iDamage;
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send Damage error!\n");
		return false;
	}
	delete pPacket;

	return true;
}

bool CServerManager::SendAddEquip(const string & strTag, const string& strEquipTag, EQUIP_TYPE eType)
{
	PCS_AddEquip pPacket = new CS_AddEquip;
	strcpy(pPacket->name, strTag.c_str());
	strcpy(pPacket->equipName, strEquipTag.c_str());
	pPacket->eEquipType = eType;
	pPacket->type = PT_ADDEQUIP;
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send AddEquip error!\n");
		return false;
	}

	delete pPacket;

	return true;
}

bool CServerManager::SendEraseEquip(const string& strTag, EQUIP_TYPE eType)
{
	PCS_EraseEquip pPacket = new CS_EraseEquip;
	strcpy(pPacket->name, strTag.c_str());
	pPacket->eEquipType = eType;
	pPacket->type = PT_ERASEEQUIP;
	pPacket->size = sizeof(*pPacket);

	int nSendLen = send(m_hServSock, (char*)pPacket, sizeof(*pPacket), 0);
	if (nSendLen == -1)
	{
		//printf("Send EraseEquip error!\n");
		return false;
	}

	delete pPacket;

	return true;
}

void CServerManager::DisconnectServer()
{
	PPACKETMESSAGE example = new PACKETMESSAGE;
	memset(&example->message, '\0', sizeof(example->message));
	strcpy(example->message, "exit");
	example->size = sizeof(*example);
	if (send(m_hServSock, (char*)example, example->size, 0) == SOCKET_ERROR)
	{
		//printf("Send error! Error Code : %d\n", WSAGetLastError());
	}
	delete example;
}