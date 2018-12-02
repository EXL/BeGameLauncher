/************************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2018 EXL
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy
 ** of this software and associated documentation files (the "Software"), to deal
 ** in the Software without restriction, including without limitation the rights
 ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ** copies of the Software, and to permit persons to whom the Software is
 ** furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 ** SOFTWARE.
 ************************************************************************************/

#ifndef BEUTILS_H
#define BEUTILS_H

#include <GraphicsDefs.h>
#include <FindDirectory.h>
#include <String.h>

#include <cstdio>
#include <cmath>
#include <limits>

#define G_DEFAULT_FONT_SIZE            12.0f
#define G_SMALL_FONT_SIZE              10.0f
#define G_DEFAULT_GAP                  10.0f
#define G_DEFAULT_VERSION              "1.0.0"

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

	template<typename T>
	static bool AlmostEqual(T first, T second)
	{
		return std::fabs(first - second) < std::numeric_limits<T>::epsilon();
	}

	static bool OpenLinkViaWebBrowser(const BString &url);
};

extern const rgb_color K_RED;
extern const rgb_color K_GREEN;
extern const rgb_color K_BLUE;
extern const rgb_color K_BLACK;

#endif // BEUTILS_H
