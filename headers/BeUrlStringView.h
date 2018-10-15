#ifndef BEURLSTRINGVIEW_H
#define BEURLSTRINGVIEW_H

#include <StringView.h>
#include <Rect.h>
#include <Point.h>
#include <Message.h>
#include <String.h>
#include <SupportDefs.h>
#include <View.h>

class BeUrlStringView : public BStringView
{
	const float fFontSize;

	bool drawUnderline;

	BString fUrl;
	BString fText;

	void ShowOpenLinkWarning(const BString &url);
public:
	BeUrlStringView(BRect frame, const char* name, const char* text, const char *url = NULL,
	                float fontSize = 12.0f, uint32 resizingFlags = B_FOLLOW_LEFT);

	virtual	void Draw(BRect bounds);
	virtual void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage);
	virtual void MouseDown(BPoint point);

	static float GetOffsetStripeGapSmall();
	static float GetOffsetStripeGapBig();
};

#endif // BEURLSTRINGVIEW_H
