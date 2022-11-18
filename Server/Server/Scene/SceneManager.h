#pragma once
#include "../server.h"

class CScene;
class CLayer;

class CSceneManager
{
	DECLARE_SINGLE(CSceneManager)

private:
	map<int, CScene*> m_mapScene;

public:
	CScene* FindScene(int iNum);

public:
	void ChangeScene(class CObj* pObj, int iSceneNum); // obj�� iSceneNum�� ���� scene���� �̵�

public:
	bool Init();
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void BroadCast(float fDeltaTime);

private:
	template<typename T>
	T* CreateScene(const string& strTag)
	{
		T* pScene = new T;

		if (!pScene->Init())
		{
			SAFE_DELETE(pScene);
			return NULL;
		}

		pScene->SetTag(strTag);
		m_mapScene.insert(make_pair(pScene->GetNum(), pScene));

		return pScene;
	}
};