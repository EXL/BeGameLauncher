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
	                         float fontSize = G_DEFAULT_FONT_SIZE, uint32 resizingFlags = B_FOLLOW_LEFT);
	virtual ~BeUrlStringView(void);
};

#endif // BEURLSTRINGVIEW_H
