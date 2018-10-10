#include "BeUtils.h"

#include <posix/limits.h>

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
BeUtils::GetPathToHomeDir()
{
	return FindPathInner(B_USER_DIRECTORY);
}

BString
BeUtils::GetPathToUserDataDir()
{
	return FindPathInner(B_USER_DATA_DIRECTORY);
}

BString
BeUtils::GetPathToUserNonPackedDataDir()
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
	return FindPathInner(B_SYSTEM_APPS_DIRECTORY) << BString("/") << BString(packageName) << BString("/") << BString(executableName);
}
