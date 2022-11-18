#include "Core/Core.h"

#include "Core/DBManager.h"

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
		printf("Close Server\n");
	}
	return FALSE;
}

int main()
{
	BOOL fSuccess = SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

	if (!GET_SINGLE(CCore)->Init())
	{
		DESTROY_SINGLE(CCore);
		return 0;
	}

	int iRev = GET_SINGLE(CCore)->Run();

	DESTROY_SINGLE(CCore);
	printf("Close Server\n");

	return iRev;
}