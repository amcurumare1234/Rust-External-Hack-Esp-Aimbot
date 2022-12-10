// project.Timestamp 15.09.2020-25.01.2021 | Barracuda | Mixed launch x64 
//Debug injection support.1-load driver. 2-start debug
#include <iostream>
#include <Windows.h>

#include "GuiEngine.h"
#include "threads.h"
#include "driverRW.h"
#include "tools.h"





using namespace std;

/*
void HideModule(HINSTANCE hModule)
{
	DWORD dwPEB_LDR_DATA = 0;
	_asm
	{
		pushad;
		pushfd;
		mov eax, fs: [30h] ;
		mov eax, [eax + 0Ch];
		mov dwPEB_LDR_DATA, eax;
		mov esi, [eax + 0Ch];
		mov edx, [eax + 10h];
	LoopInLoadOrderModuleList:
		lodsd;
		mov esi, eax;
		mov ecx, [eax + 18h];
		cmp ecx, hModule;
		jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList
			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList
			InMemoryOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]
			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, hModule
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList
			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList
			InInitializationOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]
			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, hModule
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished
			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList
			Finished :
		popfd;
		popad;
	}
}
*/
void EraseHeader(HINSTANCE hModule)
{
	if (!hModule)
		return;

	DWORD size, protect;


	PIMAGE_DOS_HEADER pDoH = (PIMAGE_DOS_HEADER)(hModule);
	PIMAGE_NT_HEADERS pNtH = (PIMAGE_NT_HEADERS)((LONG)hModule + ((PIMAGE_DOS_HEADER)hModule)->e_lfanew);

	size = sizeof(IMAGE_DOS_HEADER);
	if (VirtualProtect(pDoH, size, PAGE_READWRITE, &protect))
		for (DWORD i = 0; i < size; i++)
			*(BYTE*)((BYTE*)pDoH + i) = 1;

	size = sizeof(IMAGE_NT_HEADERS);
	if (pNtH && VirtualProtect(pNtH, size, PAGE_READWRITE, &protect))
		for (DWORD i = 0; i < size; i++)
			*(BYTE*)((BYTE*)pNtH + i) = 1;

	return;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
        #ifndef _DEBUG 
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
        #endif
	
		EraseHeader(hModule);
		//HideModule(hModule);

		//AllocConsole();
		//freopen("CONOUT$", "w", stdout);
		//SetConsoleTitle("DEBUG CONSOLE");

		while (!get_process_pid("RustClient.exe"))Sleep(1000);
	
		if (!kernelHandler.attach("RustClient.exe"))
		{
			MessageBox(0, "ERROR attach", "ERROR", MB_OK | MB_ICONERROR);
			return 1;
		}


		
		
		/*
		DWORD64 baseProjectile = kernelHandler.get_module_base("GameAssembly.dll") + 0x588DD0;	
		char a = 0x99;
        kernelHandler.virtual_protect(baseProjectile, 1, PAGE_EXECUTE_READWRITE);
		kernelHandler.write_memory(kernelHandler.get_module_base("GameAssembly.dll") + 0x588DD0, (uint64_t)(LPBYTE)&a, sizeof(a));
	   */
	   

	
		
			//return FALSE;
		while (Vars::Config::BaseNetworkable == NULL || Vars::Config::GameObjectManager == NULL)
		{
			Vars::Config::GameObjectManager = read(kernelHandler.get_module_base("UnityPlayer.dll") + oGameObjectManager, DWORD64);
			Vars::Config::BaseNetworkable = read(kernelHandler.get_module_base("GameAssembly.dll") + oBaseNetworkable, DWORD64);
			Sleep(1);

			cout <<"GameObjectManager:"<< std::hex << Vars::Config::GameObjectManager << endl;
			cout << "BaseNetworkable:" << std::hex << Vars::Config::BaseNetworkable << endl;
		}
	
	//	std::thread sock(socket_checker);
		//sock.detach();

		std::thread menu_thr(menu_thread);
		menu_thr.detach();

	   std::thread esp_thr(BN_thread);
	   esp_thr.detach();

		std::thread GOM_thr(GOM_thread);
		GOM_thr.detach();

		std::thread ESP_thr(ESP_thread);
		ESP_thr.detach();

		std::thread aim_thr(aim_thread);
		aim_thr.detach();

		


	}



	return TRUE;
}

