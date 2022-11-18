#pragma once

#define SAFE_DELETE(p) if(p) { delete p; p = NULL; }
#define SAFE_DELETE_ARR(p) if(p) { delete[] p; p = NULL; }
#define SAFE_RELEASE(p) if(p) {p->Release(); p = NULL; }

#define DECLARE_SINGLE(Type) \
	private:\
		static Type* m_pInst;\
	public:\
		static Type* GetInst()\
		{\
			if (!m_pInst)\
				m_pInst = new Type;\
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}\
	private:\
		Type();\
		~Type();

#define DEF_SINGLE(Type) Type* Type::m_pInst = NULL
#define GET_SINGLE(Type) Type::GetInst()
#define DESTROY_SINGLE(Type) Type::DestroyInst()

#define GETRESOLUTION CCore::GetInst()->GetResolution()

// Math const
#define PI 3.141592f
#define GRAVITY 450.f
#define FRICTION 10.f
#define FORCE_MIN -450.f

#define PACKET_LENGTH 1024
#define MAX_CHAR_LEN 30
#define MAX_MESSAGE_LEN 128

#define MAX_SCENE_NUM 3

#define gotoxy(x, y) {COORD pos = {x, y};\
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);}