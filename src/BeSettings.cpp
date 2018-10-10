#include "BeSettings.h"
#include "BeUtils.h"

#include <File.h>
#include <Flattenable.h>

BeSettings::BeSettings(const char *fileName)
	: BMessage()
{
	pathToSettingsFile = BeUtils::GetPathToSettingsFile(fileName);
}

bool
BeSettings::DumpSettingsToFile()
{
	BFile out;
	if(out.SetTo(pathToSettingsFile.String(), B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE) != B_OK)
	{
		return false;
	}
	if(Flatten(&out) != B_OK)
	{
		return false;
	}
	out.Unset();
	return true;
}

bool
BeSettings::ReadSettingsFromFile()
{
	BFile in;
	if(in.SetTo(pathToSettingsFile.String(), B_READ_ONLY) != B_OK)
	{
		return false;
	}
	if(Unflatten(&in) != B_OK)
	{
		return false;
	}
	return true;
}

void
BeSettings::UpdateSettings(std::map<int, int> options)
{
	settings = options;
}
