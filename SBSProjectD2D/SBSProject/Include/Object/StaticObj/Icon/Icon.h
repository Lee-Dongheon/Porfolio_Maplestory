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
	POSITION m_tIconPos; // ������ ��ġ
	_SIZE m_tIconSize;
	POSITION m_tIconPivot;

	POSITION m_tKeyPos; // Ű���� â������ ��ġ
	POSITION m_tQuickPos; // ������ ��ǥ

	BUTTON_TYPE m_eButtonType; // ��ư ����
	ICON_TYPE m_eIconType; // �������� ����

	CTexture* m_pIcon[BT_END];
	CTexture* m_pIconTexture;

	bool m_bSelected; // ���콺�� Ŭ���Ǿ�����
	function<void()> m_IconFunction; // Ű/��ư�� ������ ����ϴ� �Լ� �迭

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
	
	
	// ������ ������Ʈ
	int IconUpdate(float fDeltaTime);
	void IconRender(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	// �������� ������ ui������ ��ġ, ��ų�� ��ų ui������ ��ġ,
	// ��ư�� ��ư�� ���� ui������ ��ġ�� ������
	// Ű���� ui������ ��ġ�� ������
	// ���� ������Ʈ�� ������ �� ��
	// ������� ���ϱ� ����

	// Ű���� â���� ������Ʈ
	int KeyUpdate(float fDeltaTime);
	void KeyRender(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	// �����Կ��� ������Ʈ
	int QuickUpdate(float fDeltaTime);
	void QuickRender(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	// �����Կ��� ������
	// �������� �켱 Ű���ÿ� �ö� �ִ� �� �߿� �����Կ� ������ �͸� ������ ��
	// ������ ������ �켱 icon�� ����Ұ����� ����Ǿ�
	// �������� render �Լ����� �־���.
	// ���� ����Ǵ� �͵��� ��� ������ �迭�̹Ƿ�!

	// ���콺 ����
	void MouseRender(POSITION tPos, ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime, float fOpacity = 0.5);
	virtual CIcon* Clone();

public:
    void LoadButton(const string& strTag, const wchar_t* pFileName, POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	void LoadTextureIcon(const string& strTag, const wchar_t* pFileName, POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	void LoadItemIcon(const string& strTag);
}CButton;

