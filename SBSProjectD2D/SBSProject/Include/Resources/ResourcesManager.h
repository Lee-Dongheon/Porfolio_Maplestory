#pragma once
#include "../Game.h"

class CTexture;
class CSound;

class CResourcesManager
{
	DECLARE_SINGLE(CResourcesManager)

private:
	unordered_map<string, CTexture*> m_mapTexture;
	unordered_map<string, CSound*> m_mapSound;
	HINSTANCE m_hInst;
	HDC m_hDC;
	HWND m_hWnd;

	HFONT m_hNewFont;
	HFONT m_hOldFont;

	// 더블버퍼링
	HDC m_hBackBufferDC;
	HBITMAP m_hBackBitmapOld;
	HBITMAP m_hBackBitmap;

	// D2D
	ID2D1Factory* m_pFactory; // d2d 구성 객체를 생성하는 객체
	ID2D1HwndRenderTarget* m_pRenderTarget; // d2d rendering 객체
	ID2D1GdiInteropRenderTarget* m_pGDIRenderTarget; // DC에 그릴 수 있는 renderTarget
	
	// WIC
	IWICImagingFactory* m_pWICFactory;
	IWICBitmapDecoder* m_pDecoder;

public:
	ID2D1HwndRenderTarget* GetRenderTarget() const { return m_pRenderTarget; }
	ID2D1GdiInteropRenderTarget* GetGDIRenderTarget() const { return m_pGDIRenderTarget; }
	
public:
	bool Init(HINSTANCE hInst, HDC hDC, HWND hWnd);
	CTexture* LoadTexture(const string & strKey, const wchar_t * pFileName,
		POSITION& tOrigin, _SIZE& tSize, POSITION& tLT, POSITION& tRB);
	CTexture* FindTexture(const string& strKey);

public:
	CSound* LoadSoundTrack(const string& strKey, const wchar_t* pFileName);
	CSound* FindSoundTrack(const string& strKey);

public:
	void SetFont(HFONT& newFont, HFONT& oldFont, HDC hDC, int size);
	void ReleaseFont(HFONT& newFont, HFONT& oldFont, HDC hDC);
};

