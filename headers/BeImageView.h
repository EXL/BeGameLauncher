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

	BBitmap *fBitmap;

	BBitmap *GetIconFromSignature(const char *signature);
public:
	BeImageView(BRect rect, const char *name, BitmapIndex index, uint32 resizeFlags);

	virtual void Draw(BRect rect);
};

#endif // BEIMAGEVIEW_H
