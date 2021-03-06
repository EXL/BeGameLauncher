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

#include "BeUtils.h"

#include <Roster.h>
#include <Entry.h>
#include <Errors.h>

#include <posix/limits.h>

const rgb_color K_RED                  = { 200,   0,   0, 255 };
const rgb_color K_GREEN                = {   0, 100,   0, 255 };
const rgb_color K_BLUE                 = {   0,   0, 200, 255 };
const rgb_color K_BLACK                = {   0,   0,   0, 255 };

#define OPEN_BINARY_PATH               "/bin/open"

#if __cplusplus < 201103L
#include <cstdarg>

void
BeDebug(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}
#endif

BeUtils::BeUtils(void)
{

}

BString
BeUtils::FindPathInner(directory_which dir)
{
	char path[PATH_MAX];
	if (find_directory(dir, -1, false, path, PATH_MAX) != B_OK)
	{
		return BString("/");
	}
	return BString(path, PATH_MAX);
}

const BString
BeUtils::GetPathToHomeDir(void)
{
	return FindPathInner(B_USER_DIRECTORY);
}

const BString
BeUtils::GetPathToUserDataDir(void)
{
	return FindPathInner(B_USER_DATA_DIRECTORY);
}

const BString
BeUtils::GetPathToUserNonPackedDataDir(void)
{
	return FindPathInner(B_USER_NONPACKAGED_DATA_DIRECTORY);
}

const BString
BeUtils::GetPathToSettingsFile(const char *settingsFileName)
{
	return FindPathInner(B_USER_SETTINGS_DIRECTORY) << BString("/") << BString(settingsFileName);
}

const BString
BeUtils::GetPathToPackage(const char *packageName)
{
	return FindPathInner(B_SYSTEM_APPS_DIRECTORY) << BString("/") <<  BString(packageName);
}

const BString
BeUtils::GetPathToExecutable(const char *packageName, const char *executableName)
{
	return FindPathInner(B_SYSTEM_APPS_DIRECTORY) << BString("/") << BString(packageName)
	                                              << BString("/") << BString(executableName);
}

bool
BeUtils::OpenLinkViaWebBrowser(const BString &url)
{
	entry_ref ref;
	if(get_ref_for_path(OPEN_BINARY_PATH, &ref) != B_OK)
	{
		return false;
	}

	BString normalizedUrl;
	if(url.FindFirst("://") == B_ERROR)
	{
		normalizedUrl << "http://";
	}
	normalizedUrl << url;
	const char *argv[] = { OPEN_BINARY_PATH, normalizedUrl.String(), NULL };
	be_roster->Launch(&ref, 2, argv); // 2 = argc

	return true;
}

BeUtils::~BeUtils(void)
{

}
