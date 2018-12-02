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

#include "BeImageView.h"
#include "BeUtils.h"

#include <Size.h>
#include <InterfaceDefs.h>
#include <GraphicsDefs.h>

#include <TranslationUtils.h>
#include <TranslatorFormats.h>

#define G_IMAGE_ONE_PIXEL_SIZE         1.0f
#define G_IMAGE_GENERAL_WIDTH          64.0f

BeImageView::BeImageView(const char *name, BitmapIndex index)
             : BView(name, B_WILL_DRAW), fIndex(index)
{
	SetFlags(Flags() | B_FULL_UPDATE_ON_RESIZE);
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	float width;
	float height;
	fBitmap = BTranslationUtils::GetBitmap(B_PNG_FORMAT, fIndex);
	fSuccessful = ((fBitmap != NULL) && (fBitmap->IsValid()));
	if(fSuccessful)
	{
		width = fBitmap->Bounds().Width();
		height = fBitmap->Bounds().Height();
	}
	else
	{
		SetViewColor(K_RED);
		width = G_IMAGE_GENERAL_WIDTH;
		height = G_IMAGE_GENERAL_WIDTH;
	}

	if(fIndex != K_ICON)
	{
		SetExplicitSize(BSize(width, B_SIZE_UNSET));
		SetExplicitPreferredSize(BSize(width, B_SIZE_UNLIMITED));
	}
	else
	{
		SetExplicitSize(BSize(width, height));
		SetExplicitPreferredSize(BSize(width, height));
	}
}

void
BeImageView::Draw(BRect rect)
{
	BeUnused(rect);

	if(fSuccessful)
	{
		const BRect bitmapRect = fBitmap->Bounds();
		BRect drawRect = Bounds();
		drawRect = BRect(drawRect.left,
		                 drawRect.top,
		                 drawRect.right,
		                 drawRect.bottom);

		SetDrawingMode(B_OP_ALPHA);
		SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);

		if(fIndex != K_ICON)
		{
			DrawBitmapAsync(fBitmap, bitmapRect, drawRect);
		}
		else
		{
			DrawBitmapAsync(fBitmap);
		}
	}
}

const BBitmap *
BeImageView::GetIconBitmapByIndex(BitmapIndex index)
{
	const BBitmap *bitmap = BTranslationUtils::GetBitmap(B_PNG_FORMAT, index);
	if((bitmap != NULL) && (bitmap->IsValid()))
	{
		return bitmap;
	}
	return NULL;
}

float
BeImageView::GetImageOnePixelSize(void)
{
	return G_IMAGE_ONE_PIXEL_SIZE;
}

float
BeImageView::GetGeneralWidth(void)
{
	return G_IMAGE_GENERAL_WIDTH;
}

BeImageView::~BeImageView(void)
{
	delete fBitmap;
	fBitmap = NULL;
}
