#ifndef BEUTILS_H
#define BEUTILS_H

#include <FindDirectory.h>
#include <String.h>

class BeUtils
{
	static BString FindPathInner(directory_which dir);
public:
	BeUtils();
	static BString GetPathToHomeDir(void);
	static BString GetPathToUserDataDir(void);
	static BString GetPathToUserNonPackedDataDir(void);
	static BString GetPathToSettingsFile(const char* settingsFileName);
	static BString GetPathToPackage(const char *packageName);
	static BString GetPathToExecutable(const char *packageName, const char *executableName);
};

#endif // BEUTILS_H
