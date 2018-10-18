#ifndef BEABOUTSTRIPEVIEW_H
#define BEABOUTSTRIPEVIEW_H

#include <View.h>
#include <Rect.h>
#include <Bitmap.h>

class BeAboutStripeView : public BView
{
	BBitmap *fIcon;

public:
	BeAboutStripeView(BBitmap *icon);
	virtual ~BeAboutStripeView();

	static float GetIconX(void);
	static float GetIconY(void);
	static float GetIconOffset(void);
	static float GetStripeWidth(void);

protected:
	virtual void Draw(BRect rect);
};

#endif // BEABOUTSTRIPEVIEW_H
