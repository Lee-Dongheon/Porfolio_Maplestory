#include "Core/Core.h"

BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_BREAK_EVENT:
	default:
		DESTROY_SINGLE(CCore);
	}
	return FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (!GET_SINGLE(CCore)->Init(hInstance))
	{
		DESTROY_SINGLE(CCore);
		return 0;
	}

	int iRev = GET_SINGLE(CCore)->Run();

	DESTROY_SINGLE(CCore);

	return iRev;
}