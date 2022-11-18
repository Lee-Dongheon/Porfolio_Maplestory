#include "EquipParts.h"

CEquipParts::CEquipParts()
{
}

CEquipParts::CEquipParts(const CEquipParts & obj):
	CObj(obj)
{
}

CEquipParts::~CEquipParts()
{
}

bool CEquipParts::Init()
{
	return true;
}

void CEquipParts::Input(float fDeltaTime)
{
	CObj::Input(fDeltaTime);
}

int CEquipParts::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);

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
				tempYPos = (int)(m_tPos.y);
				if ((tempYPos / 30) % 2 == 0)
					m_pAnimation->ChangeClip("Rope1");
				else
					m_pAnimation->ChangeClip("Rope2");
				break;
			case OBJ_STATE_LADDER:
				tempYPos = (int)(m_tPos.y);
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

int CEquipParts::LateUpdate(float fDeltaTime)
{
	CObj::LateUpdate(fDeltaTime);
	return 0;
}

void CEquipParts::Collision(float fDeltaTime)
{
	CObj::Collision(fDeltaTime);
}

void CEquipParts::Render(ID2D1GdiInteropRenderTarget * pGDIRenderTarget, ID2D1HwndRenderTarget * pRenderTarget, float fDeltaTime)
{
	CObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CEquipParts * CEquipParts::Clone()
{
	return new CEquipParts(*this);
}
