#include "Scene.h"
#include "Layer.h"
#include "UILayer.h"
#include "../Object/Obj.h"
#include "../Resources/Sound.h"
#include "../Math.h"

//unordered_map<string, CObj*> CScene::m_mapPrototype;

CScene::CScene() :
	m_pBGM(NULL),
	m_tSceneRS(RESOLUTION(0, 0)),
	m_tSceneBackRS(RESOLUTION(0, 0)),
	m_pPlayer(NULL),
	m_tDefaultPos(POSITION()),
	m_fYPosMax(650.f)
{
	CreateLayer("Background", 0);
	CreateLayer("Object", 1);
	CreateUILayer("UI");

	if (m_LayerList.size() >= 2)
		m_LayerList.sort(CScene::LayerSort);
}

CScene::~CScene()
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; iter++)
	{
		SAFE_DELETE((*iter));
	}

	m_LayerList.clear();

	SAFE_RELEASE(m_pBGM);
	SAFE_RELEASE(m_pPlayer);
}

void CScene::ReleasePlayer()
{
	if (m_pPlayer)
	{
		// Object Layer에서 player 제거
		CLayer* pBeforeLayer = m_pPlayer->GetLayer();
		if (pBeforeLayer)
			pBeforeLayer->EraseObject(m_pPlayer);
		CUILayer* pLayer = (CUILayer*)FindLayer("UI");
		if (pLayer)
			pLayer->ReleasePlayer();
		SAFE_RELEASE(m_pPlayer);
	}
}

void CScene::KillOtherPlayer()
{
	// ReleasePlayer 전에 실행해야됨
	if (m_pPlayer)
	{
		CLayer* pPlayerLayer = m_pPlayer->GetLayer();
		// 플레이어와 같은 레이어에 속한 OT_PLAYER이지만 player와 다른 객체들
		// 즉 otherplayer를 지워준다.
		if (pPlayerLayer)
			pPlayerLayer->KillOtherPlayer(m_pPlayer);
	}
}

void CScene::SetPlayer(CObj * pObj, int iSceneNum)
{
	m_pPlayer = pObj;
	m_pPlayer->AddRef();

	CLayer* pLayer = FindLayer("Object");
	pLayer->AddObject(m_pPlayer);

	// 기존 scene으로 향하는 포탈의 위치로 player위치 설정
	// Tag가 없으면 default 위치로
	if (iSceneNum != -1)
	{
		POSITION pPos = pLayer->GetPortalPos(iSceneNum);
		if (pPos.x == 0.f && pPos.y == 0.f)
			pPos = m_tDefaultPos;
		m_pPlayer->SetPos(pPos);
	}
	else
		m_pPlayer->SetPos(m_tDefaultPos);
	
	pLayer = FindLayer("UI");

	if (pLayer)
		((CUILayer*)pLayer)->SetPlayer(m_pPlayer);
}

CLayer * CScene::CreateLayer(const string & strTag, int iZOrder)
{
	CLayer* pLayer = new CLayer;

	pLayer->SetTag(strTag);
	pLayer->SetZOrder(iZOrder);
	pLayer->SetScene(this);

	m_LayerList.push_back(pLayer); // Layer 생성시 바로 넣어줌

	return pLayer;
}

CLayer * CScene::CreateUILayer(const string & strTag, int iZOrder)
{
	CLayer* pLayer = new CUILayer;

	pLayer->SetTag(strTag);
	pLayer->SetZOrder(iZOrder);
	pLayer->SetScene(this);

	m_LayerList.push_back(pLayer);

	return pLayer;
}

CLayer * CScene::FindLayer(const string & strTag)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetTag() == strTag)
			return *iter;
	}
	return NULL;
}

void CScene::GetObjList(OBJECT_TYPE eType, list<CObj*>& objList, RECTANGLE & tRect)
{
	CLayer* pLayer = FindLayer("Object");

	if (!pLayer)
		return;

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = pLayer->m_ObjList.end();

	for (iter = pLayer->m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetObjType() == eType // 원하는 타입이고
			&& (*iter)->GetEnable() // 활성화된 상태이며
			&& PointInRect(tRect, (*iter)->GetPos())) // 지정한 rect안에 있으면
			objList.push_back((*iter));
	}
}

bool CScene::Init()
{
	CUILayer* pLayer = (CUILayer*)FindLayer("UI");
	if (!pLayer)
		return false;

	if (!pLayer->Init())
		return false;

	return true;
}

void CScene::Input(float fDeltaTime)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable()) // 비활성화 레이어일시
		{
			iter++;
			continue;
		}

		(*iter)->Input(fDeltaTime);

		if (!(*iter)->GetLife()) // 레이어 X
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			iter++;
	}
}

int CScene::Update(float fDeltaTime)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable()) // 비활성화 레이어일시
		{
			iter++;
			continue;
		}

		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLife()) // 레이어 X
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			iter++;
	}

	return 0;
}

int CScene::LateUpdate(float fDeltaTime)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable()) // 비활성화 레이어일시
		{
			iter++;
			continue;
		}

		(*iter)->LateUpdate(fDeltaTime);

		if (!(*iter)->GetLife()) // 레이어 X
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			iter++;
	}

	return 0;
}

void CScene::Collision(float fDeltaTime)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable()) // 비활성화 레이어일시
		{
			iter++;
			continue;
		}

		(*iter)->Collision(fDeltaTime);

		if (!(*iter)->GetLife()) // 레이어 X
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			iter++;
	}
}

void CScene::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable()) // 비활성화 레이어일시
		{
			iter++;
			continue;
		}

		(*iter)->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

		if (!(*iter)->GetLife()) // 레이어 X
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			iter++;
	}
}

bool CScene::LayerSort(CLayer * pL1, CLayer * pL2)
{
	return pL1->GetZOrder() < pL2->GetZOrder(); // <= 사용 절대금지! ZOrder가 가장 큰 레이어가 앞으로 오게됨
}