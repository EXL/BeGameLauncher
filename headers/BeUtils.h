#ifndef BEUTILS_H
#define BEUTILS_H

#include <FindDirectory.h>
#include <String.h>
#include <GraphicsDefs.h>

#include <cstdio>

#define BeDebug(...) fprintf(stderr, __VA_ARGS__)

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

	static bool OpenLinkViaWebBrowser(const BString &url);
};

extern const rgb_color K_RED;
extern const rgb_color K_GREEN;
extern const rgb_color K_BLUE;
extern const rgb_color K_BLACK;

#endif // BEUTILS_H
