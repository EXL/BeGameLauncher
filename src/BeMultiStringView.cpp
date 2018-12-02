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

#include "BeMultiStringView.h"

#include <View.h>
#include <Size.h>

#define G_MIN_WIDTH                    240.0f
#define G_MIN_HEIGHT                   200.0f
#define G_GENERAL_INSET                0.0f

BeMultiStringView::BeMultiStringView(const char *name, const rgb_color *color)
                   : BTextView(name, NULL, color, B_WILL_DRAW)
{
	MakeEditable(false);
	MakeSelectable(true);
	// MakeResizable(true); ??

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

BeMultiStringView::~BeMultiStringView(void)
{

}
