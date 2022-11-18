#pragma once

#include "../Game.h"

// overlapped 확장 구조체
// 동시에 completion key로 사용
//typedef struct _tagOverlappedEx
//{
//	WSAOVERLAPPED overlapped;
//	WSABUF dataBuf;
//	SOCKET socket;
//
//	//int sendBytes;
//	//int recvBytes;
//
//	char buffer[PACKET_LENGTH];
//	SOCKET_OPERATION_TYPE eType; // 소켓 정보 확인
//
//}SOCKETINFO, *PSOCKETINFO;

class CServerManager
{
	DECLARE_SINGLE(CServerManager)

private:
	const int m_iServerPort; // 서버 포트 주소
	const char* m_strServerAddr; // 서버 아이피 주소
	WSADATA m_wsaData; // wsadata
	SOCKET m_hServSock; // 소켓
	SOCKADDR_IN m_ServAddr; // 서버 주소

	//HANDLE m_hIocp;
	//HANDLE* m_pWorkerThreadHandle; // 작업 스레드 핸들

	//int m_iThreads; // 작업스레드 수
	//bool m_bWorkerThread;
	HANDLE* m_hThread; // Recv 무한루프 스레드 핸들

	int m_iLoginSceneNum; // 접속시 받아오는 scene num

public:
	bool Init();
	void Run();
	//bool CreateWorkerThread();
	
public:
	int GetLoginSceneNum() const { return m_iLoginSceneNum; }
/*public:
	void WorkerThread();*/
	/*void Send(PSOCKETINFO pSockInfo);
	void Recv(PSOCKETINFO pSockInfo);*/
/*
public:
	bool PacketProcess(PSOCKETINFO pSockInfo);*/

public:
	void RecvCreateAccount(char* buffer);
	void RecvLogIn(char* buffer);
	void RecvStatus(char* buffer);
	void RecvSkillInfo(char* buffer);
	void RecvItemInfo(char* buffer);
	void RecvEquipInfo(char* buffer);
	void RecvPosition(char* buffer);
	void RecvMessagePacket(char* buffer);
	void RecvObjectPacket(char* buffer);
	void RecvEnterPacket(char* buffer);
	void RecvLeavePacket(char* buffer);
	void RecvUseSkillPacket(char* buffer);
	void RecvGetItem(char* buffer);
	void RecvLevelUp(char* buffer);
	void RecvAddEquip(char* buffer);
	void RecvEraseEquip(char* buffer);

public:
	bool SendCreateAccount(char* id, char* pw);
	bool SendLogIn(char* id, char* pw);
	bool SendMoveCharacter(class CObj* pObj);
	bool SendMessagePacket(const char* name, const char * message);
	bool SendPortal(int iSceneFrom, int iSceneTo);
	bool SendUseSkill(int iSceneNum, int iSkillStep, const string& strName, const string& strSkillName);
	bool SendChangeSkill(const string& strTag, int iLevel);
	bool SendChangeItem(const string& strTag, ITEM_TYPE eType, int iNum, POSITION tPos, bool bErased = false);
	bool SendThrowItem(const string& strTag, ITEM_TYPE eType, int iNum, POSITION tPos);
	bool SendGetItem(const string& strTag, ITEM_TYPE eType, POSITION tPos, POSITION tCoord);
	bool SendDamage(int iSceneNum, POSITION tOrigin, int iDamage);
	bool SendAddEquip(const string& strTag, const string& strEquipTag, EQUIP_TYPE eType);
	bool SendEraseEquip(const string& strTag, EQUIP_TYPE eType);

public:
	void DisconnectServer();
};

