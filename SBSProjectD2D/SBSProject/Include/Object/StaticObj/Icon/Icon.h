#pragma once
#include "../../../Resources/Texture.h"
#include "../StaticObj.h"

typedef class CIcon :
    public CStaticObj
{
public:
    CIcon();
    CIcon(const CIcon& icon);
    virtual ~CIcon();

protected:
	POSITION m_tIconPos; // 아이콘 위치
	_SIZE m_tIconSize;
	POSITION m_tIconPivot;

	POSITION m_tKeyPos; // 키세팅 창에서의 위치
	POSITION m_tQuickPos; // 퀵슬롯 좌표

	BUTTON_TYPE m_eButtonType; // 버튼 상태
	ICON_TYPE m_eIconType; // 아이콘의 종류

	CTexture* m_pIcon[BT_END];
	CTexture* m_pIconTexture;

	bool m_bSelected; // 마우스에 클릭되었는지
	function<void()> m_IconFunction; // 키/버튼을 누르면 사용하는 함수 배열

public:
	void SetIconPos(POSITION tPos)					{ m_tIconPos = tPos; }
	void SetIconPos(float x, float y)				{ m_tIconPos.x = x; m_tIconPos.y = y; }
	
	void SetIconSize(float x, float y)				{ m_tIconSize.x = x; m_tIconSize.y = y; }
	void SetIconSize(_SIZE tSize)					{ m_tIconSize = tSize; }
	void SetIconPivot(float x, float y)				{ m_tIconPivot.x = x; m_tIconPivot.y = y; }
	void SetIconPivot(POSITION tPivot)				{ m_tIconPivot = tPivot; }
	
	void SetButtonType(BUTTON_TYPE eButtonType)		{ m_eButtonType = eButtonType; }
	void SetKeyPos(POSITION tPos)					{ m_tKeyPos = tPos; }
	void SetKeyPos(float x, float y)				{ m_tKeyPos.x = x; m_tKeyPos.y = y; }

	void SetQuickPos(POSITION tPos)					{ m_tQuickPos = tPos; }
	void SetQuickPos(float x, float y)				{ m_tQuickPos.x = x; m_tQuickPos.y = y; }

	void SetIconType(ICON_TYPE eType)				{ m_eIconType = eType; }
	
	void SetSelected(bool bSelected)				{ m_bSelected = bSelected; }

	template<typename T>
	void SetIconFunction(T* pObj, void (T::*function)())
	{
		m_IconFunction = bind(function, pObj);
	}

public:	
	POSITION GetIconPos() const			{ return m_tIconPos; }
	_SIZE GetIconSize() const			{ return m_tIconSize; }
	POSITION GetIconPivot() const		{ return m_tIconPivot; }

	BUTTON_TYPE GetButtonType() const	{ return m_eButtonType; }
	POSITION GetKeyPos() const			{ return m_tKeyPos; }
	POSITION GetQuickPos() const		{ return m_tQuickPos; }

	ICON_TYPE GetIconType() const		{ return m_eIconType; }
	bool GetSelected() const			{ return m_bSelected; }

#ifdef _DEBUG
	int GetRef() const { return m_iRef; }
#endif

public:
	void UseIcon()
	{
		if (m_IconFunction)
		{
			m_IconFunction();
		}
	}

public:
    virtual bool Init();
    virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	
	// 아이콘 업데이트
	int IconUpdate(float fDeltaTime);
	void IconRender(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	// 아이템은 아이템 ui에서의 위치, 스킬은 스킬 ui에서의 위치,
	// 버튼은 버튼을 가진 ui에서의 위치로 렌더링
	// 키세팅 ui에서의 위치로 렌더링
	// 같은 오브젝트를 렌더링 할 것
	// 복사생성 안하기 위함

	// 키세팅 창에서 업데이트
	int KeyUpdate(float fDeltaTime);
	void KeyRender(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	// 퀵슬롯에서 업데이트
	int QuickUpdate(float fDeltaTime);
	void QuickRender(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	// 퀵슬롯에서 렌더링
	// 퀵슬롯은 우선 키세팅에 올라가 있는 것 중에 퀵슬롯에 지정된 것만 보여줄 것
	// 렌더링 투명도는 우선 icon만 사용할것으로 예상되어
	// 아이콘의 render 함수에만 넣었다.
	// 투명도 적용되는 것들은 모두 아이콘 계열이므로!

	// 마우스 렌더
	void MouseRender(POSITION tPos, ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime, float fOpacity = 0.5);
	virtual CIcon* Clone();

public:
    void LoadButton(const string& strTag, const wchar_t* pFileName, POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	void LoadTextureIcon(const string& strTag, const wchar_t* pFileName, POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	void LoadItemIcon(const string& strTag);
}CButton;

