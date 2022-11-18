#pragma once
#include "UI.h"

class CUIMain :
	public CUI
{
private:
	friend class CObj;

protected:
	CUIMain();
	CUIMain(const CUIMain& ui);
	virtual ~CUIMain();

private:
	bool m_bQuickExtend;
	bool m_bQuickChange;
	float m_fQuickChangeTime;
	float m_fQuickChangeLimitTime;

	float m_fQuickFoldPosX;
	float m_fQuickExtendPosX;

	map<int, CTexture*> m_mapNum;
	map<int, CTexture*> m_mapLevelNum;
	map<POSITION, string> m_mapQuickPos; // ������ ��ǥ�� �� ��ġ�� Ű

	RECTANGLE m_tMessageBox; // ä�� ��� �ڽ�
	RECTANGLE m_tChatBox; // ä�� �ڽ�
	RECTANGLE m_tSendButton; // ������ ��ư
	class CEditBox* m_pChatEditBox; // ä�� �ڽ�

	char* m_strMessageBuffer; // �޼��� ����

	int m_iLastMessage; // �޼����� ������ index
	char* m_strMessages[5]; // ��ȯ ������ �޼����� ����

public:
	void RecvMessage(PPACKETMESSAGE pPacket);

private:
	CTexture* FindNum(int i);
	CTexture* FindLevelNum(int i);

public:
	CEditBox* GetChatBox() const { return m_pChatEditBox; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CUIMain* Clone();

public:
	void QuickSlot();
	void Setting();
	void Exit();

public:
	void AddNums(const string & strTag, const wchar_t * pFileName, POSITION& tOrigin, POSITION& tLT, POSITION& tRB, bool bIsLevel = false);

	void KeyImageInit();
	void QuickSlotInit();
	string FindQuickTag(POSITION tPos);
};