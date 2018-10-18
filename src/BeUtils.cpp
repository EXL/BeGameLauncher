#include "BeUtils.h"

#include <Roster.h>
#include <Entry.h>

#include <posix/limits.h>

const rgb_color K_RED         = { 200,   0,   0, 255 };
const rgb_color K_GREEN       = {   0, 100,   0, 255 };
const rgb_color K_BLUE        = {   0,   0, 200, 255 };
const rgb_color K_BLACK       = {   0,   0,   0, 255 };

#define OPEN_BINARY_PATH        "/bin/open"

BeUtils::BeUtils()
{

}

BString
BeUtils::FindPathInner(directory_which dir)
{
	char path[PATH_MAX];
	if (find_directory(dir, -1, false, path, PATH_MAX) != B_OK) {
		return BString("/");
	}
	return BString(path, PATH_MAX);
}

BString
BeUtils::GetPathToHomeDir(void)
{
	return FindPathInner(B_USER_DIRECTORY);
}

BString
BeUtils::GetPathToUserDataDir(void)
{
	return FindPathInner(B_USER_DATA_DIRECTORY);
}

BString
BeUtils::GetPathToUserNonPackedDataDir(void)
{
	return FindPathInner(B_USER_NONPACKAGED_DATA_DIRECTORY);
}

BString
BeUtils::GetPathToSettingsFile(const char *settingsFileName)
{
	return FindPathInner(B_USER_SETTINGS_DIRECTORY) << BString("/") << BString(settingsFileName);
}

BString
BeUtils::GetPathToPackage(const char *packageName)
{
	return FindPathInner(B_SYSTEM_APPS_DIRECTORY) << BString("/") <<  BString(packageName);
}

BString
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
	else
	{
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
}

BeUtils::~BeUtils()
{

}
