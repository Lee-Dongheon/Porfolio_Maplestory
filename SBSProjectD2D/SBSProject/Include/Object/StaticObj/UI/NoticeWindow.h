#pragma once
#include "../StaticObj.h"
#include "../Icon/Icon.h"

class CNoticeWindow :
	public CStaticObj
{
public:
	CNoticeWindow();
	CNoticeWindow(const CNoticeWindow& obj);
	~CNoticeWindow();

public:
	POSITION m_tNoticePos; // �ȳ����� ��ġ(��� ����, ���� �����߸� �ٸ�
	CTexture* m_pTextures[WS_END]; // �ȳ����� �ؽ��� �迭
	WINDOW_STATE m_eWindowState; // ���� ������ ����

	POSITION m_tCancelButtonPos; // ��� ��ư ��ġ
	CButton* m_pButton; // ��� ��ư

	POSITION m_tConnectingPos; // ������ �ִϸ��̼� ��ġ
	class CLoadingObj* m_pConnecting; // ������ �ִϸ��̼� ����� ������Ʈ

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);

	virtual CNoticeWindow* Clone();

public:
	void SetWindowState(WINDOW_STATE eWindowState);

public:
	void DisableWindow(); // ��ҹ�ư�� ������ �Լ�
};

