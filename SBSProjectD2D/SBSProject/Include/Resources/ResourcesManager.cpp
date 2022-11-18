#include "ResourcesManager.h"
#include "Texture.h"
#include "Sound.h"

DEF_SINGLE(CResourcesManager);

CResourcesManager::CResourcesManager() :
	m_pRenderTarget(NULL),
	m_pGDIRenderTarget(NULL),
	m_pFactory(NULL),
	m_pWICFactory(NULL)
{
}

CResourcesManager::~CResourcesManager()
{
	Safe_Release_Map(m_mapTexture);
	Safe_Release_Map(m_mapSound);

	SAFE_RELEASE(m_pGDIRenderTarget);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pWICFactory);
	SAFE_RELEASE(m_pFactory);
	CoUninitialize();
}

bool CResourcesManager::Init(HINSTANCE hInst, HDC hDC, HWND hWnd)
{
	m_hInst = hInst;
	m_hDC = hDC;
	m_hWnd = hWnd;

	// d2d 세팅
	// 컴포넌트 사용
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// d2d factory 객체 생성
	//D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pFactory);
	if (m_pFactory == NULL)
		throw std::runtime_error("D2DFactory Exception!");

	// WIC factory 생성
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));

	RECT r;
	GetClientRect(m_hWnd, &r);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	props.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
	props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

	// RenderTarget 생성
	m_pFactory->CreateHwndRenderTarget(props,
		HwndRenderTargetProperties(m_hWnd, SizeU(r.right, r.bottom)), &m_pRenderTarget);

	// GDI Compatible hWnd render Target 생성
	m_pRenderTarget->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (void**)&m_pGDIRenderTarget);

	if (!m_pRenderTarget->IsSupported(props))
	{
		//printf("GDI RenderTarget failed!\n");
		return false;
	}

	//printf("Init RenderTarget!\n");

	return true;
}

CTexture * CResourcesManager::LoadTexture(const string & strKey, const wchar_t * pFileName,
	POSITION& tOrigin, _SIZE& tSize, POSITION& tLT, POSITION& tRB)
{
	CTexture* pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;
	//HRESULT hResult;

	// png 파일 bitmap 디코더
	IWICBitmapDecoder* pDecoder;
	int len = wcslen(pFileName);
	wchar_t tempPath[] = L"../Bin/Texture/";
	len += wcslen(tempPath) + 1;
	wchar_t* tempFileName = new wchar_t[len];
	wcscpy(tempFileName, tempPath);
	wcscat(tempFileName, pFileName);
	m_pWICFactory->CreateDecoderFromFilename(tempFileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);

	// frame 선택, gif가 아니므로 인덱스는 0으로 고정
	IWICBitmapFrameDecode* pFrame;
	pDecoder->GetFrame(0, &pFrame);

	// wic bitmap을 d2d bitmap으로 변환
	IWICFormatConverter* pConverter;
	m_pWICFactory->CreateFormatConverter(&pConverter);
	pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);

	m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, &pTexture->m_pD2DBitmap);

	pTexture->SetTag(strKey);
	pTexture->SetOrigin(tOrigin);
	pTexture->SetSize(tSize);
	pTexture->SetLT(tLT);
	pTexture->SetRB(tRB);
	m_mapTexture.insert(make_pair(strKey, pTexture));

	SAFE_RELEASE(pConverter);
	SAFE_RELEASE(pFrame);
	SAFE_RELEASE(pDecoder);
	SAFE_DELETE_ARR(tempFileName);

	pTexture->AddRef();
	return pTexture;
}

CTexture * CResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, CTexture*>::iterator iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CSound* CResourcesManager::LoadSoundTrack(const string & strKey, const wchar_t * pFileName)
{
	CSound* pSound = FindSoundTrack(strKey);

	if (pSound)
		return pSound;

	pSound = new CSound;

	if (!pSound->Init())
	{
		SAFE_RELEASE(pSound);
		return NULL;
	}		

	int len = wcslen(pFileName);
	wchar_t* tempFileName = new wchar_t[len + 16];
	wcscpy(tempFileName, L"../Bin/Music/");
	wcscat(tempFileName, pFileName);

	pSound->LoadSoundTrack(strKey, tempFileName);
	pSound->AddRef();
	m_mapSound.insert(make_pair(strKey, pSound));

	delete[] tempFileName;

	return pSound;
}

CSound * CResourcesManager::FindSoundTrack(const string & strKey)
{
	unordered_map<string, CSound*>::iterator iter = m_mapSound.find(strKey);

	if (iter == m_mapSound.end())
		return NULL;

	return iter->second;
}

void CResourcesManager::SetFont(HFONT & newFont, HFONT & oldFont, HDC hDC, int size)
{
	newFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("돋움"));
	oldFont = (HFONT)SelectObject(hDC, newFont);
	SetBkMode(hDC, TRANSPARENT);
}

void CResourcesManager::ReleaseFont(HFONT & newFont, HFONT & oldFont, HDC hDC)
{
	SelectObject(hDC, oldFont);
	DeleteObject(newFont);
}
