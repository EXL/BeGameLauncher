#ifndef BEUTILS_H
#define BEUTILS_H

#include <GraphicsDefs.h>
#include <FindDirectory.h>
#include <String.h>

#define G_DEFAULT_FONT_SIZE            12.0f
#define G_SMALL_FONT_SIZE              10.0f
#define G_DEFAULT_GAP                  10.0f
#define G_DEFAULT_VERSION              "1.0.0"

#include <cstdio>

#define BeUnused(x)                    (void)(x)
#if __cplusplus >= 201103L
#define BeDebug(...)                   fprintf(stderr, __VA_ARGS__)
#else
extern void BeDebug(const char *format, ...);
#endif // __cplusplus == 201103L

class BeUtils
{
	static BString FindPathInner(directory_which dir);

	// Forbidden.
	explicit BeUtils(void);
	~BeUtils(void);

public:
	static const BString GetPathToHomeDir(void);
	static const BString GetPathToUserDataDir(void);
	static const BString GetPathToUserNonPackedDataDir(void);
	static const BString GetPathToSettingsFile(const char* settingsFileName);
	static const BString GetPathToPackage(const char *packageName);
	static const BString GetPathToExecutable(const char *packageName, const char *executableName);

	static bool OpenLinkViaWebBrowser(const BString &url);
};

extern const rgb_color K_RED;
extern const rgb_color K_GREEN;
extern const rgb_color K_BLUE;
extern const rgb_color K_BLACK;

#endif // BEUTILS_H
