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

#include "BeUrlStringView.h"

#include <Cursor.h>
#include <Alert.h>
#include <Window.h>

#include <Catalog.h>

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "BeGameLauncher"

#define L_URL_ALERT_WARNING_H          B_TRANSLATE("Open Link Error")
#define L_URL_ALERT_WARNING            B_TRANSLATE("Cannot open link: ")
#define L_URL_ALERT_BUTTON_OK          B_TRANSLATE("OK")

BeUrlStringView::BeUrlStringView(const char* name, const char* text, const char *url,
                                 bool smallFontSize, uint32 resizingFlags)
                 : BeUnderlineStringView(name, text, smallFontSize), fText(text)
{
	if(url != NULL)
	{
		fUrl << url;
	}
	else
	{
		fUrl = "";
	}

	drawUnderline = false;

	SetResizingMode(resizingFlags);
	SetHighColor(K_BLUE);
	ResizeToPreferred();
}

void
BeUrlStringView::MouseMoved(BPoint point, uint32 transit, const BMessage *dragMessage)
{
	BeUnused(point);
	BeUnused(dragMessage);

	BCursor linkCursor(B_CURSOR_ID_FOLLOW_LINK);
	SetViewCursor(&linkCursor);

	drawUnderline = transit < B_EXITED_VIEW;

	Invalidate();
}

void
BeUrlStringView::MouseDown(BPoint point)
{
	BeUnused(point);

	BString url;
	if(!fUrl.IsEmpty())
	{
		url = fUrl;
	}
	else
	{
		url = fText;
	}

	if(!BeUtils::OpenLinkViaWebBrowser(url.String()))
	{
		ShowOpenLinkWarning(url);
	}
}

void
BeUrlStringView::ShowOpenLinkWarning(const BString &url)
{
	BString openLinkWarning(L_URL_ALERT_WARNING);
	openLinkWarning << url;
	BAlert *openLinkWarningAlert = new BAlert(L_URL_ALERT_WARNING_H, openLinkWarning, L_URL_ALERT_BUTTON_OK);
	openLinkWarningAlert->SetType(B_WARNING_ALERT);
	openLinkWarningAlert->SetFlags(openLinkWarningAlert->Flags() | B_CLOSE_ON_ESCAPE);

	openLinkWarningAlert->Go();
}

BeUrlStringView::~BeUrlStringView(void)
{

}
