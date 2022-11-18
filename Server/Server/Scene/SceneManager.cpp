#include "SceneManager.h"

#include "Scene.h"

#include "VillageScene.h"
#include "MushroomScene.h"
#include "MushmomScene.h"

#include "../Core/Core.h"

#include "../Object/Obj.h"

DEF_SINGLE(CSceneManager);

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
	Safe_Delete_Map(m_mapScene);
}

CScene * CSceneManager::FindScene(int iNum)
{
	map<int, CScene*>::iterator iter = m_mapScene.find(iNum);
	if (iter == m_mapScene.end())
		return NULL;

	return iter->second;
}

void CSceneManager::ChangeScene(CObj* pObj, int iSceneNum)
{
	CScene* pCurrentScene = pObj->GetScene();

}

bool CSceneManager::Init()
{
	// Scene 생성하기
	CVillageScene* pVillageScene = CreateScene<CVillageScene>("VillageScene");
	CMushroomScene* pMushroomScene = CreateScene<CMushroomScene>("MushroomScene");
	CMushmomScene* pMushmomScene = CreateScene<CMushmomScene>("MushmomScene");

	return true;
}

void CSceneManager::Input(float fDeltaTime)
{
	map<int, CScene*>::iterator iter;
	map<int, CScene*>::iterator iterEnd = m_mapScene.end();

	for (iter = m_mapScene.begin(); iter != iterEnd; iter++)
	{
		(iter->second)->Input(fDeltaTime);
	}
}

int CSceneManager::Update(float fDeltaTime)
{
	map<int, CScene*>::iterator iter;
	map<int, CScene*>::iterator iterEnd = m_mapScene.end();

	for (iter = m_mapScene.begin(); iter != iterEnd; iter++)
	{
		(iter->second)->Update(fDeltaTime);
	}

	return 0;
}

int CSceneManager::LateUpdate(float fDeltaTime)
{
	map<int, CScene*>::iterator iter;
	map<int, CScene*>::iterator iterEnd = m_mapScene.end();

	for (iter = m_mapScene.begin(); iter != iterEnd; iter++)
	{
		(iter->second)->LateUpdate(fDeltaTime);
	}

	return 0;
}

void CSceneManager::Collision(float fDeltaTime)
{
	map<int, CScene*>::iterator iter;
	map<int, CScene*>::iterator iterEnd = m_mapScene.end();

	for (iter = m_mapScene.begin(); iter != iterEnd; iter++)
	{
		(iter->second)->Collision(fDeltaTime);
	}
}

void CSceneManager::BroadCast(float fDeltaTime)
{
	map<int, CScene*>::iterator iter;
	map<int, CScene*>::iterator iterEnd = m_mapScene.end();

	for (iter = m_mapScene.begin(); iter != iterEnd; iter++)
	{
		(iter->second)->BroadCast(fDeltaTime);
	}
}
