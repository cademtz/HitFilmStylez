#include <Windows.h>
#include "resource.h"
/*#include <string>

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
}*/

#define HF_DEFAULT "C:\\Program Files\\FXHOME\\HitFilm Express\\"
#define HF_FILE "HitFilmExpress.exe"
#define HF_RENAME "HitFilmExpress_Original.exe"
#define HF_CAPTION "HitFilm Stylez"

int ExtractRes(HINSTANCE hInst, ULONG_PTR IntRes, const char* Type, const char* Path)
{
	HANDLE hFile = 0;
	HRSRC hResInfo = FindResourceA(hInst, MAKEINTRESOURCEA(IntRes), Type);
	HGLOBAL hResData = LoadResource(hInst, hResInfo);
	void* res = LockResource(hResData);

	if (!res ||
		!(hFile = CreateFileA(Path, GENERIC_ALL, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)) ||
		!(WriteFile(hFile, res, SizeofResource(hInst, hResInfo), 0, 0)))
	{
		MessageBoxA(0, "Failed to extract a file to HitFilm directory", HF_CAPTION, MB_ICONERROR);
		if (hFile)
			CloseHandle(hFile);
		return 0;
	}

	CloseHandle(hFile);
	return 1;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	if (MessageBox(0, "Installing HitFilm Stylez.\nClick Ok to select your HitFilm EXE.",
		"HitFilm Stylez", MB_OKCANCEL | MB_ICONINFORMATION));// != IDOK)
		//return 0;

	char szfile[MAX_PATH] = { 0 }, dir[MAX_PATH] = { 0 };

	OPENFILENAME file;
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hInstance = hInstance;
	file.lpstrFile = szfile;
	file.nMaxFile = sizeof(szfile);

	if (GetFileAttributesA(HF_DEFAULT) & FILE_ATTRIBUTE_DIRECTORY)
	{
		file.lpstrInitialDir = HF_DEFAULT;
		strcpy_s(szfile, sizeof(szfile), HF_FILE);
	}

	if (!GetOpenFileNameA(&file))
	{
		MessageBoxA(0, "Cancelled installation.", HF_CAPTION, MB_OK);
		return 0;
	}
	
	strncpy_s(dir, sizeof(dir), szfile, (size_t)(strrchr(szfile, '\\') - szfile) + 1);

	char newfile[MAX_PATH] = { 0 };
	strcpy_s(newfile, sizeof(newfile), dir);
	strcat_s(newfile, sizeof(newfile), HF_RENAME);

	if (GetFileAttributesA(newfile) != INVALID_FILE_ATTRIBUTES)
	{
		MessageBoxA(0, "An existing installation of HitFilm Stylez was detected.\nCancelling...", HF_CAPTION, MB_ICONWARNING);
		return 0;
	}

	if (!MoveFileA(szfile, newfile))
	{
		MessageBoxA(0, "Failed to rename file", HF_CAPTION, MB_ICONERROR);
		return 0;
	}

	strcpy_s(newfile, sizeof(newfile), dir);
	strcat_s(newfile, sizeof(newfile), HF_FILE);

	if (!ExtractRes(hInstance, IDR_INJECTOR, "exe", newfile))
		return 0;

	strcpy_s(newfile, sizeof(newfile), dir);
	strcat_s(newfile, sizeof(newfile), "Patcher.dll");

	if (!ExtractRes(hInstance, IDR_PATCHER, "dll", newfile))
		return 0;

	strcpy_s(newfile, sizeof(newfile), dir);
	strcat_s(newfile, sizeof(newfile), "Uninstall HitFilm Stylez.exe");

	if (!ExtractRes(hInstance, IDR_UNINSTALLER, "exe", newfile))
		return 0;

	MessageBoxA(0, "Successfully installed HitFilm Stylez!", HF_CAPTION, MB_OK);
	return 0;
}