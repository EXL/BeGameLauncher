/************************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2018 EXL
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy
 ** of this software and associated documentation files (the "Software"), to deal
 ** in the Software without restriction, including without limitation the rights
 ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ** copies of the Software, and to permit persons to whom the Software is
 ** furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 ** SOFTWARE.
 ************************************************************************************/

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
