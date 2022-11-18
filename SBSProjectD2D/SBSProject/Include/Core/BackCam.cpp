#include "BackCam.h"
#include "../Object/Obj.h"

DEF_SINGLE(CBackCam);

CBackCam::CBackCam() :
	m_pTarget(NULL),
	m_tPos(POSITION(0.f,0.f))
{
}

CBackCam::~CBackCam()
{
	SAFE_RELEASE(m_pTarget);
}

void CBackCam::SetTarget(CObj * pObj)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pObj;

	if (m_pTarget)
		m_pTarget->AddRef();
}

bool CBackCam::Init(const POSITION & tPos, const RESOLUTION & tRS, const RESOLUTION & tWorldRS)
{
	m_tPos = tPos;
	m_tClientRS = tRS;
	m_tWorldRS = tWorldRS;
	m_tPivot = POSITION(0.5f, 0.7f);

	return true;
}

void CBackCam::Input(float fDeltaTime)
{
	if (!m_pTarget)
	{

	}
}

void CBackCam::Update(float fDeltaTime)
{
	if (m_pTarget)
	{
		POSITION tPos = m_pTarget->GetPos();
		POSITION tPivot = m_pTarget->GetPivot();
		_SIZE tSize = m_pTarget->GetSize();

		/*float fL = tPos.x - tPivot.x * tSize.x;
		float fT = tPos.y - tPivot.y * tSize.y;
		float fR = fL + tSize.x;
		float fB = fT + tSize.y;*/

		float fLeftArea = m_tClientRS.iW * m_tPivot.x;
		float fTopArea = m_tClientRS.iH * m_tPivot.y;
		float fRightArea = m_tClientRS.iW - fLeftArea;
		float fBottomArea = m_tClientRS.iH - fTopArea;

		if (tPos.x <= fLeftArea) // target이 left 경계 왼쪽에 있을경우 카메라 움직임 X
			m_tPos.x = 0.f;
		else if (tPos.x >= m_tWorldRS.iW - fRightArea) // target이 right 경계 오른쪽일 경우 카메라 움직임 X, 이때 카메라 값은 world - client
			m_tPos.x = m_tWorldRS.iW - m_tClientRS.iW;
		else // 그외의 경우 카메라는 player를 따라다님
			m_tPos.x = tPos.x - m_tClientRS.iW * m_tPivot.x;

		// y축도 마찬가지
		if (tPos.y <= fTopArea)
			m_tPos.y = 0.f;
		else if (tPos.y >= m_tWorldRS.iH - fBottomArea)
			m_tPos.y = m_tWorldRS.iH - m_tClientRS.iH;
		else
			m_tPos.y = tPos.y - m_tClientRS.iH * m_tPivot.y;
	}
}
