#include "BeDirectoryFilePanel.h"

#include <Catalog.h>

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT     "BeDirectoryFilePanel"

#define L_BTN_SELECT              B_TRANSLATE("Select")

BeDirectoryFilePanel::BeDirectoryFilePanel(file_panel_mode mode,
                                           BMessenger* target, const entry_ref* directory,
                                           uint32 nodeFlavors, bool allowMultipleSelection,
                                           BMessage* message, BRefFilter* refFilter,
                                           bool modal, bool hideWhenDone)
	: BFilePanel(mode, target, directory, nodeFlavors, allowMultipleSelection,
	            message, refFilter, modal, hideWhenDone)
{

}

void
BeDirectoryFilePanel::Show()
{
	SetButtonLabel(B_DEFAULT_BUTTON, L_BTN_SELECT);
	BFilePanel::Show();
}
