#pragma once

#define QTCORE "Qt5Core.dll"
#define QTWIDGETS "Qt5Widgets.dll"

void* Import(const char* Module, const char* Name);

template <class T>
inline T* Import(const char* Module, const char* Name) {
	return (T*)Import(Module, Name);
}

enum QDirFilters
{
	QDir_Dirs = 0x001,	// List directories that match the filters.
	QDir_AllDirs= 	0x400,	// List all directories; i.e.don't apply the filters to directory names.
	QDir_Files= 	0x002,	// List files.
	QDir_Drives= 	0x004,	// List disk drives(ignored under Unix).
	QDir_NoSymLinks= 	0x008, // Do not list symbolic links(ignored by operating systems that don't support symbolic links).
	QDir_NoDot= 	0x2000,	// Do not list the special entry ".".
	QDir_NoDotDot	= 0x4000,	// Do not list the special entry "..".
	QDir_NoDotAndDotDot = QDir_NoDot | QDir_NoDotDot,	//Do not list the special entries "." and "..".
	QDir_AllEntries= QDir_Dirs | QDir_Files | QDir_Drives, //	List directories, files, drives and symlinks(this does not list broken symlinks unless you specify System).
	QDir_Readable= 	0x010,	// List files for which the application has read access.The Readable value needs to be combined with Dirs or Files.
	QDir_Writable	= 0x020,	// List files for which the application has write access.The Writable value needs to be combined with Dirs or Files.
	QDir_Executable= 	0x040,	// List files for which the application has execute access.The Executable value needs to be combined with Dirs or Files.
	QDir_Modified= 	0x080,	// Only list files that have been modified(ignored on Unix).
	QDir_Hidden	= 0x100	,// List hidden files(on Unix, files starting with a ".").
	QDir_System= 	0x200,	// List system files(on Unix, FIFOs, sockets and device files are included; on Windows, .lnk files are included)
	QDir_CaseSensitive= 	0x800,	// The filter should be case sensitive.
};

class QGhetto
{
protected:
	void* m_ptr;
public:
	inline void* Ptr() const { return m_ptr; }
};

class QString : public QGhetto
{
public:
	QString(const char* Text);
	~QString();

	const wchar_t* constData() const;
};

class QDirIterator : public QGhetto
{
public:
	QDirIterator(const QString& Path, int Filters, int IteratorFlag);
	~QDirIterator();
	
	bool hasNext();
	QString next();
};