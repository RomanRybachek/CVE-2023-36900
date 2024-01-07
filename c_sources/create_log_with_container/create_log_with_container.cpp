#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wchar.h>
#include <iostream>
#include <clfsw32.h>
#include <Clfsmgmtw32.h>

#pragma comment(lib, "Clfsw32.lib")

#define ARRAY_LENGTH 5
#define RECORD_LENGTH 20

void ErrorExit(const char* id)
{
    DWORD errNum = GetLastError();
    std::cerr << "Encountered unexpected error from " << id << ": " << std::endl
        << errNum << std::endl;
    exit(errNum);
}

HANDLE g_logHndl;
void* g_marsh_context;

void create_log(int argc, wchar_t* argv[]) {

	if (argc < 2) {
		std::cout << "An absoulte path to log is needed";
		exit(1);
	}

	g_logHndl = 0;
	g_marsh_context = NULL;

	std::wstring path(L"LOG:");
	path += argv[1];

	g_logHndl = CreateLogFile(
		path.c_str(),
		GENERIC_READ | GENERIC_WRITE | DELETE,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_ALWAYS,
		NULL);

	if (g_logHndl == INVALID_HANDLE_VALUE) {
		ErrorExit("Creating/opening log file error.");
	}
	else
	{
		std::wcout << L"Log " << path << L" is created/opened." << std::endl;
	}

	ULONGLONG pcbContainer = 0x10000;

	std::wstring folder(L"");
	folder += argv[1];
	size_t slash_pos = folder.find(L"\\");
	folder = folder.substr(0, slash_pos);
	folder += L"\\C1";

	//WCHAR container_name[] = L"log_files\\C1";
	WCHAR *container_name = _wcsdup(folder.c_str());

	if (AddLogContainer(g_logHndl, &pcbContainer, container_name, NULL) == 0) {
		ErrorExit("Something wrong with container creating.");
	}
	else {
		std::wcout << L"Container " << container_name << L" is created. Size:" << pcbContainer << std::endl;
	}


	if (CreateLogMarshallingArea(	g_logHndl, 
									NULL, 
									NULL, 
									NULL, 
									0x1000, 
									1, 
									1, 
									&g_marsh_context
								) == 0) {
		ErrorExit("Marshalling area error (return value is NULL)");
	}
	else if (g_marsh_context == NULL)
	{
		ErrorExit("Marshalling area error (return pointer is NULL)");
	}
	else {
		std::wcout << L"Marshalling area is created." << std::endl;
	}
}

void clear_all() {
	CloseHandle(g_logHndl);
	DeleteLogMarshallingArea(g_marsh_context);
}

int wmain(int argc, wchar_t* argv[])
{
	create_log(argc, argv);

	CLFS_WRITE_ENTRY* w_entry_array = new CLFS_WRITE_ENTRY[ARRAY_LENGTH];
	for (SIZE_T i = 0; i < ARRAY_LENGTH; i++)
	{
		w_entry_array[i].Buffer = new char[RECORD_LENGTH];
		w_entry_array[i].ByteLength = RECORD_LENGTH;
		memset(w_entry_array[i].Buffer, 0, RECORD_LENGTH);
		sprintf_s((char*)w_entry_array[i].Buffer, RECORD_LENGTH, "record%lld", i);
	}


	if (ReserveAndAppendLog(g_marsh_context,
		w_entry_array,
		ARRAY_LENGTH,
		0, 0, 0, 0, CLFS_FLAG_NO_FLAGS, 0, 0) == 0)
	{
		ErrorExit("ReserveAndAppendLog error");
	}
	else {
		std::cout << "logs are added" << std::endl;
	}

	
	if (FlushLogBuffers(g_marsh_context, 0) == 0)
	{
		ErrorExit("FlushLogBuffers error");
	}
	else {
		std::cout << "logs are flushed!" << std::endl;
	}

	clear_all();
    return 0;
}
