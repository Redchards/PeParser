// dllmain.cpp : Définit le point d'entrée pour l'application DLL.
#include <stdafx.h>

#include <Log.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			LOG(LogLevel::info, "Attaching process\n");
			break;
		case DLL_THREAD_ATTACH:
			LOG(LogLevel::info, "Attaching thread\n");
			break;
		case DLL_THREAD_DETACH:
			LOG(LogLevel::info, "Detaching thread\n");
			break;
		case DLL_PROCESS_DETACH:
			LOG(LogLevel::info, "Detaching process\n");
			break;
	}
	return TRUE;
}

