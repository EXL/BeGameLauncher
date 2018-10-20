#ifndef BEIMAGEVIEW_H
#define BEIMAGEVIEW_H

#include <View.h>
#include <Rect.h>
#include <Bitmap.h>

enum BitmapIndex
{
	K_BANNER,
	K_ICON
};

class BeImageView : public BView
{
	const BitmapIndex fIndex;

	const BBitmap *fBitmap;

	bool fSuccessful;

protected:
	virtual void Draw(BRect rect);

public:
	explicit BeImageView(const char *name, BitmapIndex index);
	virtual ~BeImageView(void);

	static const BBitmap* GetIconBitmapByIndex(BitmapIndex index);

	static float GetImageOnePixelSize(void);
	static float GetGeneralWidth(void);
};

#endif // BEIMAGEVIEW_H
