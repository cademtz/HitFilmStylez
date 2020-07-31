#include "Base.h"

BOOL WINAPI DllMain(HINSTANCE Inst, DWORD Reason, LPVOID)
{
	if (Reason == DLL_PROCESS_ATTACH)
		Base::OnAttach(Inst);
	return true;
}