#include "Skill.h"

CSkill::CSkill():
	m_iLevel(0),
	m_iStepLevel(0)
{
}

CSkill::CSkill(const CSkill & obj)
{
}

CSkill::~CSkill()
{
}

bool CSkill::Init()
{
	return true;
}

void CSkill::Input(float fDeltaTime)
{
}

int CSkill::Update(float fDeltaTime)
{
	return 0;
}

int CSkill::LateUpdate(float fDeltaTime)
{
	return 0;
}

//void CSkill::Collision(float fDeltaTime)
//{
//}

CSkill * CSkill::Clone()
{
	return new CSkill(*this);
}
