#ifndef BEUNDERLINESTRINGVIEW_H
#define BEUNDERLINESTRINGVIEW_H

#include "BeUtils.h"

#include <StringView.h>

class BeUnderlineStringView : public BStringView
{
	float fFontSize;

protected:
	bool drawUnderline;

	virtual void Draw(BRect rect);
public:
	BeUnderlineStringView(const char* name, const char* text, float fontSize = G_DEFAULT_FONT_SIZE);
	virtual ~BeUnderlineStringView();

	static float GetOffsetStipeGapBig(void);
	static float GetOffsetStipeGapSmall(void);
};

#endif // BEUNDERLINESTRINGVIEW_H
