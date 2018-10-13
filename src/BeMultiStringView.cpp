#include "BeMultiStringView.h"

#include <View.h>
#include <InterfaceDefs.h>

BeMultiStringView::BeMultiStringView(const char *name, BRect rect)
	: BTextView(rect, name, rect, B_FOLLOW_ALL, B_WILL_DRAW)
{
	MakeEditable(false);
	MakeSelectable(true);
	// MakeResizable(true); ??

	SetFlags(Flags() | B_FRAME_EVENTS);
	SetWordWrap(true);
	SetStylable(true);

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

void BeMultiStringView::FrameResized(float newWidth, float newHeight)
{
	SetTextRect(BRect(0.0f, 0.0f, newWidth, newHeight));
	BTextView::FrameResized(newWidth, newHeight);
}
