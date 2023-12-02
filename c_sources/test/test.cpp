#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wchar.h>
#include <iostream>
#include <clfsw32.h>
#include <Clfsmgmtw32.h>
#include <libloaderapi.h>
#include <winternl.h>

#pragma comment(lib, "Clfsw32.lib")

#define ARRAY_LENGTH 5
#define RECORD_LENGTH 20

typedef NTSTATUS (CALLBACK* NTCREATEFILE)(	PHANDLE, \
											ACCESS_MASK, \
											POBJECT_ATTRIBUTES, \
											PIO_STATUS_BLOCK, \
											PLARGE_INTEGER, \
											ULONG, ULONG, ULONG, ULONG, PVOID, ULONG);


void ErrorExit(const char* id)
{
    DWORD errNum = GetLastError();
    std::cerr << "Encountered unexpected error from " << id << ": " << std::endl
        << errNum << std::endl;
    exit(errNum);
}


void via_clfs_api(wchar_t* argv[]) {

	HANDLE logHndl;

	std::wstring path(L"LOG:");
	path += argv[1];
	logHndl = CreateLogFile(
		path.c_str(),
		GENERIC_READ | GENERIC_WRITE | DELETE,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		NULL);
	if (logHndl == INVALID_HANDLE_VALUE) {
		ErrorExit("Creating/opening log file error.");
	}
	else
	{
		std::wcout << L"Log " << path << L" is created/opened." << std::endl;
	}
}

void via_native_api(wchar_t* argv[]) {
	HMODULE ntdll_handle =  LoadLibrary(L"ntdll.dll");

	NTSTATUS (*NtCreateFile)(
			 PHANDLE            FileHandle, \
			 ACCESS_MASK        DesiredAccess, \
			 POBJECT_ATTRIBUTES ObjectAttributes, \
			 PIO_STATUS_BLOCK   IoStatusBlock, \
			 PLARGE_INTEGER     AllocationSize, \
			 ULONG              FileAttributes, \
			 ULONG              ShareAccess, \
			 ULONG              CreateDisposition, \
			 ULONG              CreateOptions, \
			 PVOID              EaBuffer, \
			 ULONG              EaLength \
			);
	NtCreateFile = (NTCREATEFILE)GetProcAddress(ntdll_handle, "NtCreateFile");

	HANDLE d = CreateFile(L"\\\\.\\c:", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0  );
	BY_HANDLE_FILE_INFORMATION i;
	HANDLE f = CreateFile(L"c:\\bla.bla", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ULONG bla;
	WriteFile(f, "helloworld", 11, &bla, NULL);
	CloseHandle(f);
	f = 0;
	const wchar_t* filename = L"c:\\bla.bla";

	UNICODE_STRING fidstr;
	fidstr.Buffer = _wcsdup(filename);
	fidstr.Length = wcslen(filename) * 2;
	fidstr.MaximumLength = fidstr.Length + 2;

	OBJECT_ATTRIBUTES oa = {0};
    InitializeObjectAttributes (&oa, &fidstr, OBJ_CASE_INSENSITIVE, d, NULL);

	IO_STATUS_BLOCK status_block;
    ULONG status = NtCreateFile(&f, GENERIC_ALL, &oa, &status_block, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_NON_DIRECTORY_FILE, NULL, 0);
	printf("status: %X, handle: %x\n", status, f);
	CloseHandle(f);
	CloseHandle(d);
}

int wmain(int argc, wchar_t* argv[])
{
	via_native_api(argv);
}
