#ifndef BESETTINGS_H
#define BESETTINGS_H

#include <Message.h>
#include <String.h>

#include <map>

class BeSettings : public BMessage
{
	BString pathToSettingsFile;
	void SetString(const char *name, const char *string);
public:
	BeSettings(const char *fileName);
	bool DumpSettingsToFile();
	bool ReadSettingsFromFile();
	void UpdateSettings(const std::map<BString, BString> &options);
	const char *GetString(const char *name) const;
};

#endif // BESETTINGS_H
