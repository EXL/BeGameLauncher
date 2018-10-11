#include "BeSettings.h"
#include "BeUtils.h"

#include <File.h>
#include <Flattenable.h>

BeSettings::BeSettings(const char *fileName)
	: BMessage()
{
	pathToSettingsFile = BeUtils::GetPathToSettingsFile(fileName);
}

void
BeSettings::SetString(const char *name, const char *string)
{
	BString buffer;
	if(FindString(name, &buffer) != B_OK)
	{
		AddString(name, string);
	}
	else
	{
		ReplaceString(name, string);
	}
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
BeSettings::UpdateSettings(const std::map<BString, BString> &options)
{
	std::map<BString, BString>::const_iterator it = options.begin();
	
	while(it != options.end())
	{
		BString key = it->first;
		BString value = it->second;

		SetString(key.String(), value.String());

		++it;
	}
}

const char *
BeSettings::GetString(const char *name) const
{
	BString buffer;
	if (FindString(name, &buffer) != B_OK)
	{
		return "";
	}
	else
	{
		return buffer.String();
	}
}
