#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	STARTUPINFO startup;
	ZeroMemory(&startup, sizeof(startup));
	startup.cb = sizeof(startup);

	PROCESS_INFORMATION info;
	ZeroMemory(&info, sizeof(info));

	bool success = false;
	const char* lib = "Patcher.dll";

	if (CreateProcessA("HitFilmExpress_Original.exe", 0, 0, 0, 0, CREATE_SUSPENDED | CREATE_NEW_PROCESS_GROUP, 0, 0, &startup, &info))
	{
		if (LPVOID Data = VirtualAllocEx(info.hProcess, 0, MAX_PATH, MEM_COMMIT, PAGE_READWRITE)) {
			if (WriteProcessMemory(info.hProcess, Data, lib, strlen(lib) + 1, 0)) {
				if (HANDLE thred = CreateRemoteThread(info.hProcess, 0, 0, (LPTHREAD_START_ROUTINE)&LoadLibraryA, Data, 0, 0))
				{
					WaitForSingleObject(thred, INFINITE);
					success = true;
				}
			}
		}

		ResumeThread(info.hThread);

		CloseHandle(info.hProcess);
		CloseHandle(info.hThread);
	}

	if (!success)
	{
		std::string msg = "Failed to launch HitFilm Stylez correctly.\nWindows error: " + GetLastErrorAsString();
		MessageBoxA(0, msg.c_str(), "HitFilm Stylez", MB_ICONERROR);
	}

	return 0;
}