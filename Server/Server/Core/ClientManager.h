#pragma once

#include "../server.h"

// overlapped Ȯ�� ����ü
// ���ÿ� completion key�� ���
typedef struct _tagOverlappedEx
{
	WSAOVERLAPPED overlapped;
	WSABUF dataBuf;
	SOCKET socket;

	// send �Ǵ� recv ��û�ø��� ī��Ʈ�� ����
	// ����ü ���� �� ī��Ʈ�� 0���� üũ�� ��.
	int send = 0;
	int recv = 0;

	char buffer[PACKET_LENGTH];
	SOCKET_OPERATION_TYPE eType; // ���� ���� Ȯ��

	void Init(SOCKET _socket, SOCKET_OPERATION_TYPE _eType)
	{
		memset(&overlapped, 0, sizeof(WSAOVERLAPPED));
		dataBuf.buf = buffer;
		dataBuf.len = PACKET_LENGTH;
		socket = _socket;
		eType = _eType;
	}

	void Init(SOCKET_OPERATION_TYPE _eType)
	{
		memset(&overlapped, 0, sizeof(WSAOVERLAPPED));
		dataBuf.buf = buffer;
		dataBuf.len = PACKET_LENGTH;
		eType = _eType;
	}

}SOCKETINFO, *PSOCKETINFO;

class CClientManager
{
	DECLARE_SINGLE(CClientManager)

private:
	const int m_iServerPort; // ���� ��Ʈ �ּ�
	WSADATA m_wsaData; // wsadata
	SOCKET m_hServSock; // ���� ����
	SOCKADDR_IN m_ServAddr; // ���� �ּ�
	
	map<int, SOCKET> m_mapSocket; // ���� ���� ��
	map<SOCKET, class CObj*> m_mapCharacter; // ����-ĳ���� ���� ��
	PSOCKETINFO m_pSocketInfo; // ���� �ֱٿ� Accept ��� ��Ų socketinfo
	HANDLE m_hIocp; // IOCP ��ü �ڵ�
	HANDLE* m_pWorkerThreadsHandle; // �۾� ������ �ڵ� ������

	int m_iThreads; // �۾� ������ ��
	bool m_bAccept; // accept �÷���
	bool m_bWorkerThread; // �۾� ������ �÷���

public:
	SOCKET FindSocket(CObj* pObj);

public:
	void WorkerThread(); // �� �۾������� ã�� IO �����ϴ� �Լ�
	void Send(PSOCKETINFO pSockInfo);
	void Recv(PSOCKETINFO pSockInfo);

public:
	bool Init();
	bool CreateWorkerThreads();

public:
	bool PacketProcess(PSOCKETINFO pSockInfo);

public:
	void BroadCast(PSOCKETINFO pSockInfo);

public:
	bool CreateAccount(PSOCKETINFO pSockInfo);
	bool LogIn(PSOCKETINFO pSockInfo);
	bool DeleteAccount(PSOCKETINFO pSockInfo);
	bool MoveCharacter(PSOCKETINFO pSockInfo);
	bool Message(PSOCKETINFO pSockInfo);
	bool Portal(PSOCKETINFO pSockInfo);
	bool UseSkill(PSOCKETINFO pSockInfo);
	bool ChangeSkill(PSOCKETINFO pSockInfo);
	bool ChangeItem(PSOCKETINFO pSockInfo);
	bool ThrowItem(PSOCKETINFO pSockInfo);
	bool GetItem(PSOCKETINFO pSockInfo);
	bool Damage(PSOCKETINFO pSockInfo);
	bool AddEquip(PSOCKETINFO pSockInfo);
	bool EraseEquip(PSOCKETINFO pSockInfo);

public:
	void SendLevelUp(CObj* pObj);

public:
	void BroadCast(CObj* pObj, list<CObj*>& ObjList);
	// pObj : ���� Ŭ���̾�Ʈ ĳ���� obj
	// objlist : Ŭ���̾�Ʈ�� ���� scene�� ��ü objlist
	void EnterPlayer(CObj* pObj); // scene ����� �ٸ� �������� �˸��� ��ε�ĳ��Ʈ �Լ�
	void LeavePlayer(CObj* pObj); // scene ����� �ٸ� �������� �˸��� ��ε�ĳ��Ʈ �Լ�
	void DeleteObjectBroadcast(CObj* pObj);
	// pObj : ������ ������Ʈ
	void EquipBroadCast(CObj* pObj, char* buffer);
	// equip ������ ���� buffer

public:
	void JoinWorkerThread();
};

