#ifndef BEMULTISTRINGVIEW_H
#define BEMULTISTRINGVIEW_H

#include <GraphicsDefs.h>
#include <TextView.h>

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
