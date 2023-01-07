#include <windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>
#include <cstdio>
#include <processthreadsapi.h>
#include <iostream>
#include <fstream>
#include <Lmcons.h>
#include "stdio.h"

using namespace std;

void get_proc_id(const char* window_title, DWORD& process_id)
{
	GetWindowThreadProcessId(FindWindowA(NULL, window_title), &process_id);
}


bool file_exists(string file_name)
{
	struct stat buffer;
	return (stat(file_name.c_str(), &buffer) == 0);
}

void White(const char* white_message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_INTENSITY);
	MessageBoxA(NULL, white_message, "", NULL);
}
void Purple(const char* purple_message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED + FOREGROUND_BLUE);
	MessageBoxA(NULL, purple_message, "", NULL);
}
void Error(const char* error_title, const char* error_message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED + FOREGROUND_BLUE);
	MessageBoxA(NULL, error_message, error_title, NULL);
}

void rndmTitle() {
	constexpr int length = 15;
	const auto characters = TEXT("abcdefghi9182345jklmnopqrstuv211935960473wxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	TCHAR title[length + 1]{};

	for (int j = 0; j != length; j++)
	{
		title[j] += characters[rand() % 80];
	}

	SetConsoleTitle(title);
}

int main()
{
	rndmTitle();
	char buffer[UNLEN + 1]; 
	DWORD size;
	size = sizeof(buffer); 
	GetUserNameA(buffer, &size);
	string name = buffer;

	setlocale(LC_ALL, "eng");

	DWORD proc_id = NULL;
	char dll_patch[MAX_PATH];

	const char* window_title = "ARK: Survival Evolved";


		string dll_name = "C:\\Users\\Default\\AppData\\Local\\Temp\\xingark_3.dll";
		if (!file_exists(dll_name.c_str()))
		{
			Error("XingLoader", "Error 101");
			return 0;
		}

		if (!GetFullPathNameA(dll_name.c_str(), MAX_PATH, dll_patch, nullptr))
		{
			get_proc_id(window_title, proc_id);
			Sleep(1000);
			Error("XingLoader", "Error 102");
			return 0;
		}

		get_proc_id(window_title, proc_id);
		if (proc_id == NULL)
		{
			Error("XingLoader", "Error 103");
			return 0;
		}

		HANDLE h_proces = OpenProcess(PROCESS_ALL_ACCESS, NULL, proc_id);
		if (!h_proces)
		{
			Error("XingLoader", "Error 104");
			return 0;
		}

		void* allocated_memory = VirtualAllocEx(h_proces, nullptr, strlen(dll_name.c_str()), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		if (!allocated_memory)
		{
			Error("XingLoader", "Error 105");
			return 0;
		}

		if (!WriteProcessMemory(h_proces, allocated_memory, dll_name.c_str(), strlen(dll_name.c_str()), nullptr))
		{
			Error("XingLoader", "Error 106");
			return 0;
		}
		auto loadLib = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		HANDLE h_thread = CreateRemoteThread(h_proces, nullptr, NULL, (LPTHREAD_START_ROUTINE)loadLib, allocated_memory, NULL, nullptr);

		if (!h_thread)
		{
			Error("XingLoader", "Error 107");
			return 0;
		}

		CloseHandle(h_proces);
		VirtualFreeEx(h_proces, allocated_memory, NULL, MEM_RELEASE);
		Purple("Thanks for using Xing!");
	}