#include "BeImageView.h"
#include "BeUtils.h"

#include <InterfaceDefs.h>

#include <TranslationUtils.h>
#include <TranslatorFormats.h>

#define G_IMAGE_ONE_PIXEL_SIZE                       1.0f
#define G_IMAGE_GENERAL_WIDTH                        64.0f

BeImageView::BeImageView(const char *name, BitmapIndex index)
	: BView(BRect(), name, B_FOLLOW_LEFT, B_WILL_DRAW), fIndex(index)
{
	SetFlags(Flags() | B_FULL_UPDATE_ON_RESIZE);
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	float width;
	fBitmap = BTranslationUtils::GetBitmap(B_PNG_FORMAT, fIndex);
	if(fBitmap && fBitmap->IsValid())
	{
		fSuccessful = true;
		width = fBitmap->Bounds().Width();
	}
	else
	{
		fSuccessful = false;
		SetViewColor(K_RED);
		width = G_IMAGE_GENERAL_WIDTH;
	}

	SetExplicitSize(BSize(width, B_SIZE_UNSET));
	SetExplicitPreferredSize(BSize(width, B_SIZE_UNLIMITED));
}

void
BeImageView::Draw(BRect rect)
{
	_UNUSED(rect);

	BRect bitmapRect = fBitmap->Bounds();
	BRect drawRect = Bounds();
	drawRect = BRect(drawRect.left, drawRect.top, drawRect.right, drawRect.bottom - G_IMAGE_ONE_PIXEL_SIZE);

	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);

//	if(fStripe)
//	{
//		rgb_color oldColor = HighColor();
//		SetHighColor(tint_color(ViewColor(), B_DARKEN_1_TINT));
//		//FillRect(BRect(0.0f, 0.0f, BeAboutWindow::GetStripeOffsetX(), bitmapRect.Height()));
//		SetHighColor(oldColor);
//	}

	BeDebug("%f %f %f %f | %f %f %f %f | %f %f %f %f\n", bitmapRect.left, bitmapRect.top, bitmapRect.right, bitmapRect.bottom,
	        drawRect.left, drawRect.bottom, drawRect.right, drawRect.bottom,
	        rect.left, rect.bottom, rect.right, rect.bottom);

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

}
