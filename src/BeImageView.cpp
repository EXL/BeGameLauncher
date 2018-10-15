#include "BeImageView.h"
#include "BeUtils.h"

#include <GraphicsDefs.h>

#include <TranslationUtils.h>
#include <TranslatorFormats.h>

BeImageView::BeImageView(BRect rect, const char *name, BitmapIndex index, uint32 resizeFlags)
	: BView(rect, name, resizeFlags, B_WILL_DRAW)
{
	if(index == K_ICON)
	{

	}
	else
	{
		fBitmap = BTranslationUtils::GetBitmap(B_PNG_FORMAT, index);
	}
	if(fBitmap && fBitmap->IsValid())
	{
		ResizeTo(fBitmap->Bounds().Width(), fBitmap->Bounds().Width());
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

	SetDrawingMode(B_OP_ALPHA);

	if(fSuccessful && fBitmap)
	{
		DrawBitmap(fBitmap);
	}
}

BBitmap *BeImageView::GetIconFromSignature(const char *signature)
{

}
