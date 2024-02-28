
#include "GameManager.h"/*
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)*/


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(146423);
	//_CrtDumpMemoryLeaks();
	//new char;

#ifndef _WIN64
	srand((unsigned int)GetTickCount());
#else
	srand((unsigned int)GetTickCount64());
#endif

	rand();

	if (!CGameManager::GetInst()->Init(hInstance))
	{
		CGameManager::DestroyInst();
		return 0;
	}

	int RetValue = CGameManager::GetInst()->Run();

	CGameManager::DestroyInst();

	return RetValue;
}
