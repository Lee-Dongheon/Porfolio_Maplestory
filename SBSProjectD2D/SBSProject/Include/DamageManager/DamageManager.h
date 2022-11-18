#pragma once
#include "../Game.h"
#include "../Resources/Texture.h"

typedef struct _tagDamage
{
	vector<CTexture*> m_vecDamage; // 데미지 텍스쳐
	vector<POSITION> m_vecPos; // 위치
	int iLength; // 데미지 길이
	float fDelay; // 데미지 딜레이
	bool bEnable; // 딜레이가 끝나면 활성화시켜줄 플래그
	float fExistTime; // 존재한 시간

	_tagDamage():
		fDelay(0.f),
		bEnable(false),
		fExistTime(0.f)
	{

	}
}DAMAGE, *PDAMAGE;

class CDamageManager
{
	DECLARE_SINGLE(CDamageManager)

private:
	map<string, CTexture*> m_mapNoCriTexture; // 크리티컬 X 텍스쳐 맵
	map<string, CTexture*> m_mapCriTexture; // 크리티컬 O 텍스쳐 맵
	list<PDAMAGE> m_DamageList; // 현재 출력해야하는 데미지 리스트

	float m_fExistLimitTime; // 데미지가 남아있는 시간

private:
	CTexture* FindNumTexture(string strNum, bool bIsCri);

public:
	void CreateDamage(int iDamage, POSITION& tObjPos, bool bIsCri, int i = 1 /*몇번째 데미지인지*/); // 출력해야하는 데미지의 값과 위치를 받아 데미지 구조체 생성하는 함수

public:
	bool Init();
	int Update(float fDeltaTime);
	void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
};

