#ifndef BEIMAGEVIEW_H
#define BEIMAGEVIEW_H

#include <View.h>
#include <Rect.h>
#include <Bitmap.h>
#include <SupportDefs.h>

enum BitmapIndex
{
	K_BANNER,
	K_ICON
};

class BeImageView : public BView
{
	bool fSuccessful;
	bool fStripe;

	BBitmap *fBitmap;
public:
	BeImageView(BRect rect, const char *name, BitmapIndex index, uint32 resizeFlags, bool stripe = false);

	virtual void Draw(BRect rect);

	static BBitmap* GetIconBitmap();
};

#endif // BEIMAGEVIEW_H
