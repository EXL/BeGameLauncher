#include "BeUrlStringView.h"
#include "BeUtils.h"

#include <Cursor.h>
#include <Catalog.h>
#include <Alert.h>

#define G_OFFSET_STRIPE_GAP_SMALL       -2.0f
#define G_OFFSET_STRIPE_GAP_BIG         -3.0f

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT           "BeUrlStringView"

#define L_URL_ALERT_WARNING_H           B_TRANSLATE("Open Link Error")
#define L_URL_ALERT_WARNING             B_TRANSLATE("Cannot open link: ")
#define L_URL_ALERT_BUTTON_OK           B_TRANSLATE("OK")

BeUrlStringView::BeUrlStringView(BRect frame, const char* name, const char* text, const char *url,
                                 float fontSize, uint32 resizingFlags)
	: BStringView(frame, name, text), fFontSize(fontSize)
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
	SetFontSize(fFontSize);
	ResizeToPreferred();
}

void
BeUrlStringView::Draw(BRect bounds)
{
	BStringView::Draw(bounds);

	if(drawUnderline)
	{
		float offset = (fFontSize >= 12.0f) ? G_OFFSET_STRIPE_GAP_BIG : G_OFFSET_STRIPE_GAP_SMALL;

		StrokeLine(bounds.OffsetToSelf(0.0f, offset).LeftBottom(),
		           bounds.OffsetToSelf(0.0f, offset).RightBottom());
	}
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

float
BeUrlStringView::GetOffsetStripeGapSmall()
{
	return G_OFFSET_STRIPE_GAP_SMALL;
}

float
BeUrlStringView::GetOffsetStripeGapBig()
{
	return G_OFFSET_STRIPE_GAP_BIG;
}
