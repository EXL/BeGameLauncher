#ifndef BESETTINGS_H
#define BESETTINGS_H

#include <Message.h>
#include <String.h>

class BeSettings : public BMessage
{
	BString pathToSettingsFile;

public:
	explicit BeSettings(const char *fileName);
	virtual ~BeSettings(void);

	bool DumpSettingsToFile(void);
	bool ReadSettingsFromFile(void);

	const char *GetString(const char *name) const;
	void SetString(const char *name, const char *string);
};

#endif // BESETTINGS_H
