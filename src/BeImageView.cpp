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
	if(fBitmap && fBitmap->IsValid())
	{
		fSuccessful = true;
		width = fBitmap->Bounds().Width();
		height = fBitmap->Bounds().Height();
	}
	else
	{
		fSuccessful = false;
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
	_UNUSED(rect);

	BRect bitmapRect = fBitmap->Bounds();
	BRect drawRect = Bounds();
	drawRect = BRect(drawRect.left,
	                 drawRect.top,
	                 drawRect.right,
	                 drawRect.bottom - G_IMAGE_ONE_PIXEL_SIZE);

	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);

	if(fSuccessful)
	{
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

BBitmap *
BeImageView::GetIconBitmapByIndex(BitmapIndex index)
{
	BBitmap *bitmap = BTranslationUtils::GetBitmap(B_PNG_FORMAT, index);
	if(bitmap && bitmap->IsValid())
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

BeImageView::~BeImageView()
{
	delete fBitmap;
	fBitmap = NULL;
}
