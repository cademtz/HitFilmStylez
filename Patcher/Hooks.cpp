#define _CRT_SECURE_NO_WARNINGS
#include "Hooks.h"
#include "GhettoQt.h"
#include <cstdio>
#include <cstdlib>

#include "resource.h"

int Hooked_SetStyleSheet(void* This, const QString& Style)
{
	static char dir[MAX_PATH] = { 0 }, szdefault[MAX_PATH], szcur[MAX_PATH];
	if (!dir[0])
	{
		sprintf_s(dir, "%s%s", getenv("appdata"), "\\HitFilm Stylez\\");
		sprintf_s(szdefault, "%s%s", dir, "Default (Delete to reset).css");
		sprintf_s(szcur, "%s%s", dir, "Current Style.css");
	}

	bool isNew = CreateDirectoryA(dir, 0);

	if (HANDLE stile = CreateFileA(szdefault, GENERIC_ALL, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0))
	{
		size_t len = wcslen(Style.constData());
		char* buf = new char[len];
		WideCharToMultiByte(CP_UTF8, WC_DEFAULTCHAR, Style.constData(), len, buf, len, NULL, NULL);

		WriteFile(stile, buf, len, 0, 0);
		CloseHandle(stile);
		delete[] buf;
	}

	char* utf8 = 0;

	if (HANDLE stile = CreateFileA(szcur, GENERIC_ALL, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0))
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			if (HRSRC hResInfo = FindResource(Base::hInst, MAKEINTRESOURCE(IDR_DARKSTYLE1), "DARKSTYLE")) {
				if (HGLOBAL hRes = LoadResource(Base::hInst, hResInfo))
				{
					DWORD size = SizeofResource(Base::hInst, hResInfo);
					void* raw = LockResource(hRes);
					WriteFile(stile, raw, size, 0, 0);

					utf8 = new char[size + 1];
					utf8[size] = 0;
					memcpy(utf8, raw, size);
				}
			}
		}
		else
		{
			DWORD size = -1;
			size |= GetFileSize(stile, &size);
			utf8 = new char[(size_t)size + 1];
			utf8[size] = 0;
			ReadFile(stile, utf8, size, 0, 0);
		}
		CloseHandle(stile);
	}

	if (isNew)
	{
		char msg[MAX_PATH + 128];
		sprintf_s(msg, "First time using HitFilm Stylez?\nI put some styles in \"%s\"\nFeel free to edit them!", dir);
		MessageBoxA(0, msg, "HitFilm Stylez", MB_ICONINFORMATION);
	}

	if (!utf8)
	{
		MessageBoxA(0, "Failed to grab custom stylesheet!", "HitFilm Stylez", MB_ICONERROR);
		return SetStyleSheet(This, Style);
	}

	QString newstyle(utf8);
	delete[] utf8;
	return SetStyleSheet(This, newstyle);
}


void* HookImport(HINSTANCE h, const char* Module, const char* Name, void* Hook)
{
	void* target = GetProcAddress(GetModuleHandleA(Module), Name);

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)h;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((UINT_PTR)h + pDosHeader->e_lfanew);
	PIMAGE_IMPORT_DESCRIPTOR imps = (PIMAGE_IMPORT_DESCRIPTOR)((UINT_PTR)h
		+ pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	for (; imps->Name; imps++)
	{
		PSTR szMod = (PSTR)((UINT_PTR)h + imps->Name);
		if (!szMod || strcmp(szMod, Module))
			continue;

		PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)((UINT_PTR)h + imps->FirstThunk);
		for (; thunk->u1.Function; thunk++)
		{
#ifdef _WIN64
			if (!(thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG64))
#else
			if (!(thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG32))
#endif
			{
				void** ppfn = (void**)&thunk->u1.Function;
				if (*ppfn == target)
				{
					void* result = *ppfn;

					DWORD flOld;
					VirtualProtect(&thunk->u1.Function, sizeof(ppfn), PAGE_EXECUTE_READWRITE, &flOld);
					*(void**)&thunk->u1.Function = Hook;
					VirtualProtect(&thunk->u1.Function, sizeof(ppfn), flOld, &flOld);

					return result;
				}
			}
		}
	}
	return nullptr;
}