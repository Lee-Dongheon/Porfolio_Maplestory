#include "PlayerParts.h"

CPlayerParts::CPlayerParts() :
	m_pPlayer(NULL),
	m_ePlayerObjState(OBJ_STATE_DEFAULT)
{
}

CPlayerParts::CPlayerParts(const CPlayerParts & parts) :
	CObj(parts)
{
	m_pPlayer = parts.m_pPlayer;
	m_ePlayerObjState = OBJ_STATE_DEFAULT;
}

CPlayerParts::~CPlayerParts()
{
}

void CPlayerParts::InitParts(PART_TYPE eType)
{
	if (eType == PART_HAIR)
	{

	}
	else if (eType == PART_FACE)
	{

	}
	else if (eType == PART_BODY)
	{

	}
	else if (eType == PART_ARM)
	{

	}
	else if (eType == PART_LHAND)
	{

	}
	else if (eType == PART_RHAND)
	{

	}
	else
	{
		printf("Init Parts error(wrong partType)\n");
	}
}

bool CPlayerParts::Init()
{
	SetObjType(OT_PART);

	return true;
}

void CPlayerParts::Input(float fDeltaTime)
{
	CObj::Input(fDeltaTime);
}

int CPlayerParts::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);

	// 플레이어 상태 받아오기 && 플레이어 위치에 맞게 파츠 위치 보정
	if (m_pPlayer)
	{
		m_ePlayerObjState = m_pPlayer->GetObjState();
		SetPos(m_pPlayer->GetPos());
	}

	return 0;
}

int CPlayerParts::LateUpdate(float fDeltaTime)
{
	CObj::LateUpdate(fDeltaTime);

	// 플레이어 상태에 따라 애니메이션 업데이트
	if (m_pAnimation)
	{
		int tempYPos;
		// objstate는 static obj면 default, moveobj면 그 외의 값
		if (m_eObjState != OBJ_STATE_DEFAULT)
		{
			switch (m_eObjState)
			{
			case OBJ_STATE_IDLE:
				m_pAnimation->ChangeClip("Idle");
				break;
			case OBJ_STATE_RUN:
				m_pAnimation->ChangeClip("Run");
				break;
			case OBJ_STATE_JUMP:
				m_pAnimation->ChangeClip("Jump");
				break;
			case OBJ_STATE_HIT:
				m_pAnimation->ChangeClip("Hit");
				break;
			case OBJ_STATE_ATTACK:
				m_pAnimation->ChangeClip("Attack");
				break;
			case OBJ_STATE_PRONE:
				m_pAnimation->ChangeClip("Prone");
				break;
			case OBJ_STATE_ROPE:
				tempYPos = (int)(m_pPlayer->GetPos().y);
				if ((tempYPos / 30) % 2 == 0)
					m_pAnimation->ChangeClip("Rope1");
				else
					m_pAnimation->ChangeClip("Rope2");
				break;
			case OBJ_STATE_LADDER:
				tempYPos = (int)(m_pPlayer->GetPos().y);
				if ((tempYPos / 30) % 2 == 0)
					m_pAnimation->ChangeClip("Ladder1");
				else
					m_pAnimation->ChangeClip("Ladder2");
				break;
			default:
				break;
			}
		}
	}

	return 0;
}

void CPlayerParts::Collision(float fDeltaTime)
{
	CObj::Collision(fDeltaTime);
}

void CPlayerParts::Render(ID2D1GdiInteropRenderTarget * pGDIRenderTarget, ID2D1HwndRenderTarget * pRenderTarget, float fDeltaTime)
{
	CObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CPlayerParts * CPlayerParts::Clone()
{
	return new CPlayerParts(*this);
}
