#pragma once
#include "../StaticObj.h"
#include "../Icon/Icon.h"
#include "../Tab/Tab.h"

class CTitleUI :
	public CStaticObj
{
private:
	friend class CObj;

private:
	CTitleUI();
	CTitleUI(const CTitleUI& ui);
	~CTitleUI();

private:
	class CEditBox* m_pIDBox;
	class CEditBox* m_pPWBox;

	RECTANGLE m_ID;
	RECTANGLE m_PW;

	char* m_strID;
	char* m_strPW;
	
	bool m_bButtonClicked;

	static bool m_bCreateAccount;
	static bool m_bLogin;
	static int m_iLogin; // 로그인 성공/실패 여부
	// -1 : default
	// 0 : 아이디 틀림
	// 1 : 비밀번호 틀림
	// 2 : 성공

	float m_fLoginTime;

	list<CTexture*> m_TextureList; // UI에 들어가는 texture
	list<CButton*> m_ButtonList;
	unordered_map<string, CTab*> m_mapTab;
	map<string, POSITION> m_mapPos; // texture와 button들의 위치를 가지는 맵

	class CNoticeWindow* m_pNoticeWindow; // 안내메세지 창

public:
	static void SetCreateAccount(bool bAccount) { m_bCreateAccount = bAccount; }
	static void SetLogin(bool bLogin) { m_bLogin = bLogin; }
	static void SetLoginNum(int iLogin) { m_iLogin = iLogin; }

public:
	void EditInit();
	void DestroyEdit();

	void Login();
	void SignUp();
	void Quit();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CTitleUI* Clone();

public:
	CButton* AddButton(const string & strKey, const wchar_t * pFileName, POSITION& tPos,
		POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	CTexture* AddUITexture(const string & strKey, const wchar_t * pFileName, POSITION& tPos,
		POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	CTab* AddTab(const string & strKey, const wchar_t * pFileName, POSITION& tPos,
		POSITION& tOrigin, POSITION& tLT, POSITION& tRB);

public:
	void SetPosition(const string& strKey, POSITION& tPos);

public:
	POSITION FindPosition(const string& strKey);
};