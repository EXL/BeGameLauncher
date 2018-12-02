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

#ifndef BEURLSTRINGVIEW_H
#define BEURLSTRINGVIEW_H

#include "BeUnderlineStringView.h"
#include "BeUtils.h"

#include <Point.h>
#include <String.h>
#include <Message.h>
#include <SupportDefs.h>
#include <View.h>

class BeUrlStringView : public BeUnderlineStringView
{
	const BString fText;
	BString fUrl;

	void ShowOpenLinkWarning(const BString &url);

protected:
	virtual void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage);
	virtual void MouseDown(BPoint point);

public:
	explicit BeUrlStringView(const char* name, const char* text, const char *url = NULL,
	                         bool smallFontSize = false, uint32 resizingFlags = B_FOLLOW_LEFT);
	virtual ~BeUrlStringView(void);
};

#endif // BEURLSTRINGVIEW_H
