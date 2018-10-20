#ifndef BEABOUTSTRIPEVIEW_H
#define BEABOUTSTRIPEVIEW_H

#include <View.h>
#include <Rect.h>
#include <Bitmap.h>

class BeAboutStripeView : public BView
{
	const BBitmap *fIcon;

protected:
	virtual void Draw(BRect rect);

public:
	explicit BeAboutStripeView(const BBitmap *icon);
	virtual ~BeAboutStripeView(void);

	static float GetIconX(void);
	static float GetIconY(void);
	static float GetIconOffset(void);
	static float GetStripeWidth(void);
};

#endif // BEABOUTSTRIPEVIEW_H
