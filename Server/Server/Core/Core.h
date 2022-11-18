#pragma once

#include "../server.h"

class CCore
{
	DECLARE_SINGLE(CCore)

private:
	static bool m_bLoop;

public:
	float m_fLogicTime;

public:
	bool Init();
	int Run();

private:
	void Logic();
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void BroadCast(float fDeltaTime);
	void Render(float fDeltaTime);

public:
	void Exit();
};

