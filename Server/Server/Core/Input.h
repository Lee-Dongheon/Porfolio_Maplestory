#pragma once
#include "../server.h"

typedef struct _tagKeyInfo
{
	string strName; // 키 이름
	vector<DWORD> vecKey; // 눌려야 할 키들
	bool bPress; // 누름
	bool bDown; // 계속 누름
	bool bUp; // 뗌

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
	unordered_map<string, PKEYINFO> m_mapKey; // 이름을 키로 가지는 map 생성, 어떤 input 을 받을것인지에 대한 정보가 모두 담김
											  //unordered_map<PKEYINFO, CObj*> m_mapCustomKey; // Key와 소비, 스킬, 키 "아이콘"을 연결해주는 map
	PKEYINFO m_pCreateKey;
	vector<string> m_vecKeyPress; // 가장 최근에 keypress된 키의 string값
	vector<string> m_vecKeyDown; // 가장 최근에 keydown된 키의 string값
	vector<string> m_vecKeyUp; // 가장 최근에 keyup된 키의 string값

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

		if (strcmp(pTType, "char") == 0 || // 버튼에 대한 정보인 경우
			strcmp(pTType, "int") == 0)
		{
			m_pCreateKey->vecKey.push_back((DWORD)data); // vecKey에 저장
		}
		else // 이름인 경우
		{
			m_pCreateKey->strName = data; // 이름에 저장
			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey)); // map에 저장
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

