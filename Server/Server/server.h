#pragma once
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <crtdbg.h>
#include <string>
#include <functional>
#include <conio.h>
#include <iostream>
#include <time.h>

#pragma warning(disable : 4996)

using namespace std;

// 서버 관련
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")
// AcceptEx
#include <MSWSock.h>
#pragma comment(lib, "mswsock.lib")

// 패킷 빈공간 제거
#pragma pack(push, 1)

// 데이터베이스 관련
#include <mysql.h>
#include <my_global.h>

#pragma comment(lib, "libmySQL.lib")

// 사용자 헤더
#include "macro.h"
#include "flag.h"
#include "types.h"
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