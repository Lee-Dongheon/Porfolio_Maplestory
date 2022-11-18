#include "Timer.h"

DEF_SINGLE(CTimer);

CTimer::CTimer() :
	m_fTimeScale(1.f)
{
}

CTimer::~CTimer()
{
}

bool CTimer::Init()
{
	QueryPerformanceFrequency(&m_tSecond);
	QueryPerformanceCounter(&m_tTime);

	m_fDeltaTime = 0.f;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFrame = 0;

	return true;
}

void CTimer::Update()
{
	srand((unsigned int)time(NULL));

	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);

	m_fDeltaTime = (tTime.QuadPart - m_tTime.QuadPart) / (float)m_tSecond.QuadPart;

	m_tTime = tTime;

	m_fFPSTime += m_fDeltaTime;
	m_iFrame++;

	if (m_fFPSTime >= 1.f)
	{
		m_fFPS = m_iFrame / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iFrame = 0;
//#ifdef _DEBUG
//		gotoxy(100, 0);
//		printf("FPS : %.f ", m_fFPS);
//#endif
	}
}
