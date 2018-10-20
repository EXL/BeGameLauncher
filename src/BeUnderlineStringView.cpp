#include "BeUnderlineStringView.h"

#define G_OFFSET_STRIPE_GAP_SMALL      -2.0f
#define G_OFFSET_STRIPE_GAP_BIG        -3.0f

BeUnderlineStringView::BeUnderlineStringView(const char* name, const char* text, float fontSize)
                     : BStringView(BRect(), name, text), fFontSize(fontSize)
{
	drawUnderline = true;

	SetFontSize(fFontSize);
}

void
BeUnderlineStringView::Draw(BRect rect)
{
	BStringView::Draw(rect);

	if(drawUnderline)
	{
		float offset = (fFontSize >= G_DEFAULT_FONT_SIZE) ? G_OFFSET_STRIPE_GAP_BIG : G_OFFSET_STRIPE_GAP_SMALL;

		StrokeLine(rect.OffsetToSelf(0.0f, offset).LeftBottom(),
		           rect.OffsetToSelf(0.0f, offset).RightBottom());
	}
}

float
BeUnderlineStringView::GetOffsetStipeGapBig(void)
{
	return G_OFFSET_STRIPE_GAP_BIG;
}

float
BeUnderlineStringView::GetOffsetStipeGapSmall(void)
{
	return G_OFFSET_STRIPE_GAP_SMALL;
}

BeUnderlineStringView::~BeUnderlineStringView(void)
{

}
