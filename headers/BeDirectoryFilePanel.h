#ifndef BEDIRECTORYFILEPANEL_H
#define BEDIRECTORYFILEPANEL_H

#include <Messenger.h>
#include <Message.h>

#include <FilePanel.h>

class BeDirectoryFilePanel : public BFilePanel
{
public:
	explicit BeDirectoryFilePanel(BMessenger *target,
	                              BMessage *message,
	                              BRefFilter *refFilter,
	                              const char *startPath);
	virtual ~BeDirectoryFilePanel(void);

	void Show(void);
};

#endif // BEDIRECTORYFILEPANEL_H
