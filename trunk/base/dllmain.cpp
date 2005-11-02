#if (defined(_MSC_VER) && defined(_DEBUG))
#define WIN32_LEAN_AND_MEAN
#	include <crtdbg.h>
#endif

#include <windows.h>

BOOL WINAPI DllMain(
					HINSTANCE hinstDLL,
					DWORD fdwReason,
					LPVOID lpvReserved
					)
{
#if defined(WIN32) && defined(_DEBUG)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF);
#endif

	return TRUE;
}
