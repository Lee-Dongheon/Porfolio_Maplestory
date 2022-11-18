#include <process.h>
#include "ClientManager.h"
#include "Input.h"
#include "DBManager.h"

#include "../Object/Obj.h"
#include "../Object/MoveObj/Player/Player.h"
#include "../Object/MoveObj/Monster/Monster.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"

#include "../Object/StaticObj/Item/Item.h"
#include "../Object/StaticObj/Skill/Skill.h"

DEF_SINGLE(CClientManager);

// worker thread ȣ�� �Լ�
unsigned int WINAPI CallWorkerThread(void* p)
{
	GET_SINGLE(CClientManager)->WorkerThread();
	return 0;
}

CClientManager::CClientManager() :
	m_iServerPort(9190),
	m_pWorkerThreadsHandle(NULL),
	m_bAccept(true),
	m_bWorkerThread(true),
	m_pSocketInfo(NULL)
{
}

CClientManager::~CClientManager()
{
	closesocket(m_hServSock);
	WSACleanup();

	SAFE_DELETE(m_pSocketInfo);
	JoinWorkerThread();
	SAFE_DELETE_ARR(m_pWorkerThreadsHandle);
}

SOCKET CClientManager::FindSocket(CObj * pObj)
{
	map<SOCKET, CObj*>::iterator iter;
	map<SOCKET, CObj*>::iterator iterEnd = m_mapCharacter.end();

	for (iter = m_mapCharacter.begin(); iter != iterEnd; iter++)
	{
		if (iter->second == pObj)
			return iter->first;
	}

	return NULL;
}

void CClientManager::WorkerThread()
{
	DWORD recvBytes;

	// completion key ������
	PSOCKETINFO pCompletionKey;
	// overlapped Ȯ�� ����ü ������
	PSOCKETINFO pSocketInfo;
	// ��� ���� ����!!

	DWORD flags = 0;

	while (m_bWorkerThread)
	{
		// Waiting Thread Queue�� ������ ����
		// �Ϸ�� �����带 �˷���
		if (!GetQueuedCompletionStatus(m_hIocp, &recvBytes, (PULONG_PTR)&pCompletionKey, (LPOVERLAPPED*)&pSocketInfo, INFINITE))
		{
			if (WSAGetLastError() != 64)
			{
				if (WSAGetLastError() == 995)
				{
					printf("Closing Server...\n");
					break;
				}

				printf("GetQueuedCompletionStatus error! Error Code : %d\n", WSAGetLastError());
				// ���� && recvByte�� 0�� ���
				if (recvBytes == 0)
				{
					if (pSocketInfo && pCompletionKey)
					{
						printf("Socket[%d] disconnected!\n", pSocketInfo->socket);
						closesocket(pSocketInfo->socket);
						SAFE_DELETE(pSocketInfo);
					}
					continue;
				}
			}
		}

		pSocketInfo->dataBuf.len = recvBytes;

		// �����ߴµ� recvBytes�� 0 > ������ ������
		/*if (recvBytes == 0)
		{
			closesocket(pSocketInfo->socket);
			delete pSocketInfo;
			continue;
		}*/

		// accept�� ���,  accept�� �� �ش� Ŭ���̾�Ʈ socket�� iocp�� ��� ��
		// �ٽ� acceptEx�� �����Ͽ� �ٸ� Ŭ���̾�Ʈ�� ������ ���� �غ�
		if (pSocketInfo->eType == SO_ACCEPT)
		{
			PSOCKADDR pRemoteSocketAddr = nullptr;
			PSOCKADDR pLocalSocketAddr = nullptr;
			INT pRemoteSocketAddrLength = 0;
			INT pLocalSocketAddrLength = 0;

			// AcceptEx ���� ��������
			GetAcceptExSockaddrs(&pSocketInfo->buffer, 0,
				sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
				&pLocalSocketAddr, &pLocalSocketAddrLength,
				&pRemoteSocketAddr, &pRemoteSocketAddrLength);

			SOCKADDR_IN remoteAddr = *(PSOCKADDR_IN)pRemoteSocketAddr;
			// Ŭ���̾�Ʈ IP, port ���� ���
			printf("Client[%s:%d] Accept!\n", inet_ntoa(remoteAddr.sin_addr), remoteAddr.sin_port);

			// Ŭ���̾�Ʈ socket IOCP ����ϱ�
			m_hIocp = CreateIoCompletionPort((HANDLE)pSocketInfo->socket, m_hIocp, (DWORD)pSocketInfo, 0);

			// socketInfo �ʱ�ȭ
			m_pSocketInfo = new SOCKETINFO;
			ZeroMemory(&m_pSocketInfo->overlapped, sizeof(m_pSocketInfo->overlapped));
			SOCKET socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			m_pSocketInfo->dataBuf.len = PACKET_LENGTH;
			m_pSocketInfo->dataBuf.buf = m_pSocketInfo->buffer;
			m_pSocketInfo->eType = SO_ACCEPT;
			m_pSocketInfo->socket = socket;

			// AcceptEx ȣ��
			if (AcceptEx(m_hServSock, socket, &m_pSocketInfo->buffer, 0,
				sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL,
				(LPOVERLAPPED)(&m_pSocketInfo->overlapped)) == SOCKET_ERROR)
			{
				printf("AcceptEx error! Error Code : %d\n", WSAGetLastError());
				return;
			}
		}
		else if (pSocketInfo->eType == SO_RECV) // Ŭ���̾�Ʈ���� ��Ŷ�� ���� ���
		{
			pSocketInfo->recv--;
			// PacketProcess���� false���� ��
			// Ŭ���̾�Ʈ ����޼����� false �����̹Ƿ� üũ�� ��.
			if (!PacketProcess(pSocketInfo))
			{
				// �޼��� �������� Ȯ��
				if ((PPACKET_BASE(pSocketInfo->buffer))->type == PT_MESSAGE)
				{
					if (strcmp((PPACKETMESSAGE(pSocketInfo->buffer))->message, "exit") == 0)
					{
						printf("Socket[%d] disconnected!\n", pSocketInfo->socket);
						// ����޼����� ������ ó���� �͵�
						// �ش� ���Ͽ� �����ϴ� �÷��̾� ������Ʈ�� �־����� üũ
						// �����ٸ� ������ �ݰ� �׳� �����Ѵ�.
						// �־��ٸ� ���� + ���� ���� Ŭ���̾�Ʈ���� ���� broadcast���ֱ�

						// �÷��̾� �˻�
						map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSocketInfo->socket);

						// ���� ĳ���Ͱ� �ִٸ�
						if (iter != m_mapCharacter.end())
						{
							// �ش� scene�� object�� ������ ���̰�,
							// ���� ������ ������ �������̹Ƿ� �Ӱ迵�� Ȯ��
							iter->second->GetScene()->SetCriticEnter();
							iter->second->GetScene()->EraseUser(iter->second);
							iter->second->Die();
							iter->second->GetScene()->SetCriticLeave();
							SAFE_RELEASE(iter->second);
							m_mapCharacter.erase(iter);
						}
						closesocket(pSocketInfo->socket);
						SAFE_DELETE(pSocketInfo);

						continue;
					}
					else // ����޽����� �ƴѰ�� �׳� PacketProcess�� ������ ��
					{
						printf("Packet processing error!\n");
						continue;
					}
				}				
				else // ����޽����� �ƴѰ�� �׳� PacketProcess�� ������ ��
				{
					printf("Packet processing error!\n");
					continue;
				}
			}
		}
		else if (pSocketInfo->eType == SO_SEND)
		{
			pSocketInfo->send--;
			if (pSocketInfo->send == 0)
			{
				//printf("Delete SocketInfo!\n");
				SAFE_DELETE(pSocketInfo);
				continue;
			}
		}
		else
		{
			printf("Worker Thread error![INVALID_PACKET]\n");
			return;
		}

		// ó�� ���� recv ���
		Recv(pSocketInfo);
	}
}

void CClientManager::Send(PSOCKETINFO pSockInfo)
{
	DWORD sendBytes;
	DWORD flags = 0;
	pSockInfo->eType = SO_SEND;
	pSockInfo->send++;

	if (WSASend(pSockInfo->socket, &(pSockInfo->dataBuf), 1, &sendBytes, flags, (LPWSAOVERLAPPED)(&pSockInfo->overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("Send failed! Error Code : %d\n", WSAGetLastError());
			
			// ������ ���� ��� ó��
			if (WSAGetLastError() == 10038 || WSAGetLastError() == 10054)
			{
				map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
				if (iter == m_mapCharacter.end())
					return;

				closesocket(iter->first);
				iter->second->Die();
				m_mapCharacter.erase(iter);
			}
		}
	}
}

void CClientManager::Recv(PSOCKETINFO pSockInfo)
{
	//DWORD recvBytes;
	DWORD flags = 0;

	// pSockInfo �ʱ�ȭ
	ZeroMemory(&(pSockInfo->overlapped), sizeof(WSAOVERLAPPED));
	ZeroMemory(pSockInfo->buffer, PACKET_LENGTH);
	pSockInfo->dataBuf.len = PACKET_LENGTH;
	pSockInfo->dataBuf.buf = pSockInfo->buffer;
	pSockInfo->eType = SO_RECV;

	if (WSARecv(pSockInfo->socket, &(pSockInfo->dataBuf), 1, (LPDWORD)&pSockInfo, &flags, (LPWSAOVERLAPPED)&(pSockInfo->overlapped), NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("Recv failed! Error Code : %d\n", WSAGetLastError());
		}
	}
}

bool CClientManager::Init()
{
	// ��������
	// 1. ���ν����忡�� accept üũ(����) > ����
	// 2. �۾� �����忡�� iocp ó��

	// ���� ���̺귯�� �ʱ�ȭ version 2.2
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
	{
		printf("WSAStartup error!\n");
		return false;
	}

	// listen �� servsock �ʱ�ȭ overlapped flag ����
	m_hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	// ���� ���� ���� ����ó��
	if (m_hServSock == INVALID_SOCKET)
	{
		printf("Socket init failed!\n");
		return false;
	}

	memset(&m_ServAddr, 0, sizeof(m_ServAddr)); // ���� �ּ� ����ü �޸� �ʱ�ȭ
	m_ServAddr.sin_family = AF_INET;
	m_ServAddr.sin_addr.s_addr = htonl(INADDR_ANY); // ���� ip
	m_ServAddr.sin_port = htons(m_iServerPort); // ���� ��Ʈ

	// ���Ͽ� IP�ּҿ� ��Ʈ��ȣ �Ҵ�
	if (::bind(m_hServSock, (SOCKADDR*)&m_ServAddr, sizeof(m_ServAddr)) == SOCKET_ERROR)
	{
		printf("Socket bind error!\n");
		return false;
	}

	// AcceptEx ����� ���� listen ���� �ڵ����� accept ȣ���ϴ� ���� ����
	BOOL on = TRUE;
	setsockopt(m_hServSock, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&on, sizeof(on));

	// listen �Լ� ȣ���� ���ؼ� ������ ������ ���� �������� �ϼ�
	if (listen(m_hServSock, 5) == SOCKET_ERROR)
	{
		printf("Socket listen error!\n");
		return false;
	}

	// �ý��� ���� ��������
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf("CPU ���� Ȯ�� : %d\n", sysInfo.dwNumberOfProcessors);

	// ������ ���� cpu * 2�� ����
	m_iThreads = sysInfo.dwNumberOfProcessors;

	// IOCP �ʱ�ȭ ó��
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_iThreads);

	// Worker Thread �����
	if (!CreateWorkerThreads())
	{
		printf("Create Worker Threads failed!\n");
		return false;
	}

	// ���� ó��
	if (m_hIocp == NULL)
	{
		printf("Create IOCP error! Error Code : %d\n", WSAGetLastError());
		return false;
	}

	// client accept�� ���� Ŭ���̾�Ʈ ����
	SOCKET hClientSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	int szAddr = sizeof(SOCKADDR_IN);

	PSOCKETINFO m_pSocketInfo = new SOCKETINFO;
	//m_mapSocketInfo.insert(make_pair(hClientSock, m_pSocketInfo));
	memset(&m_pSocketInfo->overlapped, 0, sizeof(m_pSocketInfo->overlapped));
	m_pSocketInfo->socket = hClientSock;
	m_pSocketInfo->dataBuf.len = PACKET_LENGTH;
	m_pSocketInfo->dataBuf.buf = m_pSocketInfo->buffer;
	m_pSocketInfo->eType = SO_ACCEPT;

	// listen socket�� IOCP�� ���
	m_hIocp = CreateIoCompletionPort((HANDLE)m_hServSock, m_hIocp, 0, 0);
	//m_hIocp = CreateIoCompletionPort((HANDLE)hClientSock, m_hIocp, (DWORD)pSocketInfo, 0);

	// AcceptEx
	if (AcceptEx(m_hServSock, hClientSock, &m_pSocketInfo->buffer, 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL,
		(LPOVERLAPPED)(&m_pSocketInfo->overlapped)) == SOCKET_ERROR)
	{
		printf("AcceptEx error!\n");
		return false;
	}

	printf("Starting server...\n");

	return true;
}

bool CClientManager::CreateWorkerThreads()
{
	// worker thread ����
	unsigned int threadID;

	// thread handle �迭 �ʱ�ȭ
	m_pWorkerThreadsHandle = new HANDLE[m_iThreads];
	// ����
	for (int i = 0; i < m_iThreads; i++)
	{
		m_pWorkerThreadsHandle[i] = (HANDLE*)_beginthreadex(NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &threadID);
		// ���� ó��
		if (!m_pWorkerThreadsHandle[i])
		{
			printf("Worker Thread error!\n");
			return false;
		}
		ResumeThread(m_pWorkerThreadsHandle[i]);
	}
	printf("Create Worker Threads Success!\n");

	return true;
}

bool CClientManager::PacketProcess(PSOCKETINFO pSockInfo)
{
	PACKET_TYPE ePacketType;
	ePacketType = (PPACKET_BASE(pSockInfo->dataBuf.buf))->type;

	switch (ePacketType)
	{
	case PT_CREATEACCOUNT:
		return CreateAccount(pSockInfo);
	case PT_LOGIN:
		return LogIn(pSockInfo);
	case PT_MOVE:
		return MoveCharacter(pSockInfo);
	case PT_MESSAGE:
		return Message(pSockInfo);
	case PT_PORTAL:
		return Portal(pSockInfo);
	case PT_SKILL:
		return UseSkill(pSockInfo);
	case PT_CHANGESKILL:
		return ChangeSkill(pSockInfo);
	case PT_CHANGEITEM:
		return ChangeItem(pSockInfo);
	case PT_THROWITEM:
		return ThrowItem(pSockInfo);
	case PT_GETITEM:
		return GetItem(pSockInfo);
	case PT_DAMAGE:
		return Damage(pSockInfo);
	case PT_ADDEQUIP:
		return AddEquip(pSockInfo);
	case PT_ERASEEQUIP:
		return EraseEquip(pSockInfo);
	default:
		printf("INVALID PACKET!\n");
		//return false;
		break;
	}

	return true;
}

void CClientManager::BroadCast(PSOCKETINFO pSockInfo)
{
	// �޽����� ���� ������ ���� �ʿ� �ִ� �������Ը� message ������
	map<SOCKET, CObj*>::iterator iterCharacter = m_mapCharacter.find(pSockInfo->socket);
	if (iterCharacter == m_mapCharacter.end())
	{
		printf("Message BroadCast error(cannot find user)\n");
		return;
	}

	CScene* pScene = iterCharacter->second->GetScene();
	if (!pScene)
	{
		printf("Message BroadCast error(cannot find scene)\n");
		return;
	}

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = pScene->m_UserList.end();

	for (iter = pScene->m_UserList.begin(); iter != iterEnd; iter++)
	{
		SOCKET sock = FindSocket((*iter));
		if (!sock)
		{
			printf("Message BroadCast error(INVALID SOCKET)\n");
			continue;
		}

		PSOCKETINFO pSocketInfo = new SOCKETINFO;
		pSocketInfo->Init(sock, SO_SEND);
		// ������ ��Ŷ �״�� ����
		memcpy(pSocketInfo->buffer, pSockInfo->buffer, sizeof(pSockInfo->buffer));

		Send(pSocketInfo);
	}
}

bool CClientManager::CreateAccount(PSOCKETINFO pSockInfo)
{
	PCS_CreateAccountPacket pPacket = (PCS_CreateAccountPacket)(pSockInfo->buffer);

	char* id = pPacket->id;
	char* pw = pPacket->pw;

	PSOCKETINFO pSocketInfo = new SOCKETINFO;
	pSocketInfo->Init(pSockInfo->socket, SO_SEND);

	PSC_CreateAccountPacket pPack = new SC_CreateAccountPacket;
	pPack->type = PT_CREATEACCOUNT;

	if (!GET_SINGLE(CDBManager)->CreateAccount(id, pw))
		pPack->accept = false;
	else
		pPack->accept = true;

	memcpy(pSocketInfo->buffer, pPack, sizeof(*pPack));
	delete pPack;
	Send(pSocketInfo);

	printf("Client[%d] : Create account!\n", pSockInfo->socket);

	return true;
}

bool CClientManager::LogIn(PSOCKETINFO pSockInfo)
{
	PCS_CreateAccountPacket pPacket = (PCS_CreateAccountPacket)(pSockInfo->buffer);

	char* id = pPacket->id;
	char* pw = pPacket->pw;

	PSOCKETINFO pSocketInfo = new SOCKETINFO;
	pSocketInfo->Init(pSockInfo->socket, SO_SEND);

	// �α��� ��Ŷ
	PSC_LoginPacket pPack = new SC_LoginPacket;
	pPack->type = PT_LOGIN;

	int iResult = GET_SINGLE(CDBManager)->LogIn(id, pw);
	bool bResult = false;

	if (iResult == 0) // ���̵����
	{
		pPack->accept = false;
		pPack->idExist = false;
	}
	else if (iResult == 1) // ��й�ȣ Ʋ��
	{
		pPack->accept = false;
		pPack->idExist = true;
	}
	else
	{
		pPack->accept = true;
		bResult = true;
	}

	memcpy(pSocketInfo->buffer, pPack, sizeof(*pPack));
	delete pPack;
	Send(pSocketInfo);

	if (bResult)
	{
		// �α��� ���� �� ĳ���� ���� �� ������ �ҷ��ͼ� �ʱ�ȭ
		string tempID(id);
		CObj* pObj = CObj::CreateObj<CPlayer>(tempID);
		if (!GET_SINGLE(CDBManager)->CharacterInit(pObj))
			return false;

		CPlayer* pPlayer = (CPlayer*)pObj;
		// statusPacket
		PSC_StatusPacket pStatusPack = new SC_StatusPacket;
		pStatusPack->type = PT_STATUS;
		pStatusPack->level = pPlayer->GetLevel();
		pStatusPack->exp = pPlayer->GetExp();
		pStatusPack->hp = pPlayer->GetHP();
		pStatusPack->mp = pPlayer->GetMP();
		pStatusPack->size = sizeof(*pStatusPack);

		// ���� ����
		pSocketInfo = new SOCKETINFO;
		pSocketInfo->Init(pSockInfo->socket, SO_SEND);
		memcpy(pSocketInfo->buffer, pStatusPack, sizeof(*pStatusPack));
		delete pStatusPack;
		Send(pSocketInfo);

		// skillPacket
		PSC_SkillInfoPacket pSkillPack = new SC_SkillInfoPacket;
		pSkillPack->type = PT_SKILLINFO;

		list<CSkill*>::iterator iter;
		list<CSkill*>::iterator iterEnd = pPlayer->m_SkillList.end();

		for (iter = pPlayer->m_SkillList.begin(); iter != iterEnd; iter++)
		{
			strcpy(pSkillPack->skillName, (*iter)->GetTag().c_str());
			pSkillPack->skillStepLevel = (*iter)->GetSkillStep();
			pSkillPack->skillLevel = (*iter)->GetLevel();
			pSkillPack->size = sizeof(*pSkillPack);

			// ���� ����
			pSocketInfo = new SOCKETINFO;
			pSocketInfo->Init(pSockInfo->socket, SO_SEND);
			memcpy(pSocketInfo->buffer, pSkillPack, sizeof(*pSkillPack));
			Send(pSocketInfo);
		}
		delete pSkillPack;

		// itemPacket
		PSC_ItemInfoPacket pItemPack = new SC_ItemInfoPacket;
		pItemPack->type = PT_ITEMINFO;

		list<CItem*>::iterator iterItem;
		list<CItem*>::iterator iterItemEnd = pPlayer->m_ItemList.end();

		for (iterItem = pPlayer->m_ItemList.begin(); iterItem != iterItemEnd; iterItem++)
		{
			strcpy(pItemPack->itemName, (*iterItem)->GetTag().c_str());
			pItemPack->itemCount = (*iterItem)->GetNum();
			pItemPack->eItemType = (*iterItem)->GetItemType();
			pItemPack->x = int((*iterItem)->GetCoord().x);
			pItemPack->y = int((*iterItem)->GetCoord().y);
			pItemPack->size = sizeof(*pItemPack);

			// ���� ����
			pSocketInfo = new SOCKETINFO;
			pSocketInfo->Init(pSockInfo->socket, SO_SEND);
			memcpy(pSocketInfo->buffer, pItemPack, sizeof(*pItemPack));
			Send(pSocketInfo);
		}
		delete pItemPack;

		// equipPacket
		PSC_EquipInfoPacket pEquipPack = new SC_EquipInfoPacket;
		pEquipPack->type = PT_EQUIPINFO;

		list<CItem*>::iterator iterEquip;
		list<CItem*>::iterator iterEquipEnd = pPlayer->m_EquipList.end();

		for (iterEquip = pPlayer->m_EquipList.begin(); iterEquip != iterEquipEnd; iterEquip++)
		{
			strcpy(pEquipPack->equipName, (*iterEquip)->GetTag().c_str());
			pEquipPack->eEquipType = (*iterEquip)->GetEquipType();
			pEquipPack->size = sizeof(*pEquipPack);

			// ���� ����
			pSocketInfo = new SOCKETINFO;
			pSocketInfo->Init(pSockInfo->socket, SO_SEND);
			memcpy(pSocketInfo->buffer, pEquipPack, sizeof(*pEquipPack));
			Send(pSocketInfo);
		}
		delete pEquipPack;

		// positionPacket
		PSC_PositionPacket pPosPack = new SC_PositionPacket;
		pPosPack->type = PT_POSITION;
		pPosPack->sceneNum = pPlayer->GetScene()->GetNum();
		pPosPack->tPos = pPlayer->GetPos();
		pPosPack->size = sizeof(*pPosPack);

		// ���� ����
		pSocketInfo = new SOCKETINFO;
		pSocketInfo->Init(pSockInfo->socket, SO_SEND);
		memcpy(pSocketInfo->buffer, pPosPack, sizeof(*pPosPack));
		delete pPosPack;
		Send(pSocketInfo);

		// socket-player �ʿ� �߰�
		m_mapCharacter.insert(make_pair(pSocketInfo->socket, pObj));
		// ���� scene�� ���� �÷��̾�� ������ �˸�
		EnterPlayer(pObj);

		printf("Client[%d] : Login!\n", pSockInfo->socket);
	}

	return true;
}

bool CClientManager::DeleteAccount(PSOCKETINFO pSockInfo)
{
	return true;
}

bool CClientManager::MoveCharacter(PSOCKETINFO pSockInfo)
{
	PCS_MovePacket pMovePacket = PCS_MovePacket(pSockInfo->buffer);

	map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
	if (iter == m_mapCharacter.end())
	{
		printf("Move Character error!\n");
		return false;
	}

	iter->second->SetPos(pMovePacket->tPos);
	iter->second->SetDir(pMovePacket->eDir);
	iter->second->SetObjState(pMovePacket->objState);

	return true;
}

bool CClientManager::Message(PSOCKETINFO pSockInfo)
{
	PPACKETMESSAGE pMessage = PPACKETMESSAGE(pSockInfo->buffer);
	printf("Client[%d] : %s\n", pSockInfo->socket, pMessage->message);
	
	// ����޽����� false ����
	if (strcmp(pMessage->message, "exit") == 0)
		return false;
	else // �ƴ϶�� ���� �� Ŭ���̾�Ʈ���� �޼��� broadcast
		BroadCast(pSockInfo);

	return true;
}

bool CClientManager::Portal(PSOCKETINFO pSockInfo)
{
	PCS_Portal pPortalPacket = (PCS_Portal)(pSockInfo->buffer);
	// ���� scene
	CScene* pSceneFrom = GET_SINGLE(CSceneManager)->FindScene(pPortalPacket->sceneNumFrom);
	if (!pSceneFrom)
	{
		printf("SceneFrom error!\n");
		return false;
	}
	map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
	if (iter == m_mapCharacter.end())
	{
		printf("Find Character error!\n");
		return false;
	}
	LeavePlayer(iter->second); // ��Ŷ����

	// �Ӱ迵�� �ȿ��� object �����ֱ�
	pSceneFrom->SetCriticEnter();
	pSceneFrom->EraseObject(iter->second);
	pSceneFrom->EraseUser(iter->second);
	pSceneFrom->SetCriticLeave();

	// ���� scene
	CScene* pSceneTo = GET_SINGLE(CSceneManager)->FindScene(pPortalPacket->sceneNumTo);
	if (!pSceneTo)
	{
		printf("SceneTo error!\n");
		return false;
	}
	// �Ӱ迵�� �ȿ��� object �߰��ϱ�
	pSceneTo->SetCriticEnter();
	pSceneTo->AddObject(iter->second);
	pSceneTo->SetCriticLeave();

	EnterPlayer(iter->second); // ��Ŷ����

	//printf("Client[%d] : Use portal(%d to %d)\n", pSockInfo->socket, pSceneFrom->GetNum(), pSceneTo->GetNum());

	return true;
}

bool CClientManager::UseSkill(PSOCKETINFO pSockInfo)
{
	// ������ ��ų ���� �ش� scene�� ��� �������� ��ų�� ������ �ϹǷ�
	// ���� �̸��� ���� ������Ʈ�� �ش� ��ų�� enable�� ���鵵�� broadcast�ϱ�
	// �ٽ��� ������ ��ġ�⸸ �� �� ���������� ������ ����
	// CS_UseSkill�� SC_UseSkill�� ���� ����ü�̹Ƿ� ���ο� ���� ������ �ʿ����
	PCS_UseSkill pPacket = (PCS_UseSkill)pSockInfo->buffer;

	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(pPacket->sceneNum);
	if (!pScene)
	{
		printf("UseSkill error(scene is nullptr)!\n");
		return false;
	}

	// string �̸�
	string tempName(pPacket->name);

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = pScene->m_UserList.end();

	for (iter = pScene->m_UserList.begin(); iter != iterEnd; iter++)
	{
		// �ڱ� �ڽſ��Դ� ������ �ʵ��� �̸����� üũ
		if ((*iter)->GetTag() == tempName)
			continue;

		SOCKET sock = FindSocket((*iter));
		if (!sock)
		{
			printf("UseSkill error(FindSocket error)!\n");
			continue;
		}

		// ���ۿ� overlapped Ȯ�� ����ü ����
		PSOCKETINFO pSocketInfo = new SOCKETINFO;
		pSocketInfo->Init(sock, SO_SEND);

		// ��Ŷ ����
		memcpy(pSocketInfo->buffer, pPacket, sizeof(*pPacket));
		pSocketInfo->socket = sock;

		Send(pSocketInfo);
	}

	return true;
}

bool CClientManager::ChangeSkill(PSOCKETINFO pSockInfo)
{
	PCS_ChangeSkill pPacket = (PCS_ChangeSkill)pSockInfo->buffer;

	map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
	if (iter == m_mapCharacter.end())
	{
		printf("ChangeSkill error(Cannot find object)!\n");
		return false;
	}

	CPlayer* pPlayer = (CPlayer*)iter->second;
	if (!pPlayer)
	{
		printf("ChangeSkill error(null player)!\n");
		return false;
	}
	pPlayer->ChangeSkill(string(pPacket->name), pPacket->skillLevel);

	return true;
}

bool CClientManager::ChangeItem(PSOCKETINFO pSockInfo)
{
	PCS_ChangeItem pPacket = (PCS_ChangeItem)pSockInfo->buffer;

	map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
	if (iter == m_mapCharacter.end())
	{
		printf("ChangeItem error(Cannot find object)!\n");
		return false;
	}

	CPlayer* pPlayer = (CPlayer*)iter->second;
	if (!pPlayer)
	{
		printf("ChangeItem error(null player)!\n");
		return false;
	}
	pPlayer->ChangeItem(string(pPacket->name), pPacket->eType, pPacket->tPos, pPacket->iNum, pPacket->erased);

	return true;
}

bool CClientManager::ThrowItem(PSOCKETINFO pSockInfo)
{
	PCS_ThrowItem pPacket = (PCS_ThrowItem)pSockInfo->buffer;

	map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
	if (iter == m_mapCharacter.end())
	{
		printf("ThrowItem error(cannot find player)\n");
		return false;
	}
	CScene* pScene = iter->second->GetScene();
	if (!pScene)
	{
		printf("ThrowItem error(cannot find scene)\n");
		return false;
	}
	CItem* pItem = new CItem;
	if (!pItem->Init())
	{
		printf("ThrowItem error(item init)\n");
		return false;
	}
	pItem->SetTag(string(pPacket->name));
	pItem->SetItemType(pPacket->eType);
	pItem->SetPos(pPacket->tPos);
	pItem->SetNum(pPacket->iNum);

	pScene->AddObject(pItem);
	SAFE_RELEASE(pItem);

	return true;
}

bool CClientManager::GetItem(PSOCKETINFO pSockInfo)
{
	PCS_GetItem pPacket = (PCS_GetItem)pSockInfo->buffer;

	map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
	if (iter == m_mapCharacter.end())
	{
		printf("GetItem error(cannot find player)\n");
		return false;
	}
	CScene* pScene = iter->second->GetScene();
	if (!pScene)
	{
		printf("GetItem error(cannot find scene)\n");
		return false;
	}

	// ���� position = (-1.f, -1.f)�̸� �ʵ�ȹ�� �������� �ƴϴ�! > ������ ��ã��
	// �� �� ���� ���������.
	CItem* pItem = (CItem*)pScene->FindItem(string(pPacket->name), pPacket->tPos);
	
	if (pItem)
	{
		// ȹ���� �÷��̾��� ������ ��Ͽ� �ش� ������ �߰�
		((CPlayer*)iter->second)->AddItem(pItem->GetTag(), pItem->GetItemType(), pPacket->tCoord, pItem->GetNum());
		// ȹ���� �������� scene���� ���� > broadcast���� ��
		pScene->EraseObject(pItem);
	}
	else
	{
		pItem = ((CPlayer*)iter->second)->CreateItem(pPacket->name, pPacket->eType, pPacket->tCoord, 1);
		GET_SINGLE(CDBManager)->AddItem(iter->second->GetTag(), pItem);
	}

	// �߰� �ϷḦ Ŭ���̾�Ʈ�� �˸���(��Ŷ �״�� ��Ȱ��)
	PSOCKETINFO pSocketInfo = new SOCKETINFO;
	pSocketInfo->Init(pSockInfo->socket, SO_SEND);

	memcpy(pSocketInfo->buffer, pPacket, sizeof(*pPacket));
	Send(pSocketInfo);

	SAFE_RELEASE(pItem);

	return true;
}

bool CClientManager::Damage(PSOCKETINFO pSockInfo)
{
	// ���Ͱ� ���� ������ ó��
	// ���� ���� ü���� 0 ���϶��
	// �ش� ������Ʈ ��Ȱ��ȭ �� ����ġ ��Ŷ �÷��̾�� ����
	PCS_Damage pPacket = (PCS_Damage)pSockInfo->buffer;

	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(pPacket->sceneNum);
	if (!pScene)
	{
		printf("Damage error(scene is nullptr)!\n");
		return false;
	}
	CMonster* pMonster = (CMonster*)pScene->FindMonster(pPacket->tOrigin);
	if (!pMonster)
	{
		printf("Damage error(cannot find monster)!\n");
		return false;
	}
	if (pMonster->GetDamage(pPacket->iDamage)) // ������ true
	{
		// ���� �÷��̾�� ����ġ �ֱ�
		map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
		if (iter == m_mapCharacter.end())
		{
			printf("Damage error(cannot find player)\n");
			return false;
		}
		((CPlayer*)iter->second)->AddExp(pMonster->GetExp());
	}
	else
	{
		map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
		if (iter == m_mapCharacter.end())
		{
			printf("Damage error(cannot find player)\n");
			return false;
		}
		pMonster->SetAggressiveObj(iter->second);
	}

	SAFE_RELEASE(pMonster);

	return true;
}

bool CClientManager::AddEquip(PSOCKETINFO pSockInfo)
{
	PCS_AddEquip pPacket = (PCS_AddEquip)pSockInfo->buffer;

	map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
	if (iter == m_mapCharacter.end())
	{
		printf("AddEquip error(cannot find player)\n");
		return false;
	}

	// �÷��̾�� ��� �߰�
	CItem* pItem = ((CPlayer*)iter->second)->CreateEquip(pPacket->equipName, pPacket->eEquipType);
	// DB�� �߰�
	GET_SINGLE(CDBManager)->AddEquip(iter->second->GetTag(), pItem);
	SAFE_RELEASE(pItem);

	EquipBroadCast(iter->second, pSockInfo->buffer);

	return true;
}

bool CClientManager::EraseEquip(PSOCKETINFO pSockInfo)
{
	PCS_EraseEquip pPacket = (PCS_EraseEquip)pSockInfo->buffer;

	map<SOCKET, CObj*>::iterator iter = m_mapCharacter.find(pSockInfo->socket);
	if (iter == m_mapCharacter.end())
	{
		printf("AddEquip error(cannot find player)\n");
		return false;
	}

	// �÷��̾� ��� ����, �Լ� �ȿ��� DB����
	((CPlayer*)iter->second)->EraseEquip(pPacket->eEquipType);

	EquipBroadCast(iter->second, pSockInfo->buffer);

	return true;
}

void CClientManager::SendLevelUp(CObj * pObj)
{
	// �ش� ������ ���� �˻�
	SOCKET sock = FindSocket(pObj);
	if (!sock)
	{
		printf("SendLevelUp error! INVALID SOCKET!\n");
		if (pObj)
			pObj->Die();
		return;
	}

	PSOCKETINFO pSocketInfo = new SOCKETINFO;
	pSocketInfo->Init(sock, SO_SEND);

	PSC_LevelUp pPacket = new SC_LevelUp;
	pPacket->levelup = true;
	pPacket->size = sizeof(*pPacket);

	memcpy(pSocketInfo->buffer, pPacket, sizeof(*pPacket));
	delete pPacket;
	Send(pSocketInfo);
}

void CClientManager::BroadCast(CObj * pObj, list<CObj*>& ObjList)
{
	// �ش� ������ ���� �˻�
	SOCKET sock = FindSocket(pObj);
	if (!sock)
	{
		printf("BroadCast error! INVALID SOCKET!\n");
		if (pObj)
		{
			CScene* pScene = pObj->GetScene();
			if (!pScene)
			{
				printf("BroadCast error(cannot find scene)\n");
				return;
			}
			pScene->EraseObject(pObj);
			pScene->EraseUser(pObj);
		}
		return;
	}

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = ObjList.end();

	for (iter = ObjList.begin(); iter != iterEnd; iter++)
	{
		PSC_ObjectPacket pPacket = new SC_ObjectPacket;
		memset(pPacket, 0, sizeof(*pPacket));
		pPacket->type = PT_OBJECT;
		strcpy(pPacket->name, (*iter)->GetTag().c_str());
		pPacket->sceneNum = (*iter)->GetScene()->GetNum();
		pPacket->objState = (*iter)->GetObjState();
		pPacket->objType = (*iter)->GetObjType();
		pPacket->eDir = (*iter)->GetDir();
		pPacket->enable = (*iter)->GetEnable();
		pPacket->life = true;

		if (pPacket->objType == OT_MONSTER)
		{
			pPacket->tOrigin = ((CMonster*)(*iter))->GetOriginPos();
			pPacket->hp = ((CMonster*)(*iter))->GetHP();
			pPacket->mp = ((CMonster*)(*iter))->GetMP();
		}
		else if (pPacket->objType == OT_ITEM)
			pPacket->iNum = ((CItem*)(*iter))->GetNum();
		else if (pPacket->objType == OT_PLAYER)
		{
			CPlayer* pPlayer = (CPlayer*)(*iter);

			list<CItem*>::iterator iterEquip;
			list<CItem*>::iterator iterEquipEnd = pPlayer->m_EquipList.end();

			for (iterEquip = pPlayer->m_EquipList.begin(); iterEquip != iterEquipEnd; iterEquip++)
			{
				strcpy(pPacket->equipName[(*iterEquip)->GetEquipType()], (*iterEquip)->GetTag().c_str());
			}
		}

		// �ڱ� �ڽ� üũ
		if ((*iter) == pObj)
		{
			pPacket->isSelf = true;
			pPacket->hp = ((CPlayer*)(*iter))->GetHP();
			pPacket->mp = ((CPlayer*)(*iter))->GetMP();
			pPacket->exp = ((CPlayer*)(*iter))->GetExp();
		}
		pPacket->tPos = (*iter)->GetPos();
		pPacket->size = sizeof(*pPacket);

		PSOCKETINFO pSocketInfo = new SOCKETINFO;
		pSocketInfo->Init(sock, SO_SEND);
		memcpy(pSocketInfo->buffer, pPacket, sizeof(*pPacket));
		delete pPacket;

		Send(pSocketInfo);
	}
}

void CClientManager::EnterPlayer(CObj * pObj)
{
	CScene* pScene = pObj->GetScene();
	if (!pScene)
	{
		printf("Enter error(scene error)!\n");
		return;
	}

	// ������ ��Ŷ �ʱ�ȭ
	PSC_CharacterEnter pPacket = new SC_CharacterEnter;
	memset(pPacket, 0, sizeof(*pPacket));
	memcpy(pPacket->name, pObj->GetTag().c_str(), sizeof(pPacket->name));
	pPacket->objState = pObj->GetObjState();
	pPacket->tPos = pObj->GetPos();

	CPlayer* pPlayer = (CPlayer*)pObj;

	list<CItem*>::iterator iterEquip;
	list<CItem*>::iterator iterEquipEnd = pPlayer->m_EquipList.end();

	for (iterEquip = pPlayer->m_EquipList.begin(); iterEquip != iterEquipEnd; iterEquip++)
	{
		strcpy(pPacket->equipName[(*iterEquip)->GetEquipType()], (*iterEquip)->GetTag().c_str());
	}

	pPacket->type = PT_ENTER;
	pPacket->size = sizeof(*pPacket);

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = pScene->m_UserList.end();

	for (iter = pScene->m_UserList.begin(); iter != iterEnd; iter++)
	{
		// �ڱ��ڽ� ����
		if (pObj == (*iter))
			continue;

		SOCKET sock = FindSocket((*iter));
		if (!sock)
		{
			printf("Enter error(socket error)!\n");
			continue;
		}

		PSOCKETINFO pSocketInfo = new SOCKETINFO;
		pSocketInfo->Init(sock, SO_SEND);
		memcpy(pSocketInfo->buffer, pPacket, sizeof(*pPacket));

		Send(pSocketInfo);
	}

	delete pPacket;
}

void CClientManager::LeavePlayer(CObj * pObj)
{
	CScene* pScene = pObj->GetScene();
	if (!pScene)
	{
		printf("Leave error(scene error)!\n");
		return;
	}

	// ������ ��Ŷ �ʱ�ȭ
	PSC_CharacterLeave pPacket = new SC_CharacterLeave;
	memset(pPacket, 0, sizeof(*pPacket));
	memcpy(pPacket->name, pObj->GetTag().c_str(), sizeof(pPacket->name));
	pPacket->type = PT_LEAVE;
	pPacket->size = sizeof(*pPacket);

	pScene->SetCriticEnter();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = pScene->m_UserList.end();

	for (iter = pScene->m_UserList.begin(); iter != iterEnd; iter++)
	{
		// �ڱ��ڽ� ����
		if (pObj == (*iter))
			continue;

		SOCKET sock = FindSocket((*iter));
		if (!sock)
		{
			printf("Leave error(socket error)!\n");
			return;
		}

		PSOCKETINFO pSocketInfo = new SOCKETINFO;
		pSocketInfo->Init(sock, SO_SEND);
		memcpy(pSocketInfo->buffer, pPacket, sizeof(*pPacket));

		Send(pSocketInfo);
	}

	pScene->SetCriticLeave();

	delete pPacket;
}

void CClientManager::DeleteObjectBroadcast(CObj * pObj)
{
	CScene* pScene = pObj->GetScene();
	if (!pScene)
	{
		printf("DeleteObjectBroadcast error(Cannot find scene)!\n");
		return;
	}

	pScene->SetCriticEnter();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = pScene->m_UserList.end();

	for (iter = pScene->m_UserList.begin(); iter != iterEnd; iter++)
	{
		// �ڱ��ڽ� ����
		if ((*iter) == pObj)
			continue;

		// �ش� ������ ���� �˻�
		SOCKET sock = FindSocket((*iter));
		if (!sock)
		{
			printf("DeleteObjectBroadcast error! INVALID SOCKET!\n");
			// ������Ʈ�� ����ִٸ� Die ȣ��
			if ((*iter)->GetLife())
				(*iter)->Die();
			continue;
		}

		PSOCKETINFO pSocketInfo = new SOCKETINFO;
		pSocketInfo->Init(sock, SO_SEND);

		// ���⼱ ������Ʈ ������ ���� �ּ����� ������ ��
		PSC_ObjectPacket pPacket = new SC_ObjectPacket;
		memset(pPacket, 0, sizeof(*pPacket));
		pPacket->type = PT_OBJECT;
		strcpy(pPacket->name, pObj->GetTag().c_str());
		pPacket->tPos = pObj->GetPos();
		pPacket->sceneNum = pObj->GetScene()->GetNum();
		pPacket->objType = pObj->GetObjType();
		pPacket->life = false;

		if (pPacket->objType == OT_MONSTER)
			pPacket->tOrigin = ((CMonster*)(*iter))->GetOriginPos();

		memcpy(pSocketInfo->buffer, pPacket, sizeof(*pPacket));
		delete pPacket;

		Send(pSocketInfo);
	}

	pScene->SetCriticLeave();
}

void CClientManager::EquipBroadCast(CObj* pObj, char* buffer)
{
	// �ش� ������ scene �˻�
	CScene* pScene = pObj->GetScene();
	if (!pScene)
	{
		printf("EquipBroadCast error(Cannot find scene)!\n");
		return;
	}

	pScene->SetCriticEnter();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = pScene->m_UserList.end();

	for (iter = pScene->m_UserList.begin(); iter != iterEnd; iter++)
	{
		// �ڱ��ڽ� ����
		if ((*iter) == pObj)
			continue;

		// �ش� ������ ���� �˻�
		SOCKET sock = FindSocket((*iter));
		if (!sock)
		{
			printf("EquipBroadCast error! INVALID SOCKET!\n");
			// ������Ʈ�� ����ִٸ� Die ȣ��
			if ((*iter)->GetLife())
				(*iter)->Die();
			continue;
		}

		PSOCKETINFO pSocketInfo = new SOCKETINFO;
		pSocketInfo->Init(sock, SO_SEND);
		if (PPACKET_BASE(buffer)->type == PT_ADDEQUIP)
		{
			PSC_AddEquip pPacket = new SC_AddEquip;
			strcpy(pPacket->name, PSC_AddEquip(buffer)->name);
			strcpy(pPacket->equipName, PSC_AddEquip(buffer)->equipName);
			pPacket->eEquipType = PSC_AddEquip(buffer)->eEquipType;
			pPacket->type = PT_ADDEQUIP;
			pPacket->size = sizeof(*pPacket);

			memcpy(pSocketInfo->buffer, pPacket, sizeof(*pPacket));
			delete pPacket;
		}
		else
		{
			PSC_EraseEquip pPacket = new SC_EraseEquip;
			strcpy(pPacket->name, PSC_EraseEquip(buffer)->name);
			pPacket->eEquipType = PSC_EraseEquip(buffer)->eEquipType;
			pPacket->type = PT_ERASEEQUIP;
			pPacket->size = sizeof(*pPacket);

			memcpy(pSocketInfo->buffer, pPacket, sizeof(*pPacket));
			delete pPacket;
		}

		Send(pSocketInfo);
	}

	pScene->SetCriticLeave();
}

void CClientManager::JoinWorkerThread()
{
	for (int i = 0; i < m_iThreads; i++)
	{
		WaitForSingleObject(m_pWorkerThreadsHandle[i], INFINITE);
	}
}
