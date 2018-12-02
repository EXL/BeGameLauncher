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

#include "BeUnderlineStringView.h"

#include <Font.h>

#define G_OFFSET_STRIPE_GAP_SMALL      (-2.0f)
#define G_OFFSET_STRIPE_GAP_BIG        (-3.0f)

BeUnderlineStringView::BeUnderlineStringView(const char* name, const char* text, bool smallFontSize)
                       : BStringView(BRect(), name, text), fSmallFontSize(smallFontSize)
{
	drawUnderline = true;

	BFont font;
	GetFont(&font);
	fFontSize = font.Size();

	if(fSmallFontSize)
	{
		fFontSize += G_OFFSET_STRIPE_GAP_SMALL; // -2.0f
	}

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
BeUnderlineStringView::GetOffsetStripeGapBig(void)
{
	return G_OFFSET_STRIPE_GAP_BIG;
}

float
BeUnderlineStringView::GetOffsetStripeGapSmall(void)
{
	return G_OFFSET_STRIPE_GAP_SMALL;
}

BeUnderlineStringView::~BeUnderlineStringView(void)
{

}
