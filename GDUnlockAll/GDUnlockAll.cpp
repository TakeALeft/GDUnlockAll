#include <Windows.h>
#include <stdio.h>
#include <psapi.h>
#include <TlHelp32.h>

uintptr_t GetModule(DWORD ProcessID) {
	uintptr_t BaseAddress = 0;
	HANDLE Module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
	MODULEENTRY32 ModuleEntry;
	ModuleEntry.dwSize = sizeof(ModuleEntry);

	if (Module32First(Module, &ModuleEntry)) {
		do {
			if (!strcmp(ModuleEntry.szModule, "GeometryDash.exe")) {
				BaseAddress = (uintptr_t)ModuleEntry.modBaseAddr;
				break;
			}
		} while (Module32Next(Module, &ModuleEntry));
	}

	CloseHandle(Module);
	return BaseAddress;
}

int main() {
	char* ModuleAddress;
	HWND Window;
	DWORD ProcessID;
	DWORD Error;
	HANDLE ProcessHandle;

	SetConsoleTitleA("All Icons Unlocker");
	system("color b");

	do {
		Window = FindWindowA(0, "Geometry Dash");
		Sleep(500);
	} while (!Window);

	GetWindowThreadProcessId(Window, &ProcessID);
	ProcessHandle = OpenProcess(0x38, 0, ProcessID);

	if (!ProcessHandle) {
		Error = GetLastError();
		printf("Error opening process! Code: %d", Error);
	}

	char Patch[5] = { 0xB0, 0x1, 0x90, 0x90, 0x90 };

	ModuleAddress = (char*)GetModule(ProcessID);
	WriteProcessMemory(ProcessHandle, ModuleAddress + 0xC50A8, &Patch, 5, 0);
	WriteProcessMemory(ProcessHandle, ModuleAddress + 0xC54BA, &Patch, 5, 0);

	if (GetLastError()) {
		Error = GetLastError();
		printf("Error opening process! Code: %d", Error);
	}

	printf("Icons succesfully unlocked!");
	
	Sleep(1000);
	return 1;
}