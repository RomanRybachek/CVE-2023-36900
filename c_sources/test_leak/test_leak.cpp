// test_leak.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <winternl.h>

typedef NTSTATUS(CALLBACK* NTQUERYSYSTEMINFORMATION)(SYSTEM_INFORMATION_CLASS, \
	PVOID, \
	ULONG, \
	PULONG);

typedef struct SYSTEM_MODULE_INFORMATION {
	ULONG                ModulesCount;
	SYSTEM_MODULE        Modules[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

int main()
{
	HMODULE ntdll_handle =  LoadLibrary(L"ntdll.dll");

	NTSTATUS (*NtQuerySystemInformation)(SYSTEM_INFORMATION_CLASS, \
											PVOID, \
											ULONG, \
											PULONG);

	NtQuerySystemInformation = ( NTQUERYSYSTEMINFORMATION)GetProcAddress(ntdll_handle, "NtQuerySystemInformation");

	ULONG len = 0;
	NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)0x0B, NULL, 0, &len);
	PSYSTEM_MODULE_INFORMATION pModuleInfo = 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
