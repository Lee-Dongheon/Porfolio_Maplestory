#pragma once

#define _WINSOCKAPI_

#include <Windows.h>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <crtdbg.h>
#include <typeinfo>
#include <string>
#include <functional>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

// 서버 헤더
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

// 패킷 빈공간 제거
#pragma pack(push, 1)

// dshow 설정
#include <dshow.h>
#pragma comment(lib, "strmiids.lib")

// d2d 설정
#include <d2d1.h>
#pragma comment(lib, "D2D1.lib")

// WIC 설정
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

#pragma warning(disable : 4996)

// 디버깅 콘솔
//#ifdef _DEBUG
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#endif

// 이용시 _cprintf() 이용

using namespace std;

// d2d namespace
using namespace D2D1;

#include "Macro.h"
#include "resource.h"
#include "Flag.h"
#include "Types.h"
#include "packet.h"

template<typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; iter++)
	{
		SAFE_DELETE((*iter));
	}

	p.clear();
}

template<typename T>
void Safe_Release_VecList(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; iter++)
	{
		SAFE_RELEASE((*iter));
	}

	p.clear();
}

template<typename T>
void Safe_Delete_Map(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}

	p.clear();
}

template<typename T>
void Safe_Release_Map(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; iter++)
	{
		SAFE_RELEASE(iter->second);
	}

	p.clear();
}