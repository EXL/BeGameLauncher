#ifndef BEMULTISTRINGVIEW_H
#define BEMULTISTRINGVIEW_H

#include <TextView.h>
#include <GraphicsDefs.h>

class BeMultiStringView : public BTextView
{
public:
	BeMultiStringView(const char* name, const rgb_color *color);
	virtual ~BeMultiStringView();

	static float GetMinWidth(void);
	static float GetMinHeight(void);
	static float GetGeneralInset(void);
};

#endif // BEMULTISTRINGVIEW_H
