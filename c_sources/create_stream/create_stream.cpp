#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <winbase.h>
#include <wchar.h>
#include <iostream>
#include <clfsw32.h>
#include <Clfsmgmtw32.h>
#include <string>

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
}

#define ATOMS_NUM 0x2AAAAAA
#define TRIES 10

void create_atoms() {

	LoadLibrary(L"User32");
	std::string atom_wstr("my_atom");
	ATOM **atoms = new ATOM*[TRIES];

	for (int i = 0; i < TRIES; i++) {
		atoms[i] = new ATOM[ATOMS_NUM];
	}

	std::cout << atom_wstr;
	for (int cur_try = 0; cur_try < TRIES; cur_try++) {

		for (int cur_atom = 0; cur_atom < ATOMS_NUM; cur_atom++) {
			std::string atom_wstr_num = atom_wstr + "_" +  std::to_string(cur_try) + "_" +std::to_string(cur_atom);

			atoms[cur_try][cur_atom] = GlobalAddAtomA(atom_wstr_num.c_str());
			std::cout << atom_wstr_num << " " << atoms[cur_try][cur_atom] << std::endl;

			if (GlobalFindAtomA(atom_wstr_num.c_str()) == 0) {
				std::cout << "CAN'T FOUND ATOM" << std::endl;
				exit(1);
			}
			else {
				std::cout << "Found!" << std::endl;
			}

		}
		if (cur_try != 0 && cur_try != TRIES - 1) {
			GlobalDeleteAtom(atoms[cur_try][0]);
			std::cout << "deleted " << atoms[cur_try][0] << std::endl;
		}
	}
}

int wmain(int argc, wchar_t* argv[])
{
	//create_atoms();
	create_log(argc, argv);

    return 0;
}
