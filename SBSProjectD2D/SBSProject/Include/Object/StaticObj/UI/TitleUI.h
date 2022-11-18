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
	static int m_iLogin; // �α��� ����/���� ����
	// -1 : default
	// 0 : ���̵� Ʋ��
	// 1 : ��й�ȣ Ʋ��
	// 2 : ����

	float m_fLoginTime;

	list<CTexture*> m_TextureList; // UI�� ���� texture
	list<CButton*> m_ButtonList;
	unordered_map<string, CTab*> m_mapTab;
	map<string, POSITION> m_mapPos; // texture�� button���� ��ġ�� ������ ��

	class CNoticeWindow* m_pNoticeWindow; // �ȳ��޼��� â

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