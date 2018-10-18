#include "BeUrlStringView.h"
#include "BeUtils.h"

#include <Cursor.h>
#include <Alert.h>

#include <Catalog.h>

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT           "BeGameLauncher"

#define L_URL_ALERT_WARNING_H           B_TRANSLATE("Open Link Error")
#define L_URL_ALERT_WARNING             B_TRANSLATE("Cannot open link: ")
#define L_URL_ALERT_BUTTON_OK           B_TRANSLATE("OK")

BeUrlStringView::BeUrlStringView(const char* name, const char* text, const char *url,
                                 float fontSize, uint32 resizingFlags)
	: BeUnderlineStringView(name, text, fontSize)
{
	fText << text;
	if(url)
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
	_UNUSED(point);
	_UNUSED(dragMessage);

	BCursor linkCursor(B_CURSOR_ID_FOLLOW_LINK);
	SetViewCursor(&linkCursor);

	drawUnderline = transit < B_EXITED_VIEW;

	Invalidate();
}

void
BeUrlStringView::MouseDown(BPoint point)
{
	_UNUSED(point);

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

BeUrlStringView::~BeUrlStringView()
{

}
