#pragma once
#include "../Game.h"
#include "../Resources/Texture.h"

typedef struct _tagDamage
{
	vector<CTexture*> m_vecDamage; // ������ �ؽ���
	vector<POSITION> m_vecPos; // ��ġ
	int iLength; // ������ ����
	float fDelay; // ������ ������
	bool bEnable; // �����̰� ������ Ȱ��ȭ������ �÷���
	float fExistTime; // ������ �ð�

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
	map<string, CTexture*> m_mapNoCriTexture; // ũ��Ƽ�� X �ؽ��� ��
	map<string, CTexture*> m_mapCriTexture; // ũ��Ƽ�� O �ؽ��� ��
	list<PDAMAGE> m_DamageList; // ���� ����ؾ��ϴ� ������ ����Ʈ

	float m_fExistLimitTime; // �������� �����ִ� �ð�

private:
	CTexture* FindNumTexture(string strNum, bool bIsCri);

public:
	void CreateDamage(int iDamage, POSITION& tObjPos, bool bIsCri, int i = 1 /*���° ����������*/); // ����ؾ��ϴ� �������� ���� ��ġ�� �޾� ������ ����ü �����ϴ� �Լ�

public:
	bool Init();
	int Update(float fDeltaTime);
	void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
};

