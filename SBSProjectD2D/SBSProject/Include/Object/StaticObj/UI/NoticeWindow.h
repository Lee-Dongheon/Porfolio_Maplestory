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
	POSITION m_tNoticePos; // 안내문구 위치(모두 동일, 서버 연결중만 다름
	CTexture* m_pTextures[WS_END]; // 안내문구 텍스쳐 배열
	WINDOW_STATE m_eWindowState; // 현재 윈도우 상태

	POSITION m_tCancelButtonPos; // 취소 버튼 위치
	CButton* m_pButton; // 취소 버튼

	POSITION m_tConnectingPos; // 연결중 애니메이션 위치
	class CLoadingObj* m_pConnecting; // 연결중 애니메이션 출력할 오브젝트

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
	void DisableWindow(); // 취소버튼과 연결할 함수
};

