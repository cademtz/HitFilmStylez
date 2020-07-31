#pragma once
#include "Base.h"

class QString;

int Hooked_SetStyleSheet(void* thisptr, const QString& Style);
inline decltype(Hooked_SetStyleSheet)* SetStyleSheet;

void* HookImport(HINSTANCE h, const char* Module, const char* Name, void* Hook);
