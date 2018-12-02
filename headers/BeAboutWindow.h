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

#ifndef BEABOUTWINDOW_H
#define BEABOUTWINDOW_H

#include "BeMainWindow.h"
#include "BeAboutView.h"
#include "BeMultiStringView.h"
#include "BeUtils.h"

#include <String.h>
#include <Rect.h>
#include <Box.h>

class BeAboutWindow : public BeMainWindow
{
	const char *fTitle;
	BString fVersion;

	BeAboutView *fAboutView;
	BeMultiStringView *fInformationView;
	BBox *fAdditionalBox;

	void CreateForm(void);

protected:
	virtual void FrameResized(float newWidth, float newHeight);

	virtual bool QuitRequested(void);

public:
	explicit BeAboutWindow(const BRect &frame, const char *title, const char *version = G_DEFAULT_VERSION);
	virtual ~BeAboutWindow(void);

	BeAboutView *GetMainView(void) const;
	BeMultiStringView *GetInformationView(void) const;
	BBox *GetAdditionalBox(void) const;
};

#endif // BEABOUTWINDOW_H
