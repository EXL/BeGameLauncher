#ifndef BESETTINGS_H
#define BESETTINGS_H

#include <Message.h>
#include <String.h>

class BeSettings : public BMessage
{
	BString pathToSettingsFile;
public:
	BeSettings(const char *fileName);

	bool DumpSettingsToFile();
	bool ReadSettingsFromFile();
	const char *GetString(const char *name) const;
	void SetString(const char *name, const char *string);
};

#endif // BESETTINGS_H
