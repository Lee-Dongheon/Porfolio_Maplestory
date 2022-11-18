#pragma once

#include "../server.h"

class CCriticalSectionObj
{
public:
	CCriticalSectionObj();
	~CCriticalSectionObj();

private:
	CRITICAL_SECTION m_CriticalSection;

public:
	void Enter();
	void Leave();
};

