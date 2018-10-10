#ifndef BESETTINGS_H
#define BESETTINGS_H

#include <Message.h>
#include <String.h>

#include <map>

class BeSettings : public BMessage
{
	std::map<int, int> settings;
	BString pathToSettingsFile;
public:
	BeSettings(const char *fileName);
	bool DumpSettingsToFile();
	bool ReadSettingsFromFile();
	void UpdateSettings(std::map<int, int> options);
};

#endif // BESETTINGS_H
