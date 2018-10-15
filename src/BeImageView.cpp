#include "BeImageView.h"
#include "BeUtils.h"

#include <GraphicsDefs.h>
#include <InterfaceDefs.h>

#include <TranslationUtils.h>
#include <TranslatorFormats.h>

BeImageView::BeImageView(BRect rect, const char *name, BitmapIndex index, uint32 resizeFlags)
	: BView(rect, name, resizeFlags, B_WILL_DRAW)
{
	SetFlags(Flags() | B_FULL_UPDATE_ON_RESIZE);
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	if(index == K_ICON)
	{

	}
	else
	{
		fBitmap = BTranslationUtils::GetBitmap(B_PNG_FORMAT, index);
	}
	if(fBitmap && fBitmap->IsValid())
	{
		ResizeTo(fBitmap->Bounds().Width(), fBitmap->Bounds().Height());
		fSuccessful = true;
	}
	else
	{
		SetViewColor(K_RED);
		fSuccessful = false;
	}
}

void
BeImageView::Draw(BRect rect)
{
	_UNUSED(rect);

	BRect bitmapRect = fBitmap->Bounds();
	BRect drawRect = Bounds();

	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);

	if(fSuccessful && fBitmap)
	{
		DrawBitmapAsync(fBitmap, bitmapRect, drawRect);
	}
}

BBitmap *
BeImageView::GetIconFromSignature(const char *signature)
{

}
