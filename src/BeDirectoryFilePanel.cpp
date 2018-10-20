#include "BeDirectoryFilePanel.h"

#include <StorageDefs.h>
#include <Entry.h>

#include <Catalog.h>

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "BeGameLauncher"

#define L_BTN_SELECT                   B_TRANSLATE("Select")

BeDirectoryFilePanel::BeDirectoryFilePanel(BMessenger *target,
                                           BMessage *message,
                                           BRefFilter *refFilter,
                                           const char *startPath)
                    : BFilePanel(B_OPEN_PANEL, target, NULL, B_DIRECTORY_NODE,
                                 false, message, refFilter, true, true)
{
	entry_ref start_point;
	BEntry entry(startPath);
	entry.GetRef(&start_point);
	SetPanelDirectory(&start_point);
}

void
BeDirectoryFilePanel::Show(void)
{
	// Just rename button from "Open" to "Select".
	SetButtonLabel(B_DEFAULT_BUTTON, L_BTN_SELECT);
	BFilePanel::Show();
}

BeDirectoryFilePanel::~BeDirectoryFilePanel(void)
{

}
