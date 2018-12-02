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

#ifndef BELAUNCHERVIEW_H
#define BELAUNCHERVIEW_H

#include <GroupView.h>
#include <TextControl.h>
#include <Box.h>
#include <Button.h>
#include <StringView.h>

class BeLauncherView : public BGroupView
{
	BTextControl *fTextControl;
	BButton *fBrowseButton;
	BBox *fAdditionalBox;
	BStringView *fStatusStringView;
	BButton *fAboutButton;
	BButton *fRunButton;
	BButton *fExitButton;

public:
	explicit BeLauncherView(bool showIcon = false);
	virtual ~BeLauncherView(void);

	BTextControl *GetTextControl(void) const;
	BButton *GetBrowseButton(void) const;
	BBox *GetAdditionalBox(void) const;
	BStringView *GetStatusStringView(void) const;
	BButton *GetAboutButton(void) const;
	BButton *GetRunButton(void) const;
	BButton *GetExitButton(void) const;

	static const char *GetReadyString(void);
};

#endif // BELAUNCHERVIEW_H
