#include "DamageManager.h"
#include "../Resources/ResourcesManager.h"

DEF_SINGLE(CDamageManager);

CDamageManager::CDamageManager() :
	m_fExistLimitTime(1.f)
{
}

CDamageManager::~CDamageManager()
{
	Safe_Release_Map(m_mapNoCriTexture);
	Safe_Release_Map(m_mapCriTexture);
	Safe_Delete_VecList(m_DamageList);
}

CTexture * CDamageManager::FindNumTexture(string strNum, bool bIsCri)
{
	map<string, CTexture*>::iterator iter;

	if (bIsCri)
	{
		iter = m_mapCriTexture.find(strNum);
		if (iter == m_mapCriTexture.end())
			return NULL;

		return iter->second;
	}
	else
	{
		iter = m_mapNoCriTexture.find(strNum);
		if (iter == m_mapNoCriTexture.end())
			return NULL;

		return iter->second;
	}
}

void CDamageManager::CreateDamage(int iDamage, POSITION& tObjPos, bool bIsCri, int i /*몇번째 데미지인지*/)
{
	// damage의 숫자 개수 * 20 / 2 = 첫 x 위치
	// 첫 숫자의 높이는 8 높게, 나머지는 1씩 차이나도록
	// y 좌표 간격 >> 8, 0, 1, 0, 1, 0, 1, ...
	// x 좌표 간격은 20으로 고정하고,
	// 첫 y 위치는 obj position - 100 으로 하고,
	// 데미지를 여러번 줄 경우 y간격은 26으로 하자.

	PDAMAGE pDamage = new DAMAGE;
	pDamage->fDelay = i * 0.1f;
	pDamage->bEnable = false;
	pDamage->fExistTime = 0.f;

	string strDamage = to_string(iDamage);
	pDamage->iLength = strDamage.length();
	POSITION tFirstPos = POSITION(tObjPos.x - (float)pDamage->iLength * 20.f / 2.f, tObjPos.y - 100.f - (i - 1) * 26.f);
	CTexture* pFirstTex = FindNumTexture(strDamage.substr(0, 1), bIsCri);
	pDamage->m_vecDamage.push_back(pFirstTex);
	pDamage->m_vecPos.push_back(tFirstPos);

	for (int i = 1; i < pDamage->iLength; i++)
	{
		CTexture* pTex = FindNumTexture(strDamage.substr(i, 1), bIsCri);
		pDamage->m_vecDamage.push_back(pTex);
		POSITION tPos = POSITION(tFirstPos.x + i * 20.f, tFirstPos.y + 8.f - (float)(i % 2));
		pDamage->m_vecPos.push_back(tPos);
	}

	m_DamageList.push_back(pDamage);
}

bool CDamageManager::Init()
{
	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);

	// NoCri Init
	CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri0", L"DamageSkin/NoCri/0.png", tOrigin, _SIZE(31.f, 33.f), tLT, POSITION(31.f, 33.f));
	m_mapNoCriTexture.insert(make_pair("0", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri1", L"DamageSkin/NoCri/1.png", tOrigin, _SIZE(22.f, 32.f), tLT, POSITION(22.f, 32.f));
	m_mapNoCriTexture.insert(make_pair("1", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri2", L"DamageSkin/NoCri/2.png", tOrigin, _SIZE(29.f, 33.f), tLT, POSITION(29.f, 33.f));
	m_mapNoCriTexture.insert(make_pair("2", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri3", L"DamageSkin/NoCri/3.png", tOrigin, _SIZE(28.f, 32.f), tLT, POSITION(28.f, 32.f));
	m_mapNoCriTexture.insert(make_pair("3", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri4", L"DamageSkin/NoCri/4.png", tOrigin, _SIZE(31.f, 33.f), tLT, POSITION(31.f, 33.f));
	m_mapNoCriTexture.insert(make_pair("4", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri5", L"DamageSkin/NoCri/5.png", tOrigin, _SIZE(29.f, 32.f), tLT, POSITION(29.f, 32.f));
	m_mapNoCriTexture.insert(make_pair("5", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri6", L"DamageSkin/NoCri/6.png", tOrigin, _SIZE(31.f, 33.f), tLT, POSITION(31.f, 33.f));
	m_mapNoCriTexture.insert(make_pair("6", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri7", L"DamageSkin/NoCri/7.png", tOrigin, _SIZE(29.f, 32.f), tLT, POSITION(29.f, 32.f));
	m_mapNoCriTexture.insert(make_pair("7", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri8", L"DamageSkin/NoCri/8.png", tOrigin, _SIZE(31.f, 33.f), tLT, POSITION(31.f, 33.f));
	m_mapNoCriTexture.insert(make_pair("8", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("NoCri9", L"DamageSkin/NoCri/9.png", tOrigin, _SIZE(31.f, 33.f), tLT, POSITION(31.f, 33.f));
	m_mapNoCriTexture.insert(make_pair("9", pTex));

	// Cri Init
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri0", L"DamageSkin/Cri/0.png", tOrigin, _SIZE(37.f, 38.f), tLT, POSITION(37.f, 38.f));
	m_mapCriTexture.insert(make_pair("0", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri1", L"DamageSkin/Cri/1.png", tOrigin, _SIZE(25.f, 38.f), tLT, POSITION(25.f, 38.f));
	m_mapCriTexture.insert(make_pair("1", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri2", L"DamageSkin/Cri/2.png", tOrigin, _SIZE(34.f, 38.f), tLT, POSITION(34.f, 38.f));
	m_mapCriTexture.insert(make_pair("2", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri3", L"DamageSkin/Cri/3.png", tOrigin, _SIZE(33.f, 38.f), tLT, POSITION(31.f, 38.f));
	m_mapCriTexture.insert(make_pair("3", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri4", L"DamageSkin/Cri/4.png", tOrigin, _SIZE(37.f, 38.f), tLT, POSITION(31.f, 38.f));
	m_mapCriTexture.insert(make_pair("4", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri5", L"DamageSkin/Cri/5.png", tOrigin, _SIZE(33.f, 38.f), tLT, POSITION(31.f, 38.f));
	m_mapCriTexture.insert(make_pair("5", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri6", L"DamageSkin/Cri/6.png", tOrigin, _SIZE(37.f, 38.f), tLT, POSITION(31.f, 38.f));
	m_mapCriTexture.insert(make_pair("6", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri7", L"DamageSkin/Cri/7.png", tOrigin, _SIZE(33.f, 38.f), tLT, POSITION(31.f, 38.f));
	m_mapCriTexture.insert(make_pair("7", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri8", L"DamageSkin/Cri/8.png", tOrigin, _SIZE(34.f, 39.f), tLT, POSITION(31.f, 39.f));
	m_mapCriTexture.insert(make_pair("8", pTex));
	pTex = GET_SINGLE(CResourcesManager)->LoadTexture("Cri9", L"DamageSkin/Cri/9.png", tOrigin, _SIZE(36.f, 38.f), tLT, POSITION(31.f, 38.f));
	m_mapCriTexture.insert(make_pair("9", pTex));
	return true;
}

int CDamageManager::Update(float fDeltaTime)
{
	list<PDAMAGE>::iterator iter;
	list<PDAMAGE>::iterator iterEnd = m_DamageList.end();

	for (iter = m_DamageList.begin(); iter != iterEnd;)
	{
		if ((*iter)->bEnable)
		{
			for (int i = 0; i < (*iter)->iLength; i++)
				(*iter)->m_vecPos[i].y -= 26.f * fDeltaTime;
			
			if ((*iter)->fExistTime >= m_fExistLimitTime) // 데미지 존재 제한시간이 지났다면 지워주기
			{
				SAFE_DELETE((*iter));
				iter = m_DamageList.erase(iter);
				iterEnd = m_DamageList.end();
			}
			else
			{
				(*iter)->fExistTime += fDeltaTime; // 데미지가 존재한 시간에 delta time을 더해줌
				if ((*iter)->fExistTime >= m_fExistLimitTime)
					(*iter)->fExistTime = m_fExistLimitTime;
				iter++;
			}
		}
		else
		{
			if ((*iter)->fDelay <= 0.f)
				(*iter)->bEnable = true;
			else
				(*iter)->fDelay -= fDeltaTime;
			iter++;
		}
	}

	return 0;
}

void CDamageManager::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	list<PDAMAGE>::iterator iter;
	list<PDAMAGE>::iterator iterEnd = m_DamageList.end();

	for (iter = m_DamageList.begin(); iter != iterEnd; iter++)
	{
		if (!(*iter)->bEnable)
			continue;

		int iLength = (*iter)->iLength;
		for (int i = 0; i < iLength; i++)
			(*iter)->m_vecDamage[i]->Render((*iter)->m_vecPos[i], pGDIRenderTarget, pRenderTarget, fDeltaTime, (m_fExistLimitTime - (*iter)->fExistTime) / m_fExistLimitTime, true);
	}
}