#include "Skill.h"
#include "../../../../Resources/ResourcesManager.h"

CSkill::CSkill():
	m_pPlayer(NULL),
	m_iStepLevel(0),
	m_iMaxLevel(0),
	m_iLevel(0),
	m_eType(ST_DEFAULT),
	m_pSkillName(NULL)
{
	m_eButtonType = BT_DISABLE;
}

CSkill::CSkill(const CSkill & skill):
	CIcon(skill)
{
	m_pPlayer = skill.m_pPlayer;
	m_iStepLevel = skill.m_iStepLevel;
	m_iMaxLevel = skill.m_iMaxLevel;
	m_iLevel = skill.m_iLevel;
	m_eType = skill.m_eType;

	m_pSkillName = NULL;
	SetSkillName(skill.m_pSkillName);
}

CSkill::~CSkill()
{
	SAFE_DELETE(m_pSkillName);
}

void CSkill::SetSkillName(wchar_t * pSkillName)
{
	if(!m_pSkillName)
		m_pSkillName = new wchar_t[30];

	wcscpy(m_pSkillName, pSkillName);
}

void CSkill::LevelUp()
{
	if (m_iLevel == 0)
		m_eButtonType = BT_NORMAL;
	
	m_iLevel++;

	if (m_iLevel > m_iMaxLevel)
		m_iLevel = m_iMaxLevel;
}

void CSkill::AddLevel(int i)
{
	m_iLevel += i;
	if (m_iLevel > m_iMaxLevel)
		m_iLevel = m_iMaxLevel;
}

bool CSkill::Init()
{
	SetIconType(IC_SKILL);
	
	return true;
}

void CSkill::Input(float fDeltaTime)
{
	CIcon::Input(fDeltaTime);
}

int CSkill::Update(float fDeltaTime)
{
	CIcon::Update(fDeltaTime);
	return 0;
}

int CSkill::LateUpdate(float fDeltaTime)
{
	CIcon::LateUpdate(fDeltaTime);
	return 0;
}

void CSkill::Collision(float fDeltaTime)
{
	CIcon::Collision(fDeltaTime);
}

void CSkill::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CIcon::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}