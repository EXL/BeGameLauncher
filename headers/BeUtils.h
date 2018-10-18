#ifndef BEUTILS_H
#define BEUTILS_H

#include <GraphicsDefs.h>
#include <FindDirectory.h>
#include <String.h>

#include <cstdio>

#define BeDebug(...)                    fprintf(stderr, __VA_ARGS__)

#define G_DEFAULT_FONT_SIZE             12.0f
#define G_SMALL_FONT_SIZE               10.0f
#define G_DEFAULT_VERSION               "1.0.0"

class BeUtils
{
	static BString FindPathInner(directory_which dir);
public:
	BeUtils();
	~BeUtils();

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
