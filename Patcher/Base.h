#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Base
{
	void OnAttach(HMODULE Module);
	inline HMODULE hInst;
};