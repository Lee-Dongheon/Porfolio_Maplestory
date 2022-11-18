#pragma once
#include "../Game.h"

class CScene;
class CSound;
class CPlayer;
class CLayer;

class CSceneManager
{
	DECLARE_SINGLE(CSceneManager)
private:
	friend class CServerManager;

private:
	CPlayer* m_pPlayer;

	map<int, CScene*> m_mapScene; // 고유번호-Scene

	CScene* m_pCurrentScene;
	CSound* m_pCurrentBGM;
	int m_iSceneToChange;
	bool m_bActivateChanged;

	float m_fSceneChangeTime;
	float m_fSceneChangeLimitTime;

public:
	void SetActivateChanged(bool bChanged) { m_bActivateChanged = bChanged; }
	void SetSceneToChange(int iNum) { m_iSceneToChange = iNum; }
	CScene* GetCurrentScene() const { return m_pCurrentScene; }
	void SetPlayerName(const char* strName);
	string GetPlayerName() const;
	void SetPlayerChatMode(bool bChatMode);
	bool IsPlayer(class CObj* pObj);
	void StopBGM();

public:
	bool Init();
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);

private:
	CScene* FindScene(int iNum);
	void ChangeScene();

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

public:
	void CreateUIs();
};