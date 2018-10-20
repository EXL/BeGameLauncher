#ifndef BEDIRECTORYFILEPANEL_H
#define BEDIRECTORYFILEPANEL_H

#include <Messenger.h>
#include <Message.h>
#include <SupportDefs.h>
#include <Entry.h>

#include <FilePanel.h>

class BeDirectoryFilePanel : public BFilePanel
{
public:
	explicit BeDirectoryFilePanel(file_panel_mode mode = B_OPEN_PANEL,
	                              BMessenger* target = NULL,
	                              const entry_ref* directory = NULL,
	                              uint32 nodeFlavors = 0,
	                              bool allowMultipleSelection = true,
	                              BMessage* message = NULL,
	                              BRefFilter* refFilter = NULL,
	                              bool modal = false,
	                              bool hideWhenDone = true);
	virtual ~BeDirectoryFilePanel();

	void Show();
};

#endif // BEDIRECTORYFILEPANEL_H
