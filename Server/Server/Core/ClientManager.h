#pragma once

#include "../server.h"

// overlapped 확장 구조체
// 동시에 completion key로 사용
typedef struct _tagOverlappedEx
{
	WSAOVERLAPPED overlapped;
	WSABUF dataBuf;
	SOCKET socket;

	// send 또는 recv 요청시마다 카운트할 변수
	// 구조체 지울 때 카운트가 0인지 체크할 것.
	int send = 0;
	int recv = 0;

	char buffer[PACKET_LENGTH];
	SOCKET_OPERATION_TYPE eType; // 소켓 정보 확인

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
	const int m_iServerPort; // 서버 포트 주소
	WSADATA m_wsaData; // wsadata
	SOCKET m_hServSock; // 서버 소켓
	SOCKADDR_IN m_ServAddr; // 서버 주소
	
	map<int, SOCKET> m_mapSocket; // 소켓 관리 맵
	map<SOCKET, class CObj*> m_mapCharacter; // 소켓-캐릭터 연결 맵
	PSOCKETINFO m_pSocketInfo; // 가장 최근에 Accept 대기 시킨 socketinfo
	HANDLE m_hIocp; // IOCP 객체 핸들
	HANDLE* m_pWorkerThreadsHandle; // 작업 스레드 핸들 포인터

	int m_iThreads; // 작업 스레드 수
	bool m_bAccept; // accept 플래그
	bool m_bWorkerThread; // 작업 스레드 플래그

public:
	SOCKET FindSocket(CObj* pObj);

public:
	void WorkerThread(); // 빈 작업스레드 찾아 IO 진행하는 함수
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
	// pObj : 받을 클라이언트 캐릭터 obj
	// objlist : 클라이언트가 속한 scene의 전체 objlist
	void EnterPlayer(CObj* pObj); // scene 입장시 다른 유저에게 알리는 브로드캐스트 함수
	void LeavePlayer(CObj* pObj); // scene 퇴장시 다른 유저에게 알리는 브로드캐스트 함수
	void DeleteObjectBroadcast(CObj* pObj);
	// pObj : 지워질 오브젝트
	void EquipBroadCast(CObj* pObj, char* buffer);
	// equip 정보를 가진 buffer

public:
	void JoinWorkerThread();
};

