#pragma once
#include "../Core/Ref.h"

class CSound :
	public CRef
{
private:
	friend class CResourcesManager;

public:
	CSound();
	~CSound();

public:
	// Sound
	IGraphBuilder* m_pGraph;
	IMediaControl* m_pControl;
	IMediaEvent* m_pEvent;
	IMediaSeeking* m_pSeeking;
	IBasicAudio* m_pAudio;

public:
	bool Init();
	void Update();

private:
	bool LoadSoundTrack(const string& strKey, const wchar_t* pFileName);
	
public:
	bool Play();
	bool Stop();
	void SetVolume(int i);
};

