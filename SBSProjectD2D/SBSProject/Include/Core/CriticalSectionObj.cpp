#include "CriticalSectionObj.h"

CCriticalSectionObj::CCriticalSectionObj()
{
	InitializeCriticalSection(&m_CriticalSection);
}

CCriticalSectionObj::~CCriticalSectionObj()
{
}

void CCriticalSectionObj::Enter()
{
	EnterCriticalSection(&m_CriticalSection);
}

void CCriticalSectionObj::Leave()
{
	LeaveCriticalSection(&m_CriticalSection);
}
