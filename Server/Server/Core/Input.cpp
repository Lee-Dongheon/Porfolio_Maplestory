#include "Input.h"

DEF_SINGLE(CInput);

CInput::CInput() :
	m_pCreateKey(NULL)
{
}

CInput::~CInput()
{
	Safe_Delete_Map(m_mapKey);
	//Safe_Delete_Map(m_mapCustomKey);
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	AddKey(VK_LEFT, "MoveLeft");
	AddKey(VK_RIGHT, "MoveRight");
	AddKey(VK_UP, "Up");
	AddKey(VK_DOWN, "Down");

	AddKey(VK_TAB, "Tab");

	for (int i = 0; i < 26; i++)
	{
		char ABC = (char)(i + 65);
		char abc[2];
		abc[0] = (char)(i + 97);
		abc[1] = '\0';
		AddKey(ABC, abc);
	}

	AddKey(VK_MENU, "Alt");
	AddKey(VK_CONTROL, "Ctrl");
	AddKey(VK_ESCAPE, "ESC");
	AddKey(VK_SHIFT, "Shift");
	AddKey(VK_SPACE, "Space");
	AddKey(VK_OEM_5, "\\"); // '\'Ű
	AddKey(VK_OEM_6, "]"); // ] Ű
	AddKey(VK_OEM_4, "["); // ] Ű

	AddKey('1', "1");
	AddKey('2', "2");
	AddKey('3', "3");
	AddKey('4', "4");
	AddKey('5', "5");
	AddKey('6', "6");
	AddKey('7', "7");
	AddKey('8', "8");
	AddKey('9', "9");
	AddKey('0', "0");
	AddKey(VK_CONTROL, 'M', "Master");

	return true;
}

void CInput::Update(float fDeltaTime)
{
	m_vecKeyPress.clear();
	m_vecKeyDown.clear();
	m_vecKeyUp.clear();

	unordered_map<string, PKEYINFO>::iterator iter;
	unordered_map<string, PKEYINFO>::iterator iterEnd = m_mapKey.end();

	for (iter = m_mapKey.begin(); iter != iterEnd; iter++)
	{
		int iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); i++)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000) // vecKey�� �ش��ϴ� Ű�� ��� �����ִ��� üũ
				iPushCount++;
		}
		if (iPushCount == iter->second->vecKey.size()) // ���� �䱸�ϴ� Ű�� ��� ���ȴٸ�
		{
			if (!iter->second->bPress && !iter->second->bDown) // false�� ��� true�� ����������
			{
				iter->second->bPress = true;
				iter->second->bDown = true;
			}
			else if (iter->second->bPress) // ó�� ���� ���Ŀ��� �Է��� �ִ� ���
				iter->second->bPress = false;
		}
		else // ���� ��� �ȴ������� ���
		{
			if (iter->second->bPress || iter->second->bDown) // �ȴ��� �����ε� ���� ���°� ó�� �����ų� ������ �־��� ���
			{
				iter->second->bUp = true;
				iter->second->bPress = false;
				iter->second->bDown = false;
			}
			else if (iter->second->bUp) // ó�� ���� ����
				iter->second->bUp = false;
		}

		if (iter->second->bPress)
			m_vecKeyPress.push_back(iter->first);
		if (iter->second->bDown)
			m_vecKeyDown.push_back(iter->first);
		if (iter->second->bUp)
			m_vecKeyUp.push_back(iter->first);
	}
}

void CInput::Update(char* cInput)
{
}

bool CInput::KeyPress(const string & strKey) const
{
	PKEYINFO pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bPress;
}

bool CInput::KeyDown(const string & strKey) const
{
	PKEYINFO pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bDown;
}

bool CInput::KeyUp(const string & strKey) const
{
	PKEYINFO pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bUp;
}

PKEYINFO CInput::FindKey(const string & strKey) const
{
	unordered_map<string, PKEYINFO>::const_iterator iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}
