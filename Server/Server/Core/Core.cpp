#include "Core.h"
#include "DBManager.h"
#include "ClientManager.h"
#include "Timer.h"
#include "../Scene/SceneManager.h"
#include "../Collider/CollisionManager.h"
#include "Input.h"

DEF_SINGLE(CCore);
bool CCore::m_bLoop = true;

CCore::CCore():
	m_fLogicTime(0.f)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(161);
}

CCore::~CCore()
{
	DESTROY_SINGLE(CDBManager);
	DESTROY_SINGLE(CClientManager);
	DESTROY_SINGLE(CTimer);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CCollisionManager);

#ifdef _DEBUG
	FreeConsole();
#endif
}

bool CCore::Init()
{
	if (!GET_SINGLE(CDBManager)->Init())
		return false;

	if (!GET_SINGLE(CClientManager)->Init())
		return false;

	if (!GET_SINGLE(CTimer)->Init())
		return false;

	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	return true;
}

int CCore::Run()
{
	while (m_bLoop)
	{
		Logic();
	}

	return 0;
}

void CCore::Logic()
{
	GET_SINGLE(CTimer)->Update();

	float fDeltaTime = GET_SINGLE(CTimer)->GetDeltaTime();

	Input(fDeltaTime);
	Update(fDeltaTime);
	LateUpdate(fDeltaTime);
	Collision(fDeltaTime);
	BroadCast(fDeltaTime);
	// 서버 렌더링은 디버깅용 명령창 렌더링
	Render(fDeltaTime);
}

void CCore::Input(float fDeltaTime)
{
	// Client에게서 패킷을 받아 정보 업데이트
	GET_SINGLE(CSceneManager)->Input(fDeltaTime);
}

int CCore::Update(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->Update(fDeltaTime);

	return 0;
}

int CCore::LateUpdate(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->LateUpdate(fDeltaTime);
	return 0;
}

void CCore::Collision(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->Collision(fDeltaTime);
	//GET_SINGLE(CCollisionManager)->Collision(fDeltaTime);
}

void CCore::BroadCast(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->BroadCast(fDeltaTime);
}

void CCore::Render(float fDeltaTime)
{
	m_fLogicTime += fDeltaTime;

	if (m_fLogicTime >= 3.f)
	{
		m_fLogicTime -= 3.f;
		printf("Logic Working...\n");
	}
}

void CCore::Exit()
{
	m_bLoop = false;
}
