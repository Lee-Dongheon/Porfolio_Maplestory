#include "Sound.h"

CSound::CSound() :
	m_pGraph(NULL),
	m_pControl(NULL),
	m_pEvent(NULL),
	m_pSeeking(NULL)
{
}

CSound::~CSound()
{
	SAFE_RELEASE(m_pAudio);
	SAFE_RELEASE(m_pSeeking);
	SAFE_RELEASE(m_pEvent);
	SAFE_RELEASE(m_pControl);
	SAFE_RELEASE(m_pGraph);
}

bool CSound::Init()
{
	// Create Filter Graph
	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void**)&m_pGraph);

	if (FAILED(hr))
		return false;

	hr = m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pControl);
	hr = m_pGraph->QueryInterface(IID_IMediaEvent, (void**)&m_pEvent);
	hr = m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)&m_pSeeking);
	hr = m_pGraph->QueryInterface(IID_IBasicAudio, (void**)&m_pAudio);

	return true;
}

void CSound::Update()
{
	long evCode;
	long param1, param2;
	HRESULT hr = m_pEvent->GetEvent(&evCode, &param1, &param2, 0);
	if (FAILED(hr))
		return;
	
	if (evCode == EC_COMPLETE)
		Play();
}

bool CSound::LoadSoundTrack(const string & strKey, const wchar_t * pFileName)
{
	SetTag(strKey);

	HRESULT hr = m_pGraph->RenderFile(pFileName, NULL);
	if (FAILED(hr))
		return false;

	return true;
}

bool CSound::Play()
{
	HRESULT hr = m_pControl->Run();
	if (FAILED(hr))
		return false;
	
	/*long evCode;
	m_pEvent->WaitForCompletion(INFINITE, &evCode);*/

	return true;
}

bool CSound::Stop()
{
	HRESULT hr = m_pControl->Stop();
	if (FAILED(hr))
		return false;

	LONGLONG nStartPosition = 0;
	hr = m_pSeeking->SetPositions(&nStartPosition, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	if (FAILED(hr))
		return false;

	return true;
}

void CSound::SetVolume(int i)
{
	HRESULT hr = m_pAudio->put_Volume(i);
	if (FAILED(hr))
		return;
}
