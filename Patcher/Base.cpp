#define _CRT_SECURE_NO_WARNINGS

#include "Base.h"
#include "Hooks.h"
#include "GhettoQt.h"
#include <stdio.h>

void Base::OnAttach(HMODULE Module)
{
	hInst = Module;
	SetStyleSheet = (decltype(SetStyleSheet))HookImport(GetModuleHandle(0),
		QTWIDGETS, "?setStyleSheet@QApplication@@QEAAXAEBVQString@@@Z", &Hooked_SetStyleSheet);
}