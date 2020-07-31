#include "GhettoQt.h"
#include "Base.h"

void* Import(const char* Module, const char* Name) {
	return (void*)GetProcAddress(GetModuleHandleA(Module), Name);
}

QString::QString(const char* Text)
{
	static auto fromutf8 = Import<void*(void**, const char*, DWORD)>(QTCORE, "?fromUtf8@QString@@SA?AV1@PEBDH@Z");
	fromutf8(&m_ptr, Text, 0xFFFFFFFF);
}

QString::~QString()
{
	static auto dtor = Import<void(void**)>(QTCORE, "??1QString@@QEAA@XZ");
	dtor(&m_ptr);
}

const wchar_t* QString::constData() const {
	return (const wchar_t*)(*(UINT_PTR*)((UINT_PTR)m_ptr + 0x10) + (UINT_PTR)m_ptr);
}

QDirIterator::QDirIterator(const QString& Path, int Filters, int IteratorFlag)
{
	static auto ctor = Import<void(void**, void*, int, int)>(QTCORE, "??0QDirIterator@@QEAA@AEBVQString@@V?$QFlags@W4Filter@QDir@@@@V?$QFlags@W4IteratorFlag@QDirIterator@@@@@Z");
	ctor(&m_ptr, (void*)&Path, Filters, IteratorFlag);
}

QDirIterator::~QDirIterator()
{
	static auto dtor = Import<void(void**)>(QTCORE, "??1QDirIterator@@QEAA@XZ");
	dtor(&m_ptr);
}

bool QDirIterator::hasNext()
{
	static auto _hasnext = Import<bool(void**)>(QTCORE, "?hasNext@QDirIterator@@QEBA_NXZ");
	return _hasnext(&m_ptr);
}

QString QDirIterator::next()
{
	static auto _next = Import<void*(void**, void**)>(QTCORE, "?next@QDirIterator@@QEAA?AVQString@@XZ");
	void* lol;
	_next(&m_ptr, &lol);
	return *(QString*)&lol;
}
