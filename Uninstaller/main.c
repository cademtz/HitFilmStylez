//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <strsafe.h>

#define HF_FILE "HitFilmExpress.exe"
#define HF_RENAME "HitFilmExpress_Original.exe"
#define HF_CAPTION "HitFilm Stylez"

#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

void DelMe()
{
	TCHAR szModuleName[MAX_PATH];
	TCHAR szCmd[2 * MAX_PATH];
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	GetModuleFileName(NULL, szModuleName, MAX_PATH);

	StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

	CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	if (MessageBox(0, "Are you sure you want to uninstall HitFilm Stylez?", HF_CAPTION, MB_YESNO | MB_ICONQUESTION) == IDNO)
		return 0;

	DWORD attribs = GetFileAttributesA(HF_RENAME);
	if (attribs == INVALID_FILE_ATTRIBUTES)
	{
		MessageBoxA(0, "No installation detected.\n"
			"Make sure the uninstaller is in your HitFilm directory (Next to the EXE).", HF_CAPTION, MB_ICONERROR);
		return 0;
	}
	
	DeleteFileA(HF_FILE);
	DeleteFileA("Patcher.dll");
	MoveFileA(HF_RENAME, HF_FILE);

	MessageBoxA(0, "Successfully uninstalled HitFilm Stylez!", HF_CAPTION, MB_OK);
	DelMe();

	return 0;
}