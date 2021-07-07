#include "../client/utilities/csgo.hpp"
#include "features/features.hpp"
#include "menu\config\config.h"

DWORD WINAPI OnDLLAttach(LPVOID lpParameter) 
{
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

	console::initialize("obelus");
	console::log("obelus-csgo\n");

	try
	{
		//security::auth();
		client::initialize();
	}
	catch (const std::runtime_error& error) 
	{
		MessageBoxA(nullptr, error.what(), "!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(g::hDLL, 0);
	}

	while (!GetAsyncKeyState(VK_END))
		Sleep(1000);

	client::uninitialize();
	FreeLibraryAndExitThread(g::hDLL, 0);
}

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD dwReason, const LPVOID lpReserved) {
	DisableThreadLibraryCalls(hModule);

	switch (dwReason) {
		case DLL_PROCESS_ATTACH: {
			const auto hThread = CreateThread(nullptr, 0U, OnDLLAttach, hModule, 0UL, nullptr);
			if (!hThread)
				return FALSE;

			g::hDLL = hModule;

			CloseHandle(hThread);

			return TRUE;
		}
		break;
		default:
			break;
	}

	return FALSE;
}
