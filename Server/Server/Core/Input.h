#pragma once
#include "../server.h"

typedef struct _tagKeyInfo
{
	string strName; // Ű �̸�
	vector<DWORD> vecKey; // ������ �� Ű��
	bool bPress; // ����
	bool bDown; // ��� ����
	bool bUp; // ��

	_tagKeyInfo() :
		bPress(false),
		bDown(false),
		bUp(false)
	{

	}
}KEYINFO, *PKEYINFO;

class CObj;

class CInput
{
	DECLARE_SINGLE(CInput)

private:
	HWND m_hWnd;
	unordered_map<string, PKEYINFO> m_mapKey; // �̸��� Ű�� ������ map ����, � input �� ������������ ���� ������ ��� ���
											  //unordered_map<PKEYINFO, CObj*> m_mapCustomKey; // Key�� �Һ�, ��ų, Ű "������"�� �������ִ� map
	PKEYINFO m_pCreateKey;
	vector<string> m_vecKeyPress; // ���� �ֱٿ� keypress�� Ű�� string��
	vector<string> m_vecKeyDown; // ���� �ֱٿ� keydown�� Ű�� string��
	vector<string> m_vecKeyUp; // ���� �ֱٿ� keyup�� Ű�� string��

public:
	bool Init(HWND hWnd);
	void Update(float fDeltaTime);
	void Update(char* cInput);
	bool KeyPress(const string& strKey) const;
	bool KeyDown(const string& strKey) const;
	bool KeyUp(const string& strKey) const;

public:
	template<typename T>
	bool AddKey(const T& data)
	{
		const char* pTType = typeid(T).name();

		if (strcmp(pTType, "char") == 0 || // ��ư�� ���� ������ ���
			strcmp(pTType, "int") == 0)
		{
			m_pCreateKey->vecKey.push_back((DWORD)data); // vecKey�� ����
		}
		else // �̸��� ���
		{
			m_pCreateKey->strName = data; // �̸��� ����
			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey)); // map�� ����
		}
		return true;
	}

	template<typename T, typename ... Types>
	bool AddKey(const T& data, const Types& ...arg)
	{
		if (!m_pCreateKey)
			m_pCreateKey = new KEYINFO;

		const char* pTType = typeid(T).name();

		if (strcmp(pTType, "char") == 0 ||
			strcmp(pTType, "int") == 0)
		{
			m_pCreateKey->vecKey.push_back((DWORD)data);
		}
		else
		{
			m_pCreateKey->strName = data;
			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
		}

		AddKey(arg...);

		if (m_pCreateKey)
			m_pCreateKey = NULL;

		return true;
	}

public:
	vector<string> GetVecKeyPress() const { return m_vecKeyPress; }
	vector<string> GetVecKeyDown() const { return m_vecKeyDown; }
	vector<string> GetVecKeyUp() const { return m_vecKeyUp; }

private:
	PKEYINFO FindKey(const string& strKey) const;
};

