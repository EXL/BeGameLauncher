#include "BeMultiStringView.h"

#include <Size.h>
#include <View.h>
#include <InterfaceDefs.h>

#define G_MIN_WIDTH                   240.0f
#define G_MIN_HEIGHT                  200.0f
#define G_GENERAL_INSET               0.0f

BeMultiStringView::BeMultiStringView(const char *name, const rgb_color *color)
	: BTextView(name, NULL, color, B_WILL_DRAW)
{
	MakeEditable(false);
	MakeSelectable(true);
	//MakeResizable(true); ??

	SetWordWrap(true);
	SetStylable(true);
	SetExplicitMinSize(BSize(G_MIN_WIDTH, G_MIN_HEIGHT));
	SetInsets(G_GENERAL_INSET, G_GENERAL_INSET, G_GENERAL_INSET, G_GENERAL_INSET);
	// SetFlags(Flags() | B_FRAME_EVENTS);
	// SetResizingMode(B_FOLLOW_LEFT_TOP);
}

float
BeMultiStringView::GetMinWidth(void)
{
	return G_MIN_WIDTH;
}

float
BeMultiStringView::GetMinHeight(void)
{
	return G_MIN_HEIGHT;
}

float
BeMultiStringView::GetGeneralInset(void)
{
	return G_GENERAL_INSET;
}

BeMultiStringView::~BeMultiStringView()
{

}
